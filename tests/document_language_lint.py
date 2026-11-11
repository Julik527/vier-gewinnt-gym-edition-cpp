#!/usr/bin/env python3
"""Checks public-facing text for unsupported promotional quality labels.

The historical KI prompt record contains original quotes and is excluded so
that documentary evidence is not retroactively rewritten. Binary PDF and PPTX
files require separate inspection of their editable source documents.
"""
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
PATTERN = re.compile(
    r"gold[\s-]*master|gold[\s-]*standard|goldstandard|"
    r"first[\s-]*class|\bdiamond\b|\bultimate\b|"
    r"10\s*(?:von|/)\s*10",
    re.IGNORECASE,
)
IGNORED = {
    "docs/KI_PROMPT_DOKUMENTATION.md",  # Originalzitate und historische Nachweise
    "docs/CODE_STYLE_JK_AXIS.md",       # Beispiele für unzulässigen Stil
}
violations = []
for folder in [ROOT, ROOT / "docs", ROOT / "praesentation", ROOT / "school"]:
    candidates = folder.glob("*") if folder == ROOT else folder.rglob("*")
    for path in candidates:
        if not path.is_file() or path.suffix not in {".md", ".html"}:
            continue
        relative = path.relative_to(ROOT).as_posix()
        if relative in IGNORED:
            continue
        content = path.read_text(encoding="utf-8")
        for index, line in enumerate(content.splitlines(), 1):
            if PATTERN.search(line):
                violations.append(f"{relative}:{index}: {line.strip()[:120]}")
if violations:
    print("\n".join(violations), file=sys.stderr)
    sys.exit(1)
print("Documentation wording: no promotional labels in active Markdown/HTML")
