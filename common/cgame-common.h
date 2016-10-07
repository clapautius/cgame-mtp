#ifndef CGAME_COMMON_H
#define CGAME_COMMON_H

#include <functional>
#include <vector>
#include <math.h>

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
 */
template<class A, class B>
  int orientation(const A &a, const B &b)
{
    double xx = b.x() - a.x();
    double yy = b.y() - a.y();
    int angle = atan(yy / xx) * 180.0 / 3.1415926;
    //std::cerr << "angle: " << angle << std::endl;
    if (xx < 0 && yy < 0) {
        angle += 180;
    } else if (xx < 0 && yy > 0) {
        angle = 90 - angle;
    } else if (xx > 0 && yy < 0) {
        angle = 360 + angle;
    }
    return static_cast<int>(angle);
}


int diff_angle(int angle1, int angle2);

bool unit_tests_common();

#endif
