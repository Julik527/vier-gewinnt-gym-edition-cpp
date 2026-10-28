#!/usr/bin/env python3
# JK AXIS | GOLD MASTER
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   scripts/rebrand_assets.py
# Aufgabe: Dokumentiert den einmaligen Austausch der alten Projektlogos.
# ------------------------------------------------------------

"""Safe JK AXIS | GOLD MASTER replacement for project presentation and PDFs."""
from pathlib import Path
from io import BytesIO
from PIL import Image, ImageOps, ImageChops, ImageStat, ImageDraw
from pptx import Presentation
import hashlib, fitz, json, os, shutil, subprocess

ROOT=Path(__file__).resolve().parents[1]
PPTX=ROOT/"praesentation/Vier_Gewinnt_Praesentation_FINAL.pptx"
REPORT=ROOT/"docs/BRAND_QA_REPORT.md"
OLD_ID="JK-CD-DE-2026-003"
NEW_ID="JK AXIS | GOLD MASTER"
# Die alte Bildkennung hilft, gezielt das fruehere Logo zu erkennen.
LEGACY_HASH="bffb61e37fae"

def sha(blob):
    return hashlib.sha256(blob).hexdigest()

# Baut eine Version des JK-AXIS-Symbols fuer dunkle Hintergruende.
def new_brand_icon():
    # Precision geometry, derived from the master SVG stored in assets/brand.
    # The dark-background symbol preserves official white/blue/navy colors.
    im=Image.new("RGB",(512,512),(16,30,50))
    d=ImageDraw.Draw(im)
    def polygon(points, color):
        d.polygon([(round(13+x*3.68),round(4+y*3.68)) for x,y in points],fill=color)
    polygon([(17,18),(50,18),(50,75),(77,102),(59,120),(17,80)],"#FFFFFF")
    polygon([(114,18),(114,57),(77,93),(60,75),(88,47),(88,18)],"#79A5FF")
    out=BytesIO()
    im.save(out,format="PNG",optimize=True)
    return out.getvalue()

# Vergleicht zwei Bilder, damit nicht aus Versehen ein anderes Bild ersetzt wird.
def similarity(a,b):
    try:
        a=ImageOps.fit(Image.open(BytesIO(a)).convert("RGB"),(80,80))
        b=ImageOps.fit(Image.open(BytesIO(b)).convert("RGB"),(80,80))
        return sum(ImageStat.Stat(ImageChops.difference(a,b)).mean)/765
    except Exception:
        return 1

# Sucht das alte Logo in den PowerPoint-Folien und ersetzt die Bilddaten.
def update_presentation(new_image):
    presentation=Presentation(PPTX)
    matched={}
    old_image=None
    count=0
    footers=0
    for slide in presentation.slides:
        for sh in slide.shapes:
            if sh.shape_type == 13 and sha(sh.image.blob).startswith(LEGACY_HASH):
                old_image=sh.image.blob
                matched[sh.part.related_part(sh._element.blipFill.blip.rEmbed).partname]=sh.part.related_part(sh._element.blipFill.blip.rEmbed)
                count+=1
            if sh.has_text_frame:
                for paragraph in sh.text_frame.paragraphs:
                    for run in paragraph.runs:
                        if OLD_ID in run.text:
                            run.text=run.text.replace(OLD_ID,NEW_ID)
                            footers+=1
    for part in matched.values():
        part._blob=new_image
    if count:
        tmp=PPTX.with_suffix(".tmp.pptx")
        presentation.save(tmp)
        verification=Presentation(tmp)
        assert len(verification.slides)==17
        assert all(OLD_ID not in sh.text for slide in verification.slides for sh in slide.shapes if sh.has_text_frame)
        assert all(not sha(sh.image.blob).startswith(LEGACY_HASH) for slide in verification.slides for sh in slide.shapes if sh.shape_type==13)
        os.replace(tmp,PPTX)
    return old_image, {"slides":len(presentation.slides),"replaced_logo_occurrences":count,"updated_design_labels":footers}

