#!/usr/bin/env python3
"""Offline regression tests for the third-party audio installer."""
from __future__ import annotations
import hashlib
import importlib.util
from pathlib import Path
import tempfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "audio_installer", ROOT / "scripts/install-sci-fi-audio.py")
assert spec is not None and spec.loader is not None
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


def rejects(fn) -> None:
    try:
        fn()
    except (ValueError, FileNotFoundError, zipfile.BadZipFile):
        return
    raise AssertionError("Malformed archive unexpectedly accepted")


def write(path: Path, files: dict[str, bytes]) -> None:
    with zipfile.ZipFile(path, "w") as out:
        for name, content in files.items():
            out.writestr("ogg/" + name, content)


def main() -> None:
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        files = {name: b"OggS" + name.encode("ascii") * 9
                 for name in module.HASHES}
        expected = {name: hashlib.sha256(content).hexdigest()
                    for name, content in files.items()}
        good = root / "good.zip"
        write(good, files)
        target = root / "installed"
        module.install(good, target, expected)
        module.verify(target, expected)
        assert len(list(target.glob("*.ogg"))) == 8
        # Downgrading from Gym Phonk V4 must remove the profile marker.
        (target / "PROFILE.txt").write_text("JK_AXIS_GYM_PHONK_V4\\n",
                                            encoding="ascii")
        module.install(good, target, expected)
        assert not (target / "PROFILE.txt").exists()
        module.verify(target, expected)

        bad = root / "bad.zip"
        changed = dict(files)
        changed["victory.ogg"] = b"OggSdamaged"
        write(bad, changed)
        before = (target / "victory.ogg").read_bytes()
        rejects(lambda: module.install(bad, target, expected))
        assert (target / "victory.ogg").read_bytes() == before

        incomplete = root / "incomplete.zip"
        write(incomplete, {name: data for name, data in files.items()
                           if name != "hover.ogg"})
        rejects(lambda: module.install(incomplete, target, expected))

        unsafe = root / "unsafe.zip"
        with zipfile.ZipFile(unsafe, "w") as out:
            for name, data in files.items():
                out.writestr("ogg/" + name, data)
            out.writestr("../untrusted.ogg", b"OggS")
        rejects(lambda: module.install(unsafe, target, expected))
        assert not (root / "untrusted.ogg").exists()

        (target / "pulse.ogg").write_bytes(b"bad")
        rejects(lambda: module.verify(target, expected))

    print("CC0 audio installer: validated checksums, missing files, unsafe paths.")


if __name__ == "__main__":
    main()
