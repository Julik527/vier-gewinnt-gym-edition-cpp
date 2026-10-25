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
