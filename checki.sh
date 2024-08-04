#!/bin/bash

# Directories
DIR1="."
DIR2="tests/valid_tests"

# Files to check
FILES=("double_macro.ob" "double_macro.am" "valid_01.am" "valid_01.as" "valid_01.ob" "valid_02.as" "valid_02.am" "valid_02.ob" "valid_02.ent" "valid_02.ext")

# Flag to track if all files are identical
all_identical=true

# Function to compare files
compare_files() {
    file1="$1"
    file2="$2"
    
    if cmp -s "$file1" "$file2"; then
        echo "$file1 and $file2 are identical."
    else
        echo "$file1 and $file2 are not identical."
        all_identical=false
    fi
}

# Compare each file
for file in "${FILES[@]}"; do
    compare_files "$DIR1/$file" "$DIR2/$file"
done

# Print overall result
if $all_identical; then
    echo "Success: All files are identical."
else
    echo "Failure: Some files are not identical."
fi
