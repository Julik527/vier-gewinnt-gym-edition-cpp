#!/usr/bin/env python3
"""Strenge, reproduzierbare Akzeptanztests gegen Grenzfaelle und kaputte Dateien.

Alle Dateizugriffe finden in einem temporaeren Verzeichnis statt. Vorhandene
Spielstaende werden weder gelesen noch veraendert.
"""
from __future__ import annotations

import pathlib
import subprocess
import sys
import tempfile


def play(binary: pathlib.Path, workdir: pathlib.Path, commands: list[str]) -> str:
    result = subprocess.run(
        [str(binary)],
        input="\n".join(commands) + "\n",
        text=True,
        encoding="utf-8",
        errors="replace",
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        cwd=workdir,
        timeout=30,
        check=False,
    )
    if result.returncode or "Programm beendet." not in result.stdout:
        raise AssertionError(
            f"Spiel beendet sich nicht sauber: rc={result.returncode}, "
            f"stderr={result.stderr[-800:]!r}, stdout={result.stdout[-800:]!r}"
        )
    return result.stdout


def contains(text: str, *fragments: str) -> None:
    for fragment in fragments:
        if fragment not in text:
            raise AssertionError(f"Erwarteter Text fehlt: {fragment!r}\n{text[-1500:]}")


def make_dir(root: pathlib.Path, name: str) -> pathlib.Path:
    directory = root / name
    directory.mkdir(parents=True)
    return directory


def test_invalid_inputs(binary: pathlib.Path, root: pathlib.Path) -> None:
    folder = make_dir(root, "invalid-input")
    # Laenge 25 wird abgewiesen, 24 wird akzeptiert.
    output = play(
        binary, folder,
        [
            "1", "99999999999999999999999999", "2abc", "-1", "2",
            "", "x" * 25, "Ada", "Ada", "Ben",
            "-999999999999999999999999", "weder-zahl-noch-befehl",
            "1", "S", "M", "0", "j",
        ],
    )
    contains(output, "Spiel bereit".upper().replace(" BEREIT", " BEREIT")
             if False else "SPIEL BEREIT",
             "Bitte 2, 3 oder 4 eingeben.",
             "Bitte einen Namen mit 1 bis 24 Zeichen",
             "Dieser Name wird schon verwendet.",
             "Ungueltige Eingabe.", "Spiel gespeichert.")
    if output.count("Bitte 2, 3 oder 4 eingeben.") < 3:
        raise AssertionError("Uebergrosse und fehlerhafte Spielerzahlen nicht abgelehnt")
    if output.count("Ungueltige Eingabe.") < 2:
        raise AssertionError("Uebergrosse und fehlerhafte Spalten nicht abgelehnt")
    saved = (folder / "data" / "savegame.txt").read_text(encoding="utf-8")
    if "GYM1\n2 1 6 7 1\n" not in saved:
        raise AssertionError("Ungueltige Zuege haben den Spielstand veraendert")


def test_full_column(binary: pathlib.Path, root: pathlib.Path) -> None:
    folder = make_dir(root, "full-column")
    output = play(
        binary, folder,
        ["1", "2", "Ada", "Ben", *("1" for _ in range(7)), "S", "M", "0", "j"],
    )
    contains(output, "Spalte ist voll.", "Spiel gespeichert.")
    saved = (folder / "data" / "savegame.txt").read_text(encoding="utf-8")
    if "GYM1\n2 0 6 7 6\n" not in saved:
        raise AssertionError("Ein Stein wurde in eine volle Spalte gesetzt")


