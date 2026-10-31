# JK AXIS | Gym Edition 3D Signature Arena

**Stand:** 31.10.2026 · **Art:** freiwillige C++17-Erweiterung zur Pflicht-Konsole.

## Technik und Architektur

Die Datei `src/gui/Main3D.cpp` erzeugt ein echtes räumliches Spielfeld mit OpenGL: beleuchtete 3D-Zylinder als Spielsteine, einen kubischen Rahmen, Leuchtringe, eine Raumplattform, perspektivische Projektion und einen Tiefenpuffer. Die Kamera bewegt sich um das Spielfeld. Die 2D-Overlays und Bedienelemente werden separat mit SFML 3.1 gezeichnet. Das Intro ist nach vier Sekunden vorbei oder sofort überspringbar. Der Original-Soundtrack (116 BPM) bleibt standardmäßig ausgeschaltet.

`src/gui/Gym3DMath.h` implementiert das prüfbare Vektor- und Kameramodell: Kreuz- und Skalarprodukt, Kameraachsen, Perspektive, Zellkoordinaten, Kameragrenzen und zeitbasierte Animation. `tests/scene3d_tests.cpp` kontrolliert dies unabhängig von GPU und SFML.

Spieler, 7×6/9×8/11×10-Feldgrößen, Schwerkraft, Gewinn, Undo, Revanche, Siegerpunkte und das Format `GYM1` kommen unverändert aus der bestehenden `MatchSession` und `Board`. Die Konsole bleibt ein eigenständiges Schulprojekt. 2D und 3D sollten **nicht gleichzeitig dieselbe Speicherdatei beschreiben**.

## Bedienung

| Bedienung | Funktion |
|---|---|
| Maus links auf Spielbrett | Spalte auswählen und Stein einwerfen |
| Maus rechts ziehen | Kamera drehen/neigen |
| Mausrad | Zoom |
| A/D oder Q/E | Horizontal/vertikal drehen |
| C | Kamera zurücksetzen |
| Pfeiltasten + Leertaste/Enter | Spalte wählen + Stein setzen |
| U / S | Letzten Zug zurücknehmen / Spiel speichern |
| F9 / F10 / F11 | Musik starten/pausieren / leiser / lauter |
| Escape | Zurück zum Menü |
| Enter nach einem Sieg | Revanche |
| 2 / 3 / 4 im Menü | Spieleranzahl wählen |

**Aktuelle Grenzen:** Die Eingabe *neuer* Spielernamen unterstützt derzeit nur druckbare ASCII-Zeichen. Ein vorhandener GYM1-Spielstand mit anderen Namen kann geladen werden. Ein echter GPU-Treiber mit Unterstützung für klassische OpenGL-Zeichenfunktionen wird benötigt. Es gibt kein Raytracing, keine moderne Shader-/PBR-Pipeline, keine Echtzeit-Schattenkarten und keine Computergegner. Das Logo ist aus dem bestehenden Design abgeleitet, nicht als neue Marke gestaltet.

## Windows-Start

In VS Code **Datei → Ordner öffnen → `C:\Dev\GymEdition`** auswählen, nicht bloß im alten Fenster `cd` ausführen. Danach:

```powershell
git pull --ff-only
powershell.exe -NoProfile -ExecutionPolicy Bypass -File ".\scripts\preflight-school.ps1" -With3D
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
& .\build-school-3d-preflight\vier-gewinnt-3d.exe
```

Der erste Build lädt SFML 3.1.0; eine Internetverbindung ist dann nötig. Bei grafischen Treiberproblemen immer die stabile Konsolenversion als Rückfallmöglichkeit aufbewahren.

## Prüfung und Schulpriorität

Der 3D-Code erhält getrennte Geometrie-/Kamera-Unit-Tests, GUI-Headless-Smoketests und Windows-/Linux-/UCRT64-Builds. Die interaktive Prüfung von Animation, Grafik, Audio, DPI, Beamer und vollständigen Spielpartien muss auf dem echten Rechner erfolgen. Siehe [3D-Testplan](3D_TESTPLAN.md).

**Aufgabenblatt 51:** Die Schule fordert eine objektorientierte **Konsolenversion** mit persönlicher Codeerklärung. Die 3D-Arena ist ein freiwilliger Bonus, kein Ersatz für diese Bewertung.
