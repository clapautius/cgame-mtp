#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

#include "params.h" // :grep-out:
#include "cgame-common.h" // :grep-out:

using namespace std;

// uncomment to get lots of debug messages on cerr
//#define DEBUG


bool unit_tests();

//
// begin classes declarations
//
class DataPoint
{
public:
    DataPoint(int i = -1, int x = -1, int y = -1)
      : m_id(i), m_location(x, y), m_min_distance_to_enemy(100000)
    {};

    int x() const
    {
        return m_location.x();
    }

    int y() const
    {
        return m_location.y();
    }

    void set_targeted_by(int enemy_id, int distance)
    {
        m_enemy_ids.push_back(enemy_id);
        if (distance < m_min_distance_to_enemy) {
            m_min_distance_to_enemy = distance;
        }
    }

    vector<int> targeted_by() const
    {
        return m_enemy_ids;
    }

    string to_str() const
    {
        ostringstream ostr;
        ostr << "DataPoint(" << m_id << "," << m_location.x() << ","
             << m_location.y() << ")";
        return ostr.str();
    }

    int id() const
    {
        return m_id;
    }

    bool is_valid() const
    {
        return (m_id != -1);
    }

    int get_min_distance_to_enemy() const
    {
        return m_min_distance_to_enemy;
    }

    Location location() const
    {
        return m_location;
    }

private:
    int m_id;
    Location m_location;
    vector<int> m_enemy_ids;
    int m_min_distance_to_enemy;
};

static DataPoint k_invalid_point;

class Enemy
{
public:
    Enemy(int id = -1, int x = -1, int y = -1, int life = -1)
      : m_id(id), m_location(x, y), m_life(life), m_target_point_id(-1),
        m_target(k_invalid_point), m_danger(0), m_distance_to_point(-1)
    {};

    int x() const
    {
        return m_location.x();
    }

    int y() const
    {
        return m_location.y();
    }

    int id() const
    {
        return m_id;
    }

    string to_str() const
    {
        ostringstream ostr;
        ostr << "Enemy:" << m_id << " (" << m_location.x() << ","
             << m_location.y() << ") ,life=" << m_life
             << ",target=" << m_target.id() << ")";
        return ostr.str();
    }

    void set_target(const DataPoint &d)
    {
        m_target = d;
    }

    /*
    void set_target_point(int point_id)
    {
        m_target_point_id = point_id;
    }
    */

    int target_point_id() const
    {
        return m_target.id();
    }

    DataPoint target() const
    {
        return m_target;
    }

    bool is_valid() const
    {
        return (m_id != -1);
    }

    void inc_danger(int value)
    {
        m_danger += value;
    }

    int danger() const
    {
        return m_danger;
    }

    void set_distance_to_point(int dist)
    {
        m_distance_to_point = dist;
    }

    int distance_to_point() const
    {
        return m_distance_to_point;
    }

    int life() const
    {
        return m_life;
    }

    Location location() const
    {
        return m_location;
    }

    Location& location()
    {
        return m_location;
    }

private:
    int m_id;
    Location m_location;
    int m_life;
    int m_target_point_id;
    DataPoint m_target;
    int m_danger;
    int m_distance_to_point;
};

//
// end classes declarations
//

//
// begin classes definitions
//

//
// end classes definitions
//

static const Enemy k_invalid_enemy;
static vector<Enemy> g_enemies;
static int g_current_target_id = -1;
static int g_closest_enemy_idx = -1;

int enemies_around(const Location &loc, const vector<Enemy> &enemies, int radius,
                   bool check_moving_away = false);


