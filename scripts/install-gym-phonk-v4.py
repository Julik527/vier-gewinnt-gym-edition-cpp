#!/usr/bin/env python3
"""Reproducible, checksum-locked installation of JK AXIS Gym Phonk V4 OGG.

Only accepts the eight reviewed OGGs from the user's original soundtrack ZIP.
The archive itself is distributed separately and never silently committed.
"""
from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import shutil
import tempfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
DEST = ROOT / "assets/audio/sci-fi"
PROFILE = "JK_AXIS_GYM_PHONK_V4"
EXPECTED = {
    "airy.ogg": "a0f93fa05973c97b0c9f7c81c50e271ea741f2f94e261a8e22211ba04394f1fc",
    "hover.ogg": "2dc56ac3f497d0cf16135ba0260a7bbd98557c4323cac37d528f9622279f31ee",
    "pulse.ogg": "6542fd4c9767a3cb1b6859db58debe456b3ba283bf9cd945a84e5a168459c81b",
    "sector.ogg": "c731fb3c035135fb5c3c2db00b24daadf6e51b84c687a7c2fbe7f450000f302e",
    "title.ogg": "1c5ddcefe3990e68077c2bec5b23e0037acc04e991a0eef6f6a2058a7daeb66a",
    "transmission.ogg": "57b6257f178475cd532a19d62b49b73d9837af9f794e54f91a64be666cc75009",
    "urgent.ogg": "572d9638075e57cff9c5952371a8f9d02356641ff45322fe93869dc2e1a4016a",
    "victory.ogg": "9acde356c84b1d326a3e7b813e43e8849596a285796144f7fabed898f2c21007",
}
PREFIX = "assets/audio/sci-fi/"


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def verify_folder(folder: Path, expected: dict[str, str] = EXPECTED) -> None:
    if (folder / "PROFILE.txt").read_text(encoding="ascii").strip() != PROFILE:
        raise ValueError("V4-Profilkennung fehlt oder ist falsch.")
    found = {p.name for p in folder.glob("*.ogg")}
    if found != set(expected):
        raise ValueError(f"OGG-Dateiliste stimmt nicht: {sorted(found ^ set(expected))}")
    for name, digest in expected.items():
        path = folder / name
        if not path.is_file() or sha256(path.read_bytes()) != digest:
            raise ValueError(f"OGG-Checksumme falsch: {name}")


def load_archive(path: Path, expected: dict[str, str] = EXPECTED) -> dict[str, bytes]:
    tracks: dict[str, bytes] = {}
    with zipfile.ZipFile(path, "r") as archive:
        for item in archive.infolist():
            name = item.filename
            if item.is_dir():
                if name != "assets/" and name != "assets/audio/" and name != PREFIX:
                    raise ValueError(f"Nicht erlaubter Ordner: {name}")
                continue
            if name == "LIZENZ_UND_HINWEISE.txt":
                continue
            if not name.startswith(PREFIX):
                raise ValueError(f"Unbekannter ZIP-Eintrag: {name}")
            track = name[len(PREFIX):]
            if track not in expected or track in tracks or item.file_size > 10_000_000:
                raise ValueError(f"Unbekannte/doppelte/uebergrosse Datei: {name}")
            data = archive.read(item)
            if len(data) < 256 or not data.startswith(b"OggS"):
                raise ValueError(f"Keine gueltige OGG-Kopfkennung: {track}")
            if sha256(data) != expected[track]:
                raise ValueError(f"Abweichender SHA-256-Wert: {track}")
            tracks[track] = data
    if set(tracks) != set(expected):
        raise ValueError("Das Gym-Phonk-V4-Paket muss acht OGG-Dateien enthalten.")
    return tracks


def install(archive: Path, dest: Path = DEST,
            expected: dict[str, str] = EXPECTED) -> None:
    tracks = load_archive(archive, expected)
    dest.parent.mkdir(parents=True, exist_ok=True)
    # Validate a complete staged directory before changing an existing pack.
    with tempfile.TemporaryDirectory(prefix=".jk-audio-", dir=dest.parent) as temp:
        staging = Path(temp) / "new"
        staging.mkdir()
        for name, content in tracks.items():
            (staging / name).write_bytes(content)
        (staging / "PROFILE.txt").write_text(PROFILE + "\n", encoding="ascii")
        verify_folder(staging, expected)

        backup = Path(temp) / "previous"
        existed = dest.exists()
        if existed:
            if not dest.is_dir() or dest.is_symlink():
                raise ValueError("Zielpfad ist kein normaler Ordner.")
            dest.rename(backup)
        try:
            staging.rename(dest)
        except Exception:
            if existed:
                backup.rename(dest)
            raise
    verify_folder(dest, expected)


def main() -> None:
    parser = argparse.ArgumentParser(description="Install or verify Gym Phonk V4")
    parser.add_argument("--zip", type=Path, default=ROOT / "JK_AXIS_Gym_Phonk_OGG_Soundtrack_V4.zip")
    parser.add_argument("--destination", type=Path, default=DEST)
    parser.add_argument("--verify", action="store_true")
    args = parser.parse_args()
    if args.verify:
        verify_folder(args.destination)
    else:
        install(args.zip, args.destination)
    print(f"OK: acht gepruefte Gym Phonk V4 OGG-Dateien in {args.destination}")


if __name__ == "__main__":
    main()
