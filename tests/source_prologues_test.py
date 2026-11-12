#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/source_prologues_test.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Prüft Doxygen-Dateiköpfe und relative Pfadangaben aller C++-Dateien.
# ------------------------------------------------------------

"""Validate meaningful Doxygen prologues in all C++ project sources."""
from __future__ import annotations

from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
SOURCE_FOLDERS = ("include", "src", "tests", "uebungen")
REQUIRED = ("@brief", "@details", "@par Abhängigkeiten",
            "@par Wichtige Randbedingungen")


def check(path: Path) -> list[str]:
    relative = path.relative_to(ROOT).as_posix()
    content = path.read_text(encoding="utf-8-sig")
    errors: list[str] = []
    if not content.startswith("/**\n"):
        return [f"{relative}: Dateikopf beginnt nicht mit /**"]
    finish = content.find("*/")
    if finish < 0 or finish > 2500:
        return [f"{relative}: Doxygen-Dateikopf nicht korrekt beendet"]
    header = content[:finish]
    if f"@file {relative}" not in header:
        errors.append(f"{relative}: @file-Pfad fehlt oder stimmt nicht")
    for field in REQUIRED:
        match = re.search(r"^ \* " + re.escape(field) + r"(?:\s+[^\n]+)?$",
                          header, flags=re.MULTILINE)
        if not match:
            errors.append(f"{relative}: {field} fehlt")
            continue
        if match.group(0).rstrip().endswith(field):
            # For @par sections the description is on the next line.
            if field.startswith("@par"):
                rest = header[match.end():].splitlines()
                if len(rest) < 2 or not rest[1].lstrip().startswith("* "):
                    errors.append(f"{relative}: {field} ohne Text")
            else:
                errors.append(f"{relative}: {field} ohne Aussage")
    body = content[finish + 2:].strip()
    if not (body.startswith("#include") or body.startswith("#pragma")):
        errors.append(f"{relative}: nach Dateikopf fehlt die normale C++-Dateistruktur")
    return errors


def main() -> int:
    sources = sorted({
        file for folder in SOURCE_FOLDERS
        for extension in ("*.h", "*.hpp", "*.cpp")
        for file in (ROOT / folder).rglob(extension)
        if file.is_file()
    })
    if len(sources) < 36:
        print(f"FEHLER: nur {len(sources)} C++-Dateien gefunden")
        return 1
    problems = [problem for path in sources for problem in check(path)]
    if problems:
        print("\n".join(problems))
        return 1
    print(f"Doxygen-Dateiköpfe geprüft: {len(sources)}/{len(sources)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
