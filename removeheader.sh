#!/bin/bash

# Find all .c and .h files recursively from current directory
find . -type f \( -name "*.c" -o -name "*.h" \) | while read -r file; do
  # Create a backup of the original file (optional but smart)
  cp "$file" "$file.bak"

  # Remove the first 11 lines and overwrite the original file
  tail -n +12 "$file.bak" > "$file"
done

echo "First 11 lines removed from all .c and .h files in current directory and subdirectories."

