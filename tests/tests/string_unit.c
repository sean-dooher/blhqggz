#include <test.h>
#include <string.h>
#include <stdio.h>

void
test_reverse()
{
    char before[] = "atoi";
    char expected[] = "iota";

    print("Before: ");
    print(before);
    print("\n");

    reverse(before, 4);

    print("After: ");
    print(before);
    print("\n");


    print("Expected: ");
    print(expected);
    print("\n");

    assert(strcmp(before, expected) == 0, "Expected before to match");
}

void
run_test()
{
    test_reverse();
}