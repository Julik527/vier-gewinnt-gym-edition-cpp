#!/usr/bin/env python3
# JK AXIS | GOLD MASTER
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/integration_smoke.py
# Aufgabe: Prueft den Menueablauf und die wichtigsten Funktionen.
# ------------------------------------------------------------

"""Testet das C++-Konsolenspiel ueber echte Tastatureingaben.

Jeder Test benutzt einen eigenen temporaeren Ordner. Dadurch bleiben
persoenliche Spielstaende und Highscores unveraendert.
"""
import pathlib
import re
import subprocess
import sys
import tempfile


# Fuehrt eine Reihe von Eingaben wie bei einer echten Spielrunde aus.
def run_game(binary: pathlib.Path, directory: pathlib.Path, commands: list[str]) -> str:
    result = subprocess.run(
        [str(binary)],
        input="\n".join(commands) + "\n",
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        cwd=directory,
        timeout=25,
        check=False,
        encoding="utf-8",
        errors="replace",
    )
    if result.returncode != 0:
        raise AssertionError(
            f"Exit code {result.returncode}; stderr={result.stderr!r}; "
            f"stdout={result.stdout[-1200:]!r}"
        )
    if "Programm beendet." not in result.stdout:
        raise AssertionError(f"Spiel wurde nicht sauber beendet: {result.stdout[-1200:]!r}")
    return result.stdout


def require(output: str, *expected: str) -> None:
    for fragment in expected:
        if fragment not in output:
            raise AssertionError(
                f"Text fehlt: {fragment!r}. Ausgabe: {output[-1800:]!r}"
            )


def count(output: str, fragment: str, expected: int) -> None:
    found = output.count(fragment)
    if found != expected:
        raise AssertionError(f"{fragment!r}: erwartet {expected}, gefunden {found}")


