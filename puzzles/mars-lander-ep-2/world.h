#ifndef WORLD_H
#define WORLD_H

#include <utility>

#include "cgame-common.h" // :grep-out:

class World
{
public:

    void read_world();

    std::pair<cgame::Location, cgame::Location> find_flat_land() const;

private:

    std::vector<cgame::Location> m_land_points;
};

#endif