void read_data(Location &my_location, vector<DataPoint> &data_points,
               vector<Enemy> &enemies)
{
    int x;
    int y;
    bool write_debug = true;
    cin >> x >> y; cin.ignore();
    my_location.set_x(x);
    my_location.set_y(y);
    cerr << "me(" << x << "," << y << ")" << endl;

    int dataCount;
    cin >> dataCount; cin.ignore();
    data_points.clear();
    cerr << "no. data points: " << dataCount << endl;
    if (dataCount > 60) {
        write_debug = false;
    }
    for (int i = 0; i < dataCount; i++) {
        int dataId;
        int dataX;
        int dataY;
        cin >> dataId >> dataX >> dataY; cin.ignore();
        DataPoint d(dataId, dataX, dataY);
        if (write_debug) {
            cerr << "new point: " << d.to_str() << endl;
        }
        data_points.push_back(d);
    }
    int enemyCount;
    cin >> enemyCount; cin.ignore();
    cerr << "no. enemies: " << enemyCount << endl;
    enemies.clear();
    if (enemyCount > 30) {
        write_debug = false;
    }
    for (int i = 0; i < enemyCount; i++) {
        int enemyId;
        int enemyX;
        int enemyY;
        int enemyLife;
        cin >> enemyId >> enemyX >> enemyY >> enemyLife; cin.ignore();
        Enemy e(enemyId, enemyX, enemyY, enemyLife);
        if (write_debug) {
            cerr << "new enemy: " << e.to_str() << endl;
        }
        enemies.push_back(e);
    }
}


const Enemy& get_enemy_by_id(const vector<Enemy> &enemies, int id)
{
    for (auto &enemy : enemies) {
        if (enemy.id() == id) {
            return enemy;
        }
    }
    return k_invalid_enemy;
}


DataPoint& get_point_by_id(vector<DataPoint> &points, int id)
{
    for (auto &p : points) {
        if (p.id() == id) {
            return p;
        }
    }
    return k_invalid_point;
}


/**
 * @globals g_current_target_id
 */
void set_current_target(int id)
{
    g_current_target_id = id;
}


/**
 * @globals g_current_target_id
 */
int get_current_target_id()
{
    return g_current_target_id;
}


/**
 * @globals g_enemies, g_current_target_id
 */
const Enemy& get_current_target()
{
    const Enemy &ret = get_enemy_by_id(g_enemies, g_current_target_id);
    return ret;
}


bool location_is_valid_p(const Location &loc)
{
    return (loc.x() >= 0 && loc.x() < 16000 &&
            loc.y() >= 0 && loc.y() < 9000);
}


bool enemy_is_moving_away(const Location &me, const Enemy &enemy)
{
    bool rc = false;
    const DataPoint &point = enemy.target();
    int my_angle = -1;
    int my_distance = -1;
    int enemy_distance = -1;
    int enemy_angle = -1;
/*
    if (point.is_valid() && distance(me, point) > k_safe_dist_me_from_point
        && distance(enemy, point) > k_safe_dist_enemy_from_point) {
*/
    if (point.is_valid() && distance(enemy, point) > k_safe_dist_enemy_from_point) {
        my_angle = orientation(me, point);
        my_distance = distance(me, point);
        enemy_angle = orientation(enemy, point);
        enemy_distance = distance(enemy, point);
        if (my_distance > enemy_distance + k_safe_dist_me_from_enemy_away &&
            diff_angle(my_angle, enemy_angle) < k_safe_angle) {
            rc = true;
        }
#ifdef DEBUG
        cerr << "Me and enemy " << enemy.to_str() << ": mdist=" << my_distance
             << ", edist=" << enemy_distance << ", diff_angle="
             << diff_angle(my_angle, enemy_angle) << ", away=" << rc << endl;
#endif
    }
    return rc;
}


