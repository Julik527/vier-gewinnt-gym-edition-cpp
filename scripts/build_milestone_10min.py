#!/usr/bin/env python3
"""Create the current ten-minute school presentation in PPTX and PDF.

The older 15+15-minute presentation remains an archived reference.
The timing is a rehearsal target, not an assertion of measured speech time.
"""
from pathlib import Path
from textwrap import wrap
from pptx import Presentation
from pptx.util import Inches, Pt
from pptx.dml.color import RGBColor
from pptx.enum.shapes import MSO_SHAPE
from reportlab.pdfgen import canvas
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.pdfbase import pdfmetrics

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "praesentation"
OUT.mkdir(exist_ok=True)
STEM = "Vier_Gewinnt_Meilenstein_10_Minuten"
NAVY, BLUE, LIGHT, WHITE, INK, MUTED = "#142B48", "#2563A6", "#EFF5FA", "#FFFFFF", "#1C2D40", "#5D6D7E"

SLIDES = [
    dict(time="00:00–00:40", secs=40, over="01 / PROJEKT",
         title="Vier Gewinnt | Gym Edition",
         bullets=["C++17-Projekt mit objektorientiertem Konsolenspiel",
                  "Ziel: Regeln, Eingaben und Spielstände nachvollziehbar umsetzen",
                  "Fokus heute: Programmaufbau, zwei Algorithmen und Tests"],
         note="Ich stelle den aktuellen Stand meines C++17-Projekts Vier Gewinnt Gym Edition vor. Das wichtigste Ergebnis ist eine objektorientierte Konsolenanwendung. Ich erkläre den schulischen Pflichtumfang, die Klassen, die Schwerkraft beim Spielzug, die Siegerkennung und den Umgang mit Dateien. Danach zeige ich eine kurze Funktion am laufenden Programm. Die grafischen Varianten sind freiwillige Ergänzungen."),
    dict(time="00:40–01:50", secs=70, over="02 / ANFORDERUNGEN",
         title="Spielfeld und Spielregeln",
         bullets=["2 Personen: 7 Spalten × 6 Zeilen; 3: 9 × 8; 4: 11 × 10",
                  "Ein Symbol fällt in das unterste freie Feld der Spalte",
                  "Vier gleiche Symbole in einer Linie bedeuten einen Sieg",
                  "Volle Spalten und ungültige Eingaben ändern den Zustand nicht"],
         note="Das Programm ist für zwei, drei oder vier Personen vorgesehen. Mit mehr Spielern wird das Raster größer. Ein Zug beginnt mit der Spaltenwahl. Das Programm sucht dort das unterste freie Feld. Ein Sieg liegt vor, wenn vier Symbole waagerecht, senkrecht oder diagonal zusammenhängen. Ist die Spalte voll, wird der Zug abgelehnt. Genauso darf eine ungültige Eingabe nicht zum ungewollten Spielerwechsel führen. Diese Regeln lassen sich einzeln testen."),
    dict(time="01:50–03:10", secs=80, over="03 / ARCHITEKTUR",
         title="Klassen mit klaren Aufgaben",
         bullets=["Player: Name und Spielsymbol",
                  "Board: Raster, Schwerkraft und Gewinnerkennung",
                  "Game: Menüs, Zugreihenfolge, Laden und Speichern",
                  "ConsoleUI: Texte, Spielfeldansicht und Rückmeldungen"],
         note="Ich habe die Konsolenanwendung in mehrere Verantwortlichkeiten aufgeteilt. Player verwaltet Daten eines Spielers. Board enthält das Raster und die eigentlichen Spielregeln. Game organisiert die Partie und die Eingaben. ConsoleUI übernimmt die Darstellung im Terminal. Das hilft beim Testen, weil ich die Board-Regeln unabhängig von Menüs und Farben prüfen kann. Die grafischen Zusatzprogramme verwenden eine MatchSession und denselben Board-Regelkern, aber nicht denselben vollständigen Controller."),
    dict(time="03:10–04:40", secs=90, over="04 / CODE",
         title="So funktioniert ein Spielzug",
         bullets=["Board::nextFreeRow(column) sucht von unten nach oben",
                  "Bei voller oder ungültiger Spalte liefert die Methode -1",
                  "Board::drop setzt das Symbol und meldet den Erfolg",
                  "rowPlaced gibt die tatsächlich belegte Zeile zurück"],
         note="Hier ist der wichtigste Ablauf eines Zuges. nextFreeRow startet am unteren Rand des gewählten Feldes und prüft die Zellen nach oben. Sobald ein Platz frei ist, erhält der Aufrufer die Zeilennummer. Bei einer ungültigen oder bereits vollen Spalte kommt minus eins zurück. drop setzt das Symbol nur dann, wenn der Platz gültig ist. Der Referenzparameter rowPlaced gibt die tatsächliche Zeile zurück. Dadurch kann Game den Zug in der Historie speichern, eine Animation erzeugen und anschließend den Gewinner prüfen."),
    dict(time="04:40–06:10", secs=90, over="05 / ALGORITHMUS",
         title="Gewinnerkennung in vier Achsen",
         bullets=["Waagerecht, senkrecht, diagonal ↗ und diagonal ↘",
                  "Ab dem letzten Stein jeweils in beide Richtungen zählen",
                  "Der letzte Stein selbst wird nur einmal gezählt",
                  "Ab vier zusammenhängenden Symbolen ist die Runde gewonnen"],
         note="Die Methode hasWon muss nicht das ganze Spielfeld wahllos absuchen. Es reicht, den zuletzt gesetzten Stein als Ausgangspunkt zu nehmen. Für jede der vier Achsen zählen wir zunächst in die eine und dann in die entgegengesetzte Richtung. So erkennt das Programm auch eine Viererreihe, bei der der zuletzt gesetzte Stein in der Mitte liegt. Der Ausgangsstein darf bei der Addition nicht doppelt gezählt werden. Wenn in einer Achse mindestens vier gleiche Symbole zusammenkommen, meldet die Methode einen Sieg."),
    dict(time="06:10–07:15", secs=65, over="06 / DATEIEN",
         title="Spielstände sicher verarbeiten",
         bullets=["Speicherformat GYM1 mit Spieler- und Rasterdaten",
                  "Beim Laden zuerst validieren, dann Zustand übernehmen",
                  "Beim Speichern erst temporäre Datei vollständig schreiben",
                  "Einschränkung: kein Schutz vor konkurrierenden Spielprozessen"],
         note="Spielstände werden in einem definierten Textformat mit Kennung GYM1 gespeichert. Beim Laden werden die Daten zuerst geprüft. Der laufende Spielzustand wird erst ersetzt, wenn die Prüfung erfolgreich war. Beim Schreiben erstellt das Programm eine temporäre Datei und ersetzt anschließend die Zieldatei. Das reduziert das Risiko eines beschädigten Spielstands. Es ersetzt jedoch keine Transaktion zwischen zwei gleichzeitig gestarteten Anwendungen und ist keine Garantie gegen jeden Stromausfall."),
    dict(time="07:15–08:50", secs=95, over="07 / VORFÜHRUNG",
         title="Die Konsole kurz demonstrieren",
         bullets=["Spiel mit zwei Personen starten und Spalte auswählen",
                  "Fallrichtung und aktuellen Spieler zeigen",
                  "Ungültige Spalte oder Zugvorschau demonstrieren",
                  "Undo oder Speichern zeigen, abhängig von der Vorführzeit"],
         note="Jetzt zeige ich die Pflichtkonsole. Ich starte eine Partie mit zwei Spielern, wähle eine Spalte und zeige, wo das Symbol landet. Danach demonstriere ich einen Grenzfall, etwa eine ungültige Spaltennummer, oder die Vorschau, bei der noch kein Stein gesetzt wird. Anschließend zeige ich den Undo-Befehl oder das Speichern. Entscheidend ist, dass die Vorführung auf einem bereits getesteten Programm stattfindet. Falls der Live-Start scheitert, kann ich denselben Ablauf im Code und anhand eines vorher geprüften Screenshots erklären."),
    dict(time="08:50–10:00", secs=70, over="08 / TESTS UND GRENZEN",
         title="Was ist nachgewiesen, was offen?",
         bullets=["Automatisiert: Builds sowie Regel- und Integrationstests",
                  "Prüfstatus immer anhand der konkreten Commit-SHA lesen",
                  "Noch offen: echter Schul-PC, Bedienung, Anzeige und Audio",
                  "Nächster Schritt: persönliche Generalprobe mit Stoppuhr"],
         note="Zum Abschluss trenne ich überprüfte Ergebnisse von offenen Punkten. Das Repository verwendet automatisierte Build- und Funktionstests. Deren Aussage gilt jeweils für einen bestimmten Commit und die tatsächlich ausgeführten Prüfungen. Ein erfolgreicher CI-Lauf ist keine Bestätigung einer vollständigen Windows-Vorführung oder einer Schulnote. Vor der Abgabe müssen Programmstart, Eingaben, Grafik und gegebenenfalls Audio am vorgesehenen Rechner getestet werden. Auch die Präsentation selbst muss mit Stoppuhr geübt werden. Vielen Dank.")
]
assert len(SLIDES) == 8 and sum(s["secs"] for s in SLIDES) == 600

