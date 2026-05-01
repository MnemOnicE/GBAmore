"""
Tests for the GBAmore build system changes.

Covers the PR that migrated from C array generation to standard devkitARM
assembly (grit -fts) for graphics processing. These tests verify:
  - Makefile variable expansion logic for graphics files
  - Removal of legacy CFILES patsubst rules for .png/.bmp
  - Addition of OFILES_SOURCES += rules mapping .png/.bmp -> .o
  - Removal of HFILES entries for graphics files
  - Removal of custom grit %.c %.h pattern rules
  - chipmunk.grit contains the -fts (assembly output) flag
"""

import os
import re
import unittest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MAKEFILE_PATH = os.path.join(REPO_ROOT, "Makefile")
CHIPMUNK_GRIT_PATH = os.path.join(REPO_ROOT, "graphics", "chipmunk.grit")


def read_file(path):
    with open(path, "r") as f:
        return f.read()


class TestChipmunkGrit(unittest.TestCase):
    """Tests for graphics/chipmunk.grit configuration."""

    def setUp(self):
        self.grit_content = read_file(CHIPMUNK_GRIT_PATH)
        self.lines = [
            line.strip()
            for line in self.grit_content.splitlines()
            if line.strip()
        ]

    def test_fts_flag_present(self):
        """The -fts flag must be present so grit emits assembly output."""
        self.assertIn("-fts", self.lines,
                      "chipmunk.grit must contain the -fts flag for assembly output")

    def test_fts_flag_is_standalone(self):
        """The -fts flag must appear as its own line, not embedded in another option."""
        fts_lines = [l for l in self.lines if "-fts" in l]
        self.assertTrue(
            any(l == "-fts" for l in fts_lines),
            "Expected '-fts' to be a standalone directive line in chipmunk.grit"
        )

    def test_4bpp_flag_retained(self):
        """-gB4 (4 bits-per-pixel) setting must be preserved."""
        self.assertIn("-gB4", self.lines)

    def test_transparency_flag_retained(self):
        """-gt transparency flag must be preserved."""
        self.assertIn("-gt", self.lines)

    def test_transparent_color_retained(self):
        """-gTFF00FF (magenta transparent color) must be preserved."""
        self.assertIn("-gTFF00FF", self.lines)

    def test_output_prefix_retained(self):
        """-O chipmunk output prefix must be preserved."""
        self.assertIn("-O chipmunk", self.lines)

    def test_grit_flags_count(self):
        """Verify the expected number of non-comment directives are present."""
        directive_lines = [l for l in self.lines if not l.startswith("#")]
        # Expected: -gB4, -gt, -gTFF00FF, -O chipmunk, -fts = 5 directives
        self.assertEqual(len(directive_lines), 5,
                         f"Expected 5 directives in chipmunk.grit, got {len(directive_lines)}: {directive_lines}")

    def test_no_ftc_flag(self):
        """The old C-array output flag -ftc must NOT be present (replaced by -fts)."""
        self.assertNotIn("-ftc", self.lines,
                         "chipmunk.grit must not contain -ftc; assembly (-fts) is now used")


