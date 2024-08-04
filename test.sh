#!/bin/bash

./assembler course_example.as
./assembler invalid_01.as
./assembler invalid_02.as

./checkc.sh
./checki.sh