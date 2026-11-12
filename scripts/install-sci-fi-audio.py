#!/usr/bin/env python3
"""Safely install the eight user-supplied SRG774 CC0 OGG files from ogg.zip."""
from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import tempfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
DESTINATION = ROOT / "assets" / "audio" / "sci-fi"
HASHES = {
    "airy.ogg": "ce86ac7d78b0b3257b5242fa7de4fb378616a7a85cb4ea4a17b5a74f76a7d003",
    "hover.ogg": "2dc56ac3f497d0cf16135ba0260a7bbd98557c4323cac37d528f9622279f31ee",
    "pulse.ogg": "6350f0e1ab342c0d358346094334dfb74ed842abda33d8e1653336c13dd530ae",
    "sector.ogg": "d5862627e6d0c424fc157eaffaa5421bae4f0bb177e7628d520bfb6a45aa9676",
    "title.ogg": "b74814f52e99a59d1dce05e2340b51be6b0818e14e21e0c0b8dfdb1034141c5c",
    "transmission.ogg": "5749841250bf836795f9f83334dc46bc1debf5cf13700a48ab3a873662f64d7e",
    "urgent.ogg": "f8c36b37cac782f0d2b23b52b2c2736af3a748360b829212b415c07fc5d504e5",
    "victory.ogg": "9acde356c84b1d326a3e7b813e43e8849596a285796144f7fabed898f2c21007",
}


def checksum(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read_archive(archive: Path, expected: dict[str, str] = HASHES) -> dict[str, bytes]:
    files: dict[str, bytes] = {}
    with zipfile.ZipFile(archive) as packed:
        for entry in packed.infolist():
            if entry.is_dir():
                if entry.filename != "ogg/":
                    raise ValueError(f"Unbekannter ZIP-Ordner: {entry.filename}")
                continue
            if not entry.filename.startswith("ogg/"):
                raise ValueError(f"Unsicherer ZIP-Pfad: {entry.filename}")
            name = entry.filename[4:]
            if name not in expected or "/" in name or "\\" in name:
                raise ValueError(f"Nicht erlaubte Datei: {entry.filename}")
            if name in files or entry.file_size > 10_000_000:
                raise ValueError(f"Doppelte oder zu grosse Datei: {name}")
            data = packed.read(entry)
            if not data.startswith(b"OggS") or checksum(data) != expected[name]:
                raise ValueError(f"OGG/SHA256 stimmt nicht: {name}")
            files[name] = data
    if set(files) != set(expected):
        raise ValueError("ZIP ist unvollstaendig: Es werden acht OGG-Dateien erwartet")
    return files


def verify(destination: Path, expected: dict[str, str] = HASHES) -> None:
    for name, digest in expected.items():
        path = destination / name
        if not path.is_file() or checksum(path.read_bytes()) != digest:
            raise ValueError(f"Installierte Musikdatei fehlt/ist defekt: {name}")


def install(archive: Path, destination: Path,
            expected: dict[str, str] = HASHES) -> None:
    files = read_archive(archive, expected)
    destination.parent.mkdir(parents=True, exist_ok=True)
    # Validate the entire new folder before switching the existing source.
    # This removes a stale V4 marker when switching back to the CC0 originals.
    with tempfile.TemporaryDirectory(prefix=".staged-audio-",
                                     dir=destination.parent) as tmp:
        staging = Path(tmp) / "new"
        staging.mkdir()
        for name, content in files.items():
            (staging / name).write_bytes(content)
        verify(staging, expected)
        backup = Path(tmp) / "old"
        existed = destination.exists()
        if existed:
            if not destination.is_dir() or destination.is_symlink():
                raise ValueError("Ziel muss ein normaler Ordner sein.")
            destination.rename(backup)
        try:
            staging.rename(destination)
        except Exception:
            if existed:
                backup.rename(destination)
            raise
    verify(destination, expected)


def main() -> None:
    parser = argparse.ArgumentParser(description="Install CC0 game music from ogg.zip")
    parser.add_argument("--zip", type=Path, default=ROOT / "ogg.zip")
    parser.add_argument("--destination", type=Path, default=DESTINATION)
    parser.add_argument("--verify", action="store_true")
    args = parser.parse_args()
    if args.verify:
        verify(args.destination)
    else:
        install(args.zip, args.destination)
    print(f"OK: 8 CC0-OGG-Dateien SHA256-verifiziert in {args.destination}")


if __name__ == "__main__":
    main()
