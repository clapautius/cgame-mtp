#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

using Location = pair<int, int>;


static vector<pair<int, int> > coords_around(int x, int y, bool include_center = false);

enum EntityType
{
    EntityEmpty,
    EntityBox,
    EntityBombMine,
    EntityBombEnemy,
    EntityMark,
    EntityWall,
    EntityGoodie
};


class World
{
public:

    /**
     * This is to avoid changing a lot of code.
     */
    vector<vector<EntityType>> & matrix()
    {
        return m_matrix;
    };

    int width() const
    {
        return m_matrix.size();
    }

    int height() const
    {
        return m_matrix[0].size();
    }

    EntityType entity(int x, int y) const
    {
        return m_matrix[x][y];
    }

    /**
     * @return true if the player can move there.
     */
    bool is_empty(int x, int y) const
    {
        // :fixme: - check rules (bombs, other players)
        return (entity(x, y) != EntityBox && entity(x, y) != EntityWall);
    }

    void set_world_size(int width, int height)
    {
        m_matrix.resize(width);
        for (int i = 0; i < width; i++) {
            m_matrix[i].resize(height);
        }
    }

    /**
     * Determines what cells are accesible.
     */
    void compute_access_zone(int start_x, int start_y);

    bool is_accesible(int x, int y) const;

private:

    void compute_access_zone_rec(int x, int y);

    vector<vector<EntityType> > m_matrix;
    vector<vector<EntityType> > m_access_matrix;

};


void World::compute_access_zone(int start_x, int start_y)
{
    m_access_matrix.clear();
    m_access_matrix = m_matrix;

    compute_access_zone_rec(start_x, start_y);
}


void World::compute_access_zone_rec(int x, int y)
{
    m_access_matrix[x][y] = EntityMark;
    vector<pair<int, int> > around = coords_around(x, y);
    for (auto &coord : around) {
        if (m_access_matrix[coord.first][coord.second] != EntityMark) {
            if (entity(coord.first, coord.second) != EntityBox &&
                entity(coord.first, coord.second) != EntityWall) {
                compute_access_zone_rec(coord.first, coord.second);
            }
        }
    }
}


bool World::is_accesible(int x, int y) const
{
    return m_access_matrix[x][y] == EntityMark;
}


class Player
{
public:

    Player(int type, int x = 0, int y = 0, int bombs_avail = 0)
      : m_type(type), m_x(x), m_y(y), m_bombs_avail(bombs_avail),
        m_range(0)
    {
    }

    int get_x() const
    {
        return m_x;
    }

    int get_y() const
    {
        return m_y;
    }

    void set_params(int i1, int i2, int i3, int i4)
    {
        cerr << ":debug: new params for player " << m_type << ": " << i1 << "," << i2
             << ", bombs=" << i3 << ", range=" << i4 << endl;
        m_x = i1;
        m_y = i2;
        m_bombs_avail = i3;
        m_range = i4;
    }

    int bombs_available() const
    {
        return m_bombs_avail;
    }

    int bomb_range() const
    {
        return m_range;
    }

private:

    int m_type;

    int m_x;

    int m_y;

    int m_bombs_avail;

    int m_range;
};


class Bomb
{
public:

    Bomb(int owner, int x, int y, int time_remaining, int range = 3)
      : m_owner(owner), m_x(x), m_y(y), m_time_remaining(time_remaining),
        m_range(range)
    {
    }

    int get_x() const
    {
        return m_x;
    }

    int get_y() const
    {
        return m_y;
    }

    void set_params(int i1, int i2, int i3, int i4)
    {
        m_x = i1;
        m_y = i2;
        m_owner = i3;
        m_range = i4;
    }

    int timeout() const
    {
        return m_time_remaining;
    }

private:

    int m_owner;

    int m_x;

    int m_y;

    int m_time_remaining;

    int m_range;
};


World g_world;

Player g_me(0) ;
Player g_enemy(1); // :fixme: make an array or something
vector<Bomb> g_enemy_bombs;

