#ifndef CGAME_COMMON_H
#define CGAME_COMMON_H

#include <functional>
#include <vector>
#include <math.h>

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


#endif
