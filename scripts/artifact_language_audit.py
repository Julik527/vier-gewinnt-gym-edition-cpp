#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   scripts/artifact_language_audit.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Prüft die Sprache und Formulierungen in Präsentations- und PDF-Artefakten.
# ------------------------------------------------------------

"""Audit PDFs and PowerPoint text without modifying original archives.

Current exam presentation must be free of promotional quality claims.
Historical files are reported only, so original project evidence remains intact.
"""
from pathlib import Path
import re
import sys

import fitz
from pptx import Presentation

ROOT = Path(__file__).resolve().parents[1]
PAT = re.compile(
    r"gold[\s-]*master|gold[\s-]*standard|goldstandard|"
    r"first[\s-]*class|\bdiamond\b|\bultimate\b|"
    r"10\s*(?:von|/)\s*10",
    re.IGNORECASE,
)
CURRENT = {
    "praesentation/Vier_Gewinnt_Meilenstein_10_Minuten.pdf",
    "praesentation/Vier_Gewinnt_Meilenstein_10_Minuten.pptx",
}

def pdf_text(path):
    with fitz.open(path) as doc:
        lines = []
        for i, page in enumerate(doc, 1):
            lines.extend((i, line) for line in page.get_text("text").splitlines())
        return lines

def pptx_text(path):
    items = []
    p = Presentation(path)
    for i, slide in enumerate(p.slides, 1):
        for shape in slide.shapes:
            if shape.has_text_frame:
                items.extend((i, x) for x in shape.text.splitlines())
        notes = slide.notes_slide.notes_text_frame
        if notes is not None:
            items.extend((i, x) for x in notes.text.splitlines())
    return items

def main():
    errors = []
    files = sorted([*ROOT.glob("reference/*.pdf"),
                    *ROOT.glob("praesentation/*.pdf"),
                    *ROOT.glob("praesentation/*.pptx")])
    for path in files:
        relative = path.relative_to(ROOT).as_posix()
        try:
            rows = pptx_text(path) if path.suffix == ".pptx" else pdf_text(path)
            hits = [(n, line.strip()[:110]) for n, line in rows if PAT.search(line)]
            status = "CURRENT" if relative in CURRENT else "HISTORICAL"
            print(f"{status}: {relative}: {len(hits)} promotional matches")
            if status == "CURRENT" and hits:
                errors.extend(f"{relative}:{n}: {v}" for n, v in hits)
            if status == "CURRENT" and len(rows) < 20:
                errors.append(f"{relative}: unexpectedly little extractable text")
        except Exception as exc:
            errors.append(f"{relative}: cannot inspect: {exc}")
    for relative in CURRENT:
        if not (ROOT / relative).is_file():
            errors.append(f"Missing current presentation: {relative}")
    if errors:
        print("\n".join(errors), file=sys.stderr)
        return 1
    print("Current PPTX/PDF wording verified; historical documents were not altered.")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
