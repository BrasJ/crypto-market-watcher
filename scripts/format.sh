#!/usr/bin/env bash
set -e
if ! command -v clang-format &> /dev/null; then
  echo "clang-format not found."
  exit 1
fi
find include src tests -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i