class TestMakefileOfilesSourcesPngBmp(unittest.TestCase):
    """Tests that OFILES_SOURCES lists .png/.bmp graphics as .o (not .c) files."""

    def setUp(self):
        self.makefile = read_file(MAKEFILE_PATH)

    def test_ofiles_sources_appends_png_as_o(self):
        """OFILES_SOURCES += must map .png files to .o, not .c."""
        self.assertRegex(
            self.makefile,
            r"OFILES_SOURCES\s*\+?=.*patsubst\s+%\.png\s*,\s*%\.o",
            "Makefile must have 'OFILES_SOURCES += ... patsubst %.png,%.o,...' rule"
        )

    def test_ofiles_sources_appends_bmp_as_o(self):
        """OFILES_SOURCES += must map .bmp files to .o, not .c."""
        self.assertRegex(
            self.makefile,
            r"OFILES_SOURCES\s*\+?=.*patsubst\s+%\.bmp\s*,\s*%\.o",
            "Makefile must have 'OFILES_SOURCES += ... patsubst %.bmp,%.o,...' rule"
        )

    def test_ofiles_sources_png_not_c(self):
        """OFILES_SOURCES must not map .png -> .c anywhere."""
        # Ensure no line like: OFILES_SOURCES += $(patsubst %.png,%.c,...)
        matches = re.findall(
            r"OFILES_SOURCES.*patsubst\s+%\.png\s*,\s*%\.c",
            self.makefile
        )
        self.assertEqual(matches, [],
                         "OFILES_SOURCES must not map .png to .c files")

    def test_ofiles_sources_bmp_not_c(self):
        """OFILES_SOURCES must not map .bmp -> .c anywhere."""
        matches = re.findall(
            r"OFILES_SOURCES.*patsubst\s+%\.bmp\s*,\s*%\.c",
            self.makefile
        )
        self.assertEqual(matches, [],
                         "OFILES_SOURCES must not map .bmp to .c files")

    def test_ofiles_sources_png_uses_wildcard_foreach(self):
        """The .png -> .o rule must use the standard wildcard/foreach expansion."""
        self.assertRegex(
            self.makefile,
            r"OFILES_SOURCES\s*\+=.*wildcard.*GRAPHICS.*\*\.png",
            "OFILES_SOURCES += for .png must use wildcard over GRAPHICS directories"
        )

    def test_ofiles_sources_bmp_uses_wildcard_foreach(self):
        """The .bmp -> .o rule must use the standard wildcard/foreach expansion."""
        self.assertRegex(
            self.makefile,
            r"OFILES_SOURCES\s*\+=.*wildcard.*GRAPHICS.*\*\.bmp",
            "OFILES_SOURCES += for .bmp must use wildcard over GRAPHICS directories"
        )


class TestMakefileLegacyCfilesRemoved(unittest.TestCase):
    """Tests that the legacy CFILES patsubst rules for .png/.bmp were removed."""

    def setUp(self):
        self.makefile = read_file(MAKEFILE_PATH)

    def test_cfiles_png_patsubst_removed(self):
        """CFILES must no longer contain a patsubst for .png -> .c."""
        matches = re.findall(
            r"export\s+CFILES\s*\+?=.*patsubst\s+%\.png\s*,\s*%\.c",
            self.makefile
        )
        self.assertEqual(matches, [],
                         "Legacy 'export CFILES += $(patsubst %.png,%.c,...)' must be removed")

    def test_cfiles_bmp_patsubst_removed(self):
        """CFILES must no longer contain a patsubst for .bmp -> .c."""
        matches = re.findall(
            r"export\s+CFILES\s*\+?=.*patsubst\s+%\.bmp\s*,\s*%\.c",
            self.makefile
        )
        self.assertEqual(matches, [],
                         "Legacy 'export CFILES += $(patsubst %.bmp,%.c,...)' must be removed")


class TestMakefileHfilesGraphicsRemoved(unittest.TestCase):
    """Tests that HFILES no longer includes auto-generated headers for graphics."""

    def setUp(self):
        self.makefile = read_file(MAKEFILE_PATH)

    def test_hfiles_no_png_headers(self):
        """HFILES must not reference .png -> .h patsubst (removed since -fts doesn't emit .h)."""
        matches = re.findall(
            r"HFILES.*patsubst\s+%\.png\s*,\s*%\.h",
            self.makefile
        )
        self.assertEqual(matches, [],
                         "HFILES must not map .png files to .h headers")

    def test_hfiles_no_bmp_headers(self):
        """HFILES must not reference .bmp -> .h patsubst."""
        matches = re.findall(
            r"HFILES.*patsubst\s+%\.bmp\s*,\s*%\.h",
            self.makefile
        )
        self.assertEqual(matches, [],
                         "HFILES must not map .bmp files to .h headers")

    def test_hfiles_retains_binfiles_entry(self):
        """HFILES must still list headers derived from BINFILES."""
        self.assertRegex(
            self.makefile,
            r"export\s+HFILES\s*:=.*addsuffix\s+\.h.*BINFILES",
            "HFILES must still contain the BINFILES addsuffix .h rule"
        )

    def test_hfiles_is_single_line_assignment(self):
        """HFILES export should be a compact single-expression assignment (no continuation for graphics)."""
        # The new HFILES line should not have a backslash continuation for
        # graphics-related entries. Check that there's no second patsubst line.
        hfiles_block = re.search(
            r"export\s+HFILES\s*:=(.+?)(?=\nexport|\n\n)",
            self.makefile,
            re.DOTALL
        )
        self.assertIsNotNone(hfiles_block, "Could not locate HFILES assignment block")
        block_text = hfiles_block.group(1)
        self.assertNotIn("patsubst", block_text,
                         "HFILES assignment must not contain patsubst expressions for graphics headers")


