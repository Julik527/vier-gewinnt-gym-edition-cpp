# Qualitätssicherung und Testnachweis

**Testdatum:** 25.10.2026 · **Umgebung:** Linux / g++ 14.2.0 / C++17.

| ID | Prüfung | Ergebnis | Einschränkung |
|:--|:--|:--|:--|
| Q-01 | Projekt mit `-std=c++17 -Wall -Wextra -pedantic` kompilieren | Bestanden, ohne Warnungen | Nicht auf Windows reproduziert |
| Q-02 | Board-Testprogramm kompilieren | Bestanden | Nur Board |
| Q-03 | Board-Assertions ausführen | Bestanden | Dimensionen, Eingabegrenzen, vier Richtungen, Undo |
| Q-04 | Menüfolge `6, 8, 0` | Bestanden | Highscoreanzeige, falsche Auswahl, sauberer Exit |

## Nicht als bestanden auszuweisen
- Mehrspielerablauf und leere/doppelte Namen als reale End-to-End-Tests.
- Speichern/Laden, Dateibeschädigung und Neustart.
- Remis und Highscore nach tatsächlicher Spielpartie.
- Ausführung am Schulrechner / Windows.
- Schulische Schlussabnahme.

## Wiederholbare Befehle
~~~bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp -o vier-gewinnt
./vier-gewinnt
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/board_tests.cpp src/Board.cpp -o board_tests
./board_tests
~~~

Die Dokumentation enthält ausdrücklich keine fiktiven Testergebnisse.