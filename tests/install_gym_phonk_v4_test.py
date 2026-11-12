#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/install_gym_phonk_v4_test.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Testet die Installation des optionalen Gym-Phonk-Audiopakets.
# ------------------------------------------------------------

"""Offline archive verification, corruption and rollback regression tests."""
from __future__ import annotations
import hashlib
import importlib.util
from pathlib import Path
import tempfile
import zipfile

SCRIPT = Path(__file__).resolve().parents[1] / "scripts/install-gym-phonk-v4.py"
spec = importlib.util.spec_from_file_location("gym_phonk_installer", SCRIPT)
assert spec is not None and spec.loader is not None
installer = importlib.util.module_from_spec(spec)
spec.loader.exec_module(installer)


def rejects(action, reason: str) -> None:
    try:
        action()
    except (ValueError, zipfile.BadZipFile):
        return
    raise AssertionError(reason)


def make_zip(path, tracks, extra=None):
    with zipfile.ZipFile(path, "w") as archive:
        for name, data in tracks.items():
            archive.writestr("assets/audio/sci-fi/" + name, data)
        if extra is not None:
            archive.writestr(extra, b"OggS")


def run() -> None:
    with tempfile.TemporaryDirectory() as folder:
        root = Path(folder)
        original = {n: b"OggS" + (n.encode("ascii") * 90)
                    for n in installer.EXPECTED}
        checksums = {n: hashlib.sha256(v).hexdigest() for n, v in original.items()}
        ok = root / "ok.zip"
        target = root / "assets/audio/sci-fi"
        make_zip(ok, original)
        installer.install(ok, target, checksums)
        installer.verify_folder(target, checksums)
        assert (target / "PROFILE.txt").read_text().strip() == installer.PROFILE

        corrupted = dict(original)
        corrupted["urgent.ogg"] = b"OggS" + b"manipulated" * 40
        bad = root / "bad.zip"
        make_zip(bad, corrupted)
        old_sha = hashlib.sha256((target / "urgent.ogg").read_bytes()).hexdigest()
        rejects(lambda: installer.install(bad, target, checksums), "corrupt accepted")
        assert hashlib.sha256((target / "urgent.ogg").read_bytes()).hexdigest() == old_sha

        missing = dict(original)
        del missing["title.ogg"]
        incomplete = root / "missing.zip"
        make_zip(incomplete, missing)
        rejects(lambda: installer.install(incomplete, target, checksums), "incomplete accepted")

        unsafe = root / "unsafe.zip"
        make_zip(unsafe, original, "../outside.ogg")
        rejects(lambda: installer.install(unsafe, target, checksums), "unsafe path accepted")
        assert not (root / "outside.ogg").exists()

        (target / "title.ogg").write_bytes(b"modified")
        rejects(lambda: installer.verify_folder(target, checksums), "tampering accepted")
    print("Gym Phonk V4 archive and installation regression tests: PASS")


if __name__ == "__main__":
    run()
