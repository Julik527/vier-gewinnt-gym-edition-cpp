#!/usr/bin/env python3
"""Check V6 milestone import inventory, hashes, document containers and links."""
from __future__ import annotations

import hashlib
from pathlib import Path
import re
import sys
from urllib.parse import unquote
import zipfile

ROOT = Path(__file__).resolve().parents[1]
V6 = ROOT / "milestones" / "meilenstein-1-und-2" / "v6"
MANIFEST = V6 / "SHA256SUMS.txt"
LINK_DOCS = [
    ROOT / "milestones" / "README.md",
    ROOT / "milestones" / "2026-11-26" / "README.md",
    ROOT / "milestones" / "meilenstein-1-und-2" / "README.md",
    ROOT / "deliverables" / "README.md",
]
errors: list[str] = []
seen: set[str] = set()

if not MANIFEST.is_file():
    print("FAIL: V6 SHA256SUMS.txt is missing", file=sys.stderr)
    raise SystemExit(1)

for lineno, line in enumerate(MANIFEST.read_text(encoding="utf-8").splitlines(), 1):
    if not line.strip():
        continue
    match = re.fullmatch(r"([a-f0-9]{64})  (.+)", line)
    if not match:
        errors.append(f"Invalid SHA256SUMS line {lineno}")
        continue
    expected, rel = match.groups()
    if rel in seen:
        errors.append(f"Duplicate manifest entry: {rel}")
        continue
    seen.add(rel)
    candidate = (V6 / rel).resolve()
    if not candidate.is_relative_to(V6.resolve()):
        errors.append(f"Manifest path escapes V6: {rel}")
        continue
    if not candidate.is_file():
        errors.append(f"Missing V6 file: {rel}")
        continue
    actual = hashlib.sha256(candidate.read_bytes()).hexdigest()
    if expected != actual:
        errors.append(f"SHA-256 mismatch: {rel}")
    try:
        if candidate.suffix.lower() in {".docx", ".pptx"}:
            with zipfile.ZipFile(candidate) as archive:
                invalid = archive.testzip()
                if invalid:
                    errors.append(f"Broken OOXML member: {rel}::{invalid}")
        elif candidate.suffix.lower() == ".pdf":
            if not candidate.read_bytes().startswith(b"%PDF-"):
                errors.append(f"Invalid PDF header: {rel}")
    except (OSError, zipfile.BadZipFile) as exc:
        errors.append(f"Invalid document container: {rel}: {exc}")

actual_files = {
    p.relative_to(V6).as_posix()
    for p in V6.rglob("*") if p.is_file() and p != MANIFEST
}
for rel in sorted(actual_files - seen):
    errors.append(f"V6 file missing from manifest: {rel}")
for rel in sorted(seen - actual_files):
    errors.append(f"Manifest entry has no V6 file: {rel}")

for doc in [*V6.rglob("*.md"), *LINK_DOCS]:
    try:
        content = doc.read_text(encoding="utf-8")
    except OSError as exc:
        errors.append(f"Cannot read markdown: {doc}: {exc}")
        continue
    for raw in re.findall(r"(?<!!)\[[^\]]+\]\(([^)]+)\)", content):
        address = raw.strip().split("#", 1)[0]
        if not address or re.match(r"^(?:https?://|mailto:)", address, re.I):
            continue
        link_path = unquote(address)
        target = (doc.parent / link_path).resolve()
        if not target.is_relative_to(ROOT.resolve()) or not target.exists():
            errors.append(f"Broken relative link: {doc.relative_to(ROOT)} -> {address}")

for name in ("schulunterlagen", "praesentationen", "vorfuehrung", "nachweise"):
    if not (V6 / name).is_dir():
        errors.append(f"Missing required directory: {name}")
for path in V6.rglob("*"):
    if path.is_file() and (path.name.startswith(".~lock.") or path.name.endswith(".zip")):
        errors.append(f"Unwanted transient or duplicate archive: {path.relative_to(ROOT)}")

if errors:
    for error in errors:
        print("FAIL:", error, file=sys.stderr)
    raise SystemExit(1)
print(f"PASS: {len(seen)} V6 files, SHA-256, OOXML/PDF structure, links and inventory")
