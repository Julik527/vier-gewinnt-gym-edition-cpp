#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   scripts/verify_windows_release.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Überprüft die zusammengestellte Windows-Veröffentlichung.
# ------------------------------------------------------------

"""Validate the *contents* of an assembled portable Windows game ZIP.

Designed for Linux/Windows CI and end-user V4 release candidates. No
graphics or audio hardware is needed: this verifies the actual EXE, source
commit ID, archive integrity and every OGG checksum when assets are present.
"""
from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import re
import runpy
import zipfile

ROOT = Path(__file__).resolve().parents[1]
V4 = runpy.run_path(str(ROOT / "scripts/install-gym-phonk-v4.py"))["EXPECTED"]
ORIGINAL = runpy.run_path(str(ROOT / "scripts/install-sci-fi-audio.py"))["HASHES"]


def verify_archive(path: Path, mode: str, require_v4: bool = False) -> dict[str, str]:
    exe = "vier-gewinnt-gui.exe" if mode == "2d" else "vier-gewinnt-3d.exe"
    mandatory = {exe, "START_HIER.txt", "AUDIO_PRUEFEN.cmd", "VERSION.txt"}
    prefix = "assets/audio/sci-fi/"
    with zipfile.ZipFile(path) as archive:
        entries = archive.namelist()
        if len(entries) != len(set(entries)):
            raise ValueError("Doppelte ZIP-Pfade")
        file_names = {name for name in entries if not name.endswith("/")}
        if not mandatory.issubset(file_names):
            raise ValueError(f"Pflichtdatei fehlt: {mandatory - file_names}")
        if archive.testzip() is not None:
            raise ValueError("ZIP-Beschädigung")
        with archive.open(exe) as binary:
            if binary.read(2) != b"MZ":
                raise ValueError("Keine Windows-PE-EXE im ZIP")
        commit = archive.read("VERSION.txt").decode("utf-8").strip()
        if not re.fullmatch(r"[0-9a-f]{40}", commit):
            raise ValueError("VERSION.txt muss eine echte Git-Commit-SHA enthalten")
        audio = {p[len(prefix):] for p in file_names
                 if p.startswith(prefix) and p.endswith(".ogg")}
        marker = prefix + "PROFILE.txt"
        profile = ("gym-phonk-v4" if marker in file_names
                   and archive.read(marker).decode("ascii").strip()
                   == "JK_AXIS_GYM_PHONK_V4" else "original")
        if marker in file_names and profile == "original":
            raise ValueError("Unbekannte Audio-Profilkennung")
        if require_v4 and (profile != "gym-phonk-v4" or audio != set(V4)):
            raise ValueError("Gefordertes Gym Phonk V4 Paket nicht enthalten")
        if audio:
            hashes = V4 if profile == "gym-phonk-v4" else ORIGINAL
            if audio != set(hashes):
                raise ValueError("Unvollständige oder unbekannte OGG-Dateiliste")
            for name, sha in hashes.items():
                digest = hashlib.sha256(archive.read(prefix + name)).hexdigest()
                if digest != sha:
                    raise ValueError(f"OGG-SHA-256-Fehler: {name}")
        elif marker in file_names:
            raise ValueError("Profilkennung ohne Musikdateien")
        elif require_v4:
            raise ValueError("V4-Musik fehlt")
    return {"mode": mode, "commit": commit, "audio": profile if audio else "none"}


def main() -> None:
    parser = argparse.ArgumentParser(description="Verify Windows release ZIP")
    parser.add_argument("--zip", type=Path, required=True)
    parser.add_argument("--mode", choices=("2d", "3d"), required=True)
    parser.add_argument("--require-v4", action="store_true")
    args = parser.parse_args()
    report = verify_archive(args.zip, args.mode, args.require_v4)
    print("PASS | portable Windows ZIP | " +
          " | ".join(f"{k}={v}" for k, v in report.items()))


if __name__ == "__main__":
    main()
