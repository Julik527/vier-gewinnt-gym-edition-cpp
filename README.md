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

## Übersicht

Die **C++17-Konsolenversion** ist das eigentliche Schulprojekt. Die Varianten **SFML 2D** und **OpenGL 3D** sind zusätzliche Entwicklungs- und Portfolioarbeiten. Alle drei Programme sind gemeinsam in `main` integriert; die Konsolenabgabe lässt sich separat erzeugen.

| Ich möchte ... | Hier geht es weiter |
|:--|:--|
| Das Spiel starten | [Schnellstart](#schnellstart) |
| Verstehen, wie der Code aufgebaut ist | [Architektur](docs/ARCHITECTURE.md) |
| Die Schulabgabe vorbereiten | [Schulordner](school/README.md) |
| Für die Prüfung am 19.11.2026 lernen | [PT-Lernzettel](docs/PT_PRUEFUNG_19_11_2026.md) |
| Die aktuelle 10-Minuten-Arbeitsfassung öffnen | [Präsentationsordner](praesentation/README.md) |
| Zwei 8-Folien-Fassungen auseinanderhalten | [Versionsregister](milestones/PRAESENTATION_VERSIONEN.md) |
| Tests und Qualitätsnachweise einsehen | [Tests](docs/QUALITY_AND_TESTS.md) |
| Alle Projektunterlagen finden | **[Dokumentationsübersicht](docs/README.md)** |
| Meilensteine chronologisch ansehen | [Meilensteinübersicht](milestones/README.md) |
| Wichtige Abgabe- und Präsentationsdateien finden | [Abgabeübersicht](deliverables/README.md) |
| Aktuellen Qualitätsstand überprüfen | [Prüfübersicht](quality/README.md) |

## Das Spiel

2 bis 4 Personen setzen abwechselnd Spielsteine in ein Raster. Wer vier Felder waagerecht, senkrecht oder diagonal verbindet, gewinnt.

| Personen | Spielfeld | Version |
|:--:|:--:|:--|
| 2 | 7 × 6 | Konsole, 2D, 3D |
| 3 | 9 × 8 | Konsole, 2D, 3D |
| 4 | 11 × 10 | Konsole, 2D, 3D |

Die **Konsole** bietet unter anderem Undo, Revanche, Spielstandverwaltung und Highscores. Die grafischen Versionen ergänzen die Bedienung und Darstellung; passende Grafik-, SFML- bzw. OpenGL-Voraussetzungen sind erforderlich.

## Schnellstart

**Konsole mit CMake** (C++17-Compiler und CMake ab Version 3.16):

```bash
cmake -S . -B build -DGYM_BUILD_GUI=OFF
cmake --build build
ctest --test-dir build --output-on-failure
```

Das Programm heißt je nach Plattform und Generator beispielsweise `build/vier-gewinnt` (Linux) oder `build/vier-gewinnt.exe` (Windows mit Ninja). Bei Visual Studio kann es in `build/Debug/` liegen.

Unter Windows mit VS Code gibt es **Drei F5-Profile** für Konsole, 2D und 3D. Für die beiden grafischen Versionen sind weitere Abhängigkeiten nötig. [Windows-Anleitung](docs/WINDOWS_QUICKSTART.md) · [VS-Code-Debugging](docs/VS_CODE_DEBUG.md) · [3D-Arena](docs/3D_ARENA.md).

**Isolierte Schulabgabe vorbereiten:**

```bash
python scripts/package-school-console.py
python scripts/package-school-console.py --verify
```

Die Schul-ZIP enthält die Konsole und die relevanten Tests und Unterlagen, keine 2D-/3D-Quellen. [Abgabe-Checkliste](school/ABGABE_CHECKLISTE.md).

## Projektstruktur

```text
include/          Header und gemeinsame Schnittstellen
src/              Spielregeln, Konsolenlogik und GUI
  gui/            SFML 2D, OpenGL 3D und Audio
tests/            C++-, Python- und Integrationstests
milestones/       Datierte Meilensteine, mit Verweisen auf die Originaldateien
deliverables/     Übersicht der Abgabe- und Präsentationsdateien
quality/          Orientierung zu Tests, CI und Freigabe
school/           Eigenständige schulische Konsolenabgabe
praesentation/    Aktuelle Präsentation und ältere Materialien
docs/            Fachliche Dokumentation, Lern- und Prüfunterlagen
reference/        Referenz-PDFs aus dem Entwicklungsverlauf
assets/           Logos und optionale Medienhinweise
scripts/          Build-, Prüf- und Paketskripte
.github/          Automatisierte Qualitätssicherung
```

Im Dokumentationsordner werden die Dateien nach **Einstieg, Programmierung, Prüfung, Grafik/Audio, Qualität und Projektverlauf** erschlossen. Du musst dich dadurch nicht durch alle Dateien einzeln klicken: [docs/README.md](docs/README.md).

## Qualität und Grenzen

Die GitHub Actions prüfen Builds und automatisierte Tests. Eine erfolgreiche CI beweist jedoch **keinen** fehlerfreien Start auf dem Schul-PC, keine funktionierende Audioausgabe und keine bestandene Präsentation. Diese Punkte müssen vor Ort geprüft werden.

[Qualität & Tests](docs/QUALITY_AND_TESTS.md) · [Testplan](docs/TESTPLAN.md) · [Release-Abnahme](docs/RELEASE_ABNAHME.md) · [Änderungsverlauf](CHANGELOG.md).

## Prüfungsunterlagen

Für die am **19.11.2026** vorgesehene Einzelpräsentation bleibt die **8-Folien-/10-Minuten-Fassung aus `praesentation/`** die aktuelle Repository-Arbeitsfassung. Zusätzlich liegt eine **andere V6-Fassung mit acht Folien** vor. Beide sind im [Versionenregister](milestones/PRAESENTATION_VERSIONEN.md) getrennt geführt. Die schulisch verwendete Endfassung ist hier noch nicht bestätigt. Die ältere 17-Folien-Fassung und die 15+15-Minuten-Sprechkarten sind historische Arbeitsstände.

[Aktuelle Folien (PPTX)](praesentation/Vier_Gewinnt_Meilenstein_10_Minuten.pptx) · [PDF](praesentation/Vier_Gewinnt_Meilenstein_10_Minuten.pdf) · [Sprechleitfaden](praesentation/MEILENSTEIN_10_MIN_2026-11-19.md) · [Prüfungsvorbereitung](docs/PT_PRUEFUNG_19_11_2026.md).

---

<sub>ITA-Programmierprojekt 2026/27 · Stand der Übersicht: 17.11.2026 · Historische Änderungen sind über die Git-Versionierung nachvollziehbar. Details zu Herkunft und Lizenzen: [Lizenzhinweise](docs/LICENSING.md).</sub>