class TestMakefileGritRulesRemoved(unittest.TestCase):
    """Tests that the custom grit pattern rules (%.c %.h) were removed from the Makefile."""

    def setUp(self):
        self.makefile = read_file(MAKEFILE_PATH)

    def test_png_grit_rule_removed(self):
        """The '%.c %.h : %.png %.grit' pattern rule must not exist."""
        self.assertNotRegex(
            self.makefile,
            r"%\.c\s+%\.h\s*:\s*%\.png\s+%\.grit",
            "Custom grit rule '%.c %.h : %.png %.grit' must be removed"
        )

    def test_bmp_grit_rule_removed(self):
        """The '%.c %.h : %.bmp %.grit' pattern rule must not exist."""
        self.assertNotRegex(
            self.makefile,
            r"%\.c\s+%\.h\s*:\s*%\.bmp\s+%\.grit",
            "Custom grit rule '%.c %.h : %.bmp %.grit' must be removed"
        )

    def test_grit_echo_command_removed(self):
        """The '@echo grit $<' recipe line associated with the custom rule must be removed."""
        # The old rule had: @echo "grit $<" and @grit $< -fts -o$*
        # These should no longer appear in the Makefile since the rule was deleted.
        # The grit binary invocation is now handled by devkitARM's gba_rules.
        grit_recipe_matches = re.findall(
            r"@grit\s+\$<\s+-fts\s+-o\$\*",
            self.makefile
        )
        self.assertEqual(grit_recipe_matches, [],
                         "Manual '@grit $< -fts -o$*' recipe must be removed from Makefile")

    def test_no_explicit_c_output_from_grit(self):
        """No rule in the Makefile should still generate .c files from grit."""
        # Ensure there's no pattern like '%.c : %.png' or '%.c : %.bmp'
        matches = re.findall(
            r"%\.c\s*:.*%\.(png|bmp)",
            self.makefile
        )
        self.assertEqual(matches, [],
                         "No pattern rules should generate .c files from .png/.bmp via grit")


class TestMakefileOfilesIntegrity(unittest.TestCase):
    """Structural integrity tests for OFILES composition after the migration."""

    def setUp(self):
        self.makefile = read_file(MAKEFILE_PATH)

    def test_ofiles_sources_base_assignment_present(self):
        """The base OFILES_SOURCES := assignment with .cpp/.c/.s must still exist."""
        self.assertRegex(
            self.makefile,
            r"export\s+OFILES_SOURCES\s*:=.*CPPFILES.*CFILES.*SFILES",
            "Base OFILES_SOURCES := must include CPPFILES, CFILES, and SFILES"
        )

    def test_ofiles_includes_ofiles_sources(self):
        """OFILES must combine OFILES_BIN and OFILES_SOURCES."""
        self.assertRegex(
            self.makefile,
            r"export\s+OFILES\s*:=\s*\$\(OFILES_BIN\)\s*\$\(OFILES_SOURCES\)",
            "OFILES must be composed of OFILES_BIN and OFILES_SOURCES"
        )

    def test_ofiles_bin_assignment_present(self):
        """OFILES_BIN must still be defined from BINFILES."""
        self.assertRegex(
            self.makefile,
            r"export\s+OFILES_BIN\s*:=\s*\$\(addsuffix\s+\.o\s*,\s*\$\(BINFILES\)\)",
            "OFILES_BIN must be defined as addsuffix .o over BINFILES"
        )

    def test_graphics_dir_still_in_vpath(self):
        """The GRAPHICS directories must still be part of VPATH for object lookup."""
        # VPATH is a multi-line assignment using backslash continuation;
        # normalise newlines before matching.
        normalised = self.makefile.replace("\\\n", " ")
        self.assertRegex(
            normalised,
            r"VPATH.*GRAPHICS",
            "GRAPHICS directories must appear in VPATH"
        )

    def test_gba_rules_included(self):
        """The devkitARM gba_rules include must be present (provides built-in grit rules)."""
        self.assertRegex(
            self.makefile,
            r'include\s+\$\(DEVKITARM\)/gba_rules',
            "Makefile must include $(DEVKITARM)/gba_rules for built-in grit support"
        )


if __name__ == "__main__":
    unittest.main(verbosity=2)
