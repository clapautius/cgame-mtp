#include <sstream>
#include "shuttle.h" // :grep-out:

Shuttle::Shuttle()
  : m_x(0), m_y(0), m_h_speed(0), m_v_speed(0), m_angle(0), m_power(0)
{}

void Shuttle::read_data(std::istream &input)
{
    int X;
    int Y;
    int hSpeed; // the horizontal speed (in m/s), can be negative.
    int vSpeed; // the vertical speed (in m/s), can be negative.
    int fuel; // the quantity of remaining fuel in liters.
    int rotate; // the rotation angle in degrees (-90 to 90).
    int power; // the thrust power (0 to 4).
    input >> X >> Y >> hSpeed >> vSpeed >> fuel >> rotate >> power; input.ignore();

    set_position(X, Y);
    set_speed(hSpeed, vSpeed);
    set_power(power);
    set_angle(rotate);
}

std::string Shuttle::to_str() const
{
    std::ostringstream ostr;
    ostr << "Shuttle: pos=(" << m_x << ", " << m_y << "; v=(" << m_h_speed << ", "
         << m_v_speed << "); power=" << m_power;
    return ostr.str();
}