bool location_safe_to_be_p(const Location &loc, const vector<Enemy> &enemies,
                          int min_distance)
{
    bool rc = true;
#ifdef DEBUG
    cerr << "checking location " << loc.x() << "," << loc.y() << endl;
    Enemy dangerous_enemy;
    int potential_distance = -1;
#endif
    for (auto &enemy : enemies) {
        if ((distance(loc, enemy) < k_min_distance) ||
            (distance(loc, enemy) < min_distance
             && !enemy_is_moving_away(loc, enemy))) {
#ifdef DEBUG
            dangerous_enemy = enemy;
            potential_distance = distance(loc, enemy);
#endif
            rc = false;
            break;
        }
    }
#ifdef DEBUG
    if (!rc) {
        cerr << "loc. not safe because of "
             << dangerous_enemy.to_str() << ", dist=" << potential_distance << endl;
    }
#endif
    return rc;
}


int get_min_distance_to_enemies(const Location &loc, const vector<Enemy> &enemies)
{
    int min_distance = k_max_dist;
    for (auto &enemy : enemies) {
        int d = distance(loc, enemy);
        if (d < min_distance) {
            min_distance = d;
        }
    }
    return min_distance;
}


int get_min_angle_to_enemies(const Location &loc, const vector<Enemy> &enemies)
{
    int min_angle = -1;
    for (auto &e : enemies) {
        int diff = diff_angle(orientation(e, e.target()), orientation(loc, e.target()));
#ifdef DEBUG
        cerr << "diff angle for enemy " << e.to_str() << ": " << diff << endl;
#endif
        if (diff < min_angle) {
            min_angle = diff;
        }
    }
    return min_angle;
}


/*
int shots_needed_by_distance(int life, int distance, int shots)
{
    int remaining_life = life - (125000.0 / pow(distance, 1.2));
    ++shots;
    if (remaining_life <= 0) {
        return shots;
    } else {
        return shots_needed_by_distance(remaining_life,
                                        distance + k_moving_factor_shooting, shots);
    }
}
*/

/**
 * @return the number of shots to kill an enemy, considering distance and life.
 */
int shots_needed(int life, int distance)
{
    return ceil(life / round(125000.0 / pow(distance, 1.2)));
}


/**
 * @return the number of shots to kill an enemy, considering distance and life.
 */
int shots_needed(const Location &me, const Enemy &enemy)
{
    return shots_needed(enemy.life(), distance(me, enemy));
}


/**
 * @return how far away we should be from an enemy before shooting.
 * :fixme: Needs better algorithm.
 * @globals g_enemies
 */
int recommended_distance_for_enemy(const Location &me, const Enemy &enemy, int no_enemies)
{
    int rc = k_recommended_distance_more;

    // first check the formula
    // else make some grouping based on life, distance and number of enemies
    int shots = shots_needed(me, enemy);
    if (shots == 1 || shots == 2) {
        rc = k_max_dist;
    } else {
        int no_enemies_around = enemies_around(me, g_enemies,
                                               k_recommended_dist_radius_around, true);
        cerr << "enemies around me (radius=" << k_recommended_dist_radius_around << "): "
             << no_enemies_around << endl;
        if (enemy.life() <= 25) {
            if (no_enemies_around <= 1) {
                if (enemy_is_moving_away(me, enemy)) {
                    rc = max(k_recommended_distance_one - 500, k_min_distance + 10);
                } else {
                    rc = k_recommended_distance_one;
                }
            } else if (no_enemies_around >= 6) {
                rc = k_recommended_distance_many_more;
            } else {
                rc = k_recommended_distance_more;
            }
        } else {
            if (no_enemies_around <= 1) {
                if (enemy_is_moving_away(me, enemy)) {
                    rc = max(k_recommended_distance_strong - 500, k_min_distance + 10);
                } else {
                    rc = k_recommended_distance_strong;
                }
            } else {
                rc = k_recommended_distance_more;
            }
        }
    }
    cerr << "recommended dist for enemy " << enemy.to_str()
         << " : " << rc << endl;
    return rc;
}

/**
 * @return true if the command has been executed. It is not executed if it's not safe.
 *
 * @globals g_enemies.
 */
