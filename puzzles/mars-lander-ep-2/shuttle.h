#ifndef SHUTTLE_H
#define SHUTTLE_H

#include <istream>
#include <iostream>
/*
  Shuttle class
*/
class Shuttle
{
public:
    Shuttle();

    /**
     * Read shuttle data from the specified stream.
     * Input format should be: X >> Y >> hSpeed >> vSpeed >> fuel >> rotate >> power.
     */
    void read_data(std::istream &input = std::cin);

    void set_position(int x, int y)
    {
        m_x = x;
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

    void set_speed(int hspeed, int vspeed)
    {
        m_h_speed = hspeed;
        m_v_speed = vspeed;
    }

    int h_speed() const
    {
        return m_h_speed;
    }

    int v_speed() const
    {
        return m_v_speed;
    }

    void set_angle(int angle)
    {
        m_angle = angle;
    }

    void set_power(int power)
    {
        if (power >= 0 && power <= 4) {
            m_power = power;
        } else {
            throw std::string(__FUNCTION__) + ": invalid power value.";
        }
    }

    void decrease_power()
    {
        if (m_power > 0) {
            m_power--;
        }
    }

    void increase_power()
    {
        if (m_power < 4) {
            m_power++;
        }
    }

    int power() const
    {
        return m_power;
    }

    std::string to_str() const;

private:

    int m_x, m_y;

    int m_h_speed, m_v_speed;

    int m_angle;

    int m_power;
};


#endif
