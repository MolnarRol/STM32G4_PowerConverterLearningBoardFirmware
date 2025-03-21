/*
 * test_util.c
 *
 *  Created on: Mar 21, 2025
 *      Author: molnar
 */
#include "UTIL_private_interface.h"
#include <unity.h>

void setUp(void)
{
    TEST_PRINTF("setup");
}

void tearDown(void) {
    TEST_PRINTF("teardown");
}



void test_2_v(void)
{
    TEST_PRINTF("Output from unit test 2.");
    TEST_ASSERT_TRUE(0);
}

void test_1_v(void)
{
    TEST_PRINTF("Output from unit test 1.");
    for(u32 i = 0; i < 100000000; i++) {};
    TEST_FAIL_MESSAGE("My custom fail msg");
}