bool goto_location(const Location &me, const Location &dest)
{
    bool rc = false;
    double angle = angle_deg_to_rad(orientation(me, dest));
    // :tmp:
#ifdef DEBUG
    cerr << "angle_deg=" << orientation(me, dest)
         << ", angle_rad=" << angle << endl;
#endif
    Location new_loc(me.x() + k_standard_step * cos(angle),
                     me.y() + k_standard_step * sin(angle));
    if (location_safe_to_be_p(new_loc, g_enemies, k_safe_distance)) {
        cout << "MOVE " << dest.x() << " " << dest.y() << endl;
#ifdef DEBUG
        cerr << "will probably be at (" << new_loc.x() << "," << new_loc.y()
             << ")" << endl;
#endif
        rc = true;
    }
    if (!rc) {
        cerr << "not safe to go to (" << dest.x() << ","
             << dest.y() << ")" << endl;
    }
    return rc;
}


bool attack_enemy(const Location &me, const Enemy &target, int no_enemies)
{
    bool moved = false;
    static int id_to_follow = -1;
    static bool distance_ok = false;
    static int recommended_dist = 0;

    // check if we have a new target
    if (id_to_follow != target.id()) {
        cerr << "attack: new target" << endl;
        // new target
        distance_ok = false;
        id_to_follow = target.id();
    }

    if (distance_ok) {
        double d = recommended_distance_for_enemy(me, target, no_enemies);
        recommended_dist = d * k_recommended_factor;
    } else {
        recommended_dist = recommended_distance_for_enemy(me, target, no_enemies);
    }

#ifdef DEBUG
    cerr << "attack: comparing with dist " << recommended_dist << endl;
#endif
    if (distance(me, target) > recommended_dist) {
        // too far away
        moved = goto_location(me, target.location());
        distance_ok = false;
    } else {
        distance_ok = true;
    }
    if (!moved) {
        // if we cannot move at least we could shoot
        cout << "SHOOT " << target.id() << endl;
    }
    return true;
}


/**
 * @return true if we moved (to escape some enemies)
 */
bool run_away_if_needed(Location &me, vector<Enemy> &enemies)
{
    static bool first = true;
    static vector<pair<int, int>> offsets;
    if (first) {
        first = false;
        offsets.push_back(make_pair(0, -k_runaway_step)); // north
        offsets.push_back(make_pair(0, k_runaway_step)); // south
        offsets.push_back(make_pair(k_runaway_step, 0)); // east
        offsets.push_back(make_pair(-k_runaway_step, 0)); // north
        offsets.push_back(make_pair(k_runaway_step_diag, -k_runaway_step_diag)); // ne
        offsets.push_back(make_pair(-k_runaway_step_diag, -k_runaway_step_diag)); // nw
        offsets.push_back(make_pair(k_runaway_step_diag, -k_runaway_step_diag)); // se
        offsets.push_back(make_pair(-k_runaway_step_diag, -k_runaway_step_diag)); // sw
    }

    if (!location_safe_to_be_p(me, enemies, k_safe_distance)) {
        // check north, south, east, west
        Location potential_loc(me.x(), me.y());
        cerr << "we are in danger" << endl;
        int best_distance = -1;
        Location best_location;
        Location best_location_for_angle;
        int best_angle = -1;
        for (auto &loc : offsets) {
            potential_loc.set_x(me.x() + loc.first);
            potential_loc.set_y(me.y() + loc.second);
            if (location_is_valid_p(potential_loc)
                && location_safe_to_be_p(potential_loc, enemies, k_safe_distance)) {

                // check best distance
                int angle_with_enemy_trajectory = 0;
                int dist = get_min_distance_to_enemies(potential_loc, enemies);
                if (dist > best_distance) {
                    best_distance = dist;
                    best_location = potential_loc;
                    angle_with_enemy_trajectory =
                      get_min_angle_to_enemies(potential_loc, enemies);
                }

                if (angle_with_enemy_trajectory < 10) {
                    // try to move farther
                    // check min angle
                    cerr << "trying to move farther" << endl;
                    bool safe_angle = true;
                    int min_angle = 0;
                    for (auto &e : enemies) {
                        if (distance(potential_loc, e.target()) < k_runaway_safe_dist_to_point) {
                            safe_angle = false;
                            break;
                        }
                        int diff = diff_angle(orientation(e, e.target()),
                                              orientation(potential_loc, e.target()));
#ifdef DEBUG
                        cerr << "diff angle for enemy " << e.to_str() << ": " << diff << endl;
#endif
                        if (diff < min_angle) {
                            min_angle = diff;
                        }
                    }
                    if (safe_angle) {
                        if (best_angle < min_angle) {
                            best_angle = min_angle;
                            best_location_for_angle = potential_loc;
                        }
                    }
                }
            }
        }
        if (best_angle >= 0) {
            cerr << "found safe location (angle) " << best_location_for_angle.x() << ","
                 << best_location_for_angle.y() << endl;
            return goto_location(me, best_location_for_angle); // :fixme: what if we cannot move?
        } else if (best_distance >= 0) {
            cerr << "found safe location " << best_location.x() << ","
                 << best_location.y() << endl;
            return goto_location(me, best_location); // :fixme: what if we cannot move?
        } else {
            cerr << "couldn't find a potential safe location " << endl;
            return false; // :fixme: we are in danger but couldn't find a safe location
        }
    } else {
        return false;
    }
}


