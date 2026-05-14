#!/bin/bash
set -e
echo "Running native tests..."
make test
echo "Build verification assumed to compile for GBA"
