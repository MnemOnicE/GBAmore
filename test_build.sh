#!/bin/bash
# Run static analysis tests for the build system (no devkitARM required).
# These tests verify Makefile variable logic and grit configuration without
# actually invoking the compiler or toolchain.
set -e

echo "Running build system tests..."
python3 "$(dirname "$0")/tests/test_build_system.py"
echo "All build system tests passed."