void read_data(int width, int height, int my_id)
{
    g_enemy_bombs.clear();
    for (int i = 0; i < height; i++) {
        string row;
        getline(cin, row);
///*
        cerr << row << endl; // :debug:
//*/
        for (int j = 0; j < width; j++) {
            if (row[j] == '.') {
                g_world.matrix()[j][i] = EntityEmpty;
            } else if (row[j] == '0' || row[j] == '1' || row[j] == '2' || row[j] == '3') {
                g_world.matrix()[j][i] = EntityBox;
            } else if (row[j] == 'X') {
                g_world.matrix()[j][i] = EntityWall;
            } else {
                // this should not happen but hope for the best
                g_world.matrix()[j][i] = EntityEmpty;
            }
        }
    }
    int entities;
    cin >> entities; cin.ignore();
    for (int i = 0; i < entities; i++) {
        int entityType;
        int owner;
        int x;
        int y;
        int param1;
        int param2;
        cin >> entityType >> owner >> x >> y >> param1 >> param2; cin.ignore();
        if (entityType == 0) {
            if (owner == my_id) {
                g_me.set_params(x, y, param1, param2);
            } else {
                g_enemy.set_params(x, y, param1, param2);
            }
        } else if (entityType == 1) {
            if (owner == my_id) {
                g_world.matrix()[x][y] = EntityBombMine;
            } else {
                g_world.matrix()[x][y] = EntityBombEnemy;
                Bomb b(owner, x, y, param1, param2);
                g_enemy_bombs.push_back(b);
                cerr << ":debug: enemy bomb at " << x << ", " << y << " with timer "
                     << param1 << " and range " << param2 << endl;
            }
        } else if (entityType == 2) {
            g_world.matrix()[x][y] = EntityGoodie;
        }
    }
}


vector<pair<int, int> > coords_around(int x, int y, bool include_center)
{
    vector<pair<int, int> > result;
    if (include_center) {
        result.push_back(make_pair(x, y));
    }
    if (x > 0) {
        result.push_back(make_pair(x - 1, y));
    }
    if (x < g_world.matrix().size() - 2) {
        result.push_back(make_pair(x + 1, y));
    }
    if (y > 0) {
        result.push_back(make_pair(x, y - 1));
    }
    if (y < g_world.matrix()[0].size() - 2) {
        result.push_back(make_pair(x, y + 1));
    }
/*
    cerr << ":debug: valid around (" << x << "," << y << "):";
    for (auto &c : result) {
        cerr << "(" << c.first << "," << c.second << ") ";
    }
    cerr << endl;
*/
    return result;
}


pair<int, int> find_box(vector<vector<EntityType>> &m, int x, int y)
{
    cerr << ":debug: in find_box(" << x << "," << y << ")" << endl;
    if (m[x][y] == EntityBox) {
        return make_pair(x, y);
    } else {
        m[x][y] = EntityMark;
        vector<pair<int, int> > around = coords_around(x, y);
        for (auto &coord : around) {
            if (m[coord.first][coord.second] != EntityMark) {
                pair<int, int> result = find_box(m, coord.first, coord.second);
                if (result.first != -1) {
                    return result;
                }
            }
        }
        return make_pair(-1, -1);
    }
}


bool near_box_p(int x, int y)
{
    vector<pair<int, int> > around = coords_around(x, y);
    for (auto &coord : around) {
        if (g_world.matrix()[coord.first][coord.second] == EntityBox) {
            return true;
        }
    }
    return false;
}


bool near_coord_p(int x, int y, int target_x, int target_y)
{
    if (x == target_x && y == target_y) {
        return true;
    }
    vector<pair<int, int> > around = coords_around(x, y);
    for (auto &coord : around) {
        if (coord.first == target_x && coord.second == target_y) {
            return true;
        }
    }
    return false;
}


struct Target
{
    Target(Location l = make_pair(0, 0), int t = -1, int c = 0)
      : location(l), type(t), cost(c)
    {};

    bool is_valid() const
    {
        return type != -1;
    }

    Location location;

    int type; // -1 - no target, 0 - bomb, 1 - just move there

    int cost;
};


/**
 * Compute Manhattan distance between two locations.
 */
