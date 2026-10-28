# Original-Schullogo – Bernd-Blindow-Schulen Ravensburg

Das hier verwendete Schulkennzeichen ist **nicht** das JK-AXIS-Projektlogo.

- **Werk:** „Logo BBS standard quer.jpg“ (unverändertes Bernd-Blindow-Schulen-Logo)
- **Urheber:** Bernd Blindow Gruppe
- **Quelle:** https://commons.wikimedia.org/wiki/File:Logo_BBS_standard_quer.jpg
- **Lizenz:** Creative Commons Attribution-ShareAlike 4.0 International
- **Lizenzlink:** https://creativecommons.org/licenses/by-sa/4.0/
- **Bearbeitung:** Keine. Das Bild wird lediglich proportional innerhalb der Spieloberfläche skaliert.
- **Hinweis:** Verwendung im Schulprojekt bedeutet nicht, dass die Bernd-Blindow-Schulen die Anwendung herausgeben oder offiziell unterstützen.

## Installation

Das Bild wird nicht als von uns erfundenes Vektorlogo nachgezeichnet. Installiere die echte, entsprechend lizenzierte Bilddatei auf deinem Rechner:

```powershell
.\scripts\install-school-logo.ps1
```

Der **grafische CMake-Build** versucht ebenfalls, die unveränderte Originaldatei automatisch von Wikimedia Commons herunterzuladen, kontrolliert dabei deren SHA-1-Prüfsumme und kopiert sie neben die GUI-EXE in `assets/school/`. Bei einem Offline-Build wird der Schulname als Text gezeigt. Die Anwendung prüft mehrere Asset-Pfade, damit das Logo beim Start aus dem Projektverzeichnis ebenso wie direkt aus dem Build-Ordner sichtbar wird.

Dadurch wird die Datei `assets/school/bernd-blindow-schulen.jpg` erstellt. Beim Start der grafischen Version lädt das Programm sie aus diesem lokalen Pfad. Fehlt das Bild, zeigt die Anwendung stattdessen korrekt den Text „Bernd Blindow Schulen | Ravensburg“. Die Terminalversion zeigt den Schulnamen als Text, keine vorgetäuschte Bilddatei.

Bitte prüfe vor einer Veröffentlichung oder Vorführung, ob die Logoverwendung den Vorgaben deiner Schule entspricht. Für die Präsentation darf das originale Logo nicht willkürlich umgefärbt oder verzerrt werden.
