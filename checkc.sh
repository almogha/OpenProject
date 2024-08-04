#!/bin/bash

# Directories
DIR1="."
DIR2="test"

# Files to check
FILES=("course_example.am" "course_example.ob" "course_example.ent" "course_example.ext" "invalid_01.am" "invalid_02.am")

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
