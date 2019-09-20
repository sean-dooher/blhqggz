#include <test.h>
#include <string.h>

void
test_reverse()
{
    char before[] = "atoi";
    char expected[] = "iota";

    reverse(before, 4);

    assert(strcmp(before, expected) == 0, "Expected before to match");
}

void
run_test()
{
    test_reverse();
}