#!/bin/bash

TRASH_DIR="trash_afl"
mkdir -p "$TRASH_DIR"

# List of important files or directories that should NEVER be moved
WHITELIST=("Makefile" "crash_log.txt" "afl_cleaner.sh" "afllogger.sh" "libft.a" "README.md" "LICENSE" "bin" "obj" "include" "lib" "src")

# Loop over all files in the current directory
find . -maxdepth 1 -type f -print0 | while IFS= read -r -d '' file; do
    base=$(basename "$file")

    # Skip whitelisted files
    if [[ " ${WHITELIST[@]} " =~ " $base " ]]; then
        echo "🟢 Skipping whitelisted file: $base"
        continue
    fi

    # Check for non-ASCII printable characters (anything outside 32-126)
    if echo "$base" | LC_ALL=C grep -q '[^ -~]'; then
        echo "🚮 Moving: $base"
        mv "$file" "$TRASH_DIR/"
    fi
done

