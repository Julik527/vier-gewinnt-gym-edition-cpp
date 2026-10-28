<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="../assets/brand/jk-axis-precision-symbol-inverse.svg">
    <source media="(prefers-color-scheme: light)" srcset="../assets/brand/jk-axis-precision-symbol.svg">
    <img src="../assets/brand/jk-axis-precision-symbol.svg" alt="JK AXIS" width="80">
  </picture>
</p>

# Vier Gewinnt | Gym Edition
## Menüführung und Spielstart · JK AXIS | GOLD MASTER

**Sprache:** C++17 · **Oberfläche:** Konsolenmenü  
**Version:** 28.10.2026

### Ziel

Ich möchte, dass ein neues Spiel ohne Umwege startet. Deshalb fragt das Programm erst die Spieleranzahl und direkt danach die Namen ab. Wenn alle Angaben fertig sind, beginnt das Spiel automatisch. Man muss die Einstellungen nicht mehr einzeln im Hauptmenü suchen.

### Das neue Hauptmenü

```text
  +---------------------------------------------------+
  | JK AXIS  |  VIER GEWINNT : GYM EDITION            |
  | C++17  |  LOKALER MEHRSPIELER-MODUS               |
  +---------------------------------------------------+
  | 1   Neues Spiel                                  |
  | 2   Spiel fortsetzen (keine Runde)                |
  | 3   Spielstand laden                             |
  | 4   Highscore                                    |
  | 5   Anleitung                                    |
  | 0   Beenden                                      |
  +---------------------------------------------------+
```

Die Breite des Rahmens wird vom Programm automatisch berechnet. Wenn keine Partie läuft, ist die Funktion **Fortsetzen** mit einem Hinweis versehen.

### So läuft „Neues Spiel“ ab

1. **Neues Spiel auswählen:** Im Hauptmenü die `1` drücken.
2. **Spieleranzahl festlegen:** `2`, `3` oder `4` eingeben. Die Feldgröße passt sich an (`7×6`, `9×8` oder `11×10`).
3. **Spielernamen eingeben:** Das Programm fragt Spieler 1 bis Spieler N nacheinander ab. Jeder Name darf maximal 24 Bytes lang sein und nicht doppelt vorkommen.
4. **Spielbeginn:** Eine kurze Zusammenfassung zeigt Namen, Spielsymbole und Feldgröße. Danach beginnt direkt der erste Spieler.

In beiden Einrichtungsschritten kann man mit `0` abbrechen. Beim Abbruch wird ein bereits bestehendes Spiel nicht verändert.

### Beispiel für zwei Spieler

```text
  Deine Auswahl: 1

  NEUES SPIEL  |  SCHRITT 1 VON 2
  Wie viele Spieler machen mit? (2, 3 oder 4)
  Spieleranzahl [2-4]: 2

  NEUES SPIEL  |  SCHRITT 2 VON 2
  Spieler 1 [R]: Ada
  Spieler 2 [B]: Ben

  SPIEL BEREIT
  2 Spieler | 7 Spalten x 6 Reihen
  [R] Ada
  [B] Ben

  Los geht's!
```

### Was passiert bei Fehlern?

| Situation | Verhalten |
|:--|:--|
| Spielerzahl außerhalb 2–4 | Neue Abfrage, kein Absturz |
| Leerer oder zu langer Name | Verständliche Fehlermeldung und erneute Eingabe |
| Doppelter Name | Erneute Namensabfrage |
| `0` während der Einrichtung | Zurück zum Hauptmenü, alte Runde bleibt erhalten |
| Neues Spiel bei laufender Runde | Bestätigung nötig, bevor die Runde ersetzt wird |
| `M` im Spiel | Zurück zum Hauptmenü; die Runde kann mit `2` fortgesetzt werden |
| Ungültige Speicherdatei | Fehlerhinweis, bisheriges Spiel bleibt erhalten |
| Programm beenden bei laufender Runde | Sicherheitsabfrage |

### Welche Methoden sind dafür zuständig?

- `Game::run()`: Zeigt das eine Hauptmenü an und verarbeitet die Auswahl.
- `Game::newGame()`: Verbindet Einrichtung und Spielstart in der richtigen Reihenfolge.
- `Game::configurePlayers(int&)`: Kontrolliert die Spielerzahl.
- `Game::configureNames(int, std::vector<std::string>&)`: Fragt jeden Namen ab und prüft die Eingaben.
- `Game::play()`: Führt die Partie durch.
- `Game::loadGame()`: Liest einen gespeicherten Spielstand und prüft ihn vor der Übernahme.

### Qualitätssicherung

Die Datei [`tests/integration_smoke.py`](../tests/integration_smoke.py) enthält automatisierte Szenarien für das neue Menü einschließlich Start, fehlerhafter Eingaben, Abbruch, Fortsetzen und Laden. Ein erfolgreicher Testlauf muss über CTest beziehungsweise GitHub Actions bestätigt werden. Die Dokumentation behauptet keine bestandene Windows-Prüfung ohne tatsächliches Ergebnis.

**Gestaltung:** Einheitliches JK-AXIS-Logo und ruhige ASCII-Struktur. Die Benutzeroberfläche bleibt absichtlich ein Terminalprogramm; eine grafische Windows-GUI wurde nicht vorgetäuscht.
