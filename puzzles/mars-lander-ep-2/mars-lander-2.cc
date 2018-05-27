#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*
  Shuttle class
*/
class Shuttle
{
public:
    Shuttle();

    void set_position(int x, int y)
    {
        m_x = x;
        m_y = y;
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
            cerr << __FUNCTION__ << ": invalid power: " << power << endl;
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

private:

    int m_x, m_y;

    int m_h_speed, m_v_speed;

    int m_angle;

    int m_power;
};


Shuttle::Shuttle()
  : m_x(0), m_y(0), m_h_speed(0), m_v_speed(0), m_angle(0), m_power(0)
{}

/*
  End Shuttle class
*/


void read_data(Shuttle &shuttle)
{
        int X;
        int Y;
        int hSpeed; // the horizontal speed (in m/s), can be negative.
        int vSpeed; // the vertical speed (in m/s), can be negative.
        int fuel; // the quantity of remaining fuel in liters.
        int rotate; // the rotation angle in degrees (-90 to 90).
        int power; // the thrust power (0 to 4).
        cin >> X >> Y >> hSpeed >> vSpeed >> fuel >> rotate >> power; cin.ignore();

        shuttle.set_position(X, Y);
        shuttle.set_speed(hSpeed, vSpeed);
        shuttle.set_power(power);
        shuttle.set_angle(rotate);
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int surfaceN; // the number of points used to draw the surface of Mars.
    cin >> surfaceN; cin.ignore();
    for (int i = 0; i < surfaceN; i++) {
        int landX; // X coordinate of a surface point. (0 to 6999)
        int landY; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        cin >> landX >> landY; cin.ignore();
    }

    Shuttle shuttle;
    // game loop
    while (1) {
        read_data(shuttle);

        cerr << __FUNCTION__ << ": v_speed = " << shuttle.v_speed() << ", power = "
             << shuttle.power() << endl;

        if (shuttle.v_speed() > -20) {
            shuttle.decrease_power();
        } else if (shuttle.v_speed() < -30) {
            shuttle.increase_power();
        }

        // 2 integers: rotate power. rotate is the desired rotation angle (should be 0 for level 1), power is the desired thrust power (0 to 4).
        cout << "0 " << shuttle.power() << endl;
    }
}