def rgb(value):
    return RGBColor.from_string(value[1:])

def shape(slide, x, y, w, h, color):
    s = slide.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(x), Inches(y), Inches(w), Inches(h))
    s.fill.solid()
    s.fill.fore_color.rgb = rgb(color)
    s.line.fill.background()
    return s

def textbox(slide, x, y, w, h, text, size, bold=False, color=INK):
    s = slide.shapes.add_textbox(Inches(x), Inches(y), Inches(w), Inches(h))
    tf = s.text_frame
    tf.word_wrap = True
    tf.margin_left = tf.margin_right = tf.margin_top = tf.margin_bottom = 0
    p = tf.paragraphs[0]
    p.text = text
    p.font.name = "Aptos"
    p.font.size = Pt(size)
    p.font.bold = bold
    p.font.color.rgb = rgb(color)
    return s

prs = Presentation()
prs.slide_width, prs.slide_height = Inches(13.333), Inches(7.5)
for i, item in enumerate(SLIDES, 1):
    sl = prs.slides.add_slide(prs.slide_layouts[6])
    shape(sl, 0, 0, 13.333, .14, BLUE)
    textbox(sl, .78, .42, 10.9, .3, item["over"], 12, True, BLUE)
    textbox(sl, .78, 1.07, 11.9, .8, item["title"], 30, True, NAVY)
    shape(sl, .8, 2.08, 11.75, 3.95, LIGHT)
    for j, bullet in enumerate(item["bullets"]):
        ypos = 2.35 + j * .83
        shape(sl, 1.03, ypos + .12, .12, .12, BLUE)
        textbox(sl, 1.35, ypos, 10.72, .65, bullet, 20, False, INK)
    shape(sl, .8, 6.82, 11.75, .012, "#D8E3ED")
    textbox(sl, .8, 6.96, 9.4, .25, "Vier Gewinnt · C++17 · Meilenstein 19.11.2026", 10, False, MUTED)
    textbox(sl, 10.3, 6.96, 2.2, .25, f'{i:02d}/08  {item["time"]}', 9, True, MUTED)
    notes = sl.notes_slide.notes_text_frame
    if notes is not None:
        notes.text = f'Folie {i}: {item["time"]} (Zielzeit {item["secs"]} Sekunden)\n\n{item["note"]}\n\nDie Zeitangaben sind ein Plan und noch kein gemessenes Probenergebnis.'
