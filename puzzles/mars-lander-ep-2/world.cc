#include "world.h" // :grep-out:

#include <iostream>

using namespace cgame;

void World::read_world()
{
    int surfaceN; // the number of points used to draw the surface of Mars.
    std::cin >> surfaceN; std::cin.ignore();
    for (int i = 0; i < surfaceN; i++) {
        int landX; // X coordinate of a surface point. (0 to 6999)
        int landY; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        std::cin >> landX >> landY; std::cin.ignore();
        m_land_points.push_back(cgame::Location(landX, landY));
    }
}


std::pair<Location, Location> World::find_flat_land() const
{
    bool first = true;
    Location left, right;
    for (unsigned i = 1; i < m_land_points.size() - 1; i++) {
        if (m_land_points[i].y() == 0) {
            if (first) {
                left = m_land_points[i];
                first = false;
            } else {
                right = m_land_points[i];
                break;
            }
        }
    }
    return std::pair<Location, Location>(left, right);
}
