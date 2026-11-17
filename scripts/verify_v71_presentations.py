#!/usr/bin/env python3
"""Validate the original JK AXIS V7.1 presentation package in GitHub."""
from __future__ import annotations
import hashlib
import json
from pathlib import Path
import re
import sys
import xml.etree.ElementTree as ET
import zipfile

ROOT = Path(__file__).resolve().parents[1]
PACKAGE = ROOT / "milestones" / "meilenstein-1-und-2" / "v7.1" / "Originalpaket"
INDEX = PACKAGE.parent / "README.md"
MANIFEST = PACKAGE / "SHA256SUMS.txt"
errors = []
expected = {}
if not MANIFEST.is_file():
    raise SystemExit("FAIL: V7.1 checksum manifest is missing")
for line in MANIFEST.read_text(encoding="utf-8").splitlines():
    match = re.fullmatch(r"([0-9a-f]{64})[ \t]{2,}(.+)", line)
    if not match:
        errors.append("Invalid manifest line")
        continue
    sha, name = match.groups()
    if name in expected:
        errors.append("Duplicate checksum entry: " + name)
    expected[name] = sha
    path = (PACKAGE / name).resolve()
    if not path.is_relative_to(PACKAGE.resolve()) or not path.is_file():
        errors.append("Missing or unsafe path: " + name)
        continue
    if hashlib.sha256(path.read_bytes()).hexdigest() != sha:
        errors.append("SHA-256 mismatch: " + name)
    try:
        if path.suffix.lower() == ".pdf":
            payload = path.read_bytes()
            if not payload.startswith(b"%PDF-") or b"%%EOF" not in payload[-4096:]:
                errors.append("Invalid PDF signature/EOF: " + name)
        elif path.suffix.lower() in (".pptx", ".docx"):
            with zipfile.ZipFile(path) as archive:
                if archive.testzip() is not None:
                    errors.append("Damaged Office package: " + name)
        elif path.suffix.lower() == ".png":
            if not path.read_bytes().startswith(b"\x89PNG\r\n\x1a\n"):
                errors.append("Invalid PNG signature: " + name)
    except (OSError, zipfile.BadZipFile) as ex:
        errors.append("Invalid file: " + name + " (" + str(ex) + ")")

actual = {p.relative_to(PACKAGE).as_posix() for p in PACKAGE.rglob("*") if p.is_file() and p != MANIFEST}
if actual != set(expected):
    errors.append("Manifest inventory does not match: missing=" + repr(sorted(set(expected)-actual))
                  + ", extra=" + repr(sorted(actual-set(expected))))
if len(expected) != 13:
    errors.append("Unexpected number of original files: " + str(len(expected)))
if not INDEX.is_file():
    errors.append("V7.1 GitHub overview is missing")

decks = [
    ("07_Einzelpraesentation_19_11_2026_JK_AXIS_V7_Schullogo_Herr_Funke.pptx", 9),
    ("06_Meilenstein_1_und_2_JK_AXIS_V7_Schullogo_Herr_Funke.pptx", 13),
]
for filename, count in decks:
    path = PACKAGE / filename
    if not path.exists():
        errors.append("Missing PowerPoint: " + filename)
        continue
    try:
        with zipfile.ZipFile(path) as z:
            all_slides = sorted(
                (n for n in z.namelist() if re.fullmatch(r"ppt/slides/slide[0-9]+\.xml", n)),
                key=lambda n: int(re.search(r"slide([0-9]+)\.xml", n).group(1)),
            )
            if len(all_slides) != count:
                errors.append("Wrong slide count: " + filename + " (" + str(len(all_slides)) + ")")
                continue
            if not any(n.startswith("ppt/media/") for n in z.namelist()):
                errors.append("Missing embedded images: " + filename)
            for slide in all_slides:
                if b"<p:pic>" not in z.read(slide):
                    errors.append("No visible embedded images on " + filename + ": " + slide)
            for slide in (all_slides[0], all_slides[-1]):
                root = ET.fromstring(z.read(slide))
                text = " ".join(node.text or "" for node in root.iter() if node.tag.endswith("}t"))
                if "Funke" not in text:
                    errors.append("Missing Herr Funke on cover/feedback: " + filename + ": " + slide)
    except (OSError, zipfile.BadZipFile, ET.ParseError) as ex:
        errors.append("Cannot inspect PowerPoint: " + filename + ": " + str(ex))
for failure in errors:
    print("FAIL:", failure, file=sys.stderr)
if errors:
    raise SystemExit(1)
print("PASS: 13 SHA-256 items, 14 archive entries, 9+13 PPTX slides, images, teacher attribution, PDF/PNG structure")
