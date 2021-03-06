#ifndef HYPERSONIC_UTILS_H
#define HYPERSONIC_UTILS_H

#include <utility>
#include <vector>

enum EntityType
{
    EntityEmpty,
    EntityBox,
    //EntityBombMine,
    EntityBombEnemy,
    EntityMark,
    EntityWall,
    EntityGoodieExtraBomb,
    EntityGoodieExtraRange,
    EntityEnemy
};

enum TargetType
{
    TargetInvalid = -1,
    TargetBomb = 0,
    TargetWalk = 1,
    TargetWalkAndBombs = 2
};

using Location = std::pair<int, int>;

std::vector<std::pair<int, int> > coords_around(int x, int y, bool include_center = false);

#endif
