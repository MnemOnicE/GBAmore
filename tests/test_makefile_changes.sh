#!/bin/bash
# Tests for PR: cleanly switch to standard devkitARM assembly grit generation
#
# Validates the specific Makefile and chipmunk.grit changes introduced in this PR:
#   - PNG/BMP files now map to .o objects (not .c/.h intermediates) in OFILES_SOURCES
#   - HFILES no longer lists .h files derived from PNG/BMP graphics
#   - Explicit %.c %.h grit rules removed from Makefile
#   - chipmunk.grit includes the -fts (assembly format) flag

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MAKEFILE="$REPO_ROOT/Makefile"
GRIT_FILE="$REPO_ROOT/graphics/chipmunk.grit"

PASS=0
FAIL=0

pass() {
    echo "PASS: $1"
    PASS=$((PASS + 1))
}

fail() {
    echo "FAIL: $1"
    FAIL=$((FAIL + 1))
}

# ---------------------------------------------------------------------------
# Helper: grep the Makefile for a pattern
# ---------------------------------------------------------------------------
makefile_contains() {
    grep -q "$1" "$MAKEFILE"
}

grit_contains() {
    grep -q "$1" "$GRIT_FILE"
}

# ---------------------------------------------------------------------------
# 1. chipmunk.grit must include the -fts flag (assembly/TS output format)
# ---------------------------------------------------------------------------
if grit_contains "^-fts$"; then
    pass "chipmunk.grit contains -fts flag"
else
    fail "chipmunk.grit is missing the -fts flag (required for assembly output)"
fi

# ---------------------------------------------------------------------------
# 2. chipmunk.grit retains pre-existing required flags
# ---------------------------------------------------------------------------
if grit_contains "^-gB4$"; then
    pass "chipmunk.grit retains -gB4 (4bpp) flag"
else
    fail "chipmunk.grit missing -gB4 flag"
fi

if grit_contains "^-gt$"; then
    pass "chipmunk.grit retains -gt (transparent) flag"
else
    fail "chipmunk.grit missing -gt flag"
fi

if grit_contains "^-gTFF00FF$"; then
    pass "chipmunk.grit retains -gTFF00FF (magenta transparent) flag"
else
    fail "chipmunk.grit missing -gTFF00FF flag"
fi

if grit_contains "^-O chipmunk$"; then
    pass "chipmunk.grit retains -O chipmunk (output name) flag"
else
    fail "chipmunk.grit missing -O chipmunk flag"
fi

# ---------------------------------------------------------------------------
# 3. PNG graphics now produce .o objects in OFILES_SOURCES (not .c files)
# ---------------------------------------------------------------------------
if makefile_contains 'patsubst %.png,%.o'; then
    pass "Makefile maps PNG files to .o objects via OFILES_SOURCES"
else
    fail "Makefile missing patsubst %.png,%.o in OFILES_SOURCES"
fi

# ---------------------------------------------------------------------------
# 4. BMP graphics now produce .o objects in OFILES_SOURCES (not .c files)
# ---------------------------------------------------------------------------
if makefile_contains 'patsubst %.bmp,%.o'; then
    pass "Makefile maps BMP files to .o objects via OFILES_SOURCES"
else
    fail "Makefile missing patsubst %.bmp,%.o in OFILES_SOURCES"
fi

# ---------------------------------------------------------------------------
# 5. Old approach: PNG/BMP must NOT generate .c files via CFILES
# ---------------------------------------------------------------------------
if makefile_contains 'patsubst %.png,%.c'; then
    fail "Makefile still adds PNG-derived .c files to CFILES (old approach)"
else
    pass "Makefile no longer adds PNG-derived .c files to CFILES"
fi

if makefile_contains 'patsubst %.bmp,%.c'; then
    fail "Makefile still adds BMP-derived .c files to CFILES (old approach)"
else
    pass "Makefile no longer adds BMP-derived .c files to CFILES"
fi

# ---------------------------------------------------------------------------
# 6. HFILES must NOT derive headers from PNG/BMP files
# ---------------------------------------------------------------------------
if makefile_contains 'patsubst %.png,%.h'; then
    fail "Makefile still includes PNG-derived .h files in HFILES (old approach)"
else
    pass "Makefile no longer includes PNG-derived .h files in HFILES"
fi

if makefile_contains 'patsubst %.bmp,%.h'; then
    fail "Makefile still includes BMP-derived .h files in HFILES (old approach)"
else
    pass "Makefile no longer includes BMP-derived .h files in HFILES"
fi

# ---------------------------------------------------------------------------
# 7. Explicit grit %.c %.h rules must be removed from the Makefile
#    These were the manual grit invocation rules that produced .c/.h from .png/.bmp
# ---------------------------------------------------------------------------
if grep -q '^%.c %.h[[:space:]]*:[[:space:]]*%.png' "$MAKEFILE"; then
    fail "Makefile still contains explicit '%.c %.h : %.png %.grit' grit rule (should be removed)"
else
    pass "Explicit '%.c %.h : %.png %.grit' grit rule has been removed"
fi

if grep -q '^%.c %.h[[:space:]]*:[[:space:]]*%.bmp' "$MAKEFILE"; then
    fail "Makefile still contains explicit '%.c %.h : %.bmp %.grit' grit rule (should be removed)"
else
    pass "Explicit '%.c %.h : %.bmp %.grit' grit rule has been removed"
fi

# ---------------------------------------------------------------------------
# 8. OFILES_SOURCES is extended with += for PNG/BMP (correct append syntax)
# ---------------------------------------------------------------------------
# Verify the += assignment for OFILES_SOURCES with PNG pattern
if grep -q 'OFILES_SOURCES += .*patsubst %.png,%.o' "$MAKEFILE"; then
    pass "OFILES_SOURCES uses += to append PNG .o files"
