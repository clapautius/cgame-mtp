#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>

using namespace std;

// uncomment to get lots of debug messages on cerr
//#define DEBUG

// !!
static const int k_recommended_distance_more = 63200;
static const int k_recommended_distance_one = 30900;
static const int k_recommended_distance_strong = 30900;
static const int k_safe_distance = 31000;
static const int k_enemy_distance = 33500;
static const int k_distance_between_enemies = 30000;
static const int k_runaway_step = 11000;


bool unit_tests();

//
// begin classes declarations
//
class Location
{
public:

    Location()
      : m_x(-1), m_y(-1)
    {};

    Location(int x, int y)
      : m_x(x), m_y(y)
    {};

    void set_x(int x)
    {
        m_x = x;
    }

    void set_y(int y)
    {
        m_y = y;
    }

    int x() const
    {
        return m_x;
    }

    int y() const
    {
        return m_y;
    }

private:
    int m_x;
    int m_y;
};


class Enemy
{
public:
    Enemy(int id = -1, int x = -1, int y = -1, int life = -1)
      : m_id(id), m_location(x, y), m_life(life), m_target_point_id(-1), m_danger(100000),
        m_distance_to_point(-1)
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
        ostr << "Enemy: id=" << m_id << ", x=" << m_location.x() << ", y="
             << m_location.y() << ", life=" << m_life;
        return ostr.str();
    }

    void set_target_point(int point_id)
    {
        m_target_point_id = point_id;
    }

    int target_point_id() const
    {
        return m_target_point_id;
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

private:
    int m_id;
    Location m_location;
    int m_life;
    int m_target_point_id;
    int m_danger;
    int m_distance_to_point;
};


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

private:
    int m_id;
    Location m_location;
    vector<int> m_enemy_ids;
    int m_min_distance_to_enemy;
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
static DataPoint k_invalid_point;


void read_data(Location &my_location, vector<DataPoint> &data_points,
               vector<Enemy> &enemies)
{
    int x;
    int y;
    cin >> x >> y; cin.ignore();
    my_location.set_x(x);
    my_location.set_y(y);

    int dataCount;
    cin >> dataCount; cin.ignore();
    data_points.clear();
    for (int i = 0; i < dataCount; i++) {
        int dataId;
        int dataX;
        int dataY;
        cin >> dataId >> dataX >> dataY; cin.ignore();
        DataPoint d(dataId, dataX, dataY);
        data_points.push_back(d);
    }
    int enemyCount;
    cin >> enemyCount; cin.ignore();
    enemies.clear();
    for (int i = 0; i < enemyCount; i++) {
        int enemyId;
        int enemyX;
        int enemyY;
        int enemyLife;
        cin >> enemyId >> enemyX >> enemyY >> enemyLife; cin.ignore();
        Enemy e(enemyId, enemyX, enemyY, enemyLife);
        cerr << ":debug: new enemy: " << e.to_str() << endl;
        enemies.push_back(e);
    }
}


template<class A, class B>
int distance(A a, B b)
{
    double xx = abs(a.x() - b.x());
    double yy = abs(a.y() - b.y());
    return static_cast<int>(sqrt(xx * xx + yy * yy));
}


