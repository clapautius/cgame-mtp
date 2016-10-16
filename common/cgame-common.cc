#include <iostream>
#include <utility>
#include <deque>
#include "cgame-common.h" // :grep-out:

namespace cgame
{

using std::vector;
using std::function;
using std::pair;
using std::make_pair;

// Location class inmplementation
void Location::move_towards(int x, int y, int steps)
{
    if (steps >= distance(*this, Location(x, y))) {
        m_x = x;
        m_y = y;
    } else {
        double angle = angle_deg_to_rad(orientation(*this, Location(x, y)));
        int xx = steps * cos(angle);
        int yy = steps * sin(angle);
        m_x += xx;
        m_y += yy;
    }
}


void Location::move_towards(const Location &loc, int steps)
{
    return move_towards(loc.x(), loc.y(), steps);
}


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


// time functions

time_point_t new_time_point()
{
    return std::chrono::steady_clock::now();
}

int diff_time_point_ms(time_point_t old_time_point)
{
    double ret = std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::steady_clock::now() - old_time_point).count();
    return ret;
}

// end time functions

// start bfsearch functions

static inline bool bfsearch_coord_is_valid(int x, int y, int width, int height)
{
    return (x >= 0 && y >= 0 && x < width && y < height);
}


static void bfsearch_run(vector<vector<int>> &matrix,
                         const std::vector<std::pair<int, int>> &moving_coords,
                         function<bool (int, int)> is_free,
                         std::deque<pair<int, int>> list, int max_dist)
{
    unsigned width = matrix.size();
    unsigned height = matrix[0].size();
    while (!list.empty()) {
        pair<int, int> &elt = list.front();
        int cur_value = matrix[elt.first][elt.second];
        if ((max_dist == -1) || (max_dist > 0 && cur_value <= max_dist)) {
            for (auto &p : moving_coords) {
                int xx = elt.first + p.first;
                int yy = elt.second + p.second;
                if (bfsearch_coord_is_valid(xx, yy, width, height)) {
                    if (matrix[xx][yy] == -1) {
                        if (is_free(xx, yy)) {
                            matrix[xx][yy] = cur_value + 1;
                            list.push_back(make_pair(xx, yy));
                        } else {
                            matrix[xx][yy] = -2;
                        }
                    }
                }
            }
        }
        list.pop_front();
    }
}


/**
 * First coord. is assumed to be free.
 */
void bfsearch(vector<vector<int>> &matrix,
              const std::vector<std::pair<int, int>> &moving_coords,
              function<bool (int, int)> is_free,
              int cur_x, int cur_y,  int max_dist)
{
    unsigned width = matrix.size();
    if (width == 0) {
        return;
    }
    unsigned height = matrix[0].size();
    for (unsigned i = 0; i < width; i++) {
        fill(matrix[i].begin(), matrix[i].end(), -1);
    }
    if (bfsearch_coord_is_valid(cur_x, cur_y, width, height)) {
        std::deque<pair<int, int>> list;
        matrix[cur_x][cur_y] = 0;
        list.push_back(make_pair(cur_x, cur_y));
        bfsearch_run(matrix, moving_coords, is_free, list, max_dist);
    }
}

// end bfsearch functions


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
