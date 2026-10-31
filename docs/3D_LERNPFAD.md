# Teil D: Gym Edition 3D | Lernpfad und Codeverteidigung

**Ziel:** Die 3D-Version nicht nur ausführen, sondern ihren Aufbau und die entscheidenden Methoden **selbst** erklären. Dieses Dokument ist ein Lernleitfaden, keine Bestätigung bereits beherrschter Kenntnisse.

## Lerneinheit 1 - Pflichtspiel von der 3D-Grafik trennen

**Dateien:** `include/Board.h`, `src/Board.cpp`, `include/MatchSession.h`, `src/MatchSession.cpp`.

Erkläre, weshalb `Board` nichts über OpenGL wissen muss. Zeige am Beispiel `Board::nextFreeRow`, wie Schwerkraft ohne Animation funktioniert. Anschließend beweise mit `MatchSession::drop`, dass dieselbe Regel auch von der 2D- und 3D-Anwendung aufgerufen wird.

**Selbsttest:** Kannst du ohne Hilfe die Feldgröße für drei und vier Spieler aus der Formel berechnen? Was passiert bei einer vollen Spalte? Wie verhindert die Spielsteuerung doppelte Züge?

## Lerneinheit 2 - Vektoren und die Kamera

**Datei:** `src/gui/Gym3DMath.h`.

Das räumliche Koordinatensystem ordnet **x** den Spalten, **y** der Höhe und **z** der Tiefe zu. `slot(row,col,rows,cols)` wandelt eine Spielfeldzelle in die passende Position um. Ein `Vec3` besitzt drei Komponenten.

- **Skalarprodukt (`dot`)** ermittelt, wie stark ein Vektor in die Richtung eines anderen zeigt.
- **Kreuzprodukt (`cross`)** erzeugt einen rechtwinkligen Vektor und ermöglicht die Kamerabasis.
- **Normierung (`normalize`)** macht daraus einen Einheitsvektor.
- **`eye(Camera)`** liefert die Kamera-Position um den Mittelpunkt der Arena.
- **`project`** bildet einen sichtbaren 3D-Punkt perspektivisch auf Bildschirmkoordinaten ab.

**Praxis:** Ändere vorübergehend die Kamerawerte `yaw` und `pitch`, zeichne das erwartete Ergebnis auf und vergleiche es mit dem Fenster. Danach Änderungen zurücksetzen. `scene3d_tests.cpp` kontrolliert diese Mathematik automatisch.

## Lerneinheit 3 - OpenGL und echte Körper

**Datei:** `src/gui/Main3D.cpp`.

`cube()` erzeugt die sechs Flächen eines Quaders. `disc()` erzeugt aus einem Kreis mit Vorderseite, Rückseite und Mantel einen dreidimensionalen Spielstein. `ring()` zeichnet seinen beleuchteten Rahmen.

`cameraGL()` setzt Perspektive und Model-View-Matrix. `renderWorld()` stellt Beleuchtung, Tiefentest, Gitter, Rahmen, Zellen und Steine her. Dank Tiefentest verdecken nahe Körper die dahinter liegenden. Dadurch entsteht echte räumliche Geometrie, keine statische 2D-Illusion.

**Frage:** Warum reicht ein `sf::CircleShape` allein nicht für einen dreidimensionalen Stein? Was unterscheidet Perspektive, Geometrie, Beleuchtung und Tiefentest?

**Wichtige Einschränkung:** Dieser erste Renderer verwendet klassische OpenGL-Funktionen, keine modernen Vertex-/Fragment-Shader. Das vereinfacht den Schulungsaufbau, ist aber nicht die modernste Grafikpipeline.

## Lerneinheit 4 - Ereignisse, Zustände und Animation

**Datei:** `src/gui/Main3D.cpp`, Methoden `Arena3D::run`, `keyPressed`, `click`, `startDrop`.

- In jedem Frame werden Tastatur, Maus und Fensterereignisse abgefragt.
- Die Kamera reagiert auf rechte Maustaste, Scrollrad und A/D/Q/E.
- Ein Linksklick sucht die nächstgelegene projizierte Spalte.
- `startDrop()` prüft zunächst, ob das Feld einen weiteren Stein erlaubt.
- Die Animation führt den Stein über mehrere Frames von oben nach unten.
- Erst danach bestätigt `MatchSession::drop` den tatsächlichen Spielzug.

**Schlüsselpunkt:** Rendering und Spieleingabe verändern den Zustand nur an definierten Stellen. Mehrfaches Klicken während des Falls darf keinen zweiten Stein erzeugen.

