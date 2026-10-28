#!/usr/bin/env python3
"""Checks the manual test plan structure without external packages."""
from pathlib import Path
import re
import sys


def validate(text: str) -> list[str]:
    errors = []
    if not text.startswith("# Manueller Testplan"):
        errors.append("Testplan must begin with its title, not a table fragment")
    if r"\n" in text:
        errors.append("Literal backslash-n sequence in Markdown")
    lines = text.splitlines()
    seen = []
    headers = 0
    for number, line in enumerate(lines, 1):
        if line.startswith("| ID |"):
            headers += 1
            if number >= len(lines) or not re.fullmatch(r"\|\s*:?-+:?\s*(\|\s*:?-+:?\s*){4}\|", lines[number]):
                errors.append(f"Invalid table separator after line {number}")
        if re.match(r"^\| T\d{2} \|", line):
            cells = [cell.strip() for cell in line.split("|")[1:-1]]
            if len(cells) != 5:
                errors.append(f"Test row at line {number} must have five columns")
            seen.append(cells[0])
        if line.startswith("|") and not line.endswith("|"):
            errors.append(f"Broken table line {number}")
    expected = [f"T{i:02d}" for i in range(1, 48)]
    if seen != expected:
        errors.append("Missing, duplicate or out-of-order test cases T01..T47")
    if headers != 3:
        errors.append("Expected exactly three structured test tables")
    return errors


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    if len(sys.argv) > 1:
        root = Path(sys.argv[1])
    plan = (root / "docs/TESTPLAN.md").read_text(encoding="utf-8")
    # Kontrolliert auch, dass der Validator die urspruenglichen Defekte erkennt.
    examples = [
        "| defektes Tabellenfragment |" + plan,
        plan.replace("## Rundenende", r"\n## Rundenende", 1),
        plan.replace("| T47 |", "| T46 |", 1),
    ]
    for candidate in examples:
        if not validate(candidate):
            print("FAIL: Linter uebersieht einen bekannten Defekt", file=sys.stderr)
            return 1

    failures = validate(plan)
    if failures:
        print("\n".join(f"FAIL: {error}" for error in failures), file=sys.stderr)
        return 1
    print("PASS: Testplan title, line breaks, 47 cases and table structures")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
