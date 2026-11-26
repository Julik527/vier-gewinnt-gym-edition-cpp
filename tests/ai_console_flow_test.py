#!/usr/bin/env python3
"""Windows/Linux: tatsaechlichen Konsolenmenuefluss ohne Schreibzugriffe testen."""
import os
from pathlib import Path
import subprocess
import sys


def require(predicate, message):
    if not predicate:
        raise AssertionError(message)


def main():
    exe = Path(sys.argv[1]).resolve()
    env = dict(os.environ)
    env["NO_COLOR"] = "1"
    # Einfach spielt deterministisch Mitte-zuerst: vier menschliche R in
    # Spalte 1 ergeben vor dem vierten Computerzug einen sicheren Sieg.
    commands = [
        "03", "1",            # Level Einfach
        "04", "JULIAN",       # Nutzername
        "01", "m", "02",     # Neu, Pause, Fortsetzen
        "v4", "",             # Vorschau (keine Brettmutation)
        "t", "u",             # Tipp und Undo vor erstem Zug
        "1", "u",             # Ganzes Zugpaar entfernen
        "1", "1", "1", "1", # Spieler gewinnt in Spalte 1
        "03", "n", "p", "q", # Ergebnis -> Replay -> Zurueck
        "00", "06", "",     # Menue -> Bilanz
        "00",                 # Ordentlicher Exit
    ]
    process = subprocess.run(
        [str(exe)], input="\n".join(commands) + "\n",
        text=True, capture_output=True, timeout=40, env=env,
        cwd=exe.parent,
    )
    output = process.stdout
    require(process.returncode == 0,
            f"Exit-Code {process.returncode}, stderr: {process.stderr}")
    for needed in (
        "KI-ARENA", "SCHWIERIGKEIT", "SPIELERPROFIL", "JULIAN",
        "Partie pausiert", "VORSCHAU", "Tipp: Spalte",
        "Noch kein Zugpaar", "KI-Antwort wurden entfernt", "DU GEWINNST!", "REPLAY",
        "SCHRITT 1 VON", "GESP. PARTIEN   1", "DEINE SIEGE     1",
    ):
        require(needed in output, f"Fehlt in Ausgabe: {needed}")
    require("\x1b[" not in output, "NO_COLOR muss ANSI-Ausgabe deaktivieren")
    # Ungueltige Eingabe, ein unvollstaendiges Match und EOF duerfen nicht haengen.
    short = subprocess.run([str(exe)], input="99\n01\n9\n",
                           text=True, capture_output=True, timeout=10,
                           env=env, cwd=exe.parent)
    require(short.returncode == 0, "EOF nach ungueltigen Zuegen soll normal enden")
    require("Ungueltige Eingabe" in short.stdout,
            "Ungueltige Spalte muss klar gemeldet werden")
    print("KI-CLI: Menue, Pause/Fortsetzen, Name, Vorschau, Sieg, Replay und EOF OK")


if __name__ == "__main__":
    main()