/**
 * Simple strategy - just shoot the closest enemy.
 *
 * @return true if an action has been taken.
 */
bool strategy_kill_closest_enemy(Location &me, vector<DataPoint> &points,
                                 vector<Enemy> &enemies, int closest_enemy_idx)
{
    if (closest_enemy_idx >= 0) {
        set_current_target(enemies[closest_enemy_idx].id());
        // :fixme: check result of get_enemy_by_id
        return attack_enemy(me, enemies[closest_enemy_idx], enemies.size());
    } else {
        return false;
    }
}


// Shoot the enemy closest to a data point.
// return true if it did something
bool strategy_kill_closest_to_point(Location &me, vector<DataPoint> &points,
                                    vector<Enemy> &enemies, int radius = -1)
{
    int min_distance = k_max_dist;
    int min_id = -1;
    Enemy target;
    for (auto &enemy : enemies) {
        for (auto &point : points) {
            int d = distance(point, enemy);
            if (d < min_distance && (radius != -1 && d <= radius)) {
                min_distance = d;
                target = enemy;
                min_id = enemy.id();
            }
        }
    }
    if (min_id >= 0) {
        return attack_enemy(me, target, enemies.size());
    }
    return false;
}


bool can_be_killed_p(const Location &me, const Enemy &enemy, const DataPoint &point)
{
    int rc = false;
#ifdef DEBUG
    cerr << __FUNCTION__ << ": me(" << me.x() << "," << me.y() << "), enemy: "
         << enemy.to_str() << endl;
#endif
    int enemy_to_point = distance(enemy, point);
    if (enemy_to_point <= 500) {
        rc = false;
    } else if (shots_needed(me, enemy) > enemy_to_point / 500) {
        rc = true;
    } else {
        Location new_me = me;
        new_me.move_towards(enemy.location(), 1000);
        Enemy new_enemy = enemy;
        new_enemy.location().move_towards(point.location(), 500);
        rc = can_be_killed_p(new_me, new_enemy, point);
    }
#ifdef DEBUG
    cerr << __FUNCTION__ << ": ret=" << rc << endl;
#endif
    return rc;
}


