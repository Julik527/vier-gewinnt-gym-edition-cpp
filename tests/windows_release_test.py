#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/windows_release_test.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Testet Prüfungen und Inhalte des Windows-Releasepakets.
# ------------------------------------------------------------

"""Portable ZIP negative tests without requiring a GUI or actual EXE."""
from __future__ import annotations

import hashlib
import importlib.util
from pathlib import Path
import tempfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "release_verifier", ROOT / "scripts/verify_windows_release.py")
assert spec is not None and spec.loader is not None
check = importlib.util.module_from_spec(spec)
spec.loader.exec_module(check)


def reject(action):
    try:
        action()
    except (ValueError, zipfile.BadZipFile):
        return
    raise AssertionError("Defektes Windows-Release-ZIP angenommen")


def zipfiles(path: Path, files: dict[str, bytes]):
    with zipfile.ZipFile(path, "w") as archive:
        for name, content in files.items():
            archive.writestr(name, content)


def main():
    with tempfile.TemporaryDirectory() as tmp:
        folder = Path(tmp)
        files = {
            "vier-gewinnt-gui.exe": b"MZ" + bytes(100),
            "START_HIER.txt": b"Start",
            "AUDIO_PRUEFEN.cmd": b"echo check",
            "VERSION.txt": b"a" * 40 + b"\n",
        }
        ok = folder / "2d.zip"
        zipfiles(ok, files)
        assert check.verify_archive(ok, "2d")["audio"] == "none"
        reject(lambda: check.verify_archive(ok, "2d", require_v4=True))
        reject(lambda: check.verify_archive(ok, "3d"))

        corrupt = dict(files)
        corrupt["VERSION.txt"] = b"NOT_A_COMMIT"
        bad = folder / "bad-commit.zip"
        zipfiles(bad, corrupt)
        reject(lambda: check.verify_archive(bad, "2d"))

        fake = dict(files)
        fake["vier-gewinnt-gui.exe"] = b"FAKE"
        notexe = folder / "fake.zip"
        zipfiles(notexe, fake)
        reject(lambda: check.verify_archive(notexe, "2d"))

        tampered = dict(files)
        tampered["assets/audio/sci-fi/PROFILE.txt"] = b"JK_AXIS_GYM_PHONK_V4\n"
        tampered["assets/audio/sci-fi/title.ogg"] = b"OggScorrupt"
        broken_audio = folder / "broken-audio.zip"
        zipfiles(broken_audio, tampered)
        reject(lambda: check.verify_archive(broken_audio, "2d"))

    print("Windows Release ZIP verification: PASS")


if __name__ == "__main__":
    main()
