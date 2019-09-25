#pragma once

#define TEST_PASS 0x3333
#define TEST_FAIL 0x5555


#ifdef SIFIVE_U

#define TEST_CTRL_ADDR 0x100000

#else
    
#define TEST_CTRL_ADDR 0x0

#endif

void test_power_off(int status);