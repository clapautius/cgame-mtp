#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

#define BOOST_ANGLE 25
#define BOOST_DISTANCE 4000


#define ENEMY_CLOSE_FACTOR 3000

using namespace std;

class Location2d
{
public:
    Location2d()
      : m_x(-1), m_y(-1)
    {};

    Location2d(int x, int y)
      : m_x(x), m_y(y)
    {};


    Location2d& operator=(const Location2d &rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
    }


    bool operator==(const Location2d &rhs)
    {
        return (m_x == rhs.m_x && m_y == rhs.m_y);
    }

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


/*
int angle_between(const Location2d &loc1, const Location2d &loc2)
{
}
*/

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


/**
 * @fixme : take into account the enemy
 */
bool use_boost_p(const Agent &me, const Location2d &dest)
{
    bool rc = false;
    static bool boost_used = false;
    static bool skip_first = true;
    if (!boost_used) {
        if (abs(me.get_angle_to_dest()) < BOOST_ANGLE &&
            distance_between(me.get_location(), dest) > BOOST_DISTANCE) {
            if (skip_first) {
                skip_first = false;
            } else {
                boost_used = true;
                rc = true;
            }
        }
    }
    return rc;
}


/**
 * @param[in] new_destination : the destination has changed.
 */
int distance_thrust_func(int distance, bool new_destination)
{
    static const int k_distance_close_1 = 3500;
    static const int k_thrust_close_1 = 85;
    static const int k_distance_close_2 = 3000;
    static const int k_thrust_close_2 = 60;
    static const int k_distance_close_3 = 2000;
    static const int k_thrust_close_3 = 25;
    static const int k_distance_close_4 = 1000;
    static const int k_thrust_close_4 = 35;
    static const int k_thrust_far = 100;
    static const int k_start_thrust = 65;
    int thrust = 100;
    if (new_destination) {
        thrust = k_start_thrust;
    } else if (distance < k_distance_close_4) {
        thrust = k_thrust_close_4;
    } else if (distance < k_distance_close_3) {
        thrust = k_thrust_close_3;
    } else if (distance < k_distance_close_2) {
        thrust = k_thrust_close_2;
    } else if (distance < k_distance_close_1) {
        thrust = k_thrust_close_1;
    } else {
        thrust = k_thrust_far;
    }
    return thrust;
}


/*
bool enemy_close_p(const Agent &me, const Location2d &enemy)
{
    return (distance_between(me.get_location(), enemy) <= ENEMY_CLOSE_FACTOR);
}
*/

void strategy1(const Agent &me, const Location2d &checkpoint, const Location2d &enemy,
               bool same_destination,
               int &dest_x, int &dest_y, int &thrust)
{
    dest_x = checkpoint.get_x();
    dest_y = checkpoint.get_y();
    if (me.get_angle_to_dest() > 90 || me.get_angle_to_dest() < -90) {
        thrust = 20;
    } else {
        thrust = distance_thrust_func(distance_between(me.get_location(), checkpoint),
                                      !same_destination);
    }
#if 0
    if (enemy_close_p(me, enemy)) {
        thrust = static_cast<int>(thrust * 1.5);
        if (thrust > 100) {
            thrust = 100;
        }
    }
#endif

    if (use_boost_p(me, checkpoint)) {
        thrust = -1;
    }
    cerr << ":debug: x: " << dest_x << ", y: " << dest_y << ", same_destination: "
         << same_destination << ", thrust: " << thrust << endl;
}


void game_turn()
{
    static Location2d prev_checkpoint;
    bool same_destination = true;
    Agent *p_me = nullptr;
    Location2d *p_checkpoint = nullptr;
    Location2d *p_enemy = nullptr;
    int checkpoint_dist = 0;
    read_data(&p_me, &p_checkpoint, &checkpoint_dist, &p_enemy);
    int thrust = 0;
    int go_x, go_y;
    if (prev_checkpoint == *p_checkpoint) {
        same_destination = true;
    } else {
        same_destination = false;
    }
    prev_checkpoint = *p_checkpoint;

    strategy1(*p_me, *p_checkpoint, *p_enemy, same_destination, go_x, go_y, thrust);

    if (thrust == -1) {
        cout << go_x << " " << go_y << " " << "BOOST" << endl;
    } else {
        cout << go_x << " " << go_y << " " << thrust << endl;
    }
    delete p_me;
    delete p_checkpoint;
    delete p_enemy;
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
