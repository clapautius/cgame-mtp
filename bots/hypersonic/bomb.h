#ifndef HYPERSONIC_BOMB_H
#define HYPERSONIC_BOMB_H

#include <string>
#include <sstream>
#include "utils.h" // :grep-out:

class World;

class Bomb
{
public:

    Bomb()
      : m_x(-1), m_y(-1)
    {};

    Bomb(int owner, int x, int y, int time_remaining = 8, int range = 3)
      : m_owner(owner), m_x(x), m_y(y), m_time_remaining(time_remaining),
        m_effective_timeout(time_remaining), m_range(range)
    {
    }

    int get_x() const
    {
        return m_x;
    }

    int get_y() const
    {
        return m_y;
    }

    /*
    void set_params(int i1, int i2, int i3, int i4)
    {
        m_x = i1;
        m_y = i2;
        m_owner = i3;
        m_range = i4;
    }
    */

    int timeout() const
    {
        return m_time_remaining;
    }

    int get_range() const
    {
        return m_range;
    }

    int get_owner() const
    {
        return m_owner;
    }

    std::vector<Location> get_explosion_area(const World &world);

    void set_effective_timeout(int timeout)
    {
        m_effective_timeout = timeout;
    }

    int effective_timeout() const
    {
        return m_effective_timeout;
    }

    std::string description() const
    {
        std::ostringstream ostr;
        ostr << "Bomb: loc(" << m_x << "," << m_y << "), owner=" << m_owner
             <<  ", range=" << m_range << ", time_remaining=" << m_time_remaining
             << ", m_effective_time=" << m_effective_timeout << std::endl;
        return ostr.str();
    }

private:

    int m_owner;

    int m_x;

    int m_y;

    int m_time_remaining;

    int m_effective_timeout;

    int m_range;
};

#endif
