# Umsetzung des GitHub-Reviews: Vier Gewinnt | Gym Edition

**Basis:** GitHub-main 9bd1b15 (17.11.2026)  
**Arbeitszweig:** feature/ai-replay-safe-extras-2026-11  
**Fokus:** Additive Erweiterungen ohne Regression im schulischen Konsolenprogramm.

## Implementiert (Testergebnis steht getrennt im Pull Request)

- Ein optionales eigenstaendiges C++17-Programm mit drei KI-Stufen.
- Abgesicherte KI-Zugwahl mit unmittelbarer Gewinn- und Blockerkennung.
- Minimax / Alpha-Beta-Pruning fuer die normalen und schweren Stufen.
- Validierte, nicht destruktive Wiedergabe gespeicherter Zugfolgen **innerhalb der laufenden Partie**.
- Tests fuer Sofortsieg, Blockade, deterministische Zugwahl, unveraendertes Board, Replay und Grenzen.
- Eigene CMake-Option GYM_BUILD_AI=ON, bestehender Standard weiterhin OFF.
- C++-CI um optionale AI-Builds in Linux-, Windows- und MSYS2-UCRT64-Varianten ergaenzt.

## Bewusst unveraendert

- src/Board.cpp und include/Board.h: bestehende Regeln bleiben zentrale Quelle.
- src/Game.cpp und src/main.cpp: Pflicht-Konsole unveraendert.
- src/gui/Main.cpp und Main3D.cpp: GUI/3D unveraendert.
- school/, bestehende Praesentationsfassungen und Lehrerangaben: keine stillschweigende Anpassung.

## Noch offen und nicht als bestanden darzustellen

1. **Issue #2:** Echte Schul-PC-/Windows-Bedienabnahme und Grafik-/Audio-Sicht- und Hoertests.
2. **Issue #3:** Zusammenfuehrung von Game und MatchSession zu einer gemeinsamen Spielflusslogik (grosses Regressionrisiko, getrennt zu behandeln).
3. **Issue #4:** Offizielles GitHub Release erst nach portabler Windows-ZIP, Hashpruefung, echten Screenshots, Lizenzentscheidung und manuellem Start auf einem zweiten Rechner.
4. **Issue #5:** Replay bisher nur im separaten KI-Konsolenmodus. Persistente Spielerprofile, Siegquoten in der GUI und Signature-Arena sind noch nicht implementiert.
5. **Praesentation 19.11.2026:** Eine tatsaechliche V7.1-Endfassung auswaehlen, mit Stoppuhr testen und durch den Lehrer beurteilen lassen.

## Reihenfolge zur Freigabe

1. Pull Request auf aktuelle SHA und CI-Pruefungen kontrollieren.
2. KI-Labor mit CMake auf Windows/Linux testen, danach bestehende Schulkonsole separat testen.
3. Mit dem vorhandenen Packaging-Skript Schul-ZIP bauen und SHA-256 verifizieren.
4. Manuelle Windows-/Schul-PC-Abnahme fuer Konsole, GUI, Audio und Beamer protokollieren.
5. **Erst dann** neue Funktionen auf main mergen und ein Release veroeffentlichen.

### Statusbegriff

Ein GitHub-Commit beweist nur, dass Code eingecheckt wurde. Ein gruener GitHub-Actions-Lauf belegt die jeweils ausgefuehrten automatisierten Kontrollen. Eine echte interaktive Windows- und Schulabnahme bleibt davon getrennt. Dieser Bericht ist **keine** Freigabe.