pptx_path = OUT / (STEM + ".pptx")
prs.save(pptx_path)
assert len(Presentation(pptx_path).slides) == 8

font_regular = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
font_bold = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
pdfmetrics.registerFont(TTFont("DejaVu", font_regular))
pdfmetrics.registerFont(TTFont("DejaVu-Bold", font_bold))
from reportlab.lib.colors import HexColor
pdf = canvas.Canvas(str(OUT / (STEM + ".pdf")), pagesize=(960, 540),
                    pageCompression=1, title="Vier Gewinnt | Meilenstein | 10 Minuten")
for i, item in enumerate(SLIDES, 1):
    pdf.setFillColor(HexColor(WHITE)); pdf.rect(0, 0, 960, 540, stroke=0, fill=1)
    pdf.setFillColor(HexColor(BLUE)); pdf.rect(0, 530, 960, 10, stroke=0, fill=1)
    pdf.setFillColor(HexColor(BLUE)); pdf.setFont("DejaVu-Bold", 11)
    pdf.drawString(57, 484, item["over"])
    pdf.setFillColor(HexColor(NAVY)); pdf.setFont("DejaVu-Bold", 26)
    pdf.drawString(57, 433, item["title"])
    pdf.setFillColor(HexColor(LIGHT)); pdf.roundRect(57, 110, 846, 287, 11, stroke=0, fill=1)
    for j, bullet in enumerate(item["bullets"]):
        y = 366 - 60 * j
        pdf.setFillColor(HexColor(BLUE)); pdf.rect(79, y - 1, 8, 8, stroke=0, fill=1)
        pdf.setFillColor(HexColor(INK)); pdf.setFont("DejaVu", 16)
        # Each long bullet wraps within the content area.
        words = bullet.split()
        lines = []
        current = ""
        for word in words:
            attempt = (current + " " + word).strip()
            if pdfmetrics.stringWidth(attempt, "DejaVu", 16) < 760:
                current = attempt
            else:
                if current: lines.append(current)
                current = word
        if current: lines.append(current)
        assert len(lines) <= 2, (i, bullet)
        for k, line in enumerate(lines):
            pdf.drawString(102, y - 13*k, line)
    pdf.setStrokeColor(HexColor("#D8E3ED")); pdf.line(57, 75, 903, 75)
    pdf.setFillColor(HexColor(MUTED)); pdf.setFont("DejaVu", 9)
    pdf.drawString(57, 52, "Vier Gewinnt · C++17 · Meilenstein 19.11.2026")
    pdf.drawRightString(903, 52, f'{i:02d}/08  {item["time"]}')
    pdf.showPage()
pdf.save()
print("Created:", pptx_path.name, (OUT / (STEM + ".pdf")).name, "8 slides each")
