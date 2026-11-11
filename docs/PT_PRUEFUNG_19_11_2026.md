# Programmiertechnik – PT am 19.11.2026 | prüfungsgenauer Lernzettel

**Verbindliche Primärquelle:** E-Mail von Dr. Michael Litera, „Klausur am 19.11.2026“, eingegangen am 11.11.2026. **PT:** ca. 70 Minuten. **PP:** Einzelpräsentationen, ca. 10 Minuten pro Schüler. Diese Unterlage ist eine zusätzliche Lernhilfe und keine offiziell ausgegebene Klausur.

## 1. Vollständige Themenabdeckung

| In der E-Mail genannt | Was du erklären und schreiben können musst |
|---|---|
| Strukturen | `struct`, Attribute, Initialisierung, Zugriffsstandard `public` |
| Klassen | `class`, `private/public`, Objekte, Methoden, Kapselung |
| Set/Get-Methoden | lesender Getter mit `const`; Setter mit Validierung |
| Konstruktoren | Klassenname ohne Rückgabetyp; Initialisierungsliste |
| Parameterkonstruktoren | Übergabe von Werten; z. B. `Board(int)` |
| Destruktoren | `~Klassenname()`, Objektlebensdauer, RAII |
| Dateioperationen | `<fstream>`, `ifstream`, `ofstream`, Fehlerprüfung |
| Dynamische Speicherreservierung | `new/delete`, `new[]/delete[]`, Pointer, Leaks, `std::vector` |
| this-Zeiger | `this->attribut` bzw. `*this`, kein `this` in statischen Methoden |
| Trennung der Klasse | Header mit Deklaration, .cpp mit Definition, `main.cpp` als Einstieg |

## 2. Die wichtigsten C++-Muster

### Struktur versus Klasse

```cpp
struct Move { int row; int column; int player; };
Move m{5, 3, 0};

class Player {
private:
    std::string name_;
public:
    explicit Player(std::string name) : name_(std::move(name)) {}
    const std::string& getName() const { return name_; }
};
```

*Merke:* Bei `struct` sind Mitglieder standardmäßig öffentlich, bei `class` privat. Beide können Methoden, Konstruktoren und Vererbung haben. `Move` entspricht dem Datenmodell aus `include/Game.h`; das zweite Player-Beispiel ist eine vereinfachte Übungsfassung und nicht wortgleich mit `include/Player.h`.

### Konstruktor, Destruktor und Getter/Setter

```cpp
class Konto {
    int stand_;
public:
    explicit Konto(int start) : stand_(start >= 0 ? start : 0) {}
    ~Konto() = default;
    int getStand() const { return stand_; }
    bool setStand(int wert) {
        if (wert < 0) return false;
        stand_ = wert;
        return true;
    }
};
```

- Konstruktor: **kein** Rückgabetyp, wird beim Erzeugen ausgeführt.
- Parameterkonstruktor: Werte gehen als Argumente in die Initialisierung ein.
- Initialisierungsliste: Mitglieder werden direkt initialisiert; maßgeblich ist ihre **Deklarationsreihenfolge**.
- Destruktor: wird bei der Zerstörung des Objekts ausgeführt. Moderne Container verwalten Speicher selbst.
- `const` hinter dem Getter: Die Methode darf keine nicht-`mutable` Attribute ändern.

### this und dynamischer Speicher

```cpp
class Punkt {
    int x_ = 0;
public:
    void setX(int x) { this->x_ = x; }
    Punkt& addiere(int n) { x_ += n; return *this; }
};

int* p = new int(42);
int wert = *p;
delete p;
p = nullptr;

int* feld = new int[3]{1, 2, 3};
delete[] feld;
```

`this` ist ein Zeiger auf das aktuelle Objekt; `*this` bezeichnet das Objekt. Nach `new[]` immer `delete[]`. Vermeide Double Delete, Use-after-free und Memory Leaks. **Bevorzugt:** `std::vector` oder Smart Pointer statt roher Besitzzeiger.

### Dateioperationen mit Fehlerkontrolle

```cpp
#include <fstream>
#include <stdexcept>

void speichere(int n) {
    std::ofstream out("zahl.txt");
    if (!out) throw std::runtime_error("Öffnen fehlgeschlagen");
    out << n << '\n';
    if (!out) throw std::runtime_error("Schreiben fehlgeschlagen");
}
int lade() {
    std::ifstream in("zahl.txt");
    int n{};
    if (!(in >> n)) throw std::runtime_error("Lesen fehlgeschlagen");
    return n;
}
```

