#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "cgame-common-tests.h" // :grep-out:
#include "worldmatrix2d.h"

using namespace std;
using namespace cgame;

// unit tests and other debug tests declarations
bool unit_tests();

enum Entities
{
    eFree,
    eWall,
    eGarbage,
    eSwitchButton,
    eSwitchBarrierOn,
    eSwitchBarrierOff
};


class Switch
{
public:
    Switch(int switchx, int switchy, int barrierx, int barriery, bool blocking)
    {
        m_switch_location = Location(switchx, switchy);
        m_barrier_location = Location(barrierx, barriery);
        m_blocking = blocking;
    };

private:
    Location m_switch_location;
    Location m_barrier_location;
    bool m_blocking;
};


class BenderWorld
{
public:
    BenderWorld(unsigned width, unsigned height)
      : m_world_2d(width, height)
    { };

    WorldMatrix2D m_world_2d;

    vector<Switch> m_switches;

    Location m_start;

    Location m_end;
};


/**
 * @return true if ok, false on error.
 */
bool read_world(BenderWorld &world)
{
    for (unsigned i = 0; i < world.m_world_2d.height(); i++) {
        string line;
        getline(cin, line);
        for (unsigned j = 0; j < line.length(); j++) {
            switch (line[j]) {
            case '.':
                world.m_world_2d.set_at(j, i, eFree);
                break;
            case '#':
                world.m_world_2d.set_at(j, i, eWall);
                break;
            case '+':
                world.m_world_2d.set_at(j, i, eGarbage);
                break;
            default:
                cerr << "Invalid input ??" << endl;
                return false;
            }
        }
    }
    int startX;
    int startY;
    cin >> startX >> startY; cin.ignore();
    world.m_start = Location(startX, startY);
    int targetX;
    int targetY;
    cin >> targetX >> targetY; cin.ignore();
    world.m_end = Location(targetX, targetY);
    int switchCount;
    cin >> switchCount; cin.ignore();
    for (int i = 0; i < switchCount; i++) {
        int switchX;
        int switchY;
        int blockX;
        int blockY;
        int initialState; // 1 if blocking, 0 otherwise
        cin >> switchX >> switchY >> blockX >> blockY >> initialState; cin.ignore();
        Switch sw(switchX, switchY, blockX, blockY, initialState);
        world.m_switches.push_back(sw);
        world.m_world_2d.set_at(switchX, switchY, eSwitchButton);
        if (initialState) {
            world.m_world_2d.set_at(blockX, blockY, eSwitchBarrierOn);
        } else {
            world.m_world_2d.set_at(blockX, blockY, eSwitchBarrierOff);
        }
    }
    return true;
}


int cgame_main()
{
    unsigned width;
    unsigned height;
    cin >> width >> height; cin.ignore();
    BenderWorld world(width, height);
    if (!read_world(world)) {
        cerr << ":error: Error reading world" << endl;
        return 1;
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << "RRRRDDDDRRRRUUUULRUUUUUUUUUUUUUDDUDDDDDDDDDDDDDDDDLLLLUUUUUUUULLUULLRRDDRRDDDDDDDDLLLLLLUULLUUUULLUUUUUUL" << endl;
    return 0;
}