**Frage:** Warum wird die Fallanimation mit `dt` statt mit der Anzahl gezeichneter Bilder fortgesetzt?

## Lerneinheit 5 - Spielstände, UX und Stabilität

**Dateien:** `src/MatchSession.cpp`, `src/gui/Main3D.cpp`, `src/gui/GymBeat.h`.

Erkläre das Format `GYM1`: welche Daten es speichert und warum der Loader das Raster gegen die Zugfolge validiert. Die 3D-Oberfläche hat Bestätigungen, bevor ein aktives Match ersetzt wird. Der Soundtrack ist optional und darf bei fehlendem Audiogerät das Spiel nicht blockieren.

**Selbsttest:** Wie wird ein korruptes Savegame behandelt? Kannst du eine Revanche starten, ohne bisherige Highscores zu verlieren? Warum dürfen Konsole und GUI nicht unkoordiniert gleichzeitig dieselbe Datei überschreiben?

## Lerneinheit 6 - Tests und Vorführung

**Automatische Tests:** `tests/scene3d_tests.cpp` prüft Projektion, Sichtbarkeit, Kameragrenzen und Animation. `tests/match_session_tests.cpp` prüft Spiellogik. CTest und GitHub Actions bauen zusätzlich den 3D-Renderer, aber ein Compiler kann keinen subjektiv guten Klang beweisen.

**Praktischer Lernablauf:**

1. Konsole starten und einen vollständigen Zwei-Spieler-Sieg erklären.
2. Die gleiche Partie in 2D starten und die gemeinsame Spielregel zeigen.
3. 3D starten, Kamera drehen, Stein setzen, Speicherdatei demonstrieren.
4. In VS Code Haltepunkte bei `startDrop` und `MatchSession::drop` setzen.
5. Mindestens einmal Schritt für Schritt durch den Gewinnalgorithmus gehen.
6. Drei Spieler und danach vier Spieler vorführen.
7. Alle manuellen Punkte aus `docs/3D_TESTPLAN.md` protokollieren.
8. Eine eigene zehnminütige Codeerklärung aufnehmen und Fachbegriffe präzise verwenden.

**Fünf typische Prüfungsfragen:** Warum C++17? Warum objektorientiert? Wie entscheidet die Schwerkraft? Wie funktioniert die perspektivische Kamera? Welche Fehlerfälle prüft die CI und welche nicht?

## Qualitätskriterium

**Du hast Teil D wirklich verstanden, wenn du den Weg `Mausklick → Spalte → Animation → Board::drop → Siegprüfung → 3D-Zeichnung` mit eigenem Code und ohne auswendig gelernte Floskeln erklären kannst.** Die ursprüngliche schulische Pflicht bleibt die funktionierende Konsole.

## Qualitätsnachtrag: präzise Kameraauswahl

**Datei:** `src/gui/Gym3DPicking.h`. Bei einem Klick erzeugt das Spiel aus dem Mauspunkt und den Kamerabasis-Vektoren einen Strahl durch die Perspektive. Dieser Strahl wird mit der Ebene des 3D-Bretts geschnitten. Aus dem **Weltkoordinaten-x** der Treffstelle ergibt sich die getroffene Spalte. Nur ein Treffer innerhalb der tatsächlichen Breite und Höhe des Bretts wird akzeptiert.

**Warum ist das wichtig?** Die alte Lösung verglich nur Bildschirm-x mit der obersten Zellenreihe. Bei stark gedrehter Kamera unterscheiden sich die x-Werte zwischen oberen und unteren Zellen. Die neue Lösung verwendet zusätzlich die Höhe des Klickpunkts und den Kamerawinkel.

**Zum Üben:** In `tests/scene3d_tests.cpp` werden Klicks auf sämtliche Zellen von 7×6, 9×8 und 11×10 bei neun Kameraausrichtungen getestet. Erkläre anhand eines Skalarprodukts und der Gleichung für den Schnitt einer Geraden mit einer Ebene, warum ein außerhalb liegender Klick verworfen wird.

**Datensicherheit:** Suche in `Arena3D::act` die Aktion „Sichern und beenden“. Erkläre den Unterschied zwischen erfolgreichem Schreiben und fehlgeschlagenem Schreiben. Demonstriere, warum die Anwendung beim Fehler offen bleiben muss. Während eines fliegenden Steins werden destruktive Navigationsaktionen vorübergehend blockiert.
