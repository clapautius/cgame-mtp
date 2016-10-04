#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>

using namespace std;

// uncomment to get lots of debug messages on cerr
//#define DEBUG

static int g_recommended_distance = 6200;
static const int k_safe_distance = 3000;
static const int k_enemy_distance = 4000;


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
      : m_id(id), m_location(x, y), m_life(life)
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

private:
    int m_id;
    Location m_location;
    int m_life;
};


class DataPoint
{
public:
    DataPoint(int i, int x, int y)
      : m_id(i), m_location(x, y)
    {};

    int x() const
    {
        return m_location.x();
    }

    int y() const
    {
        return m_location.y();
    }

private:
    Location m_location;
    int m_id;
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
        enemies.push_back(e);
    }
    if (enemies.size() == 1) {
        g_recommended_distance = 5000;
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
}


/**
 * @return true if we moved (to escape some enemies)
 */
bool run_away_if_needed(Location &me, vector<Enemy> &enemies)
{
    if (!location_is_safe_p(me, enemies, k_safe_distance)) {
        // check north, south, east, west
        Location potential_loc(me.x(), me.y());
        bool found = false;
        cerr << ":debug: we are in danger" << endl;
        vector<pair<int, int>> offsets;
        offsets.push_back(make_pair(0, -1000)); // north
        offsets.push_back(make_pair(0, 1000)); // south
        offsets.push_back(make_pair(1000, 0)); // east
        offsets.push_back(make_pair(-1000, 0)); // north
        offsets.push_back(make_pair(707, -707)); // ne
        offsets.push_back(make_pair(-707, -707)); // nw
        offsets.push_back(make_pair(707, -707)); // se
        offsets.push_back(make_pair(-707, -707)); // sw

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


// Simple strategy - just shoot the closest enemy.
void strategy1(Location &me, vector<DataPoint> &points, vector<Enemy> &enemies)
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
        if (min_distance >= g_recommended_distance) {
            // too far away
            cout << "MOVE " << min_loc.x() << " " << min_loc.y() << endl;
        } else {
            cout << "SHOOT " << min_id << endl;
        }
    } else {
        // just stay there
        cout << "MOVE " << me.x() << " " << me.y() << endl;
    }
}


// Shoot the enemy closest to a data point.
void strategy2(Location &me, vector<DataPoint> &points, vector<Enemy> &enemies)
{
    int min_distance = 100000;
    int min_id = -1;
    Enemy target;
    for (auto &enemy : enemies) {
        for (auto &point : points) {
            int d = distance(point, enemy);
            if (d < min_distance) {
                min_distance = d;
                target = enemy;
                min_id = enemy.id();
            }
        }
    }
    if (min_id >= 0) {
        if (distance(me, target) >= g_recommended_distance) {
            // too far away
            cout << "MOVE " << target.x() << " " << target.y() << endl;
        } else {
            cout << "SHOOT " << min_id << endl;
        }
    } else {
        // just stay there
        cout << "MOVE " << me.x() << " " << me.y() << endl;
    }
}


/**
 * @return the number of enemies near me.
 */
int enemies_around(Location &me, vector<Enemy> &enemies, int radius)
{
    return count_if<Enemy>(
      [&] (const Enemy &e) -> bool
        { return (distance(me, e) <= radius); },
      enemies);
}


void play_turn(Location &me, vector<DataPoint> &points, vector<Enemy> &enemies)
{
    // First run if enemy is close
    bool close_strategy = false;
    bool command_executed = false;
    int enemies_around_me = enemies_around(me, enemies, k_enemy_distance);
    int enemies_near_me = enemies_around(me, enemies, k_safe_distance);
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

    if(!command_executed) {
        if (close_strategy) {
            strategy1(me, points, enemies);
        } else {
            strategy2(me, points, enemies);
        }
    }
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