# Ersetzt nur eingebettete Bilder, die wirklich zum alten Logo passen.
def patch_pdf(path,old_logo,new_logo):
    doc=fitz.open(path)
    pages=len(doc)
    found=[]
    if old_logo is not None:
        checked=set()
        for page in doc:
            for entry in page.get_images(full=True):
                xref=entry[0]
                if xref in checked:continue
                checked.add(xref)
                try:
                    b=doc.extract_image(xref)["image"]
                    similarity_score=similarity(b,old_logo)
                    if similarity_score<0.085:
                        page.replace_image(xref,stream=new_logo)
                        found.append((page.number+1,xref))
                except (ValueError,RuntimeError):
                    continue
    if found:
        tmp=path.with_suffix(".tmp.pdf")
        doc.save(tmp,garbage=4,deflate=True)
        doc.close()
        check=fitz.open(tmp)
        assert len(check)==pages
        check[0].get_pixmap(matrix=fitz.Matrix(0.5,0.5))
        check.close()
        os.replace(tmp,path)
    else:
        doc.close()
    return {"pages":pages,"replaced_logo_images":len(found)}

# Dieses Skript war fuer die einmalige Umstellung der Dokumente gedacht.
def main():
    if not PPTX.is_file():raise FileNotFoundError(PPTX)
    icon=new_brand_icon()
    previous,presentation=update_presentation(icon)
    pdf_report={}
    presentation_pdf=ROOT/"praesentation/Vier_Gewinnt_Praesentation_FINAL.pdf"
    converter=shutil.which("libreoffice") or shutil.which("soffice")
    if presentation["replaced_logo_occurrences"] and converter:
        # Synchronize exported PDF with actual PPTX, including changed footers.
        folder=presentation_pdf.parent/"_brand_export"
        folder.mkdir(exist_ok=True)
        try:
            subprocess.run([converter,"-env:UserInstallation=file:///tmp/jk-gym-lo","--headless","--convert-to","pdf","--outdir",str(folder),str(PPTX)],check=True,timeout=240)
            generated=folder/PPTX.with_suffix(".pdf").name
            if generated.is_file():
                assert len(fitz.open(generated))==len(Presentation(PPTX).slides)
                shutil.move(generated,presentation_pdf)
        finally:
            shutil.rmtree(folder,ignore_errors=True)
    for directory in ("reference","praesentation"):
        folder=ROOT/directory
        if not folder.is_dir():continue
        for path in sorted(folder.glob("*.pdf")):
            pdf_report[path.relative_to(ROOT).as_posix()]=patch_pdf(path,previous,icon)
    lines=[
        "# JK AXIS | GOLD MASTER – Projektbranding",
        "",
        "Automatisierte Prüfung; keine vollständige visuelle oder fachliche Freigabe.",
        "",
        "## Präsentation",
        "",
        "- Folien: "+str(presentation["slides"]),
        "- Ersetzte Logo-Instanzen: "+str(presentation["replaced_logo_occurrences"]),
        "- Ersetzte Designkennzeichnungen: "+str(presentation["updated_design_labels"]),
        "",
        "## PDF-Bestand",
        "",
        "| Datei | Seiten | Ersetzte eingebettete Logos |",
        "|---|---:|---:|"
    ]
    for path,a in pdf_report.items():
        lines.append("| "+path+" | "+str(a["pages"])+" | "+str(a["replaced_logo_images"])+" |")
    lines+=["",
        "Ein Wert von 0 bedeutet, dass kein sicher identifizierbares altes Rasterlogo gefunden wurde. Eventuelle Vektorlogos müssen separat geprüft werden.",
        "",
        "Unverändert: C++-Quellcode, Spiellogik, Tests, Schul- und Projektinhalte.",
        "",
        "## Noch visuell zu prüfen",
        "",
        "- PDF-Seiten auf überlagerte Bildbereiche und alte Vektorlogos kontrollieren.",
        "- PPTX in Microsoft PowerPoint auf Bild- und Schriftumbruch prüfen.",
        "- Dateiverweise, Quellen, Schullogo und Vollständigkeit der Inhalte kontrollieren.",
        ""
    ]
    REPORT.parent.mkdir(exist_ok=True,parents=True)
    REPORT.write_text("\n".join(lines),encoding="utf-8")
    print(json.dumps({"presentation":presentation,"pdfs":pdf_report},ensure_ascii=False,indent=2))
if __name__=="__main__":main()
