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



}
