#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/presentation_consistency_test.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Prüft die Konsistenz der Präsentationsunterlagen.
# ------------------------------------------------------------

"""Cross-check the current 8-slide milestone PPTX and PDF.

The files are independently generated from one slide definition. This test
checks that both carry the same visible text and contiguous 10-minute timings.
It does not claim to measure a spoken presentation or verify classroom display.
"""
from __future__ import annotations

from pathlib import Path
import re
import sys
import unicodedata

import fitz
from pptx import Presentation

ROOT = Path(__file__).resolve().parents[1]
PRESENTATION = ROOT / "praesentation/Vier_Gewinnt_Meilenstein_10_Minuten.pptx"
PDF = ROOT / "praesentation/Vier_Gewinnt_Meilenstein_10_Minuten.pdf"
TIME = re.compile(r"(\d{2}):(\d{2})\s*[–-]\s*(\d{2}):(\d{2})")


def compact(text: str) -> str:
    return " ".join(unicodedata.normalize("NFKC", text).split())


def minutes_and_seconds(mm: str, ss: str) -> int:
    sec = int(ss)
    if sec >= 60:
        raise AssertionError("Invalid slide timing seconds")
    return int(mm) * 60 + sec


def validate() -> None:
    slides = Presentation(PRESENTATION)
    with fitz.open(PDF) as pdf:
        if len(slides.slides) != 8 or len(pdf) != 8:
            raise AssertionError("Current presentation must have exactly eight slides/pages")
        end_previous = 0
        slide_count = 0
        for index, (slide, page) in enumerate(zip(slides.slides, pdf), 1):
            slide_count += 1
            text_items = [compact(shape.text) for shape in slide.shapes
                          if shape.has_text_frame and shape.text.strip()]
            pdf_text = compact(page.get_text("text"))
            if len(text_items) < 6 or len(pdf_text) < 100:
                raise AssertionError(f"Slide {index}: unexpectedly little visible content")
            for item in text_items:
                if item not in pdf_text:
                    raise AssertionError(
                        f"Slide {index}: PPTX text missing from PDF: {item[:90]!r}"
                    )
            notes_frame = slide.notes_slide.notes_text_frame
            if notes_frame is None or len(notes_frame.text.strip()) < 120:
                raise AssertionError(f"Slide {index}: speaker notes missing or too short")
            timing_matches = [TIME.search(t) for t in text_items]
            timing_matches = [m for m in timing_matches if m is not None]
            if len(timing_matches) != 1:
                raise AssertionError(f"Slide {index}: expected one clear time range")
            mm_start, ss_start, mm_end, ss_end = timing_matches[0].groups()
            start = minutes_and_seconds(mm_start, ss_start)
            end = minutes_and_seconds(mm_end, ss_end)
            if start != end_previous or end <= start:
                raise AssertionError(f"Slide {index}: discontinuous or invalid timing")
            end_previous = end
            for shape in slide.shapes:
                if shape.left < 0 or shape.top < 0:
                    raise AssertionError(f"Slide {index}: negative position")
                if shape.left + shape.width > slides.slide_width + 20000:
                    raise AssertionError(f"Slide {index}: text or graphic extends right")
                if shape.top + shape.height > slides.slide_height + 20000:
                    raise AssertionError(f"Slide {index}: text or graphic extends below")
            for block in page.get_text("blocks"):
                if block[6] != 0:  # text blocks only
                    continue
                x0, y0, x1, y1 = block[:4]
                if x0 < -1 or y0 < -1 or x1 > page.rect.width + 1 or y1 > page.rect.height + 1:
                    raise AssertionError(f"Slide {index}: PDF text outside page")
        if slide_count != 8 or end_previous != 600:
            raise AssertionError(f"Expected 600 seconds in 8 consecutive slides, got {end_previous}")
        print("PPTX/PDF: eight content-matched slides, speaker notes, "
              "no off-page objects, and contiguous timing from 00:00 to 10:00.")


if __name__ == "__main__":
    try:
        validate()
    except Exception as error:
        print(f"Presentation consistency failed: {error}", file=sys.stderr)
        raise SystemExit(1)
