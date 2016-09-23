#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

class Location2d
{
public:
    Location2d(int x, int y)
      : m_x(x), m_y(y)
    {};

    int get_x() const
    {
        return m_x;
    }

    int get_y() const
    {
        return m_y;
    }

private:

    int m_x;
    int m_y;
};


/**
 * Computes distance between two locations.
 */
unsigned distance_between(const Location2d &loc1, const Location2d &loc2)
{
    unsigned lx = abs(loc1.get_x() - loc2.get_x());
    unsigned ly = abs(loc1.get_y() - loc2.get_y());
    unsigned ret = sqrt(static_cast<unsigned long>(lx) * lx +
                        static_cast<unsigned long>(ly) * ly);
    return ret;
}


class Agent
{
public:
    Agent(int x, int y, int angle_to_next_dest = 0)
      : m_location(x, y), m_angle_to_next_dest(angle_to_next_dest)
    {};

    int get_angle_to_dest() const
    {
        return m_angle_to_next_dest;
    }

    const Location2d& get_location() const
    {
        return m_location;
    }

private:
    Location2d m_location;
    int m_angle_to_next_dest;
};


/**
 * Read data.
 */
void read_data(Agent **p_me, Location2d **p_checkpoint,
               int *p_checkpoint_dist /* probably not needed? */, 
               Location2d **p_enemy)
{
    int x;
    int y;
    int nextCheckpointX; // x position of the next check point
    int nextCheckpointY; // y position of the next check point
    int nextCheckpointDist; // distance to the next checkpoint
    int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
    cin >> x >> y >> nextCheckpointX >> nextCheckpointY >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
    int opponentX;
    int opponentY;
    cin >> opponentX >> opponentY; cin.ignore();
    *p_checkpoint = new Location2d(nextCheckpointX, nextCheckpointY);
    *p_enemy = new Location2d(opponentX, opponentY);
    *p_me = new Agent(x, y, nextCheckpointAngle);
    *p_checkpoint_dist = nextCheckpointDist;
}


void game_turn()
{
    Agent *p_me = nullptr;
    Location2d *p_checkpoint = nullptr;
    Location2d *p_enemy = nullptr;
    int checkpoint_dist = 0;
    read_data(&p_me, &p_checkpoint, &checkpoint_dist, &p_enemy);
    int thrust = 0;
    if (p_me->get_angle_to_dest() > 90 || p_me->get_angle_to_dest() < -90) {
        thrust = 0;
    } else {
        thrust = 100;
        if (distance_between(p_me->get_location(), *p_checkpoint) < 2500) {
            thrust = 50;
        }
    }

    cout << p_checkpoint->get_x() << " " << p_checkpoint->get_y() << " "
         << thrust << endl;
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    while (1) {
        game_turn();
    }
}
