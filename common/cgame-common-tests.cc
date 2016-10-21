#include <vector>
#include <iostream>
#include <utility>
#include "cgame-common.h" // :grep-out:

using namespace std;

namespace cgame
{

bool unit_tests_common()
{
    static int test_no = 1;
#define TEST_EQ(X, Y) do                                                \
    { ++test_no;                                                        \
        if (X != Y) {                                                   \
            std::cerr << "test " << test_no << " fail: " << X << "," << Y << std::endl; \
            return false; };                                            \
    } while(0)

#define TEST_TRUE(X) do                                                 \
    { ++test_no;                                                        \
        if (!(X)) {                                                     \
            std::cerr << "test " << test_no << " failed " << std::endl; \
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

    // Location.move() tests
    Location loc(0, 0);
    loc.move_towards(50, 0, 10);
    TEST_EQ(loc.x(), 10);
    TEST_EQ(loc.y(), 0);

    loc = Location(0, 0);
    loc.move_towards(50, 10, 50);
    TEST_EQ(loc.x(), 50);
    TEST_EQ(loc.y(), 10);

    loc = Location(0, 0);
    loc.move_towards(5, 5, 2);
    TEST_EQ(loc.x(), 1);
    TEST_EQ(loc.y(), 1);

    loc = Location(0, 0);
    loc.move_towards(0, -5, 3);
    TEST_EQ(loc.x(), 0);
    TEST_EQ(loc.y(), -3);

    loc = Location(0, 0);
    loc.move_towards(-5, -5, 2);
    TEST_EQ(loc.x(), -1);
    TEST_EQ(loc.y(), -1);

    // bfsearch tests
    std::vector<std::vector<int>> dist_matrix;
    std::vector<int> dist_vectors;
    dist_vectors.resize(5);
    for (unsigned i = 0; i < 5; i++) {
        dist_matrix.push_back(dist_vectors);
    }
    std::vector<std::pair<int, int>> moving_coords;
    moving_coords.push_back(make_pair(0, -1));
    moving_coords.push_back(make_pair(0, 1));
    moving_coords.push_back(make_pair(-1, 0));
    moving_coords.push_back(make_pair(1, 0));

    // bfsearch test 1
    bfsearch(dist_matrix, moving_coords,
             [] (int x, int y) -> bool
             { return (!(x == 2 && y == 2)); },
             0, 0);
/*
    std::cout << matrix_to_str(dist_matrix, 3) << std::endl;
*/
    /*
      The result should be like this:
      0   1   2   3   4
      1   2   3   4   5
      2   3  -2   5   6
      3   4   5   6   7
      4   5   6   7   8
    */
    std::vector<int> result_vect_vertical;
    result_vect_vertical = { 0, 1, 2, 3, 4};
    TEST_TRUE(dist_matrix[0] == result_vect_vertical);
    result_vect_vertical = { 2, 3, -2, 5, 6};
    TEST_TRUE(dist_matrix[2] == result_vect_vertical);

    // bfsearch test 2
    dist_matrix.clear();
    for (unsigned i = 0; i < 5; i++) {
        dist_matrix.push_back(dist_vectors);
    }
    bfsearch(dist_matrix, moving_coords,
             [] (int x, int y) -> bool
             { return (!((x == 2 && y == 2) ||
                         (x == 2 && y == 1) ||
                         (x == 2 && y == 0))); },
             0, 0);
/*
    std::cout << matrix_to_str(dist_matrix, 3) << std::endl;
*/
    /*
      The result should be like this:
      0   1  -2   9  10
      1   2  -2   8   9
      2   3  -2   7   8
      3   4   5   6   7
      4   5   6   7   8
    */
    result_vect_vertical = { 0, 1, 2, 3, 4};
    TEST_TRUE(dist_matrix[0] == result_vect_vertical);
    result_vect_vertical = { -2, -2, -2, 5, 6};
    TEST_TRUE(dist_matrix[2] == result_vect_vertical);
    result_vect_vertical = { 10, 9, 8, 7, 8};
    TEST_TRUE(dist_matrix[4] == result_vect_vertical);

    return true;

#undef TEST_EQ
}

}
