#ifndef CGAME_COMMON_H
#define CGAME_COMMON_H

#include <functional>
#include <vector>
#include <math.h>
#include <chrono>

// uncomment this to get debug message for the common functions
#define CGAME_DEBUG

#ifdef CGAME_DEBUG
#include <sstream>
#endif

namespace cgame
{

class Location
{
public:
    Location(int x = -1, int y = -1)
      : m_x(x), m_y(y)
    {};

    void set_x(int x)
    {
        m_x = x;
    }

    void set_y(int y)
    {
        m_y = y;
    }

    int x() const
    {
        return m_x;
    }

    int y() const
    {
        return m_y;
    }

    void move_towards(int x, int y, int steps);
    void move_towards(const Location &loc, int steps);

private:
    int m_x;
    int m_y;
};


template<class C>
int count_if(std::function<bool(const C&)> predicate, const std::vector<C> &sequence)
{
    int ret = 0;
    for (auto &item : sequence) {
        if (predicate(item)) {
            ++ret;
        }
    }
    return ret;
}


template<class A, class B>
  int distance(const A &a, const B &b)
{
    double xx = abs(a.x() - b.x());
    double yy = abs(a.y() - b.y());
    return static_cast<int>(sqrt(xx * xx + yy * yy));
}

//#include <iostream>

/**
 * @return the angle of the vector A -> B (degrees).
 * :fixme: to be optimized
 */
template<class A, class B>
  int orientation(const A &a, const B &b)
{
    double xx = b.x() - a.x();
    double yy = b.y() - a.y();
    if (xx == 0) {
        if (yy >= 0) {
            return 90;
        } else {
            return 270;
        }
    }
    if (yy == 0) {
        if (xx >= 0) {
            return 0;
        } else {
            return 180;
        }
    }
    int angle = atan(yy / xx) * 180.0 / 3.1415926;
    //std::cerr << "angle: " << angle << std::endl;
    if (xx < 0 && yy < 0) {
        angle += 180;
    } else if (xx < 0 && yy > 0) {
        angle = 180 + angle;
    } else if (xx > 0 && yy < 0) {
        angle = 360 + angle;
    }
    return static_cast<int>(angle);
}


int diff_angle(int angle1, int angle2);

double angle_deg_to_rad(int degrees);

bool unit_tests_common();


// time functions

using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;

time_point_t new_time_point();

int diff_time_point_ms(time_point_t old_time_point);

// end time functions

/**
 * @param[in] max_dist : -1 = infinite
 *
 * @return Values you'll get in MATRIX:
 *  -1  : not explored
 *  -2  : not free
 * >= 0 : distance from current point
 */
void bfsearch(std::vector<std::vector<int>> &matrix,
              const std::vector<std::pair<int, int>> &moving_coords,
              std::function<bool (int, int)> is_free,
              int cur_x, int cur_y,  int max_dist = -1);


/**
 * @param[in] max_dist : -1 = infinite
 *
 * @return Values you'll get in MATRIX:
 *  -1 : not explored
 *   0 : not free
 *   1 : accesible
 */
void matrix_dfsearch(std::vector<std::vector<int>> &matrix,
                     const std::vector<std::pair<int, int>> &moving_coords,
                     std::function<bool (int, int)> is_free,
                     int cur_x, int cur_y,  int max_depth = -1);


// debug functions
#ifdef CGAME_DEBUG

template<class C>
  std::string matrix_to_str(const std::vector<std::vector<C>> &matrix, int elt_width = -1)
{
    std::ostringstream ostr;
    if (matrix.size() == 0) {
        return "";
    }
    for (unsigned j = 0; j < matrix[0].size(); j++) {
        for (unsigned i = 0; i < matrix.size(); i++) {
            if (elt_width > 0) {
                ostr.width(elt_width);
            }
            ostr << matrix[i][j] << " ";
        }
        ostr << std::endl;
    }
    return ostr.str();
}

#endif // CGAME_DEBUG

} // end cgame namespace

#endif
