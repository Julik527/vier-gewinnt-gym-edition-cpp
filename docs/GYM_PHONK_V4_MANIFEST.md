# JK AXIS Gym Phonk V4 | geprüfter OGG-Medienstand

**Profilkennung:** `JK_AXIS_GYM_PHONK_V4`
**Status:** Audio-Paket separat; Quellcode auf GitHub, keine OGG-Binärdateien in Git.
**Herkunft:** Benutzerbereitgestellte V4-Neuarrangements auf Grundlage der CC0-OGG-Dateien des Pakets
[Dark Sci-Fi Audio Pack](https://opengameart.org/content/dark-sci-fi-audio-pack) von SRG774.
**Format:** Ogg Vorbis, Stereo, 44.100 Hz. Für die Schul-/Lizenzprüfung Originalquelle und Bearbeitung getrennt angeben.

## Festgelegte Dateiprüfsummen SHA-256

```text
a0f93fa05973c97b0c9f7c81c50e271ea741f2f94e261a8e22211ba04394f1fc  airy.ogg
2dc56ac3f497d0cf16135ba0260a7bbd98557c4323cac37d528f9622279f31ee  hover.ogg
6542fd4c9767a3cb1b6859db58debe456b3ba283bf9cd945a84e5a168459c81b  pulse.ogg
c731fb3c035135fb5c3c2db00b24daadf6e51b84c687a7c2fbe7f450000f302e  sector.ogg
1c5ddcefe3990e68077c2bec5b23e0037acc04e991a0eef6f6a2058a7daeb66a  title.ogg
57b6257f178475cd532a19d62b49b73d9837af9f794e54f91a64be666cc75009  transmission.ogg
572d9638075e57cff9c5952371a8f9d02356641ff45322fe93869dc2e1a4016a  urgent.ogg
9acde356c84b1d326a3e7b813e43e8849596a285796144f7fabed898f2c21007  victory.ogg
```

Die Prüfsummen sind anhand des tatsächlich erzeugten
`JK_AXIS_Gym_Phonk_OGG_Soundtrack_V4.zip` ermittelt, nicht aus Platzhaltern.

## Installation und Verifikation (Windows / Python 3.11+)

Die V4-Soundtrack-ZIP in den Repository-Stammordner kopieren, dann:

```powershell
python scripts/install-gym-phonk-v4.py --zip JK_AXIS_Gym_Phonk_OGG_Soundtrack_V4.zip
python scripts/install-gym-phonk-v4.py --verify
```

Das Installationsskript liest nur die acht erwarteten Tracks, lehnt alle
anderen Archivpfade und manipulierte Daten ab, überprüft alle Dateien vor dem
Austausch und schreibt eine `PROFILE.txt`. Der Installer ersetzt vorhandene
Tracks erst nach erfolgreicher Prüfung. Bei einem Absturz während des
Dateisystemwechsels ist eine manuelle Wiederherstellung möglich; es wird
keine Garantie für absolute Stromausfallsicherheit gegeben.

Anschließend beide GUI-Versionen mit CMake neu konfigurieren/bauen. Bei
einem Windows-Release kopieren die PowerShell-Paketskripte die acht
verifizierten Dateien und die Profilkennung mit.

**Wichtig:** Die erfolgreiche CI ohne Drittanbieter-Audio prüft nur den
Quellcode und den Installer, nicht den Klang der V4-Windows-EXE auf dem
Schulrechner. Ein realer Hörtest und die Prüfung des Windows-Lautstärkemixers
bleiben erforderlich.
