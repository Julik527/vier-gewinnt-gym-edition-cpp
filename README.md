<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/brand/jk-axis-precision-symbol-inverse.svg">
    <source media="(prefers-color-scheme: light)" srcset="assets/brand/jk-axis-precision-symbol.svg">
    <img src="assets/brand/jk-axis-precision-symbol.svg" alt="JK AXIS" width="88" height="92">
  </picture>
</p>

<h1 align="center">Vier Gewinnt | Gym Edition</h1>

<p align="center"><strong>Ein C++17-Spiel mit Training-Arena, eigener 2D-Grafik und nachvollziehbarer Softwaretechnik.</strong></p>

<p align="center">
  <a href="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/cpp-quality.yml"><img src="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/cpp-quality.yml/badge.svg" alt="C++-Qualitätsprüfung"></a>
  <a href="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/docs-quality.yml"><img src="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/docs-quality.yml/badge.svg" alt="Dokumentationsprüfung"></a>
</p>

**ITA-Programmierprojekt 2026/27 · JK AXIS · C++17 · SFML 3.1.0 · CMake / CTest**

> **Versionsstatus (Integration 05.11.2026):** Diese Integrationsfassung vereint die bisherigen Konsolen-, SFML-2D- und OpenGL-3D-Quellen sowie zusätzliche automatisierte Tests. Die tatsächliche interaktive Funktion auf dem Schul-PC muss separat vorgeführt werden. Ein erfolgreicher CI-Lauf ersetzt keine manuelle Abnahme.

## Prüfung 19.11.2026: Theorie und zehnminütige Einzelpräsentation

Dr. Litera hat am 11.11.2026 per E-Mail angekündigt: **PT ca. 70 Minuten**, Themen `struct`/`class`, Getter/Setter, Konstruktor/Parameterkonstruktor, Destruktor, Dateioperationen, dynamischer Speicher, `this`-Zeiger und Header/Implementierung/`main`; **PP ca. 10 Minuten pro Schüler**. Die bislang vorhandenen 15+15-Minuten-Sprechkarten gehören zu einem anderen Format. Die aktualisierten Prüfungsunterlagen liegen hier:

- [Präziser C++-Lernzettel für PT](docs/PT_PRUEFUNG_19_11_2026.md)
- [Meilenstein-Präsentation: acht Abschnitte, Zielzeit 10:00](praesentation/MEILENSTEIN_10_MIN_2026-11-19.md)
- [Schulische Konsolenabgabe](school/README.md) (auf dem zugehörigen Schulabgabe-Branch)

**Wichtig:** Folienzeiten sind Planwerte, die tatsächliche Dauer muss laut mit Stoppuhr gemessen werden. Das vorhandene PowerPoint aus dem älteren Präsentationsformat wurde dadurch nicht stillschweigend ersetzt.

## VS Code und Merge-Konflikte

Die drei F5-Startprofile sind an ihre Build-Aufgaben gekoppelt:
`Vier Gewinnt bauen`, `Gym Edition 2D bauen` und
`Gym Edition 3D bauen`. Einzelheiten und Voraussetzungen:
[VS-Code-Debugging](docs/VS_CODE_DEBUG.md).

Bei einem Git-Merge-Konflikt darf `launch.json` oder `tasks.json` keine
Markierungen wie `<<<<<<<`, `=======` oder `>>>>>>>` enthalten.
Der Fehler liegt dann in der **nicht abgeschlossenen Git-Zusammenführung**,
nicht am JSON-Format selbst.

Für eine Lehrer-Abnahme stehen zusätzliche [Grenzfall- und
Regeltests](docs/LEHRER_ABNAHME_TESTPROTOKOLL_2026-11-05.md) sowie
[360 reproduzierbare Board-Prüffälle](tests/board_property_tests.cpp) zur
Verfügung. Die Highscore-Datei wird bei beschädigten Zeilen
**vollständig verworfen** (strenger Parser, keine teilweise Übernahme).

## Schulabgabe: ausschließlich C++-Konsole

**Verbindlicher Abgabeumfang:** das objektorientierte C++17-Konsolenspiel. Die SFML-2D- und OpenGL-3D-Versionen bleiben als freiwillige Entwicklungs-/Portfolio-Experimente im Repository, sind aber **nicht Bestandteil der Schulabgabe**. Das ist kein Beleg dafür, dass Grafikversionen ausdrücklich verboten sind; für die Bewertung hat die Konsolenpflicht Vorrang.

