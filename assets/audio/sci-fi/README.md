# JK AXIS | Dark Sci-Fi Audio Pack

**Audio:** SRG774, *Dark Sci-Fi Audio Pack*, CC0 1.0 Universal (Public Domain).
Source: https://opengameart.org/content/dark-sci-fi-audio-pack

The OGG binaries are installed locally from the supplied `ogg.zip` and are
**not stored in this Git repository**. The installer verifies all eight original
files against SHA-256 before changing anything.

```powershell
python scripts/install-sci-fi-audio.py --zip ogg.zip
python scripts/install-sci-fi-audio.py --verify
```

| Game state | Track |
|---|---|
| 3D intro | airy.ogg |
| Main menu | title.ogg |
| Name setup, help, scores | sector.ogg |
| Match | pulse.ogg |
| At least 75% board occupancy | urgent.ogg |
| Match result | transmission.ogg |
| Menu button | hover.ogg |
| Win | victory.ogg |

Press F9 to start/stop music; F10/F11 change volume. Streamed audio
crossfades 420 ms when the scene changes or a track reaches its end.
Individual gains compensate for differing source loudness.

If audio files are missing, the original synthesized 116-BPM soundtrack
plays. Windows packaging scripts copy installed OGG files into releases.
GitHub CI packages built without the OGG assets therefore contain only the
procedural fallback; do not claim otherwise.

The audio itself remains CC0, independently of the source-code license.
