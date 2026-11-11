#!/usr/bin/env python3
"""Remove promotional slogans from the archived 17-slide presentation.

The script edits only the existing presentation's text, then regenerates its PDF
from the revised PPTX. A copy of the original remains recoverable via Git history.
"""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
from pptx import Presentation
import fitz

ROOT = Path(__file__).resolve().parents[1]
PPTX = ROOT / "praesentation/Vier_Gewinnt_Praesentation_FINAL.pptx"
PDF = ROOT / "praesentation/Vier_Gewinnt_Praesentation_FINAL.pdf"
PATTERN = re.compile(
    r"gold[\s-]*master|gold[\s-]*standard|goldstandard|"
    r"first[\s-]*class|\bdiamond\b|\bultimate\b|10\s*(?:von|/)\s*10",
    re.IGNORECASE,
)

def rewrite(original: str) -> str:
    # Keep the project name and remove the unsupported award-like labels.
    text = re.sub(r"JK AXIS\s*[/|·-]\s*GOLD MASTER", "JK AXIS", original, flags=re.I)
    text = re.sub(r"GOLD MASTER", "", text, flags=re.I)
    text = re.sub(r"GOLD[\s-]*STANDARD", "Qualitätsprüfung", text, flags=re.I)
    text = re.sub(r"FIRST[\s-]*CLASS", "", text, flags=re.I)
    text = re.sub(r"\bDIAMOND\b|\bULTIMATE\b", "", text, flags=re.I)
    text = re.sub(r"10\s*(?:von|/)\s*10", "", text, flags=re.I)
    return re.sub(r"\s{2,}", " ", text).strip() if text != original else original

def iter_paragraphs(shape):
    if shape.has_text_frame:
        yield from shape.text_frame.paragraphs
    if shape.has_table:
        for row in shape.table.rows:
            for cell in row.cells:
                yield from cell.text_frame.paragraphs
    if shape.shape_type == 6:
        for child in shape.shapes:
            yield from iter_paragraphs(child)

def patch_deck(out: Path) -> int:
    deck = Presentation(PPTX)
    assert len(deck.slides) == 17
    changed = 0
    for slide in deck.slides:
        for shape in slide.shapes:
            for paragraph in iter_paragraphs(shape):
                joined = "".join(run.text for run in paragraph.runs)
                if not PATTERN.search(joined):
                    continue
                replacement = rewrite(joined)
                if replacement == joined:
                    continue
                changed += 1
                if paragraph.runs:
                    # Keep the first run's existing font, placement and color.
                    paragraph.runs[0].text = replacement
                    for run in paragraph.runs[1:]:
                        run.text = ""
                else:
                    paragraph.text = replacement
        try:
            frame = slide.notes_slide.notes_text_frame
            if frame:
                for paragraph in frame.paragraphs:
                    for run in paragraph.runs:
                        if PATTERN.search(run.text):
                            run.text = rewrite(run.text)
                            changed += 1
        except Exception:
            pass
    deck.save(out)
    verify = Presentation(out)
    assert len(verify.slides) == 17
    for slide in verify.slides:
        for shape in slide.shapes:
            for paragraph in iter_paragraphs(shape):
                assert not PATTERN.search(paragraph.text), paragraph.text
    return changed

def main() -> None:
    office = shutil.which("libreoffice") or shutil.which("soffice")
    if office is None:
        raise SystemExit("LibreOffice is required to produce a matching PDF; no changes committed")
    with tempfile.TemporaryDirectory(prefix="legacy-ppt-") as td:
        tmp = Path(td)
        deck_path = tmp / PPTX.name
        changed = patch_deck(deck_path)
        if changed == 0:
            print("No promotional text remains in the legacy PPTX")
            return
        command = [
            office, "-env:UserInstallation=file:///tmp/gym-office-legacy",
            "--headless", "--convert-to", "pdf", "--outdir", str(tmp), str(deck_path)
        ]
        subprocess.run(command, check=True, timeout=300, capture_output=True, text=True)
        rendered = tmp / PDF.name
        if not rendered.exists():
            raise RuntimeError("LibreOffice did not create a PDF")
        with fitz.open(rendered) as new_pdf, fitz.open(PDF) as old_pdf:
            if len(new_pdf) != len(old_pdf) or len(new_pdf) != 17:
                raise RuntimeError("Slide/PDF page count mismatch")
            for i, page in enumerate(new_pdf, 1):
                txt = page.get_text("text")
                if PATTERN.search(txt):
                    raise RuntimeError(f"Promotional text on generated PDF page {i}")
                if len(txt.strip()) < 15:
                    raise RuntimeError(f"Unexpectedly little text on PDF page {i}")
                if page.rect.width <= 0 or page.rect.height <= 0:
                    raise RuntimeError(f"Invalid PDF page geometry on page {i}")
        shutil.copy2(deck_path, PPTX)
        shutil.copy2(rendered, PDF)
    print(f"Revised {changed} text paragraphs, regenerated 17-slide PPTX/PDF")

if __name__ == "__main__":
    main()