Die isolierte, eigenständig baubare Konsolen-Schulabgabe wird mit `python scripts/package-school-console.py` erstellt und lässt sich mit `python scripts/package-school-console.py --verify` prüfen. Enthalten sind ausschließlich die benötigten C++-Dateien, Konsolentests, CMake und schulbezogene Unterlagen; keine GUI-, OpenGL- oder Audiodateien. Der GitHub-Workflow `School Console Only | Build & Acceptance` soll **das ausgepackte ZIP** auf Windows und Linux in Debug/Release bauen und testen.

[Anleitung zur isolierten Schulabgabe](school/README.md) · [Handout nur für Konsole](school/HANDOUT.md) · [Fachgespräch](school/FACHGESPRAECH.md) · [Offene manuelle Prüfungen](school/TESTPROTOKOLL.md)

## Schulabgabe: Aufgabenblatt 51

Die fachlichen Pflichtpunkte richten sich nach **Dr. Michael Literas Aufgabenblatt 51 (Programmiertechnik 2026/2027)**. Die Konsolenversion hat Priorität vor zusätzlichen 2D-Effekten.

- [Handout für die Präsentation](docs/HANDOUT_AUFGABENBLATT_51.md)
- [Muss-Anforderungen mit Umsetzungs- und Abnahmestatus](docs/SCHULANFORDERUNGEN_51_STATUS.md)
- [Kanban, Review und Retrospektive vom 31.10.2026](docs/SPRINT_REVIEW_2026-10-31.md)
- [Generalprobe und 30-Minuten-Vorführung](docs/GENERALPROBE_2027.md)
- [Geprüfte, isolierte Vorführspielstände](tests/fixtures/)
- [Aktuelles KI-Promptregister](docs/KI_PROMPT_DOKUMENTATION.md)

**Achtung:** Eine erfolgreiche GitHub-CI ist kein Nachweis, dass die neueste GUI auf dem Schulrechner startet oder dass ein mündliches Fachgespräch bereits bestanden wurde. Manuelle Testfälle und Lehrkraftbewertungen bleiben offen.

## Windows-Finalprüfung und Schulabgabe

Die Konsolenfassung ist der Pflichtumfang des Aufgabenblatts 51. Auf Windows lässt sich der technische Stand unabhängig von der grafischen Ausgabe mit `scripts/preflight-school.ps1` automatisiert prüfen. Das Skript nutzt ein separates Build-Verzeichnis, testet die `GYM1`-Vorführdaten und verändert keine persönlichen Spielstände. Mit `-WithGui` wird zusätzlich SFML 3.1 gebaut.

Bei einem `ß` im Windows-Benutzerpfad die Anwendung **wirklich** in `C:\Dev\GymEdition` klonen, statt SFML im alten langen Pfad neu zu konfigurieren. [Anleitung und genaue Befehle](docs/WINDOWS_QUICKSTART.md).

Die Highscore-Zähler vermeiden jetzt auch bei einem gespeicherten Maximalwert von `INT_MAX` einen vorzeichenbehafteten Integer-Überlauf. Es handelt sich um Fehlerhärtung und nicht um eine zusätzliche Schulanforderung.

## Das Spiel

Mit **2 bis 4 Personen** setzt du Gym-Hanteln in ein Raster. Vier zusammenhängende Symbole gewinnen, horizontal, vertikal oder diagonal. Die Spielbreite passt sich der Spielerzahl an.

| Spieler | Feld | Besonderheiten |
|:--:|:--:|:--|
| 2 | 7 × 6 | Klassische Vier-Gewinnt-Partie |
| 3 | 9 × 8 | Größeres gemeinsames Feld |
| 4 | 11 × 10 | Vier Farben und Zugreihenfolge |

**Bereits implementiert:** Konsolenmodus und optionale SFML-2D-Version, Maus-/Tastatursteuerung, Fallanimation, Gewinnmarkierung, Undo, Vorschau (Konsole), Revanche, Highscores, Speichern/Laden, ein eigens komponierter 116-BPM-Stereo-Soundtrack (standardmäßig aus, F9/F10/F11).

