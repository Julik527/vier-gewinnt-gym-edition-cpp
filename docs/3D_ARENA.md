# JK AXIS | Gym Edition 3D Signature Arena

**Stand:** 31.10.2026 · **Art:** freiwillige C++17-Erweiterung zur Pflicht-Konsole.

## Technik und Architektur

Die Datei `src/gui/Main3D.cpp` erzeugt ein echtes räumliches Spielfeld mit OpenGL: beleuchtete 3D-Zylinder als Spielsteine, einen kubischen Rahmen, Leuchtringe, eine Raumplattform, perspektivische Projektion und einen Tiefenpuffer. Die Kamera bewegt sich um das Spielfeld. Die Mausauswahl verwendet einen perspektivischen Strahl vom Bildschirm auf die tatsächliche Spielfeld-Ebene; so berücksichtigt die Auswahl Kameradrehung und Höhe der angeklickten Zelle. Die 2D-Overlays und Bedienelemente werden separat mit SFML 3.1 gezeichnet. Das Intro ist nach vier Sekunden vorbei oder sofort überspringbar. Der Original-Soundtrack (116 BPM) bleibt standardmäßig ausgeschaltet.

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

**Aktuelle Grenzen:** Die Namenseingabe unterstützt UTF-8 (einschließlich deutscher Umlaute) bis zur bestehenden 24-Byte-Grenze. Beim Bearbeiten werden vollständige Unicode-Codepunkte entfernt. Ein vorhandener GYM1-Spielstand mit anderen Namen kann geladen werden. Ein echter GPU-Treiber mit Unterstützung für klassische OpenGL-Zeichenfunktionen wird benötigt. Es gibt kein Raytracing, keine moderne Shader-/PBR-Pipeline, keine Echtzeit-Schattenkarten und keine Computergegner. Das Logo ist aus dem bestehenden Design abgeleitet, nicht als neue Marke gestaltet.

## Windows-Start

In VS Code **Datei → Ordner öffnen → `C:\Dev\GymEdition`** auswählen, nicht bloß im alten Fenster `cd` ausführen. Danach:

```powershell
git pull --ff-only
powershell.exe -NoProfile -ExecutionPolicy Bypass -File ".\scripts\preflight-school.ps1" -With3D
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
& .\build-school-3d-preflight\vier-gewinnt-3d.exe
```

Der erste Build lädt SFML 3.1.0; eine Internetverbindung ist dann nötig. Bei grafischen Treiberproblemen immer die stabile Konsolenversion als Rückfallmöglichkeit aufbewahren.

## Verstehen und selbst erklären

Der [Lernpfad Teil D](3D_LERNPFAD.md) führt durch `Board`, `MatchSession`, 3D-Kameramathematik, OpenGL-Geometrie, Eingaben, Dateischutz und das Fachgespräch. Die 3D-Codeerklärung ergänzt, ersetzt aber nicht das schulisch verlangte Konsolenverständnis.

## Prüfung und Schulpriorität

Der 3D-Code erhält getrennte Geometrie-/Kamera-Unit-Tests, GUI-Headless-Smoketests und Windows-/Linux-/UCRT64-Builds. Die interaktive Prüfung von Animation, Grafik, Audio, DPI, Beamer und vollständigen Spielpartien muss auf dem echten Rechner erfolgen. Siehe [3D-Testplan](3D_TESTPLAN.md).

**Aufgabenblatt 51:** Die Schule fordert eine objektorientierte **Konsolenversion** mit persönlicher Codeerklärung. Die 3D-Arena ist ein freiwilliger Bonus, kein Ersatz für diese Bewertung.

## Qualitätsverbesserung: präziser Mauszeiger und datensicherer Ausstieg

Die 3D-Auswahl berechnet nun per Ray-Plane-Intersection die getroffene Spalte. Die Tests decken alle Rastergrößen, neun Kamerapositionen und Klicks außerhalb des Brettes ab. Beim Bewegen der Maus ohne gedrückte rechte Taste folgt die Spaltenvorschau dem Zeiger; ein Klick außerhalb des Brettes löst keinen Zug aus. Wird das Fenster deaktiviert, endet eine eventuell begonnene Kamera-Drag-Geste.

**Sichern und beenden** beendet die Anwendung nur nach erfolgreichem Schreiben. Scheitert das Speichern (zum Beispiel bei einem schreibgeschützten Verzeichnis), bleibt die Partie geöffnet und zeigt eine Fehlermeldung. Der Befehl **Ohne Sichern beenden** bleibt eine separate, ausdrücklich bezeichnete Entscheidung.
