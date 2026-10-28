# Schullogo – Bernd-Blindow-Schulen Ravensburg

Das Schulkennzeichen ist **nicht** das JK-AXIS-Projektlogo.

- **Motiv:** Logo der Bernd-Blindow-Schulen, mit Schriftzug und orangem Symbol
- **Urheber des Ausgangsbildes:** Bernd Blindow Gruppe
- **Wikimedia-Original:** https://commons.wikimedia.org/wiki/File:Logo_BBS_standard_quer.jpg
- **Lizenz des Ausgangsbildes:** Creative Commons Attribution-ShareAlike 4.0 International, https://creativecommons.org/licenses/by-sa/4.0/
- **Alternative Bildquelle (verkleinerte Kopie desselben Logos):** https://gesundheitsberufe.de/bildungsanbieter/bernd-blindow-schulen-mannheim/
- **Direkter alternativer Bildhost:** https://storage.googleapis.com/media-ossnar82/media/images/Logo_BBS_standard_quer.max-1200x1200.jpg
- **Bearbeitung/Variante:** Die alternative Bilddatei ist eine bereits verkleinerte Wiedergabe. Das Spiel skaliert die Vorlage proportional, ohne Inhalte oder Farben zu verändern.
- **Hinweis:** Die Verwendung in einem Schulprojekt bedeutet keine offizielle Herausgabe oder Unterstützung durch die Bernd-Blindow-Schulen.

## Installation und HTTP 429

Wenn Wikimedia automatisierte Zugriffe mit **429 Too Many Requests** einschränkt, soll das Skript keine wiederholten Wikimedia-Anfragen erzeugen. Es verwendet zuerst die alternative Kopie und danach höchstens einen Versuch über Wikimedia Commons:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\install-school-logo.ps1
```

Es werden nur Bilddateien akzeptiert, die mindestens 10 KB groß sind und einen JPEG-Dateikopf haben. Für das Wikimedia-Original wird zusätzlich die bekannte SHA-1-Prüfsumme `71bc2f41e221b85e09907cedaeecb861b025b047` kontrolliert. Das unabhängige CI-Prüfskript kontrolliert JPEG-Bildabmessungen und ein plausibles Seitenverhältnis.

Wenn beide Quellen nicht erreichbar sind, öffne den Link zur alternativen Bilddatei im Browser, speichere sie als JPG und übergib den Dateipfad:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\install-school-logo.ps1 -LocalFile "C:\Pfad\zum\Logo.jpg"
```

Die lokale Zieladresse ist `assets/school/bernd-blindow-schulen.jpg`. Die grafische Anwendung prüft den Projektordner und den Ordner der EXE. Der grafische CMake-Build nutzt ebenfalls die alternative Quelle vor Wikimedia und kopiert das Bild neben die EXE. Ohne verfügbares Logo erscheint der Schulname als Text, statt eines gefälschten Symbols.

Die Terminalversion zeigt den Schulnamen nur als Text. Für jede öffentliche Präsentation sind die Vorgaben der Schule zu beachten.
