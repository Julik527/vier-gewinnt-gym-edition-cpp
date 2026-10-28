<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="../assets/brand/jk-axis-precision-symbol-inverse.svg">
    <source media="(prefers-color-scheme: light)" srcset="../assets/brand/jk-axis-precision-symbol.svg">
    <img src="../assets/brand/jk-axis-precision-symbol.svg" alt="JK AXIS" width="76">
  </picture>
</p>

# JK AXIS | GOLD MASTER
## Kommentar- und Codeleitfaden – Vier Gewinnt | Gym Edition

**Projekt:** C++17 · Programmiertechnik · Schulprojekt  
**Stand:** 28.10.2026  
**Ziel:** Der Code soll ordentlich aussehen und sich im Unterricht einfach erklären lassen.

### 1. Einheitlicher Dateikopf

Jede Quell- und Header-Datei beginnt mit einer kurzen Beschreibung. Der Kopf ist absichtlich schlicht: In C++ lassen sich keine verlässlichen Markenfarben darstellen, aber Name, Aufbau und Schreibweise sind einheitlich.

```cpp
// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   src/Board.cpp
// Aufgabe: Verwaltet das Spielfeld, die Spielzüge und die Gewinnerkennung.
// ------------------------------------------------------------
```

Die ergänzenden Python-, CMake- und GitHub-Actions-Dateien verwenden denselben Aufbau mit `#` statt `//`.

### 2. So schreibe ich die Kommentare

- **Einfach und direkt:** So, dass ich den Code meinem Lehrer oder einem Mitschüler erklären könnte.
- **Vor einem wichtigen Schritt:** Zum Beispiel bei einer Spielregel, einem Spielerwechsel oder beim sicheren Laden.
- **Warum statt nur was:** `// Wir prüfen alle vier Gewinnrichtungen.` ist hilfreicher als `// for-Schleife`.
- **Nicht jede Zeile kommentieren:** `++count;` muss nicht extra erklaert werden, wenn der Zusammenhang klar ist.
- **Passend zum echten Programm:** Keine Aussagen über Funktionen, die erst noch geplant sind.
- **Wenn sich der Code ändert:** Den zugehörigen Kommentar ebenfalls prüfen.

### 3. Beispiel aus der Spiellogik

```cpp
// Von unten nach oben suchen: Dort landet die nächste Hantel.
int Board::nextFreeRow(int column) const {
    if (column < 0 || column >= columns_) return -1;
    for (int row = rows_ - 1; row >= 0; --row) {
        if (grid_[row][column] == '.') return row;
    }
    return -1;
}
```

**So würde ich es erklären:** Das Programm schaut zuerst in die unterste Zeile einer Spalte. Ist das Feld schon belegt, geht es eine Zeile nach oben. Gibt es keinen Platz mehr, kommt `-1` zurück.

### 4. Welche Datei macht was?

| Datei | Aufgabe im Projekt |
|:--|:--|
| `src/main.cpp` | Startet das Programm |
| `include/Game.h`, `src/Game.cpp` | Menü, Spielablauf, Spielerwechsel, Speicherdateien |
| `include/Board.h`, `src/Board.cpp` | Spielfeld, Hanteln, Gewinnerkennung, Undo |
| `include/Player.h` | Name und Symbol eines Spielers |
| `tests/board_tests.cpp` | Testet die Spielfeldregeln |
| `tests/integration_smoke.py` | Gibt Menübefehle ein und kontrolliert das Ergebnis |
| `CMakeLists.txt` | Legt fest, wie Spiel und Tests gebaut werden |
| `scripts/document_preflight.py` | Prüft Dateien und Dokumente nur lesend |
| `scripts/rebrand_assets.py` | Historisches Werkzeug für die einmalige Logo-Umstellung, nicht Teil des Spielablaufs |

### 5. Gestaltung und Qualität

Das **JK-AXIS-Präzisionslogo** steht in der Projekt-README und in diesem Leitfaden. In den Quelltexten sorgt ein kompakter Dateikopf für Wiedererkennung. Farben, große Schriftzüge oder Sonderzeichen im C++-Code würden die Lesbarkeit im Editor eher verschlechtern.

**Wichtig:** Kommentare helfen beim Verstehen. Sie ersetzen weder funktionsfähigen Code noch Tests. Die aktuelle technische Testabdeckung steht in [QUALITY_AND_TESTS.md](QUALITY_AND_TESTS.md).
