#include "bomb.h" // :grep-out:
#include "world.h" // :grep-out:

using namespace std;

/**
 * Pure functional.
 */
vector<Location> Bomb::get_explosion_area(const World &world)
{
    // :fixme: players are also obsacles
    vector<Location> expl_area;

    expl_area.push_back(make_pair(get_x(), get_y()));
    for (int i = 1; i < get_range(); i++) {
        int xx = get_x() + i;
        int yy = get_y();
        if (world.location_valid_p(xx, yy)) {
            EntityType e = world.entity(xx, yy);
            expl_area.push_back(make_pair(xx, yy));
            if (e == EntityBox || e == EntityWall || e == EntityGoodieExtraBomb ||
                e == EntityGoodieExtraRange) {
                break;
            }
        }
    }
    for (int i = 1; i < get_range(); i++) {
        int xx = get_x() - i;
        int yy = get_y();
        if (world.location_valid_p(xx, yy)) {
            EntityType e = world.entity(xx, yy);
            expl_area.push_back(make_pair(xx, yy));
            if (e == EntityBox || e == EntityWall || e == EntityGoodieExtraBomb ||
                e == EntityGoodieExtraRange) {
                break;
            }
        }
    }
    for (int i = 1; i < get_range(); i++) {
        int xx = get_x();
        int yy = get_y() + i;
        if (world.location_valid_p(xx, yy)) {
            EntityType e = world.entity(xx, yy);
            expl_area.push_back(make_pair(xx, yy));
            if (e == EntityBox || e == EntityWall || e == EntityGoodieExtraBomb ||
                e == EntityGoodieExtraRange) {
                break;
            }
        }
    }
    for (int i = 1; i < get_range(); i++) {
        int xx = get_x();
        int yy = get_y() - i;
        if (world.location_valid_p(xx, yy)) {
            EntityType e = world.entity(xx, yy);
            expl_area.push_back(make_pair(xx, yy));
            if (e == EntityBox || e == EntityWall || e == EntityGoodieExtraBomb ||
                e == EntityGoodieExtraRange) {
                break;
            }
        }
    }
#ifdef HYPER_DEBUG
    cerr << "explosion area for bomb(" << get_x() << ", " << get_y() << ")" << endl;
    cerr << "  ";
    for (auto &loc : expl_area) {
        cerr << "(" << loc.first << "," << loc.second << ") ";
    }
    cerr << endl;
#endif

    return expl_area;
}
