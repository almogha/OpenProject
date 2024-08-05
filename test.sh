#!/bin/bash

rm course_example.am course_example.ob course_example.ent course_example.ext invalid_01.am invalid_02.am
rm double_macro.ob double_macro.am valid_01.am valid_01.ob valid_02.am valid_02.ob valid_02.ent valid_02.ext

./assembler course_example.as
./assembler invalid_01.as
./assembler invalid_02.as

./assembler double_macro.as
./assembler valid_01.as
./assembler valid_02.as

./checkc.sh
./checki.sh

# rm course_example.am course_example.ob course_example.ent course_example.ext invalid_01.am invalid_02.am
# rm double_macro.ob double_macro.am valid_01.am valid_01.ob valid_02.am valid_02.ob valid_02.ent valid_02.ext