int distance_between(const Location &loc1, const Location &loc2)
{
    return abs(loc1.first - loc2.first) + abs(loc1.second - loc2.second);
}


bool location_valid_p(int x, int y)
{
    return (x >= 0 && x < g_world.width() && y >= 0 && y < g_world.height());
}

/**
 * @param[out] action_after : 0 - nothing, 1 - continue, 2 - break
 */
int check_boxes_in_range(int x, int y, int &boxes)
{
    if (location_valid_p(x, y)) {
        if (g_world.entity(x, y) == EntityBombMine) {
            // we have a bomb there, ignore
            return 1; // to continue
        }
        if (g_world.matrix()[x][y] == EntityBox) {
            ++boxes;
            return 2; // to break
        }
        if (g_world.entity(x, y) == EntityWall) {
            // will block the explosion
            return 2;
        }
    }
    return 0;
}


void compute_cost(int cur_x, int cur_y, int x, int y, int &cost, int &target_type)
{
    if (g_world.matrix()[x][y] == EntityBox || g_world.entity(x, y) == EntityWall) {
        // we cannot go there, ignore
        target_type = -1;
        cerr << "not empty" << endl; // :debug:
        return;
    }
    int distance = distance_between(make_pair(cur_x, cur_y), make_pair(x, y));
    int boxes = 0;
    int rc = 0;
    // go left
    for (int i = x; i >= x - g_me.bomb_range() + 1; i--) {
        rc = check_boxes_in_range(i, y, boxes);
        if (rc == 1) {
            continue;
        }
        if (rc == 2) {
            break;
        }
    }
    // go right
    for (int i = x + 1; i < x + g_me.bomb_range(); i++) {
        rc = check_boxes_in_range(i, y, boxes);
        if (rc == 1) {
            continue;
        }
        if (rc == 2) {
            break;
        }
    }
    // go up
    for (int i = y; i >= y - g_me.bomb_range() + 1; i--) {
        rc = check_boxes_in_range(x, i, boxes);
        if (rc == 1) {
            continue;
        }
        if (rc == 2) {
            break;
        }
    }
    // go down
    for (int i = y + 1; i < y + g_me.bomb_range(); i++) {
        rc = check_boxes_in_range(x, i, boxes);
        if (rc == 1) {
            continue;
        }
        if (rc == 2) {
            break;
        }
    }
    // Minimum factor for a box should be 25, otherwise sometimes it doesn't worth going
    // for boxes because they are too far - we want to avoid that
    // On the other hand, if we don't have bombs available, there's no point considering
    // boxes too valuable.
    cost = boxes * (g_me.bombs_available() ? 25 : 10) - distance;
    if (distance < 5) {
        cost += 11;
    }
    target_type = 0;

    if (g_world.entity(x, y) == EntityGoodie) {
        if (g_me.bomb_range() > 5 && g_me.bombs_available() > 1) {
            cost += 26; // not that important
        } else {
            cost += 76; // important
        }
        if (boxes == 0) {
            target_type = 1;
        }
    }

    // :debug:
    cerr << "[" << x << "," << y << "]<" << boxes << "," << distance << "," << cost << "> ";
}


// :fixme: - this is really incomplete
bool location_safe_p(Location loc)
{
    bool safe = true;
    int x = loc.first;
    int y = loc.second;
    for (auto &b : g_enemy_bombs) {
        bool obstacle = true;
        if (b.get_x() == x && abs(b.get_y() - y) <= g_enemy.bomb_range()) {
            obstacle = false;
            for (int i = min(b.get_y(), y); i <= max(b.get_y(), y); i++) {
                EntityType e = g_world.entity(x, i);
                if (e == EntityBox || e == EntityWall) {
                    obstacle = true;
                    break;
                }
            }
        }
        if (!obstacle) {
            safe = false;
            break;
        }
        obstacle = true;
        if (b.get_y() == y && abs(b.get_x() - x) <= g_enemy.bomb_range()) {
            obstacle = false;
            for (int i = min(b.get_x(), x); i <= max(b.get_x(), x); i++) {
                EntityType e = g_world.entity(i, y);
                if (e == EntityBox || e == EntityWall) {
                    obstacle = true;
                    break;
                }
            }
        }
        if (!obstacle) {
            safe = false;
            break;
        }
    }
    cerr << ":debug: location " << x << ", " << y << " is "
         << (safe ? "safe" : "not safe") << endl;
    return safe;
}