def main() -> int:
    if len(sys.argv) != 2:
        print("Aufruf: integration_smoke.py <Spiel-Datei>", file=sys.stderr)
        return 2
    binary = pathlib.Path(sys.argv[1]).resolve()
    if not binary.is_file():
        raise FileNotFoundError(binary)

    with tempfile.TemporaryDirectory(prefix="gym-edition-test-") as temp:
        root = pathlib.Path(temp)

        def workdir(name: str) -> pathlib.Path:
            folder = root / name
            folder.mkdir(parents=True)
            return folder

        # Im Hauptmenue soll kein unnoetiges Einstellungs-Untermenue stehen.
        output = run_game(binary, workdir("hauptmenue"), ["2", "5", "0"])
        require(output, "JK AXIS", "GYM EDITION", "HEBEN. SETZEN. SIEGEN.",
                "Neues Spiel", "Spiel fortsetzen", "Spielstand laden",
                "Bestenliste", "Anleitung", "Keine Runde aktiv.", "ANLEITUNG")

        # Mit 1 kommt zuerst die Spielerzahl, danach jeder Name und direkt das Spiel.
        for n, cols, rows in [(2, 7, 6), (3, 9, 8), (4, 11, 10)]:
            names = [f"Spieler{i}" for i in range(1, n + 1)]
            output = run_game(
                binary, workdir(f"spieler-{n}"),
                ["1", str(n), *names, "A", "M", "0", "j"],
            )
            require(output, "SCHRITT 1 VON 2", "SCHRITT 2 VON 2",
                    f"{n} Spieler | {cols} Spalten x {rows} Reihen",
                    "SPIEL BEREIT", "TRAININGS-ARENA",
                    "TRAININGSSTAND", "Zurueck zum Hauptmenue.")
            count(output, "SCHRITT 1 VON 2", 1)
            count(output, "SCHRITT 2 VON 2", 1)
            for i, name in enumerate(names):
                require(output, f"Spieler {i+1} [", name)

        # Falsche Spieleranzahl, leere Namen und doppelte Namen abfangen.
        output = run_game(
            binary, workdir("eingabe-und-undo"),
            ["1", "8", "abc", "2", "", "Ada", "Ada", "Ben",
             "A", "0", "8", "V 1", "U", "1", "U", "M", "0", "j"],
        )
        require(output, "Bitte 2, 3 oder 4 eingeben.",
                "Bitte einen Namen mit 1 bis 24 Zeichen",
                "Dieser Name wird schon verwendet.",
                "Ungueltige Eingabe.",
                "Nur Vorschau: Feld wurde NICHT veraendert.",
                "Kein Zug vorhanden.", "Letzter Zug entfernt.")

        # Eine Vorschau muss bis zur naechsten Eingabe stehen bleiben.
        preview_at = output.index("Nur Vorschau: Feld wurde NICHT veraendert.")
        next_input_at = output.index("Spalte oder Befehl", preview_at)
        if "TRAININGS-ARENA" in output[preview_at:next_input_at]:
            raise AssertionError("Vorschau wurde sofort wieder geloescht.")

        # ESC- und andere Steuerzeichen duerfen nicht als Namen ins Terminal kommen.
        output = run_game(
            binary, workdir("sichere-namen"),
            ["1", "2", "\x1b[2J", "Ada", "Ben", "M", "0", "j"],
        )
        require(output, "Bitte einen Namen mit 1 bis 24 Zeichen", "SPIEL BEREIT")

        # Vier lange Spielernamen muessen im Statusbereich einzeln lesbar sein.
        long_names = ["Spieler_" + ("A" * 16), "Spieler_" + ("B" * 16),
                      "Spieler_" + ("C" * 16), "Spieler_" + ("D" * 16)]
        output = run_game(
            binary, workdir("lange-namen"),
            ["1", "4", *long_names, "M", "0", "j"],
        )
        for name in long_names:
            require(output, name)
        if any(len(line) > 80 for line in output.splitlines()):
            raise AssertionError("Terminalzeilen breiter als 80 Zeichen.")



        # Beim Abbrechen darf nicht aus Versehen eine Partie starten.
        output = run_game(binary, workdir("abbruch-anzahl"), ["1", "0", "0"])
        require(output, "Spielstart abgebrochen.")
        if "SPIEL BEREIT" in output:
            raise AssertionError("Spiel trotz Abbruch gestartet")
        output = run_game(binary, workdir("abbruch-name"), ["1", "2", "0", "0"])
        require(output, "Spielstart abgebrochen.")

        # Ein laufendes Spiel soll ueber das Hauptmenue fortgesetzt werden koennen.
        output = run_game(
            binary, workdir("fortsetzen"),
            ["1", "2", "Ada", "Ben", "A", "1", "M", "2",
             "U", "M", "0", "j"],
        )
        require(output, "Deine Runde wird fortgesetzt.",
                "Letzter Zug entfernt.")

        # Abgelehnter Neustart bzw. abgebrochene Einrichtung behalten die alte Runde.
        output = run_game(
            binary, workdir("abbruch-laufende-runde"),
            ["1", "2", "Ada", "Ben", "A", "1", "M",
             "1", "n", "1", "j", "2", "0",
             "2", "U", "M", "0", "j"],
        )
        require(output, "Die bisherige Runde bleibt erhalten.",
                "Spielstart abgebrochen.", "Deine Runde wird fortgesetzt.",
                "Letzter Zug entfernt.")

        # Speichern, Laden und ein anschliessendes Undo testen.
        save_dir = workdir("speichern-laden")
        output = run_game(
            binary, save_dir,
            ["1", "2", "Ada", "Ben", "A", "1", "2", "S", "M",
             "3", "j", "U", "M", "0", "j"],
        )
        require(output, "Spiel gespeichert.", "Spielstand geladen.",
                "Letzter Zug entfernt.")
        save = save_dir / "data" / "savegame.txt"
        if not save.is_file() or not save.read_text(encoding="utf-8").startswith("GYM1\n"):
            raise AssertionError("Gueltige Speicherdatei fehlt.")

        # Eine fehlgeschlagene Sicherung darf die gueltige Datei nicht zerstoeren.
        original = save.read_bytes()
        blocked_temp = save_dir / "data" / "savegame.txt.tmp"
        blocked_temp.mkdir()
        try:
            output = run_game(
                binary, save_dir,
                ["1", "2", "Ada", "Ben", "1", "S", "M", "0", "j"],
            )
            require(output, "Speichern fehlgeschlagen.")
            if save.read_bytes() != original:
                raise AssertionError("Ein fehlgeschlagener Save hat den alten Stand ersetzt.")
        finally:
            blocked_temp.rmdir()

        # Gleiches Prinzip auch fuer die langfristig gespeicherten Siege.
        safe_score_dir = workdir("highscore-sicher")
        (safe_score_dir / "data").mkdir()
        previous_score = b'"Ada" 2\n'
        high_file = safe_score_dir / "data" / "highscore.txt"
        high_file.write_bytes(previous_score)
        blocked_high_temp = safe_score_dir / "data" / "highscore.txt.tmp"
        blocked_high_temp.mkdir()
        try:
            output = run_game(
                binary, safe_score_dir,
                ["1", "2", "Ada", "Ben", "1", "7", "2", "7",
                 "3", "7", "4", "0", "0"],
            )
            require(output, "GEWONNEN: Ada!")
            if high_file.read_bytes() != previous_score:
                raise AssertionError("Fehlgeschlagenes Highscore-Speichern zerstoerte Datei.")
        finally:
            blocked_high_temp.rmdir()

        # Manipulierte Speicherdateien muessen vollstaendig abgelehnt werden.
        with save.open("a", encoding="utf-8") as stream:
            stream.write("\nTRAILING_GARBAGE\n")
        output = run_game(binary, save_dir, ["3", "0"])
        require(output, "Keine gueltige Speicherdatei")

        bad_dir = workdir("defekte-datei")
        (bad_dir / "data").mkdir()
        (bad_dir / "data" / "savegame.txt").write_text(
            "GYM1\n2 0 BAD\n", encoding="utf-8"
        )
        output = run_game(binary, bad_dir, ["3", "0"])
        require(output, "Keine gueltige Speicherdatei")

        # Hilfe, Info und ungueltige Eingaben sollen das Feld nicht staendig neu zeichnen.
        output = run_game(
            binary, workdir("ruhige-anzeige"),
            ["1", "2", "Ada", "Ben", "H", "I", "A", "xyz", "M", "0", "j"],
        )
        count(output, "|  .  ", 42)  # Genau ein einziges leeres 7x6-Feld.

        # Zwei Siege hintereinander, ohne neue Namen einzugeben.
        win_dir = workdir("revanche-highscore")
        winning_moves = ["1", "7", "2", "7", "3", "7", "4"]
        output = run_game(
            binary, win_dir,
            ["1", "2", "Ada", "Ben", *winning_moves,
             "1", *winning_moves, "0", "4", "0"],
        )
        require(output, "GEWONNEN: Ada!", "RUNDENENDE",
                "Revanche (gleiche Spieler)", "REVANCHE", "1. Ada: 2",
                "GESAMTSIEGE", "Vier Hanteln in einer Reihe!")
        count(output, "GEWONNEN: Ada!", 2)
        count(output, " {R} ", 8)  # 4 markierte Hanteln in beiden Siegrunden.
        count(output, "SCHRITT 1 VON 2", 1)
        count(output, "SCHRITT 2 VON 2", 1)
        highscore = win_dir / "data" / "highscore.txt"
        if not highscore.is_file() or not re.search(
            r'"Ada"\s+2', highscore.read_text(encoding="utf-8")
        ):
            raise AssertionError("Zwei Siege fehlen in der Highscore-Datei.")
        output = run_game(binary, win_dir, ["4", "0"])
        require(output, "1. Ada: 2")

        # Nach dem Ergebnis kann man stattdessen ganz neue Spieler einrichten.
        output = run_game(
            binary, workdir("neustart-nach-sieg"),
            ["1", "2", "Ada", "Ben", *winning_moves,
             "2", "3", "Cleo", "Dana", "Eli", "M", "0", "j"],
        )
        require(output, "GEWONNEN: Ada!", "SCHRITT 1 VON 2",
                "3 Spieler | 9 Spalten x 8 Reihen", "Cleo", "Dana", "Eli")
        count(output, "SCHRITT 1 VON 2", 2)

        # Falsche Eingabe beim Ergebnisbildschirm nicht als neue Aktion behandeln.
        output = run_game(
            binary, workdir("result-input"),
            ["1", "2", "Ada", "Ben", *winning_moves, "5", "x", "0", "0"],
        )
        require(output, "Bitte 0, 1 oder 2 eingeben.", "Programm beendet.")

    print("Integration: Gym-Arena, 2-4 Spieler, ruhige Anzeige, "
          "Gewinnmarkierung, Revanche, Save/Load und Highscore bestanden.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
