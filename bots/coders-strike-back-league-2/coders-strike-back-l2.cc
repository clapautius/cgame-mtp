#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <stdlib.h>



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
    static const int k_boost_angle = 20;
    static const int k_boost_distance = 6000;
    bool rc = false;
    static bool boost_used = false;
    static bool skip_first = true;
    if (!boost_used) {
        if (abs(me.get_angle_to_dest()) < k_boost_angle &&
            distance_between(me.get_location(), dest) > k_boost_distance) {
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
    static const int k_thrust_close_1 = 90;
    static const int k_distance_close_2 = 3000;
    static const int k_thrust_close_2 = 75;
    static const int k_distance_close_3 = 2000;
    static const int k_thrust_close_3 = 40;
    static const int k_distance_close_4 = 1000;
    static const int k_thrust_close_4 = 25;
    static const int k_thrust_far = 100;
    static const int k_start_thrust = 40;
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


bool enemy_close_p(const Agent &me, const Location2d &enemy)
{
    static const int k_enemy_close_factor =  2500;
    return (distance_between(me.get_location(), enemy) <= k_enemy_close_factor);
}


/**
 * @return true if I'm very close to the checkpoint.
 */
bool checkpoint_close_p(int distance)
{
    distance < 1200;
}


void strategy1(const Agent &me, const Location2d &checkpoint, const Location2d &enemy,
               bool same_destination,
               int &dest_x, int &dest_y, int &thrust)
{
    dest_x = checkpoint.get_x();
    dest_y = checkpoint.get_y();
    if (me.get_angle_to_dest() > 135 || me.get_angle_to_dest() < -135) {
        thrust = 0;
    } else if (me.get_angle_to_dest() > 90 || me.get_angle_to_dest() < -90) {
        thrust = 15;
    } else {
        thrust = distance_thrust_func(distance_between(me.get_location(), checkpoint),
                                      !same_destination);
    }

    // attack
    if (enemy_close_p(me, enemy) && (rand() % 100 < 25)) {
        cerr << ":debug: attack" << endl;
        thrust = 100;
        dest_x = enemy.get_x();
        dest_y = enemy.get_y();
    }


    if (use_boost_p(me, checkpoint)) {
        thrust = -1;
    }
    cerr << ":debug: x: " << dest_x << ", y: " << dest_y << ", same_destination: "
         << same_destination << ", thrust: " << thrust << endl;
}


/**
 * Determine optimal speed.
 */
int det_optimal_speed(int distance_to_dest, int angle)
{
    int opt_speed = 0;
    if (distance_to_dest < 500) {
        opt_speed = 200;
    } else if (distance_to_dest < 1000) {
        opt_speed = 170;
    } else if (distance_to_dest < 2000) {
        opt_speed = 270;
    } else if (distance_to_dest < 2500) {
        opt_speed = 320;
    } else if (distance_to_dest < 3000) {
        opt_speed = 500;
    } else {
        opt_speed = 1000;
    }
    // check also the angle
    if (abs(angle) < 4) {
        opt_speed += 100;
    }
    /*
    if (abs(angle) > 60) {
        opt_speed /= 2;
    } else if (abs(angle) > 45) {
        opt_speed -= 100;
    }
    */
    cerr << ":debug: optimal speed: " << opt_speed << endl;
    return opt_speed;
}


int thrust_func_2(int speed, int distance_to_dest, int angle)
{
    int thrust = 0;
    // special case, for big angles
    if (angle > 90 || angle < -90) {
        if (checkpoint_close_p(distance_to_dest)) {
            // just stop to be able to turn
            thrust = 0;
        } else {
            /*
            if (angle > 135 || angle < -135) {
                thrust = 5;
            } else if (angle > 90 || angle < -90) {
                thrust = 10;
            }
            */
            thrust = 0;
        }
    } else {
        int optimal_speed = det_optimal_speed(distance_to_dest, angle);
        if (abs(speed - optimal_speed) < 30) {
            // the speed is good, try to keep it the same
            thrust = 50;
        } else if (speed < optimal_speed) {
            thrust = 100;
        } else {
            thrust = 1;
        }
    }
    return thrust;
}


void apply_direction_corrections(const Agent &me, const Location2d &checkpoint, int angle,
                                 int &dest_x, int &dest_y)
{
    int distance = distance_between(me.get_location(), checkpoint);
    int cx = checkpoint.get_x();
    int cy = checkpoint.get_y();
    static const double k_pi = 3.1415926;
    //if (distance > 1000 && abs(angle) > 6) {
    //if (distance > 500 && abs(angle) > 4) {
    if (distance > 1800 && abs(angle) > 4) {
        cerr << ":debug: before correction: dest_x: " << dest_x
             << ", dest_y: " << dest_y << endl;
        double correction_angle = angle;
        double s = sin(correction_angle * k_pi / 180);
        double c = cos(correction_angle * k_pi / 180);
        cx -= me.get_location().get_x();
        cy -= me.get_location().get_y();
        double newcx = cx * c - cy * s;
        double newcy = cx * s + cy * c;
        dest_x = newcx + me.get_location().get_x();
        dest_y = newcy + me.get_location().get_y();
        cerr << ":debug: after correction: dest_x: " << dest_x
             << ", dest_y: " << dest_y << endl;
    }
}


void strategy2(const Agent &me, const Location2d &checkpoint, const Location2d &enemy,
               bool same_destination,
               int &dest_x, int &dest_y, int &thrust)
{
    static bool first = true;
    static Location2d previous_location;
    int speed;
    if (first) {
        speed = 0;
        first = false;
    } else {
        speed = distance_between(me.get_location(), previous_location);
    }
    previous_location = me.get_location();
    int distance_to_dest = distance_between(me.get_location(), checkpoint);
    thrust = thrust_func_2(speed, distance_to_dest, me.get_angle_to_dest());
    if (use_boost_p(me, checkpoint)) {
        thrust = -1;
    }
    dest_x = checkpoint.get_x();
    dest_y = checkpoint.get_y();
    if (thrust) {
        // don't apply correction if we're trying to change direction already
        apply_direction_corrections(me, checkpoint, me.get_angle_to_dest(), dest_x, dest_y);
    }
    cerr << ":debug: x: " << me.get_location().get_x()
         << ", y: " << me.get_location().get_y() << ", speed: " << speed
         << ", thrust: " << thrust << endl;
    cerr << ":debug: dest_x: " << dest_x << ", dest_y: " << dest_y << endl;
    cerr << ":debug: distance_to_dest: " << distance_to_dest
         << ", angle: " << me.get_angle_to_dest() << endl;
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

    //strategy1(*p_me, *p_checkpoint, *p_enemy, same_destination, go_x, go_y, thrust);
    strategy2(*p_me, *p_checkpoint, *p_enemy, same_destination, go_x, go_y, thrust);

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
    srand(time(NULL));
    while (1) {
        game_turn();
    }
}
