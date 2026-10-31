#!/usr/bin/env python3
"""Verify school logo is an actual JPEG with plausible dimensions (stdlib only).

Commons original: 2185x945, SHA1 71bc2f41e221b85e09907cedaeecb861b025b047
Fallback: resized copy of the same logo, not byte-identical to the Commons original.
"""
from __future__ import annotations
import hashlib
from pathlib import Path
import sys

ORIGINAL_SHA1 = "71bc2f41e221b85e09907cedaeecb861b025b047"
SOF = {0xC0, 0xC1, 0xC2, 0xC3, 0xC5, 0xC6, 0xC7, 0xC9, 0xCA, 0xCB, 0xCD, 0xCE, 0xCF}

def jpeg_dimensions(data: bytes) -> tuple[int, int]:
    if data[:3] != b"\xff\xd8\xff":
        raise ValueError("Not a JPEG file")
    index = 2
    while index + 4 < len(data):
        if data[index] != 0xFF:
            raise ValueError("Malformed JPEG marker")
        while index < len(data) and data[index] == 0xFF:
            index += 1
        marker = data[index]
        index += 1
        if marker in (0xD8, 0xD9, 0x01) or 0xD0 <= marker <= 0xD7:
            continue
        if index + 2 > len(data):
            break
        length = int.from_bytes(data[index:index + 2], "big")
        if length < 2 or index + length > len(data):
            raise ValueError("Invalid JPEG segment length")
        if marker in SOF:
            if length < 7:
                raise ValueError("Invalid SOF marker")
            height = int.from_bytes(data[index + 3:index + 5], "big")
            width = int.from_bytes(data[index + 5:index + 7], "big")
            return width, height
        index += length
    raise ValueError("Missing JPEG image dimensions")

def verify(file: Path) -> None:
    data = file.read_bytes()
    if len(data) < 10000 or len(data) > 5_000_000:
        raise ValueError("Image file size outside acceptable bounds")
    width, height = jpeg_dimensions(data)
    if width < 900 or height < 350 or not 2.0 <= width / height <= 3.0:
        raise ValueError(f"Unexpected logo dimensions {width}x{height}")
    sha = hashlib.sha1(data).hexdigest()
    provenance = "Wikimedia original (SHA1 verified)" if sha == ORIGINAL_SHA1 else "resized logo mirror"
    print(f"Schullogo OK: {file} ({width}x{height}, {provenance})")

def main() -> int:
    root = Path(sys.argv[1] if len(sys.argv) > 1 else "build-gui")
    files = list(root.rglob("assets/school/bernd-blindow-schulen.jpg"))
    if not files:
        print("Schullogo fehlt neben dem GUI-Binary.", file=sys.stderr)
        return 1
    try:
        for file in files:
            verify(file)
    except (OSError, ValueError) as error:
        print(f"Ungueltiges Schullogo: {error}", file=sys.stderr)
        return 1
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