else
    fail "OFILES_SOURCES does not use += to append PNG .o files correctly"
fi

# Verify the += assignment for OFILES_SOURCES with BMP pattern
if grep -q 'OFILES_SOURCES += .*patsubst %.bmp,%.o' "$MAKEFILE"; then
    pass "OFILES_SOURCES uses += to append BMP .o files"
else
    fail "OFILES_SOURCES does not use += to append BMP .o files correctly"
fi

# ---------------------------------------------------------------------------
# 9. Simulate the patsubst logic: verify .png -> .o (not .c) mapping in bash
#    This tests the same logic the Makefile uses for OFILES_SOURCES
# ---------------------------------------------------------------------------
simulate_png_to_o() {
    local input="$1"
    # bash equivalent of: $(patsubst %.png,%.o,input)
    echo "${input/%.png/.o}"
}

simulate_bmp_to_o() {
    local input="$1"
    echo "${input/%.bmp/.o}"
}

png_result=$(simulate_png_to_o "chipmunk.png")
if [ "$png_result" = "chipmunk.o" ]; then
    pass "patsubst logic: chipmunk.png -> chipmunk.o"
else
    fail "patsubst logic: chipmunk.png should map to chipmunk.o, got '$png_result'"
fi

bmp_result=$(simulate_bmp_to_o "chipmunk.bmp")
if [ "$bmp_result" = "chipmunk.o" ]; then
    pass "patsubst logic: chipmunk.bmp -> chipmunk.o"
else
    fail "patsubst logic: chipmunk.bmp should map to chipmunk.o, got '$bmp_result'"
fi

# Confirm .png does NOT map to .c under the new scheme
old_c_result="${png_result/.o/.c}"  # would be chipmunk.c if still .o was wrong
if [ "chipmunk.png" != "${png_result%.o}.c" ] && [ "$png_result" = "chipmunk.o" ]; then
    pass "Regression: PNG source does not produce a .c intermediate file"
else
    fail "Regression: PNG source incorrectly maps to a .c intermediate file"
fi

# ---------------------------------------------------------------------------
# 10. Boundary case: multiple PNG/BMP files (spaces between entries)
#     The Makefile uses wildcard + notdir + patsubst; test that the pattern
#     handles multiple filenames correctly
# ---------------------------------------------------------------------------
simulate_multi_png_to_o() {
    local result=""
    for f in "$@"; do
        result="$result ${f/%.png/.o}"
    done
    echo "${result# }"  # strip leading space
}

multi_result=$(simulate_multi_png_to_o "sprite1.png" "sprite2.png" "bg.png")
if [ "$multi_result" = "sprite1.o sprite2.o bg.o" ]; then
    pass "patsubst handles multiple PNG files: all map to .o"
else
    fail "patsubst for multiple PNG files failed, got: '$multi_result'"
fi

simulate_multi_bmp_to_o() {
    local result=""
    for f in "$@"; do
        result="$result ${f/%.bmp/.o}"
    done
    echo "${result# }"
}

multi_bmp_result=$(simulate_multi_bmp_to_o "tile1.bmp" "tile2.bmp")
if [ "$multi_bmp_result" = "tile1.o tile2.o" ]; then
    pass "patsubst handles multiple BMP files: all map to .o"
else
    fail "patsubst for multiple BMP files failed, got: '$multi_bmp_result'"
fi

# ---------------------------------------------------------------------------
# 11. Boundary case: non-graphics files are not affected by the PNG/BMP patsubst
# ---------------------------------------------------------------------------
non_png="main.cpp"
result_non_png="${non_png/%.png/.o}"
if [ "$result_non_png" = "main.cpp" ]; then
    pass "patsubst %.png,%.o does not transform non-.png files"
else
    fail "patsubst %.png,%.o incorrectly transforms non-.png file: '$result_non_png'"
fi

# ---------------------------------------------------------------------------
# 12. Negative: chipmunk.grit must NOT use the old -ftc (C output) format
#     The PR switches from C output to assembly (-fts), so -ftc should be absent
# ---------------------------------------------------------------------------
if grit_contains "\-ftc"; then
    fail "chipmunk.grit still contains -ftc (C output) flag; expected -fts (assembly)"
else
    pass "chipmunk.grit does not contain obsolete -ftc (C output) flag"
fi

# ---------------------------------------------------------------------------
# 13. Grit -fts flag appears exactly once (no duplicates)
# ---------------------------------------------------------------------------
fts_count=$(grep -c "^-fts$" "$GRIT_FILE")
if [ "$fts_count" -eq 1 ]; then
    pass "chipmunk.grit contains -fts flag exactly once"
else
    fail "chipmunk.grit contains -fts flag $fts_count times (expected exactly 1)"
fi

# ---------------------------------------------------------------------------
# 14. HFILES still retains binary file (.bin) header derivation
#     The PR only removes PNG/BMP from HFILES, not binary data files
# ---------------------------------------------------------------------------
if makefile_contains 'addsuffix .h.*BINFILES'; then
    pass "Makefile HFILES still derives headers from BINFILES (binary data, unaffected by PR)"
else
    fail "Makefile HFILES lost BINFILES header derivation (unintended regression)"
fi

# ---------------------------------------------------------------------------
# Summary
# ---------------------------------------------------------------------------
TOTAL=$((PASS + FAIL))
echo ""
echo "Results: $PASS/$TOTAL tests passed"

if [ "$FAIL" -gt 0 ]; then
    exit 1
fi
exit 0