**Wichtige Grenzen:** Kein KI-Gegner, kein Netzwerkspiel, keine externe 3D-Engine. Die GUI und die Konsole verwenden noch separate Ablauf-Controller; gemeinsam sind die Spielregeln im `Board` und die sichere Dateiersatzfunktion.

## Gym Edition 3D | Signature Arena

**Präzisionsupdate:** 3D-Mausklicks werden jetzt über eine perspektivische Strahl-Schnittberechnung auf das Spielbrett abgebildet. Hover-Auswahl, Schutz vor Navigation während der Fallanimation sowie ein sicherer „Sichern und beenden“-Ablauf sind ergänzt. Falls beim Gewinn die Highscore-Datei nicht geschrieben werden kann, erscheint eine Warnung mit Möglichkeit zum erneuten Schreiben, ohne doppelte Siegzählung. Siehe [3D-Testplan](docs/3D_TESTPLAN.md).


Die freiwillige 3D-Ausbaustufe ergänzt die Konsole und die 2D-Version um eine eigenständige OpenGL-Anwendung mit echten beleuchteten Volumenkörpern, perspektivischem Spielfeld, Tiefenprüfung, steuerbarer Orbit-Kamera und Sieg-Effekten. Die bestehenden Spielregeln, Spielstände (`GYM1`) und Highscores werden über `MatchSession` weiterverwendet.

**Windows 11, MSYS2 UCRT64, PowerShell im Projektordner `C:\Dev\GymEdition`:**

```powershell
git pull --ff-only
powershell.exe -NoProfile -ExecutionPolicy Bypass -File ".\scripts\preflight-school.ps1" -With3D
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
& .\build-school-3d-preflight\vier-gewinnt-3d.exe
```

In VS Code ist alternativ das neue Debugprofil **Vier Gewinnt | Gym Edition 3D** verfügbar. Dazu muss tatsächlich der Ordner `C:\Dev\GymEdition` als Workspace geöffnet sein. Der erstmalige SFML-/OpenGL-Build benötigt Internet und dauert länger.

**Bedienung:** Rechte Maustaste ziehen = Kamera drehen; Mausrad = Zoom; A/D und Q/E = Kamera; C = zurücksetzen; linke Maustaste = Spalte wählen/setzen; Pfeile und Leertaste = Spalte/Stein; U = Undo; S = Speichern; Escape = Menü; F9/F10/F11 = Originalmusik/Lautstärke. Im Menü 2, 3 oder 4 Spieler wählen.

[3D-Architektur und Bedienung](docs/3D_ARENA.md) · [3D-Abnahmetests](docs/3D_TESTPLAN.md) · [Teil D: 3D-Code verstehen](docs/3D_LERNPFAD.md)

**Grenze:** Diese erste Fassung verwendet klassische OpenGL-Zeichenfunktionen und benötigt einen geeigneten OpenGL-Kompatibilitätskontext der Grafikkarte. Es gibt keine Shader-/PBR-Pipeline und keinen Onlinemodus. Automatische CI-Prüfungen ersetzen nicht den Sicht- und Hörtest auf deinem Rechner.

## Schnellstart

**Windows 11 und Visual Studio Code:** Die 2D-Version startet nur auf dem Entwicklungsbranch. Wenn `git branch --show-current` noch `main` meldet oder `src/gui/Main.cpp` fehlt, lies zuerst die [Windows-2D-Schnellstartanleitung](docs/WINDOWS_QUICKSTART.md). Dort sind der sichere Git-Branchwechsel, die getestete MSYS2-UCRT64-Toolchain, der Windows-Pfad mit `ß`, GDB-Fehler `-environment-cd` und die Befehle zur Fehlersuche dokumentiert. Im PowerShell-Projektordner bietet `./scripts/check-2d.ps1` eine Diagnose ohne Änderungen.

**Für Spieler:** Sobald ein geprüfter Release bereitsteht, [Releases öffnen](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/releases) und das Windows-ZIP entpacken. Vor einem veröffentlichten Release lassen sich automatisch erzeugte CI-Testpakete unter [Actions](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions) als temporäre Artifacts abrufen. Diese sind keine offizielle Endfassung.

**Für Entwickler, Konsolenspiel (Windows mit MSYS2 UCRT64):**

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

**CMake, Debug und automatische Tests (Linux/Windows):**