def test_corrupted_saves(binary: pathlib.Path, root: pathlib.Path) -> int:
    folder = make_dir(root, "save-mutants")
    play(binary, folder, ["1", "2", "Ada", "Ben", "1", "2", "S", "M", "0", "j"])
    path = folder / "data" / "savegame.txt"
    valid = path.read_text(encoding="utf-8")
    lines = valid.splitlines()
    if len(lines) != 12:
        raise AssertionError(f"Unerwartetes Speicherformat: {len(lines)} Zeilen")
    # GYM1 / Header / zwei Spieler / zwei Zuege / sechs Rasterzeilen
    mutants: dict[str, str] = {
        "falsche-kennung": valid.replace("GYM1", "GYM2", 1),
        "zu-viele-spieler": valid.replace("2 0 6 7 2", "5 0 6 7 2", 1),
        "ungueltiger-spielerindex": valid.replace("2 0 6 7 2", "2 2 6 7 2", 1),
        "falscher-spieler-am-zug": valid.replace("2 0 6 7 2", "2 1 6 7 2", 1),
        "zu-viele-zuege": valid.replace("2 0 6 7 2", "2 0 6 7 43", 1),
        "doppelter-spielername": "\n".join(lines[:3] + ['"Ada" B'] + lines[4:]) + "\n",
        "falsches-spielsymbol": valid.replace('"Ada" R', '"Ada" X', 1),
        "falsche-fallhoehe": valid.replace("5 0 0", "4 0 0", 1),
        "falsche-zugreihenfolge": valid.replace("5 1 1", "5 1 0", 1),
        "manipuliertes-raster": "\n".join(
            lines[:6] + ["R......"] + lines[7:]
        ) + "\n",
        "unerwarteter-anhang": valid + "TRAILING_GARBAGE\n",
        "abgeschnitten": "\n".join(lines[:5]) + "\n",
        "negativer-zug": valid.replace("5 0 0", "-1 0 0", 1),
    }
    for label, tampered in mutants.items():
        if tampered == valid:
            raise AssertionError(f"Testmutant {label} hat die Datei nicht veraendert")
        path.write_text(tampered, encoding="utf-8")
        output = play(binary, folder, ["3", "0"])
        contains(output, "Keine gueltige Speicherdatei gefunden.")
    path.write_text(valid, encoding="utf-8")
    output = play(binary, folder, ["3", "U", "M", "0", "j"])
    contains(output, "Spielstand geladen.", "Letzter Zug entfernt.")
    return len(mutants)


def test_zero_move_save(binary: pathlib.Path, root: pathlib.Path) -> None:
    folder = make_dir(root, "zero-moves")
    output = play(binary, folder, ["1", "3", "Ada", "Ben", "Cleo", "S", "M", "0", "j"])
    contains(output, "Spiel gespeichert.")
    saved = (folder / "data" / "savegame.txt").read_text(encoding="utf-8")
    if "GYM1\n3 0 8 9 0\n" not in saved:
        raise AssertionError("Leerer Drei-Spieler-Spielstand fehlt")
    output = play(binary, folder, ["3", "1", "M", "0", "j"])
    contains(output, "Spielstand geladen.", "TRAININGS-ARENA", "Zurueck zum Hauptmenue.")


def test_bad_highscores(binary: pathlib.Path, root: pathlib.Path) -> None:
    folder = make_dir(root, "corrupt-highscore")
    (folder / "data").mkdir()
    (folder / "data" / "highscore.txt").write_text(
        '"Ada" 3\n"Negative" -1\n"Unclean" 2 extra\n'
        '"Good" 5\nnot-a-record\n',
        encoding="utf-8",
    )
    output = play(binary, folder, ["4", "0"])
    contains(output, "1. Good: 5", "2. Ada: 3")
    if "Negative:" in output or "Unclean:" in output:
        raise AssertionError("Kaputte Highscore-Zeilen uebernommen")


def test_eof(binary: pathlib.Path, root: pathlib.Path) -> None:
    folder = make_dir(root, "eof")
    result = subprocess.run(
        [str(binary)], input="1\n", text=True, encoding="utf-8",
        errors="replace", stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        cwd=folder, timeout=20, check=False,
    )
    if result.returncode != 1 or "Eingabe wurde beendet." not in result.stderr:
        raise AssertionError("Eingabeende muss kontrolliert mit Fehlermeldung abbrechen")


def main() -> int:
    if len(sys.argv) != 2:
        print("Aufruf: integration_adversarial.py <Spiel-Datei>", file=sys.stderr)
        return 2
    binary = pathlib.Path(sys.argv[1]).resolve()
    if not binary.is_file():
        raise FileNotFoundError(binary)
    with tempfile.TemporaryDirectory(prefix="gym-adversarial-") as temp:
        root = pathlib.Path(temp)
        test_invalid_inputs(binary, root)
        test_full_column(binary, root)
        mutated = test_corrupted_saves(binary, root)
        test_zero_move_save(binary, root)
        test_bad_highscores(binary, root)
        test_eof(binary, root)
    print(f"Adversarial-E2E: Eingabegrenzen, volle Spalte, {mutated} "
          "defekte Speicherdateien, leere Partie, Highscore und EOF bestanden.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