`std::ios::app` hängt an; `std::ios::trunc` leert die Zieldatei beim Öffnen; `std::getline` liest eine komplette Zeile. Im Projekt speichert `Game::saveGame` im `GYM1`-Format. `Game::loadGame` validiert einen Kandidaten, bevor der vorhandene Zustand übernommen wird. `gym::writeSafely` schreibt zunächst in eine temporäre Datei; es garantiert keine Mehrprozess-Transaktionssicherheit.

### Header, Implementierung, main

```cpp
// Counter.h
#pragma once
class Counter {
    int wert_ = 0;
public:
    void plusEins();
    int wert() const;
};

// Counter.cpp
#include "Counter.h"
void Counter::plusEins() { ++wert_; }
int Counter::wert() const { return wert_; }

// main.cpp
#include "Counter.h"
int main() { Counter c; c.plusEins(); return 0; }
```

Beide `.cpp`-Dateien müssen gebaut und gelinkt werden, wenn die extern definierten Methoden benötigt werden. `::` qualifiziert die Klassenzugehörigkeit. In deinem echten Code gilt dieselbe Trennung für `include/Board.h`, `src/Board.cpp` und `src/main.cpp`.

## 3. Projektcode, den du selbst erklären musst

1. `Board::Board(int)` berechnet 6×7, 8×9 oder 10×11 Zellen; private `rows_`, `columns_` und `grid_` werden durch die Initialisierungsliste gesetzt.
2. `Board::nextFreeRow(int)`: Grenzprüfung; Suche von `rows_ - 1` bis 0; Rückgabe freier Zeilenindex oder `-1`.
3. `Board::drop(int,char,int&)`: setzt nur gültige Symbole in eine freie Spalte und liefert über die Referenz die Landereihe zurück.
4. `Board::hasWon(...)`: vier Achsen, jeweils positive und negative Richtung; `1 + links + rechts >= 4` bedeutet Gewinn.
5. `Game`: steuert Menü, Eingaben, Spielerwechsel, Undo, Spielstand. `ConsoleUI`: stellt im Terminal dar.
6. `std::vector` speichert das dynamische Raster ohne eigenes `new[]` und `delete[]`.
7. `tests/board_tests.cpp` und `tests/integration_smoke.py`: Regeln und Abläufe mit automatischen Prüfungen belegen.

## 4. Selbsttest: Aufgaben ohne Vorlage

- A1: Erkläre `struct` vs. `class` einschließlich Standardzugriff (4 P).
- A2: Schreibe einen Getter `const` und einen validierenden Setter (6 P).
- A3: Schreibe einen Parameterkonstruktor mit Initialisierungsliste (6 P).
- A4: Erkläre Zeitpunkt und Aufgabe eines Destruktors (4 P).
- A5: Zeige `this->feld` und `return *this` (5 P).
- A6: Reserviere ein int-Array dynamisch und gib es korrekt frei (4 P).
- A7: Finde Fehler in `new[]` mit `delete`, Double Delete, Dereferenzierung nach Löschung (6 P).
- A8: Schreibe lesende und schreibende Dateioperationen mit Prüfung (8 P).
- A9: Teile eine Klasse in Header, Implementierung und `main` auf (9 P).
- A10: Erkläre `Board::nextFreeRow`, `drop`, `hasWon` direkt am echten Code (18 P).

**Gesamt:** 70 selbst vergebene Übungspunkte, nicht die offizielle Punkteverteilung. Für einen realistischen Test **70 Minuten** ohne Unterlagen setzen und jede falsche Antwort im Code überprüfen.

## 5. Letzte Prüfungsfallen

- `void Konto()` ist kein gültiger Konstruktor.
- Ein `struct` kann sehr wohl Methoden haben.
- `this` ist kein Objekt, sondern ein Zeiger; `*this` ist das Objekt.
- `int* p = nullptr;` darf nicht dereferenziert werden.
- Nach `new[]` nicht `delete`, sondern `delete[]`.
- Im Projekt: Die GUI ist Zusatzumfang; Pflicht ist die Console.
- **Keine unbestätigte Bestnote oder erfolgreiche Vor-Ort-Demo behaupten.**

**Referenzen:** Lehrer-E-Mail 11.11.2026; `include/{Board,Game,Player}.h`, `src/{Board,Game,main}.cpp` des Repositories; dokumentierter Hauptbranch-Commit `731000dc12efdddaa42e2db535ebffcc3d603d17`. Übungen und Beispielklasse Konto sind eigens konstruiert.