```bash
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

**Optional: SFML-Grafikversion (CMake mindestens 3.28):**

```bash
cmake -S . -B build-gui -DGYM_BUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-gui --config Release --target vier-gewinnt-gui
ctest --test-dir build-gui -C Release --output-on-failure
```

SFML 3.1.0 wird bei Bedarf über CMake FetchContent eingebunden. Auf Windows müssen ein geeigneter Compiler und die entsprechenden Laufzeitbibliotheken verfügbar sein.

**VS Code:** Zwei F5-Profile für Konsole und GUI liegen in `.vscode/`. Für den MSYS2-UCRT64-GUI-Build sind CMake, Ninja und die UCRT64-Toolchain erforderlich. [Details und Fehlerbehebung](docs/REPOSITORY_GUIDE.md).

**Schul-Demo in PowerShell:** `.\scripts\demo.ps1 -Mode Both` baut und startet zunächst die Grafik- und danach die Konsolenversion. Die Programme werden absichtlich **nicht parallel** ausgeführt, da sie dieselben lokalen Dateien verwenden.

## Architektur und Codequalität

```text
include/              Board, Player, Game, MatchSession, AtomicFile, ScoreFile
src/Board.cpp         Schwerkraft, Züge, Siegerkennung
src/Game.cpp          Ablauf der Konsole
src/ConsoleUI.cpp     Konsolenoberfläche
src/MatchSession.cpp  Testbare Spiellogik der grafischen Variante
src/gui/Main.cpp      SFML-Fenster, Eingaben, Zeichnen
src/gui/GymBeat.h     Originaler, synthetischer Gym-Beat
tests/                Unit-, Integrations- und Konfigurationsprüfungen
.github/workflows/    Linux-/Windows-/UCRT64-CI und Dokumentenchecks
docs/                 Architektur, Testpläne und Projektverlauf
praesentation/        Präsentation und Vortragsleitfaden
reference/            Schulische Referenzdokumente
```

- **Regeln:** `Board` kennt die Spielpositionen, aber keine Fenster oder Namen.
- **Zustand:** `Game` (Konsole) und `MatchSession` (GUI) koordinieren ihre jeweilige Partie.
- **Persistenz:** `GYM1`-Spielstände werden vor dem Übernehmen geprüft. Highscores werden strikt eingelesen; unvollständige Dateien dürfen keine gültigen Ergebnisse überschreiben.
- **Tests:** CTest umfasst Spielfeld, Spielzustand, Textoberfläche und Integration. Die GitHub-Actions-Matrix prüft unterschiedliche Compiler- und Betriebssystemkonfigurationen.

[Architektur im Detail](docs/ARCHITECTURE.md) · [Tests und Nachweise](docs/QUALITY_AND_TESTS.md) · [Windows-GUI-Testplan](docs/GUI_TESTPLAN.md)

## Präsentation und Projektnachweise

Das Projekt ist **Schulprojekt und technische Arbeitsprobe**, keine bereits benotete oder freigegebene Veröffentlichung.

[Anforderungsanalyse und Projektauftrag](docs/PROJECT_CHARTER.md) · [Product Backlog](docs/PRODUCT_BACKLOG.md) · [Entwicklungsverlauf](docs/DEVELOPMENT_LOG_2026-10-28.md) · [Änderungen](CHANGELOG.md)

[Vortragsleitfaden](praesentation/VORTRAGSLEITFADEN.md) · [Präsentationsdateien](praesentation/) · [Test- und Release-Abnahme](docs/RELEASE_CHECKLIST.md)

**Echte Spielfotos und Gameplay-Video:** noch nicht im Repository veröffentlicht. Als Nachweise sollen ausschließlich direkt aus der laufenden Anwendung aufgenommene Bilder und Videos ergänzt werden.

## Rechte und Herkunft

Eigener Anwendungscode und JK-AXIS-Markenbestandteile sowie das gekennzeichnete Logo der Bernd-Blindow-Schulen haben **unterschiedliche Rechtsgrundlagen**. Ohne explizite Quellcode-Lizenz gilt keine allgemeine Open-Source-Nutzungserlaubnis. Weitere Hinweise: [Schullogo und Attribution](docs/SCHOOL_LOGO.md) sowie [Lizenzhinweise](docs/LICENSING.md). Eine öffentliche Repository-Sichtbarkeit ist keine Freigabe aller Inhalte.

© 2026 Julian Krauß · JK AXIS. Entwicklungsstand, keine Produktzertifizierung.
