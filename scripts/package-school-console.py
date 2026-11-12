#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   scripts/package-school-console.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Erzeugt und kontrolliert ein eigenständiges ZIP mit den Konsolen-Quelldateien.
# ------------------------------------------------------------

"""Create and verify a source-only, standalone console-school ZIP.

The allowlist intentionally excludes GUI/3D files, SFML and private save data.
Every source file comes from the existing project and is not silently rewritten.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[1]
PACKAGE_FILES = {
    "school/CMakeLists.txt": "CMakeLists.txt",
    "school/README.md": "README.md",
    "school/HANDOUT.md": "HANDOUT.md",
    "school/FACHGESPRAECH.md": "FACHGESPRAECH.md",
    "school/ABGABE_CHECKLISTE.md": "ABGABE_CHECKLISTE.md",
    "school/TESTPROTOKOLL.md": "TESTPROTOKOLL.md",
    "school/KI_NUTZUNG.md": "KI_NUTZUNG.md",
    "src/main.cpp": "src/main.cpp",
    "src/Board.cpp": "src/Board.cpp",
    "src/Game.cpp": "src/Game.cpp",
    "src/ConsoleUI.cpp": "src/ConsoleUI.cpp",
    "include/Board.h": "include/Board.h",
    "include/Game.h": "include/Game.h",
    "include/Player.h": "include/Player.h",
    "include/ConsoleUI.h": "include/ConsoleUI.h",
    "include/AtomicFile.h": "include/AtomicFile.h",
    "include/GameFileLimits.h": "include/GameFileLimits.h",
    "include/ScoreFile.h": "include/ScoreFile.h",
    "tests/board_tests.cpp": "tests/board_tests.cpp",
    "tests/board_property_tests.cpp": "tests/board_property_tests.cpp",
    "tests/console_ui_tests.cpp": "tests/console_ui_tests.cpp",
    "tests/score_file_tests.cpp": "tests/score_file_tests.cpp",
    "tests/integration_smoke.py": "tests/integration_smoke.py",
    "tests/integration_adversarial.py": "tests/integration_adversarial.py",
    "data/.gitkeep": "data/.gitkeep",
}
MANIFEST = "MANIFEST.json"
DATE = (2026, 11, 11, 0, 0, 0)


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def entries() -> dict[str, bytes]:
    collected: dict[str, bytes] = {}
    for source, destination in PACKAGE_FILES.items():
        path = ROOT / source
        if not path.is_file():
            raise FileNotFoundError(f"Abgabe-Quelldatei fehlt: {source}")
        if destination in collected:
            raise ValueError(f"Doppelter Archivpfad: {destination}")
        collected[destination] = path.read_bytes()
    return collected


def write_zip(destination: Path) -> None:
    files = entries()
    expected = {name: digest(content) for name, content in sorted(files.items())}
    manifest = {
        "format": "jk-axis-console-school-v1",
        "description": "C++17 source-only standalone console submission",
        "files_sha256": expected,
    }
    files[MANIFEST] = (json.dumps(manifest, ensure_ascii=False, indent=2, sort_keys=True) + "\n").encode("utf-8")
    destination.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(destination, "w") as archive:
        for name in sorted(files):
            meta = zipfile.ZipInfo(name, date_time=DATE)
            meta.compress_type = zipfile.ZIP_DEFLATED
            meta.external_attr = 0o644 << 16
            archive.writestr(meta, files[name], compress_type=zipfile.ZIP_DEFLATED, compresslevel=9)
    signature = digest(destination.read_bytes())
    destination.with_name(destination.name + ".sha256").write_text(
        f"{signature}  {destination.name}\n", encoding="ascii"
    )


def verify_zip(path: Path) -> None:
    with zipfile.ZipFile(path) as archive:
        names = archive.namelist()
        expected_names = set(PACKAGE_FILES.values()) | {MANIFEST}
        if set(names) != expected_names or len(names) != len(expected_names):
            raise ValueError("ZIP-Dateiliste passt nicht zur Konsolen-Allowlist")
        if archive.testzip() is not None:
            raise ValueError("ZIP-Prüfsumme defekt")
        manifest = json.loads(archive.read(MANIFEST))
        if manifest.get("format") != "jk-axis-console-school-v1":
            raise ValueError("Falsches Manifestformat")
        checksums = manifest.get("files_sha256")
        if not isinstance(checksums, dict) or set(checksums) != set(PACKAGE_FILES.values()):
            raise ValueError("Manifest enthält unvollständige oder zusätzliche Dateien")
        for name, expected in checksums.items():
            if digest(archive.read(name)) != expected:
                raise ValueError(f"Datei verändert: {name}")
    sha_path = path.with_name(path.name + ".sha256")
    if sha_path.is_file():
        expected_line = f"{digest(path.read_bytes())}  {path.name}"
        if sha_path.read_text(encoding="ascii").strip() != expected_line:
            raise ValueError("Externe ZIP-SHA256-Prüfsumme stimmt nicht")


def main() -> None:
    parser = argparse.ArgumentParser(description="Build or verify console-only school package")
    parser.add_argument("--output", type=Path, default=ROOT / "dist" / "Vier-Gewinnt-Schulabgabe-Konsole.zip")
    parser.add_argument("--verify", action="store_true", help="Prüft das bereits erstellte ZIP")
    args = parser.parse_args()
    path = args.output.resolve()
    if not args.verify:
        write_zip(path)
    verify_zip(path)
    print(f"OK: {path} | {len(PACKAGE_FILES)} Konsolen-Dateien + Manifest | SHA256 verifiziert")


if __name__ == "__main__":
    main()
