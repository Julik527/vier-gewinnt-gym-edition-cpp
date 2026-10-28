<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="../assets/brand/jk-axis-precision-symbol-inverse.svg">
    <source media="(prefers-color-scheme: light)" srcset="../assets/brand/jk-axis-precision-symbol.svg">
    <img src="../assets/brand/jk-axis-precision-symbol.svg" alt="JK AXIS" width="84">
  </picture>
</p>

# JK AXIS | GOLD MASTER
## Vier Gewinnt – Gym Edition · Trainings-Arena

**Version:** 28.10.2026 · **Technik:** C++17-Konsole mit ANSI-Farben, falls verfügbar.

## Gestaltungsidee

Ich wollte, dass man beim Spielstart sofort erkennt, dass es meine Gym Edition ist: ein fester Kopf, der Spruch **„HEBEN. SETZEN. SIEGEN.“**, Hantel-Spielsteine, eine saubere Trainings-Arena und die Spieler mit ihren Siegen. Das Terminalspiel soll im Windows Terminal und im VS-Code-Terminal gut lesbar bleiben.

Die Oberfläche ist in der Klasse beziehungsweise im Namensraum `console_ui` gekapselt. `Board` berechnet weiter die Spiellogik; die Darstellung verwendet nur dessen Daten. Die Spielregeln hängen nicht von Farben, Schriftarten oder ANSI-Codes ab.

## Vorschau der Textdarstellung

```text
  +-------------------------------------------------------------------+
  | JK AXIS  /  VIER GEWINNT  /  GYM EDITION                          |
  | TRAININGS-ARENA  |  RUNDE 1                                       |
  +-------------------------------------------------------------------+
  >> [R] Ada  |  3 Siege  < AM ZUG
     [B] Ben  |  1 Sieg

           1     2     3     4     5     6     7
    +-----+-----+-----+-----+-----+-----+-----+
    |  .  |  .  |  .  |  .  |  .  |  .  |  .  |
    |  .  |  .  |  .  |  .  |  .  |  .  |  .  |
    |  .  |  .  |  .  |  .  |  .  |  .  |  .  |
    |  .  |  .  |  .  |  .  |  .  |  .  |  .  |
    |  .  |  .  |  .  |  .  |  .  |  .  |  .  |
    | [R] | [R] | [B] |  .  |  .  |  .  |  .  |
    +-----+-----+-----+-----+-----+-----+
  TRAININGSSTAND  |  Zug 4  |  7 Spalten
  EINGABE: Spalte 1-7
  U Rueckgaengig  |  V 4 Vorschau  |  S Speichern
  H Hilfe  |  I Info  |  A Animation  |  M Menue
```

Die angezeigten Spielernamen und Spielstände sind Beispiele. Das Programm setzt die echten Werte aus der aktuellen Partie ein. Bei drei oder vier Spielern wächst das Raster auf 9×8 beziehungsweise 11×10.

## Bedeutung der Hanteln

| Ansicht | Bedeutung |
|:--|:--|
| `  .  ` | Freies Feld |
| ` [R] ` | Gesetzte Hantel von Spieler R |
| ` [B] ` | Gesetzte Hantel von Spieler B |
| ` [G] ` / ` [Y] ` | Dritter oder vierter Spieler |
| ` (R) ` | Zugvorschau, noch nicht abgelegt |
| ` {R} ` | Hantel gehört zur gewinnenden Reihe |

Die Farben sind optional. Auf kompatiblen Terminals werden die Spielsteine verschieden hervorgehoben: Blau, Gold, Grün und Violett. Die gewinnende Reihe wird grün markiert. **Auch ohne Farbe sind Symbole und Klammern eindeutig.**

## Bildschirme und Bedienung

