#include <iostream>
#include "cgame-common.h" // :grep-out:

bool unit_tests_common()
{
#define TEST_EQ(X, Y) do                                                \
    { if (X != Y) {                                                     \
            std::cerr << "fail: " << X << "," << Y << std::endl; return false; }; \
    } while(0)

    std::vector<int> v = { 0, 7, -1, 3};
    TEST_EQ(count_if<int>([] (int i) -> bool { return i < 0; }, v), 1);

    TEST_EQ(orientation(Location(0,0), Location(10, 0)), 0);
    TEST_EQ(orientation(Location(0,0), Location(10, 10)), 45);
    TEST_EQ(orientation(Location(0,0), Location(10, -10)), 315);
    TEST_EQ(orientation(Location(0,0), Location(-10, 10)), 135);
    TEST_EQ(orientation(Location(0,0), Location(-10, -10)), 225);

    return true;

#undef TEST_EQ
}
