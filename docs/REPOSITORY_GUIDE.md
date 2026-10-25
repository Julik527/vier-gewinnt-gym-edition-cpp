# GitHub-Projektverwaltung

**Repository:** https://github.com/Julik527/vier-gewinnt-gym-edition-cpp  
**Branch:** main · **Sichtbarkeit:** öffentlich, vom Eigentümer bestätigt.

## Aktueller Stand
C++17-Code, Header, Tests, Übungen, README, Product Backlog, Projektauftrag, Meilensteinübersicht und KI-Prompt-Dokumentation wurden über die verbundene GitHub-Integration mit Commits hochgeladen.

**Offen:** Gestaltete PDF- und PowerPoint-Dokumente sind noch nicht als Binärdateien auf GitHub übertragen. Sie müssen unter docs/reference/ bzw. docs/praesentation/ ergänzt werden. Liste: ASSET_UPLOAD_CHECKLIST.md.

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
