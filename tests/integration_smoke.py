#!/usr/bin/env python3
# JK AXIS | GOLD MASTER
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/integration_smoke.py
# Aufgabe: Prüft das Spiel automatisch mit echten Menü-Eingaben.
# ------------------------------------------------------------

"""Repeatable black-box tests. Only Python's standard library is required."""
import pathlib
import re
import subprocess
import sys
import tempfile


# Startet das Spiel so, als wuerde ein Benutzer die Befehle eintippen.
def run_game(binary: pathlib.Path, directory: pathlib.Path, commands: list[str]) -> str:
    result = subprocess.run(
        [str(binary)],
        input="\n".join(commands) + "\n",
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        # Jeder Test nutzt seinen eigenen Ordner, damit sich Spielstände nicht mischen.
        cwd=directory,
        timeout=20,
        check=False,
        encoding="utf-8",
        errors="replace",
    )
    if result.returncode != 0:
        raise AssertionError(
            f"Exit code {result.returncode}; stderr={result.stderr!r}; stdout={result.stdout[-1200:]!r}"
        )
    if "Programm beendet." not in result.stdout:
        raise AssertionError(f"Missing clean exit. Last output: {result.stdout[-1000:]!r}")
    return result.stdout


# Sucht nach Antworten, die das Spiel bei diesem Test zeigen muss.
def require(text: str, *fragments: str) -> None:
    for fragment in fragments:
        if fragment not in text:
            raise AssertionError(f"Expected {fragment!r}. Output tail: {text[-1800:]!r}")


# Hier testen wir die wichtigsten Abläufe Schritt für Schritt.
def main() -> int:
    if len(sys.argv) != 2:
        print("Usage: integration_smoke.py <path-to-vier-gewinnt>", file=sys.stderr)
        return 2
    binary = pathlib.Path(sys.argv[1]).resolve()
    if not binary.is_file():
        raise FileNotFoundError(binary)

    # Alle Testdateien werden danach automatisch wieder entfernt.
    with tempfile.TemporaryDirectory(prefix="gym-edition-test-") as temp:
        root = pathlib.Path(temp)

        # Bei jeder Spielerzahl muss die passende Feldgröße herauskommen.
        for n, cols, rows in [(2, 7, 6), (3, 9, 8), (4, 11, 10)]:
            work = root / f"players-{n}"
            work.mkdir()
            names = [f"Spieler{index}" for index in range(1, n + 1)]
            output = run_game(binary, work, ["2", str(n), "3", *names, "1", "A", "M", "0"])
            require(output, f"Neues Spiel: {cols} Spalten x {rows} Reihen.", "Zurueck zum Hauptmenue.")

        work = root / "input-undo-preview"
        work.mkdir()
        output = run_game(
            binary, work,
            ["3", "Ada", "Ada", "Ben", "1", "A", "0", "8", "V 1", "U", "1", "U", "M", "0"],
        )
        require(
            output, "Name darf nicht leer/doppelt", "Ungueltige Eingabe.",
            "Nur Vorschau: Feld wurde NICHT veraendert.",
            "Kein Zug vorhanden.", "Letzter Zug entfernt.",
        )

        work = root / "save-load"
        work.mkdir()
        output = run_game(
            binary, work, ["3", "Ada", "Ben", "1", "A", "1", "2", "S", "M", "5", "U", "M", "0"]
        )
        require(output, "Spiel gespeichert.", "Spielstand geladen.", "Letzter Zug entfernt.")
        # Wir schauen auch nach, ob wirklich eine Speicherdatei entstanden ist.
        save = work / "data" / "savegame.txt"
        if not save.is_file() or not save.read_text(encoding="utf-8").startswith("GYM1\n"):
            raise AssertionError("Saved game missing or invalid header")
        # Zusätzlicher Text macht eine Speicherdatei ungültig.
        with save.open("a", encoding="utf-8") as output_file:
            output_file.write("\nTRAILING_GARBAGE\n")
        output = run_game(binary, work, ["5", "0"])
        require(output, "Datei fehlt oder ist ungueltig.")

        work = root / "damaged-file"
        (work / "data").mkdir(parents=True)
        (work / "data" / "savegame.txt").write_text("GYM1\n2 0 BAD\n", encoding="utf-8")
        output = run_game(binary, work, ["5", "0"])
        require(output, "Datei fehlt oder ist ungueltig.")

        work = root / "win-highscore"
        work.mkdir()
        output = run_game(
            binary, work, ["3", "Ada", "Ben", "1", "A", "1", "7", "2", "7", "3", "7", "4", "6", "0"],
        )
        require(output, "GEWONNEN: Ada!", "1. Ada: 1")
        data = work / "data" / "highscore.txt"
        if not data.is_file() or not re.search(r'"Ada"\s+1', data.read_text(encoding="utf-8")):
            raise AssertionError("Win was not saved as highscore")
        output = run_game(binary, work, ["6", "0"])
        require(output, "1. Ada: 1")

    print("Integrationstest: 2/3/4 Spieler, Namen, Eingabe, Undo/Vorschau, "
          "Save/Load, beschädigte Datei, Sieg und persistenter Highscore bestanden.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