template<class C>
int count_if(function<bool(const C&)> predicate, vector<C> &sequence)
{
    int ret = 0;
    for (auto &item : sequence) {
        if (predicate(item)) {
            ++ret;
        }
    }
    return ret;
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


bool location_is_valid_p(const Location &loc)
{
    return (loc.x() >= 0 && loc.x() < 16000 &&
            loc.y() >= 0 && loc.y() < 9000);
}


bool location_is_safe_p(const Location &loc, const vector<Enemy> &enemies,
                        int min_distance)
{
    bool rc = true;
#ifdef DEBUG
    cerr << ":debug: checking location " << loc.x() << "," << loc.y() << endl;
    Enemy dangerous_enemy;
    int potential_distance = -1;
#endif
    for (auto &enemy : enemies) {
        if (distance(loc, enemy) < min_distance) {
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
        cerr << ":debug: loc. not safe because of "
             << dangerous_enemy.to_str() << ", dist=" << potential_distance << endl;
    }
#endif
    return rc;
}


int get_min_distance_to_enemies(const Location &loc, const vector<Enemy> &enemies)
{
    int min_distance = 100000;
    for (auto &enemy : enemies) {
        int d = distance(loc, enemy);
        if (d < min_distance) {
            min_distance = d;
        }
    }
    return min_distance;
}


int recommended_distance_for_enemy(const Location &me, const Enemy &enemy, int no_enemies)
{
    int rc = k_recommended_distance_more;
    if (enemy.life() <= 5) {
        rc = 100000;
    } else if (enemy.life() <= 25) {
        if (no_enemies == 1) {
            if (enemy.distance_to_point() <= 1000) {
                // don't waste time moving if it's too close, just shoot
                rc = k_recommended_distance_more;
            } else {
                rc = k_recommended_distance_one;
            }
        } else {
            rc = k_recommended_distance_more;
        }
    } else {
        rc = k_recommended_distance_strong;
    }
    return rc;
}


void attack_enemy(const Location &me, const Enemy &target, int no_enemies)
{
    if (distance(me, target) > recommended_distance_for_enemy(me, target, no_enemies)) {
        // too far away
        cout << "MOVE " << target.x() << " " << target.y() << endl;
    } else {
        cout << "SHOOT " << target.id() << endl;
    }
}


/**
 * @return true if we moved (to escape some enemies)
 */
bool run_away_if_needed(Location &me, vector<Enemy> &enemies)
{
    if (!location_is_safe_p(me, enemies, k_safe_distance)) {
        // check north, south, east, west
        Location potential_loc(me.x(), me.y());
        cerr << ":debug: we are in danger" << endl;
        vector<pair<int, int>> offsets;
        int diag_step = k_runaway_step / 1.4142;
        offsets.push_back(make_pair(0, -k_runaway_step)); // north
        offsets.push_back(make_pair(0, k_runaway_step)); // south
        offsets.push_back(make_pair(k_runaway_step, 0)); // east
        offsets.push_back(make_pair(-k_runaway_step, 0)); // north
        offsets.push_back(make_pair(diag_step, -diag_step)); // ne
        offsets.push_back(make_pair(-diag_step, -diag_step)); // nw
        offsets.push_back(make_pair(diag_step, -diag_step)); // se
        offsets.push_back(make_pair(-diag_step, -diag_step)); // sw

        int best_distance = -1;
        Location best_location;
        for (auto &loc : offsets) {
            potential_loc.set_x(me.x() + loc.first);
            potential_loc.set_y(me.y() + loc.second);
            if (location_is_valid_p(potential_loc)
                && location_is_safe_p(potential_loc, enemies, k_safe_distance)) {
                int distance = get_min_distance_to_enemies(potential_loc, enemies);
                if (distance > best_distance) {
                    best_distance = distance;
                    best_location = potential_loc;
                }
            }
        }
        if (best_distance >= 0) {
            cerr << ":debug: found safe location " << best_location.x() << ","
                 << best_location.y() << endl;
            cout << "MOVE " << best_location.x() << " " << best_location.y() << endl;
            return true;
        } else {
            cerr << ":debug: couldn't find a potential safe location " << endl;
            return false; // :fixme: we are in danger but couldn't find a safe location
        }
    } else {
        return false;
    }
}

static int g_current_target_id = -1;

// Simple strategy - just shoot the closest enemy.
void strategy_kill_closest_enemy(Location &me, vector<DataPoint> &points,
                                 vector<Enemy> &enemies)
{
    int min_distance = 100000;
    int min_id = -1;
    Location min_loc;
    for (auto &enemy : enemies) {
        int d = distance(me, enemy);
        if (d < min_distance) {
            min_distance = d;
            min_id = enemy.id();
            min_loc.set_x(enemy.x());
            min_loc.set_y(enemy.y());
        }
    }
    if (min_id >= 0) {
        g_current_target_id = min_id;
        // :fixme: check result of get_enemy_by_id
        attack_enemy(me, get_enemy_by_id(enemies, min_id), enemies.size());
    } else {
        // just stay there
        cout << "MOVE " << me.x() << " " << me.y() << endl;
    }
}


// Shoot the enemy closest to a data point.
// return true if it did something
bool strategy_kill_closest_to_point(Location &me, vector<DataPoint> &points,
                                    vector<Enemy> &enemies, int radius = -1)
{
    int min_distance = 100000;
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
        attack_enemy(me, target, enemies.size());
        return true;
    }
    return false;
}


bool strategy_kill_dangerous(Location &me, vector<DataPoint> &points,
                             vector<Enemy> &enemies)
{
    for (auto &enemy : enemies) {
        // check if it's alone
        bool alone = true;
        for (auto &enemy2 : enemies) {
            if (enemy.id() != enemy2.id() &&
                distance(enemy, enemy2) < k_distance_between_enemies) {
                alone = false;
                break;
            }
        }
        if (alone) {
            cerr << ":debug: found alone enemy: " << enemy.to_str() << endl;
            enemy.inc_danger(1600);
        }
        // check if it has multiple points around him
        for (auto &point : points) {
            if (point.id() != enemy.target_point_id() &&
                distance(point, enemy) < k_safe_distance) {
                enemy.inc_danger(500);
            }
        }
    }

    sort(enemies.begin(), enemies.end(),
         [&] (Enemy &a, Enemy &b) -> bool
         { return (a.danger() > b.danger()); });

    bool attacked = false;
    cerr << ":debug: most dangerous: " << enemies[0].to_str() << endl;
    attack_enemy(me, enemies[0], enemies.size());
    attacked = true;
    return attacked;
}


/**
 * @return the number of enemies near the specified location.
 */
int enemies_around(Location &loc, vector<Enemy> &enemies, int radius)
{
    return count_if<Enemy>(
      [&] (const Enemy &e) -> bool
        { return (distance(loc, e) <= radius); },
      enemies);
}


void compute_connections(Location &me, vector<DataPoint> &points, vector<Enemy> &enemies)
{
    for (auto &enemy : enemies) {
        int min_dist = 100000;
        int min_id = -1;
        for (auto &point : points) {
            int d = distance(enemy, point);
            if (d < min_dist) {
                min_dist = d;
                min_id = point.id();
            }
        }
        if (min_dist != 100000) {
            enemy.inc_danger(-min_dist);
            enemy.set_distance_to_point(min_dist);
            DataPoint &min_point = get_point_by_id(points, min_id);
            if (min_point.is_valid()) {
                enemy.set_target_point(min_point.id());
            }
        }
    }

}


void play_turn(Location &me, vector<DataPoint> &points, vector<Enemy> &enemies)
{
    compute_connections(me, points, enemies);

    // First run if enemy is close
    bool close_strategy = false;
    bool command_executed = false;
    int enemies_around_me = enemies_around(me, enemies, k_enemy_distance);
    int enemies_near_me = enemies_around(me, enemies, k_safe_distance);
    cerr << ":debug: enemies near: " << enemies_near_me << ", around="
         << enemies_around_me << endl;
    if (enemies_around_me == 1) {
        cerr << ":debug: one enemy around - close fight" << endl;
        close_strategy = true;
        if (enemies_near_me) {
            command_executed = run_away_if_needed(me, enemies);
        }
    } else if (enemies_around_me > 1 || enemies_near_me >= 1) {
        cerr << ":debug: more enemies around - fight or flight" << endl;
        close_strategy = true;
        command_executed = run_away_if_needed(me, enemies);
    }

    /*
    if (!command_executed && enemies_near_me >= 1) {
        strategy_kill_closest_enemy(me, points, enemies);
        command_executed = true;
    }
    */

    if (!command_executed && close_strategy) {
        strategy_kill_closest_enemy(me, points, enemies);
        command_executed = true;
    }

    if (!command_executed && g_current_target_id >= 0) {
        const Enemy &enemy = get_enemy_by_id(enemies, g_current_target_id);
        if (enemy.is_valid()) {
            attack_enemy(me, enemy, enemies.size());
            command_executed = true;
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
        exit(unit_tests() ? 0 : 1);
    }

    Location me;
    vector<DataPoint> points;
    vector<Enemy> enemies;
    // game loop
    while (1) {
        read_data(me, points, enemies);
        play_turn(me, points, enemies);
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
    return true;
}