bool strategy_kill_dangerous(Location &me, vector<DataPoint> &points,
                             vector<Enemy> &enemies)
{
    for (auto &enemy : enemies) {
        // check if it's alone
#ifdef DEBUG
        cerr << "analyzing enemy " << enemy.to_str() << endl;
#endif
        // moving alone towards a target
        bool alone = (enemy.target().targeted_by().size() == 1);
        if (alone) {
            cerr << "found alone enemy: " << enemy.to_str() << endl;
            int factor = 1100;
            enemy.inc_danger(factor);
            cerr << "- alone + " << factor << endl;
        }
        // check if it has multiple points around him
        for (auto &point : points) {
            if (point.id() != enemy.target_point_id() &&
                distance(point, enemy) < k_safe_distance) {
                int factor = 500;
                enemy.inc_danger(factor);
                cerr << "- point near:  + " << factor << endl;
            }
        }
        /*
        // check if can be killed (:fixme: magic values)
        int steps_needed = distance(enemy, enemy.target()) / 500;
        int shots = shots_needed(me, enemy);
        if (shots <= 2 || (shots > 2 && (shots + 2 > steps_needed))) {
            enemy.inc_danger(1100);
            cerr << "- can be killed:  + 1100" << endl;
        }
        */
        if (can_be_killed_p(me, enemy, enemy.target())) {
            int factor = 1600;
            enemy.inc_danger(factor);
            cerr << "- can be killed:  + " << factor << endl;
        } else {
            int factor = -500;
            enemy.inc_danger(factor);
            cerr << "- can be killed:  + " << factor << endl;
        }
        cerr << "Danger: " << enemy.danger() << endl;
    }

    sort(enemies.begin(), enemies.end(),
         [&] (Enemy &a, Enemy &b) -> bool
         { return (a.danger() > b.danger()); });

    cerr << "most dangerous: " << enemies[0].to_str() << endl;
    bool attacked = attack_enemy(me, enemies[0], enemies.size());
    if (attacked) {
        set_current_target(enemies[0].id());
    }
    return attacked;
}


/**
 * @return the number of enemies near the specified location.
 */
int enemies_around(const Location &loc, const vector<Enemy> &enemies, int radius,
                   bool check_moving_away)
{
    return count_if<Enemy>(
      [&] (const Enemy &e) -> bool
        {
            if (check_moving_away) {
                return distance(loc, e) <= radius && !enemy_is_moving_away(loc, e);
            } else {
                return (distance(loc, e) <= radius);
            }},
      enemies);
}


void compute_connections(Location &me, vector<DataPoint> &points, vector<Enemy> &enemies)
{
    for (auto &enemy : enemies) {
        int min_dist = k_max_dist;
        int min_id = -1;
        for (auto &point : points) {
            int d = distance(enemy, point);
            if (d < min_dist) {
                min_dist = d;
                min_id = point.id();
            }
        }
        if (min_dist != k_max_dist) {
            enemy.set_distance_to_point(min_dist);
            DataPoint &min_point = get_point_by_id(points, min_id);
            if (min_point.is_valid()) {
                enemy.set_target(min_point);
            }
        }
    }
    // closest enemy (index in array)
    int min_distance = k_max_dist;
    int min_idx = -1;
    for (unsigned int i = 0; i < enemies.size(); i++) {
        Enemy &enemy = enemies[i];
        int d = distance(me, enemy);
        if (d < min_distance) {
            min_distance = d;
            min_idx = i;
        }
    }
    if (min_idx >= 0) {
        g_closest_enemy_idx = min_idx;
    } else {
        g_closest_enemy_idx = -1;
    }
}


/**
 * Kill enemies that can be killed in one shot.
 */
bool kill_weak_enemies(Location &me, const vector<Enemy> &enemies)
{
    const Enemy *target = nullptr;
    int min_dist = k_max_dist;
    for (auto &e : enemies) {
        if (shots_needed(me, e) == 1) {
            if (min_dist > e.distance_to_point()) {
                min_dist = e.distance_to_point();
                target = &e;
            }
        }
    }
    if (min_dist < k_max_dist) {
        return attack_enemy(me, *target, enemies.size());
    } else {
        return false;
    }
}


