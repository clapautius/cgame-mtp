#include "cgame-common-tests.h" // :grep-out:

void mars_tests()
{
    TEST_TRUE(true);
}

int main()
{
    cgame::unit_tests_common();

    mars_tests();

    std::cout << "Tests: " << cgame::cgame_test_no << std::endl;
    if (cgame::cgame_tests_failed) {
        std::cout << "Failed tests: " << cgame::cgame_tests_failed << std::endl;
    }
    return (cgame::cgame_tests_failed > 0);
}
