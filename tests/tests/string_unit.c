#include <test.h>
#include <string.h>
#include <stdio.h>

void
test_reverse()
{
    char before[] = "atoi";
    char expected[] = "iota";

    printf("Before: %s\n", before);
    reverse(before, 4);

    printf("After: %s\n", before);

    printf("Expected: %s\n", expected);

    assert(strcmp(before, expected) == 0, "Expected before to match");
}

void
run_test()
{
    test_reverse();
}