void play_turn(Location &me, vector<DataPoint> &points, vector<Enemy> &enemies,
               int closest_enemy_idx = -1)
{
    bool command_executed = false;
    int enemies_around_me = enemies_around(me, enemies, k_enemy_distance, true);
    int enemies_near_me = enemies_around(me, enemies, k_safe_distance, true);
    cerr << "enemies near: " << enemies_near_me << ", around: "
         << enemies_around_me << endl;
    if (enemies_near_me == 1 && enemies_around_me == 1 && closest_enemy_idx >= 0 &&
        shots_needed(me, enemies[closest_enemy_idx]) == 1) {
        command_executed = attack_enemy(me, enemies[closest_enemy_idx], enemies.size());
    } else {
        if (enemies_around_me == 1) {
            cerr << "one enemy around - close fight" << endl;
            if (enemies_near_me) {
                command_executed = run_away_if_needed(me, enemies);
            }
        } else if (enemies_around_me > 1 || enemies_near_me >= 1) {
            cerr << "more enemies around - fight or flight" << endl;
            command_executed = run_away_if_needed(me, enemies);
        }
    }

    bool close_strategy = false;
    if (enemies.size() > 30 || points.size() > 60) {
        close_strategy = true;
    }
    if (enemies_near_me >= 1 || enemies_around_me >= 1) {
        close_strategy = true;
    }

    if (!command_executed && close_strategy) {
        command_executed = strategy_kill_closest_enemy(me, points, enemies,
                                                       closest_enemy_idx);
    }

    // end of emergency situations - continue with normal strategy

    command_executed ||
      (command_executed = kill_weak_enemies(me, enemies));

    if (!command_executed && get_current_target_id() >= 0) {
        const Enemy &enemy = get_current_target();
        if (enemy.is_valid()) {
            command_executed = attack_enemy(me, enemy, enemies.size());
        } else {
            g_current_target_id = -1;
        }
    }

    command_executed ||
      (command_executed = strategy_kill_dangerous(me, points, enemies));

    command_executed ||
      (command_executed = strategy_kill_closest_to_point(me, points, enemies,
                                                         k_safe_distance));
    // just stay there
    command_executed ||
      (cout << "MOVE " << me.x() << " " << me.y() << endl);
}


/**
 * Shoot enemies before they collect all the incriminating data!
 * The closer you are to an enemy, the more damage you do but don't get too close or you'll get killed.
 **/
int main(int argc, char *argv[])
{
    if (argc > 1 && string(argv[1]) == "--tests") {
        exit((unit_tests() && unit_tests_common()) ? 0 : 1);
    }

    Location me;
    vector<DataPoint> points;
    // game loop
    while (1) {
        read_data(me, points, g_enemies);
        compute_connections(me, points, g_enemies);
        play_turn(me, points, g_enemies, g_closest_enemy_idx);
    }
}


bool unit_tests()
{
#define TEST_EQ(X, Y) do                                                \
    { if (X != Y) {                                                     \
            cerr << "fail: " << X << "," << Y << endl; return false; }; \
    } while(0)

    Location l1(0, 0);
    Location l2(2, 0);
    TEST_EQ(distance(l1, l2), 2);

    Location l3(15900,7100);
    Location l4(13545,8091);
    TEST_EQ(distance(l3, l4), 2555);

    Location me(0, 0);

    Enemy e1(1, 6200, 0, 3); // 6200 points away, life 3 -> 1 shot needed
    TEST_EQ(shots_needed(me, e1), 1);

    Enemy e2(1, 6200, 0, 10); // 6200 points away, life 10 -> 3 shots needed
    TEST_EQ(shots_needed(me, e2), 3);

    Enemy e3(1, 6200, 0, 8); // 6200 points away, life 8 -> 2 shots needed
    TEST_EQ(shots_needed(me, e3), 2);

    return true;

#undef TEST_EQ
}
