# SRG774 Sci-Fi-Soundtrack in der Gym Edition

**Quelle:** [Dark Sci-Fi Audio Pack](https://opengameart.org/content/dark-sci-fi-audio-pack), SRG774, **CC0 1.0 Universal**.
Die Lizenz erlaubt Kopieren, Bearbeiten und Verteilen, auch für kommerzielle Zwecke. Das Urheberrecht am eigenständigen Quellcode wird dadurch nicht verändert.

## Tatsächlich geprüfte Quelldateien

Der Nutzer hat `ogg.zip` mit acht Dateien hochgeladen. Jede Datei ist OGG/Vorbis, 44,1 kHz, Stereo. Der Installer prüft acht feste SHA-256-Summen und akzeptiert ausschließlich `ogg/<name>.ogg`. Manipulierte, fehlende oder unerwartete Dateien werden vor dem Schreiben abgewiesen.

Die `*.ogg`-Dateien liegen **nicht im Git-Repository**. Zum Aktivieren das Originalarchiv in den Repository-Stammordner legen und ausführen:

```powershell
python scripts/install-sci-fi-audio.py --zip ogg.zip
python scripts/install-sci-fi-audio.py --verify
```

Oder das vorbereitete `JK_AXIS_SciFi_Audio_Ready.zip` im Stammordner entpacken. Danach die GUI wie gewohnt per CMake/VS Code starten. Die Windows-Paketierung nimmt installierte und validierte OGG-Dateien mit. Bei einem öffentlichen CI-Build ohne installierte Drittanbieterdateien enthält das Programm weiterhin nur den prozeduralen Fallback.

## Szenenzuordnung

| Bildschirm | Stück | Messwert | Pegelfaktor |
|---|---|---:|---:|
| 3D-Intro | `airy.ogg` | ca. −13,5 LUFS | 0,48 |
| Hauptmenü | `title.ogg` | ca. −19,8 LUFS | 0,86 |
| Setup, Anleitung, Bestenliste | `sector.ogg` | ca. −12,0 LUFS | 0,35 |
| Laufendes Spiel | `pulse.ogg` | ca. −18,5 LUFS | 0,78 |
| Spielfeld mindestens 75 % belegt | `urgent.ogg` | ca. −19,6 LUFS | 0,85 |
| Ergebnis | `transmission.ogg` | ca. −13,1 LUFS | 0,45 |
| Button-Effekt | `hover.ogg` | ca. −12,3 LUFS | 0,34 |
| Sieg-Effekt | `victory.ogg` | ca. −12,9 LUFS | 0,46 |

Die Lautheitsangaben sind analysierte Eingangswerte, nicht eine Abnahme des tatsächlichen Lautsprecherschalldrucks. Die Pegelfaktoren gelten zusätzlich zur vom Benutzer gesetzten Lautstärke.

## Implementierung und Grenzen

Die Musik wird mittels SFML 3.1 `sf::Music` gestreamt. Es werden zwei Streams für 420-ms-Crossfades verwendet, sowohl bei Szenenwechseln als auch kurz vor dem Ende eines Tracks. Menü- und Siegsounds werden als `sf::Sound` abgespielt. F9 schaltet Audio ein/aus, F10/F11 regeln den Pegel in Zehnerschritten. Start ist weiterhin stumm, damit die bisherigen UI-Einstellungen eingehalten werden.

Ohne OGG-Dateien oder mit fehlenden einzelnen Quellen fällt das Spiel auf das ursprüngliche `GymBeat`-Thema zurück beziehungsweise verwendet einen noch spielbaren Track. Ohne Ausgabegerät bleibt das Programm stumm und benutzbar. Automatische Tests prüfen Übersetzung, Funktionen ohne Audioausgabe, Installationssicherheit und Szenenlogik. Ein **echter Hörtest** mit Audiohardware und unterschiedlicher Lautstärke ist weiterhin erforderlich.

Es gibt keine Garantie für samplegenaue musikalische Übergänge zwischen unterschiedlich komponierten Songs; das 420-ms-Fading verhindert jedoch harte Lautstärkesprünge.