bool check_iminent_explosion(Location &ret)
{
    ret = make_pair(-1, -1);
    bool fire_in_the_hole = false;
    for (auto bomb : g_enemy_bombs) {
        if (bomb.timeout() == 2) {
            fire_in_the_hole = true;
            break;
        }
    }
    if (fire_in_the_hole) {
        vector<Location> locations = coords_around(g_me.get_x(), g_me.get_y(), true);
        for (auto &l : locations) {
            if (g_world.is_empty(l.first, l.second) && location_safe_p(l)) {
                ret = l;
                break;
            }
        }
        cerr << ":debug: safe location: " << ret.first << ", " << ret.second
             << ", current loc: " << g_me.get_x() << ", " << g_me.get_y() << endl;
    }
    return fire_in_the_hole;
}


Target compute_next_target(bool ignore_current_position)
{
    vector<Target> targets;
    Target result;
    for (int i = 0; i < g_world.height(); i++) {
        cerr << ":debug: cost(...," << i << "): "; // :debug:
        for (int j = 0; j < g_world.width(); j++) {
            if (!(ignore_current_position && j == g_me.get_x() && i == g_me.get_y())) {
                if (g_world.is_accesible(j, i)) {
                    int target_type = -1;
                    int cost;
                    compute_cost(g_me.get_x(), g_me.get_y(), j, i, cost, target_type);
                    if (target_type != -1) {
                        targets.push_back(Target(make_pair(j, i), target_type, cost));
                    }
                }
            }
        }
        cerr << endl; // :debug:
    }
    // sort by cost (cost is actually gain :fixme:)
    sort(targets.begin(), targets.end(),
         [] (const Target &a, const Target &b) -> bool
         { return a.cost < b.cost; });

    if (targets.size() > 0) {
        result = targets[targets.size() - 1];
        cerr << ":debug: best target: " << result.location.first << ", "
             << result.location.second << ", type=" << result.type << ", cost="
             << result.cost << endl;
    }
    return result;
}


void game_loop(int width, int height, int my_id)
{
    Target target;
    static bool first = true;
    while (1) {
        read_data(width, height, my_id);
        g_world.compute_access_zone(g_me.get_x(), g_me.get_y());
        Location cur_loc = make_pair(g_me.get_x(), g_me.get_y());

        // first try to protect from explosions
        Location emergency_location;
        if (check_iminent_explosion(emergency_location)) {
            // preparing for explosion
            if (emergency_location.first != -1) {
                target.location = emergency_location;
                target.type = 1;
            }
        }
        if (target.is_valid()) {
            // we have a target
            if (target.location == cur_loc) {
                cerr << ":debug: we are here" << endl;
                if (target.type == 0 && g_me.bombs_available()) {
                    cout << "BOMB " << cur_loc.first << " " << cur_loc.second << endl;
                } else {
                    // just stay here
                    cout << "MOVE " << cur_loc.first << " " << cur_loc.second
                         << " a" << endl;
                }
                target.type = -1;
            } else {
                cout << "MOVE " << target.location.first << " "
                     << target.location.second << " b" << endl;
            }
        } else {
            // if not first time, ignore current position
            target = compute_next_target(!first);
            if (target.is_valid()) {
                cout << "MOVE " << target.location.first << " "
                     << target.location.second << " c" << endl;
            } else {
                // just stay here
                cout << "MOVE " << cur_loc.first << " " << cur_loc.second << " d" << endl;
            }
        }
        if (first) {
            first = false;
        }
    }
}


/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int width;
    int height;
    int myId;
    cin >> width >> height >> myId; cin.ignore();
    cerr << ":debug: width=" << width << ", height: " << height
         << ", myId=" << myId << endl;
    g_world.set_world_size(width, height);

    game_loop(width, height, myId);
}
