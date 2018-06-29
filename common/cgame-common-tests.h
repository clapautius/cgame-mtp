#include <iostream>
#include "cgame-common.h" // :grep-out:

namespace cgame
{
    extern int cgame_test_no;
    extern int cgame_tests_failed;

#define TEST_EQ(X, Y) do                                                \
    { ++cgame::cgame_test_no;                                           \
        if (X != Y) {                                                   \
            std::cerr << "test " << cgame::cgame_test_no << " fail: " << X << "," << Y << std::endl; \
            ++cgame::cgame_tests_failed; };                              \
    } while(0)

#define TEST_TRUE(X) do                                                 \
    { ++cgame::cgame_test_no;                                           \
        if (!(X)) {                                                     \
            std::cerr << "test " << cgame::cgame_test_no << " failed " << std::endl; \
            ++cgame::cgame_tests_failed; };                              \
    } while(0)

    bool unit_tests_common();

}
