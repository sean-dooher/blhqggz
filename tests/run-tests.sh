#!/bin/bash

if [ "$TESTS" = "all" ]; then
    TESTS=$TEST_LIST
fi

RED="\e[31m"
GREEN="\e[32m"
WHITE="\e[97m"
DELIM="==============================="

let total=0
let n_passed=0

echo $DELIM
echo "STARTING TESTS"
echo $DELIM

for test in $TESTS; do
    let total++
    if (ulimit -t 5; make run TARGET=test/$test.elf 1>/dev/null); then
        let n_passed++
        echo -e "$GREEN""PASS: $test""$WHITE"
    else
        echo -e "$RED""FAIL: $test""$WHITE"
    fi
done

echo $DELIM
echo "SUMMARY: $n_passed / $total tests passed"
echo $DELIM
