# Projektauftrag: Vier Gewinnt | Gym Edition

**Projekt:** ITA 2026/27, Programmiertechnik · **Entwickler:** Julian Krauß
**Startstand:** 22.10.2026 · **Aktualisierung:** 28.10.2026 · **Nächster Meilenstein:** 29.10.2026

## Ziel

Ein verständlich aufgebautes Vier-Gewinnt-Spiel in C++17 für zwei bis vier Personen am selben Gerät. Individuelle Namen, variable Feldgrößen, Gewinn- und Remiserkennung, Fehlereingaben sowie lokale Spielstände und Highscores gehören zum Kern.

Die Konsole bleibt der schulische Pflichtumfang. Als Zusatz ist eine eigenständige SFML-2D-Anwendung implementiert. Beide Versionen sollen nacheinander vorführbar sein. Die Gym Edition verwendet Hanteln und eine einheitliche JK-AXIS-Gestaltung. Das Schullogo kennzeichnet den Schulkontext.

## Lieferobjekte

1. Anforderungsanalyse FR-01 bis FR-18 und aktualisiertes Product Backlog.
2. Quellcode, Klassenmodell und verständliche Kommentare.
3. Konsolenmenü mit Spielerzahl, nacheinander eingegebenen Namen, Fortsetzen und Revanche.
4. Optionale 2D-GUI mit Maus/Tastatur, Fallanimation und schaltbarem synthetischem Gym-Beat.
5. Öffentliches Repository mit nachvollziehbaren Commits, automatisierten Prüfungen und KI-Promptregister.
6. Präsentation, PDF-Unterlagen und Vortragsleitfaden mit tatsächlichem Implementierungs- und Prüfstand.

## Umfang und Grenzen

| Bereich | Vereinbarter Umfang |
|:--|:--|
| Pflicht | C++17-Konsole, Regeln, 2–4 Spieler, Speichern/Laden, Bestenliste, GitHub und Abgabedokumente |
| Implementierter Zusatz | Vorschau, Undo, Ergebnisbildschirm, Revanche, Terminalanimation, SFML-2D-GUI, Musikschalter und Schullogo |
| Weiterhin außerhalb | 3D-Spiel, KI-Gegner, Online-Multiplayer, Benutzerkonten und Cloudspeicherung |

Die grafische Erweiterung macht die ursprünglichen 3D-Mockups nicht zu Produktbildern. Externe Bibliotheken, KI-Unterstützung und tatsächliche Eigenprüfung werden offen benannt.

## Definition of Done

Code kompiliert, passende Tests sind wirklich ausgeführt, Änderungen lassen sich über Commits zuordnen und die Dokumentation stimmt mit dem Code überein. Hinzu kommen die Vorführung am Zielgerät und eine verständliche Erklärung der wichtigsten Funktionen. Ein grüner CI-Lauf ersetzt weder den Hörtest noch die Schulabnahme.

## Aktueller Stand und Risiken

Die sechs C++-/GUI-Jobs und der Dokumentenjob am Ausgangsstand `02eb79f8` sind erfolgreich. Beleg: [CI-Snapshot](evidence/CI_SNAPSHOT_2026-10-28.json). Der lokale Windows-F5-Build, tatsächliche Audioausgabe, Beamerprobe und vollständige manuelle Abnahme sind noch offen. Die GUI ist im Draft-PR #1 und nicht in `main` enthalten.

Der zuletzt gemeldete Fehler betrifft die Auffindbarkeit von CMake. Ein Dateiersatz über `.tmp` schützt vor getesteten Schreibfehlern, koordiniert aber keine gleichzeitig gestarteten Spielprozesse. Weitere Grenzen stehen in [ARCHITECTURE.md](ARCHITECTURE.md).

## Geplante Schultermine

- 29.10.2026: Projektstart, Anforderungen, Backlog, GitHub, Klassenentwurf und Menü.
- 26.11.2026: Zwei-Spieler-Grundversion.
- 17.12.2026: Sieg und Remis geprüft.
- 28.01.2027: 2–4 Spieler, Speichern/Laden und Highscore.
- 11.02.2027: Generalprobe.
- 18.02.2027: Abschlussvorführung.

Die Termine stammen aus der bisherigen Projektplanung. Dass Funktionen vorzeitig implementiert sind, ersetzt keine termingebundene Lehrkraftabnahme.
