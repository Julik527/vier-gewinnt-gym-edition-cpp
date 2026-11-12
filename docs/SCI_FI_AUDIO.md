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


## Fehlerdiagnose: Es klingt weiterhin wie der alte Soundtrack

Ab dem Audio-Check V2 zeigen beide GUIs dauerhaft die **tatsächlich aktive Klangquelle**:
`SCI-FI AKTIV / SRG774`, `ORIGINAL 116 BPM AKTIV` oder `OGG 0/8 FEHLEN`.
Wenn ein vollständiges Pack vorhanden ist, aber nicht geladen werden kann,
wird **keine alte Musik mehr als vermeintlicher Sci-Fi-Sound abgespielt**.
Stattdessen erscheint ein klarer OGG-Fehler.

Im entpackten Windows-Paket `AUDIO_PRUEFEN.cmd` starten. Es kontrolliert
den tatsächlich laufenden EXE-Ordner, listet alle acht OGG-Dateien und
endet bei erfolgreicher Prüfung mit `SCI-FI BEREIT`. Alternativ:
```powershell
.\vier-gewinnt-gui.exe --audio-check
.\vier-gewinnt-3d.exe --audio-check
```
(Je nach Paket nur die vorhandene EXE aufrufen.)

Dies überprüft die Dateistruktur und OGG-Header ohne Audiogerät; den
echten Ton anschließend mit F9 testen. Wer die alte EXE aus einem
anderen Ordner startet, hört weiterhin die alte Komposition. Die
versionierten Komplettpakete müssen **vollständig entpackt** werden.

## Gym Phonk V4 (neues optionales Musikprofil)

V4 enthält acht eigene Gym-Phonk-Audioarrangements auf Basis des CC0-Materials.
Die acht OGG-Binärdateien werden separat verteilt. Für die reproduzierbare
Installation:

```powershell
python scripts/install-gym-phonk-v4.py --zip JK_AXIS_Gym_Phonk_OGG_Soundtrack_V4.zip
python scripts/install-gym-phonk-v4.py --verify
```

Die neue V4-Version ist über eine `PROFILE.txt` als solche erkennbar. Der
Spielstatus zeigt dann `GYM PHONK V4 AKTIV`, statt sie als unveränderten
SRG774-Sci-Fi-Soundtrack auszugeben. Der V4-Installer überprüft alle acht
SHA-256-Werte; beide Windows-Paketskripte unterstützen dieses Profil.

Die Lautheitsfaktoren wurden aus dem V4-Audiosignal separat bestimmt, damit
Titel, Spiel und Endphase bei gleicher Benutzereinstellung ausgeglichen klingen.
Ein tatsächlich durchgeführter Hörtest bleibt erforderlich. Siehe
[Gym Phonk V4 Manifest](GYM_PHONK_V4_MANIFEST.md).

## Änderung: automatische Sci-Fi-Musik und kein altes Audiothema (V3)

Wenn alle acht SRG774-OGG-Dateien am Startort gefunden werden, startet die Musik beim
normalen Öffnen der 2D-/3D-GUI **automatisch**. F9 pausiert/aktiviert sie weiterhin.
Die ältere prozedurale Musik ist in dieser Audio-Variante absichtlich nicht mehr
als Fallback vorgesehen. Fehlende Dateien, Decoderfehler oder ein fehlendes
Audiogerät führen zu einer **sichtbaren Fehlermeldung** und stillem, weiterhin
spielbarem Zustand. CI-`--window-smoke` startet weiterhin stumm. Die
Konsolenversion enthält keine Musik und ist unverändert.

Die ZIP-Dateien müssen vollständig entpackt werden. Im jeweiligen Spielordner
`AUDIO_PRUEFEN.cmd` starten: Nur `8/8` weist nach, dass alle OGG-Header
vorhanden sind. Ein separater Hörtest mit Windows-Lautsprecher und Windows-
Lautstärkemixer bleibt notwendig.
