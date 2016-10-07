#include <iostream>
#include "cgame-common.h" // :grep-out:

int diff_angle(int angle1, int angle2)
{
    int diff = abs(angle1 - angle2);
    if (diff > 180) {
        diff = 360 - diff;
    }
    return diff;
}


double angle_deg_to_rad(int degrees)
{
    return degrees * 3.1415926535 / 180.0;
}


bool unit_tests_common()
{
    static int test_no = 1;
#define TEST_EQ(X, Y) do                                                \
    { ++test_no;                                                        \
        if (X != Y) {                                                   \
            std::cerr << "test " << test_no << " fail: " << X << "," << Y << std::endl; \
            return false; };                                            \
    } while(0)

    std::vector<int> v = { 0, 7, -1, 3};
    TEST_EQ(count_if<int>([] (int i) -> bool { return i < 0; }, v), 1);

    TEST_EQ(orientation(Location(0,0), Location(10, 0)), 0);
    TEST_EQ(orientation(Location(0,0), Location(10, 10)), 45);
    TEST_EQ(orientation(Location(0,0), Location(10, -10)), 315);
    TEST_EQ(orientation(Location(0,0), Location(-10, 10)), 135);
    TEST_EQ(orientation(Location(0,0), Location(-10, -10)), 225);

    TEST_EQ(orientation(Location(8004, 774), Location(0, 2000)), 172);
    TEST_EQ(orientation(Location(5499, 4500), Location(4499, 4500)), 180);
    TEST_EQ(orientation(Location(5000, 4500), Location(5000, 5500)), 90);

    TEST_EQ(diff_angle(30, 330), 60);
    TEST_EQ(diff_angle(90, 0), 90);

    return true;

#undef TEST_EQ
}