- **Start:** Hauptmenü mit Spielstatus und dem direkten Einstieg „Neues Spiel“.
- **Einrichtung:** Anzahl 2–4, dann Namen nacheinander, anschließend sofort die Runde.
- **Arena:** Spieler mit Gesamtsiegen, nächster Spieler, Spielfeld, Zugnummer und Befehle.
- **Vorschau:** Zeigt eine mögliche Position, verändert die Spielregeln oder Zugliste nicht.
- **Ergebnis:** Zeigt das endgültige Raster mit markierter Gewinnreihe und die Gesamtsiege.
- **Danach:** Revanche mit denselben Spielern, neue Einrichtung oder Rückkehr zum Hauptmenü.

## Anleitung und Bestenliste

Weil das Hauptmenü sich in modernen Terminals an derselben Bildschirmposition aktualisiert, würden Anleitung oder Bestenliste ohne weitere Steuerung sofort verschwinden. Deshalb bleibt die Ansicht an echten Terminals bis zur Enter-Taste geöffnet. Bei automatisierten Tests mit umgeleiteter Ein- und Ausgabe wird keine zusätzliche Eingabe verlangt.

### Laufende Bildschirmaktualisierung

Bei ANSI-Unterstützung bewegt sich die Anzeige zum Anfang des Terminals und überschreibt die vorhandenen Zeilen. Es gibt **keine vollständige Löschung vor dem Neuzeichnen**. Kürzere Zeilen und überschüssige alte Inhalte werden bereinigt; dadurch sollen sichtbare Leerbilder reduziert werden. Dies ist eine Codeänderung, deren visuelle Wirkung unter Windows Terminal noch manuell zu bestätigen ist.

## Technische Entscheidungen

Die Darstellung verwendet `ConsoleUI.h` und `ConsoleUI.cpp`. Das Spielfeld wird als Text aus dem aktuellen `Board` aufgebaut und in einem Stück ausgegeben. Bei einem geeigneten interaktiven Terminal verwendet `present()` ANSI-Codes zur Aktualisierung derselben Bildschirmposition. Bei umgeleiteter Ausgabe, `NO_COLOR`, unbekannter oder alter Terminalunterstützung wird normales Textformat verwendet.

Die Fallanimation ist in der Konsole standardmäßig aus. `A` schaltet sie um. `console_ui::animateDrop` zeigt mehrere Spielfeldbilder mit wechselnder Hantelposition. Sie verändert den Endzustand des Boards nicht und verwendet keine wiederholte komplette Terminal-Löschung. Die frühere reine Fortschrittszeile wurde ersetzt.

`Board::winningCells()` liefert die tatsächlichen zusammenhängenden Gewinnfelder. Dadurch ist die Hervorhebung keine bloße optische Vermutung.

## Prüfungen

- `tests/board_tests.cpp`: Gewinnlinien und Spielfeldgrenzen.
- `tests/console_ui_tests.cpp`: Menü, Arena, Vorschau, 2/4 Spieler, markierte Gewinnreihe.
- `tests/integration_smoke.py`: echter Konsolenablauf, Abbruch, Speichern, Revanche, zwei Siege.

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

**Abgrenzung dieses Leitfadens:** Hier wird die Terminalversion beschrieben. Zusätzlich gibt es im Entwicklungsbranch eine eigenständige SFML-2D-GUI mit Mausbedienung. Die gemeinsame Regelbasis und die getrennten Controller stehen in [ARCHITECTURE.md](ARCHITECTURE.md). Eine 3D-Version ist nicht implementiert. Sichtprüfung und vollständige Windows-Abnahme bleiben offen.

## Nachweisstand

Die ConsoleUI- und Konsolen-Integrationstests sind am Ausgangsstand `02eb79f8` unter Windows und Ubuntu erfolgreich. [CI-Snapshot](evidence/CI_SNAPSHOT_2026-10-28.json). Die Terminalversion zeigt zusätzlich den Schulnamen. Die tatsächliche Darstellung mit langen Namen und ohne Farben am Schulrechner muss noch bestätigt werden.
