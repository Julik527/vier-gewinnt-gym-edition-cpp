#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/repository_consistency_test.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Prüft die Konsistenz der Dateistruktur und Projektverweise.
# ------------------------------------------------------------

"""Static consistency checks for current setup documentation and GUI structure.

This intentionally makes no claim about human Windows/visual/audio acceptance.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RepositoryConsistencyTests(unittest.TestCase):
    def read(self, name: str) -> str:
        return (ROOT / name).read_text(encoding="utf-8")

    def test_all_three_frontends_exist(self):
        for name in ("src/main.cpp", "src/gui/Main.cpp", "src/gui/Main3D.cpp"):
            with self.subTest(name=name):
                self.assertTrue((ROOT / name).is_file())

    def test_readme_has_current_quickstart(self):
        readme = self.read("README.md")
        self.assertIn("gemeinsam in `main` integriert", readme)
        self.assertNotIn("nur auf dem Entwicklungsbranch", readme)
        self.assertNotIn("Zwei F5-Profile", readme)
        self.assertIn("Drei F5-Profile", readme)
        self.assertIn("docs/RELEASE_ABNAHME.md", readme)

    def test_windows_guide_has_no_obsolete_branch_switch(self):
        guide = self.read("docs/WINDOWS_QUICKSTART.md")
        self.assertIn("git switch main", guide)
        self.assertNotIn("Draft-PR #1", guide)
        self.assertNotIn("git switch --track origin/quality/gameplay-persistence-2026-10-28", guide)
        self.assertIn("Test-Path .\\src\\gui\\Main3D.cpp", guide)

    def test_shared_platform_services(self):
        shared = self.read("src/gui/GymPlatform.h")
        gui2d = self.read("src/gui/Main.cpp")
        gui3d = self.read("src/gui/Main3D.cpp")
        self.assertIn("GetModuleFileNameW", shared)
        self.assertIn("gymui::systemFont()", gui2d)
        self.assertIn("gymui::systemFont()", gui3d)
        self.assertIn("gymui::executablePath(", gui2d)

    def test_open_gl_primitives_are_separated(self):
        header = self.read("src/gui/Gym3DRender.h")
        arena = self.read("src/gui/Main3D.cpp")
        for method in ("cube", "disc", "dumbbellRelief", "ring", "cameraGL"):
            with self.subTest(method=method):
                self.assertIn("inline void " + method + "(", header)
                self.assertIn("using gym3d_render::" + method + ";", arena)
        self.assertNotIn("void cube(", arena)
        self.assertNotIn("MatchSession", header)


if __name__ == "__main__":
    unittest.main(verbosity=2)
