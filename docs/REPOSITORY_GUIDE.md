# GitHub-Projektverwaltung

**Repository:** https://github.com/Julik527/vier-gewinnt-gym-edition-cpp  
**Branch:** main · **Sichtbarkeit:** öffentlich, vom Eigentümer bestätigt.

## Aktueller Stand
C++17-Code, Header, Tests, Übungen, README, Product Backlog, Projektauftrag, Meilensteinübersicht und KI-Prompt-Dokumentation wurden über die verbundene GitHub-Integration mit Commits hochgeladen.

**Dateien vollständig veröffentlicht:** Die acht gestalteten PDFs/PPTX-Dateien wurden durch den Eigentümer auf GitHub in den Root-Ordnern `reference/` und `praesentation/` hochgeladen, bestätigt durch [Commit 53612455](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/commit/53612455ee21bf4363b7d5627fe8cd388a8c7e8b). Siehe ASSET_UPLOAD_CHECKLIST.md.

## Projekt lokal weiterentwickeln
WICHTIG: Die ursprünglich vorbereitete ZIP-Datei enthält eine andere lokale Git-Historie. Nicht aus ihr blind pushen. Stattdessen dieses Online-Repository frisch klonen:

~~~powershell
git clone https://github.com/Julik527/vier-gewinnt-gym-edition-cpp.git
cd vier-gewinnt-gym-edition-cpp
~~~

Nach Änderungen: git add . ; git commit -m "Beschreibung" ; git push.

## Quellcode bauen
~~~powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
~~~

## Nachweise
Dateien auf GitHub, aktuelle Commit-Historie und Menüdemo separat für die Abgabe dokumentieren. Der Online-Upload ist nicht identisch mit einer abgeschlossenen Softwareabnahme.

## JK AXIS | GOLD MASTER

Seit 28.10.2026 verwendet die Projektdokumentation das aktuelle JK-AXIS-Präzisionslogo. Die einmalige Aktualisierung der PPTX- und PDF-Dateien ist in Commit [`ea8bbef8`](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/commit/ea8bbef88c2579c73c701c8f81828b202c36f46d) dokumentiert. Der schreibende Migrations-Workflow wurde nach Abschluss aus Sicherheitsgründen entfernt. `scripts/rebrand_assets.py` bleibt als historischer Migrationscode erhalten. Prüfnachweise: [BRAND_QA_REPORT.md](BRAND_QA_REPORT.md). Die Python- und GitHub-Actions-Prüfung ist keine abschließende Sichtabnahme in PowerPoint und PDF-Viewer.

## Aktuelle automatisierte Prüfungen

- `cpp-quality.yml`: C++-Builds/Unit-/Integrationstests auf Linux und Windows, jeweils Debug/Release.
- `docs-quality.yml`: rein lesende Qualitätsprüfung aller Projekt-PDFs, PowerPoint-Folien und Dateiverweise.
- `scripts/document_preflight.py`: Einzelprüfung sämtlicher 151 PDF-Seiten auf technische Renderbarkeit.

Der ältere schreibende Branding-Workflow ist bewusst **nicht mehr aktiv**. Neue Dokumentänderungen sollen geprüft und nicht unbeaufsichtigt umgeschrieben werden. Erfolgreiche Workflow-Ausführung muss vor Abgabe anhand der GitHub-Actions-Läufe bestätigt werden.
