#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <deque>
#include <sstream>

// uncomment to have more debug info (useful for unit tests)
//#define DEBUG_LOCAL

using namespace std;


enum EntityType
{
    EntityEmpty,
    EntityBox,
    //EntityBombMine,
    EntityBombEnemy,
    EntityMark,
    EntityWall,
    EntityGoodie
};

using Location = pair<int, int>;

class World;
class Player;
class Bomb;

static vector<pair<int, int> > coords_around(int x, int y, bool include_center = false);

static vector<pair<int, int> > coords_around_2(int x, int y, bool include_center = false);

static bool location_safe_for_bomb_p(const Location &loc, const Bomb &b);



class World
{
public:

    World(int width = 1, int height = 1)
    {
        set_world_size(width, height);
    }

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
        return (entity(x, y) != EntityBox && entity(x, y) != EntityWall &&
                entity(x, y) != EntityBombEnemy);
    }

    bool is_empty_or_bomb(int x, int y) const
    {
        return (entity(x, y) != EntityBox && entity(x, y) != EntityWall);
    }

    void set_world_size(int width, int height)
    {
        m_matrix.resize(width);
        for (int i = 0; i < width; i++) {
            m_matrix[i].resize(height);
        }
        m_explosion_matrix.resize(width);
        for (int i = 0; i < width; i++) {
            m_explosion_matrix[i].resize(height);
        }
        m_explosion_access_matrix.resize(width);
        for (int i = 0; i < width; i++) {
            m_explosion_access_matrix[i].resize(height);
        }
    }

    /**
     * Determines what cells are accesible.
     */
    void compute_access_zone(int start_x, int start_y);

    void compute_explosion_access_zone(int start_x, int start_y);

    void compute_explosions(vector<Bomb> &world_bombs);

    bool is_accesible(int x, int y) const;

    bool is_explosion_accesible(int x, int y) const;

    /**
     * @return no. of turns till this cell will explode.
     */
    int get_explosion_timeout(int x, int y) const;

    int vital_space() const
    {
        return m_vital_space;
    }

    bool location_valid_p(int x, int y) const
    {
        return (x >= 0 && x < width() && y >= 0 && y < height());
    }

    // debug / test functions
    void print_explosion_matrix() const
    {
        for (int i = 0; i < width(); i++) {
            for (int j = 0; j < height(); j++) {
                cerr << m_explosion_matrix[i][j];
            }
            cerr << endl;
        }
    }

private:

    void compute_access_zone_rec(int x, int y);

    void compute_explosion_access_zone_rec(int start_x, int start_y);

    vector<vector<EntityType> > m_matrix;
    vector<vector<EntityType> > m_access_matrix;
    vector<vector<EntityType> > m_explosion_access_matrix;
    vector<vector<int> > m_explosion_matrix;

    int m_vital_space;
};


class Player
{
public:

    Player(int id = -1, int x = 0, int y = 0, int bombs_avail = 0)
      : m_id(id), m_x(x), m_y(y), m_bombs_avail(bombs_avail),
        m_range(0), m_bombs_max(bombs_avail)
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

    int id() const
    {
        return m_id;
    }

    void set_params(int id, int i1, int i2, int i3, int i4)
    {
        m_id = id;
        cerr << ":debug: new params for player " << m_id << ": " << i1 << "," << i2
             << ", bombs=" << i3 << ", range=" << i4 << endl;
        m_x = i1;
        m_y = i2;
        m_bombs_avail = i3;
        m_range = i4;
        if (m_bombs_avail > m_bombs_max) {
            m_bombs_max = m_bombs_avail;
        }
    }

    int bombs_available() const
    {
        return m_bombs_avail;
    }

    int bombs_max() const
    {
        return m_bombs_max;
    }

    int bomb_range() const
    {
        return m_range;
    }

private:

    int m_id;

    int m_x;

    int m_y;

    int m_bombs_avail;

    int m_bombs_max;

    int m_range;
};


class Bomb
{
public:

    Bomb()
      : m_x(-1), m_y(-1)
    {};

    Bomb(int owner, int x, int y, int time_remaining, int range = 3)
      : m_owner(owner), m_x(x), m_y(y), m_time_remaining(time_remaining),
        m_range(range), m_effective_timeout(time_remaining)
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

    /*
    void set_params(int i1, int i2, int i3, int i4)
    {
        m_x = i1;
        m_y = i2;
        m_owner = i3;
        m_range = i4;
    }
    */

    int timeout() const
    {
        return m_time_remaining;
    }

    int get_range() const
    {
        return m_range;
    }

    int get_owner() const
    {
        return m_owner;
    }

    vector<Location> get_explosion_area(const World &world);

    void set_effective_timeout(int timeout)
    {
        m_effective_timeout = timeout;
    }

    int effective_timeout() const
    {
        return m_effective_timeout;
    }

    string description() const
    {
        ostringstream ostr;
        ostr << "Bomb: loc(" << m_x << "," << m_y << "), owner=" << m_owner
             <<  ", range=" << m_range << ", time_remaining=" << m_time_remaining
             << ", m_effective_time=" << m_effective_timeout << endl;
        return ostr.str();
    }

private:

    int m_owner;

    int m_x;

    int m_y;

    int m_time_remaining;

    int m_effective_timeout;

    int m_range;
};


World g_world;

Player g_me;

vector<Bomb> g_bombs;


//// start class implementations

void World::compute_access_zone(int start_x, int start_y)
{
    m_vital_space = 0;
    m_access_matrix.clear();
    m_access_matrix = m_matrix;
    compute_access_zone_rec(start_x, start_y);
}


void World::compute_explosion_access_zone(int start_x, int start_y)
{
    m_explosion_access_matrix.clear();
    m_explosion_access_matrix = m_matrix;
    compute_explosion_access_zone_rec(start_x, start_y);
}


void World::compute_access_zone_rec(int x, int y)
{
    m_access_matrix[x][y] = EntityMark;
    m_vital_space++;
    vector<pair<int, int> > around = coords_around(x, y);
    for (auto &coord : around) {
        if (m_access_matrix[coord.first][coord.second] != EntityMark) {
            if (is_empty(coord.first, coord.second)) {
                compute_access_zone_rec(coord.first, coord.second);
            }
        }
    }
}


void World::compute_explosion_access_zone_rec(int x, int y)
{
    m_explosion_access_matrix[x][y] = EntityMark;
    vector<pair<int, int> > around = coords_around(x, y);
    for (auto &coord : around) {
        if (m_explosion_access_matrix[coord.first][coord.second] != EntityMark) {
            if (is_empty_or_bomb(coord.first, coord.second)) {
                compute_explosion_access_zone_rec(coord.first, coord.second);
            }
        }
    }
}


int World::get_explosion_timeout(int x, int y) const
{
    return m_explosion_matrix[x][y];
}


Bomb& find_bomb_with_coords(vector<Bomb> &bombs, int x, int y)
{
    for (Bomb &bomb : bombs) {
        if (bomb.get_x() == x && bomb.get_y() == y) {
            return bomb;
        }
    }
    // we shouldn't be here
    throw string("Cannot find bomb with the specified coords");
}


/**
 * Computes a matrix containing explosion times for every cell.
 */
void World::compute_explosions(vector<Bomb> &world_bombs)
{
    // clear matrix (:fixme: optimize)
    for (int i = 0; i < width(); i++) {
        for (int j = 0; j < height(); j++) {
            m_explosion_matrix[i][j] = 9;
        }
    }
    deque<Bomb> bombs;
    // copy to deque
    for (auto &bomb : world_bombs) {
        bombs.push_back(bomb);
    }
    while (!bombs.empty()) {
        Bomb &bomb = bombs.front();
        bombs.pop_front();
        vector<Location> locations = bomb.get_explosion_area(*this);
        for (Location &loc : locations) {
            int xx = loc.first;
            int yy = loc.second;
            if (m_explosion_matrix[xx][yy] > bomb.effective_timeout()) {
                m_explosion_matrix[xx][yy] = bomb.effective_timeout();
            }
            if (bomb.get_x() != xx || bomb.get_y() != yy) {
                // don't check the same bomb
                if (entity(xx, yy) == EntityBombEnemy) {
                    Bomb &other_bomb = find_bomb_with_coords(world_bombs, xx, yy);
#ifdef DEBUG_LOCAL
                    cerr << ":debug: found a new bomb on path: "
                         << other_bomb.description() << endl;
#endif
                    if (bomb.effective_timeout() < other_bomb.effective_timeout()) {
                            other_bomb.set_effective_timeout(bomb.effective_timeout());
                            bombs.push_back(other_bomb);
                    }
                }
            }
        }
    }
}


bool World::is_accesible(int x, int y) const
{
    return m_access_matrix[x][y] == EntityMark;
}


bool World::is_explosion_accesible(int x, int y) const
{
    return m_explosion_access_matrix[x][y] == EntityMark;
}


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
            if (e == EntityBox || e == EntityWall || e == EntityGoodie) {
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
            if (e == EntityBox || e == EntityWall || e == EntityGoodie) {
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
            if (e == EntityBox || e == EntityWall || e == EntityGoodie) {
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
            if (e == EntityBox || e == EntityWall || e == EntityGoodie) {
                break;
            }
        }
    }
#ifdef DEBUG_LOCAL
    cerr << ":debug: explosion area for bomb(" << get_x() << ", " << get_y() << ")" << endl;
    cerr << ":debug: ";
    for (auto &loc : expl_area) {
        cerr << "(" << loc.first << "," << loc.second << ") ";
    }
    cerr << endl;
#endif

    return expl_area;
}

// make a function from this
        /*
        // :debug:
        for (int i = 0; i < g_world.height(); i++) {
            for (int j = 0; j < g_world.width(); j++) {
                if (g_world.is_accesible(j, i)) {
                    cerr << "O";
                } else {
                    cerr << "X";
                }
            }
            cerr << endl;
        }
        */


//// end class implementations

void read_data(int width, int height, int my_id)
{
    g_bombs.clear();
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
        //cerr << ":debug: got new entity: " << entityType << endl; // :debug:
        if (entityType == 0) {
            if (owner == my_id) {
                g_me.set_params(my_id, x, y, param1, param2);
            }
        } else if (entityType == 1) {
            g_world.matrix()[x][y] = EntityBombEnemy;
            Bomb b(owner, x, y, param1, param2);
            g_bombs.push_back(b);
            cerr << ":debug: bomb at " << x << ", " << y << " with timer "
                 << param1 << " and range " << param2 << endl;
        } else if (entityType == 2) {
            g_world.matrix()[x][y] = EntityGoodie;
            //cerr << ":debug: goodie at " << x << ", " << y << endl;
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
    if (x <= g_world.matrix().size() - 2) {
        result.push_back(make_pair(x + 1, y));
    }
    if (y > 0) {
        result.push_back(make_pair(x, y - 1));
    }
    if (y <= g_world.matrix()[0].size() - 2) {
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


/**
 * :fixme: there will be duplicates and it definitely can be improved.
 */
vector<pair<int, int> > coords_around_2(int x, int y, bool include_center)
{
    vector<Location> result;
    result = coords_around(x, y, include_center);
    vector<Location> extra_cells;
    for (auto &loc : result) {
        vector<Location> v = coords_around(loc.first, loc.second, false);
        extra_cells.insert(extra_cells.end(), v.begin(), v.end());
    }
    result.insert(result.end(), extra_cells.begin(), extra_cells.end());
    return result;
}


bool bomb_around_p(int x, int y)
{
    auto coords = coords_around(x, y);
    for (auto &c : coords) {
        if (g_world.entity(c.first, c.second) == EntityBombEnemy) {
            return true;
        }
    }
    return false;
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

    void clear()
    {
        location = make_pair(-1, -1);
        type = -1;
        cost = 0;
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


/**
 * :fixme: move to World class
 */
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
        if (g_world.entity(x, y) == EntityBombEnemy) {
            // there is a bomb there, ignore
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


int get_no_of_boxes_in_range(int x, int y)
{
    int rc = 0;
    int boxes = 0;
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
    return boxes;
}


static const int k_bonus_for_close_cell = 21;

void compute_cost(int cur_x, int cur_y, int x, int y, int &cost, int &target_type)
{
    if (!g_world.is_empty(x, y)) {
        // we cannot go there, ignore
        target_type = -1;
        cerr << "not empty" << endl; // :debug:
        return;
    }
    int distance = distance_between(make_pair(cur_x, cur_y), make_pair(x, y));
    int boxes = get_no_of_boxes_in_range(x, y);

    // Minimum factor for a box should be 25, otherwise sometimes it doesn't worth going
    // for boxes because they are too far - we want to avoid that
    // On the other hand, if we don't have bombs available, there's no point considering
    // boxes too valuable.
    cost = boxes * (g_me.bombs_available() ? 25 : 5) - distance;
    if (distance < 5) {
        cost += k_bonus_for_close_cell;
    }
    if (distance >= 10) {
        cost -= 25;
    }
    target_type = 0;

    // bomb near location (range = 1)
    bool bomb_near = false;
    if (bomb_around_p(x, y)) {
        bomb_near = true;
    }
    /*
    // check around 2 steps
    if (!bomb_near) {
        vector<Location> locations = coords_around(x, y);
        for (auto &loc : locations) {
            if (bomb_around_p(loc.first, loc.second)) {
                bomb_near = true;
                break;
            }
        }
    }
    */
    if (!bomb_near) {
        for (auto &b : g_bombs) {
            if (distance_between(make_pair(b.get_x(), b.get_y()), make_pair(x, y)) < 3) {
                bomb_near = true;
                break;
            }
        }
    }
    if (bomb_near) {
        cost -= 150;
    }

    // check if safe for bombs
    for (auto &b : g_bombs) {
        if (!location_safe_for_bomb_p(make_pair(x, y), b)) {
            cost -= 100;
        }
    }

    if (g_world.entity(x, y) == EntityGoodie) {
        if (g_me.bomb_range() >= 4 && g_me.bombs_max() >= 2) {
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
bool location_safe_for_bomb_p(const Location &loc, const Bomb &b)
{
    bool safe = true;
    int x = loc.first;
    int y = loc.second;

    bool obstacle = true;
    if (b.get_x() == x && abs(b.get_y() - y) <= b.get_range()) {
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
    }
    if (safe) {
        obstacle = true;
        if (b.get_y() == y && abs(b.get_x() - x) <= b.get_range()) {
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
        }
    }
    //cerr << ":debug: location " << x << ", " << y << " is "
    //     << (safe ? "safe" : "not safe") << " for bomb " << b.get_x() << ", "
    //     << b.get_y() << endl;
    return safe;
}


bool location_safe_p(const Location &loc)
{
    bool safe = true;
    int x = loc.first;
    int y = loc.second;
    for (auto &b : g_bombs) {
        if (!location_safe_for_bomb_p(loc, b)) {
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
    Bomb detonating_bomb;
    for (auto &bomb : g_bombs) {
        if (bomb.timeout() == 2 || bomb.timeout() == 3) {
            bool bomb_is_mine = (bomb.get_owner() == g_me.id());
            if (!bomb_is_mine) {
                g_world.compute_explosion_access_zone(g_me.get_x(), g_me.get_y());
            }
            if (bomb_is_mine ||
                g_world.is_explosion_accesible(bomb.get_x(), bomb.get_y())) {
                fire_in_the_hole = true;
                g_world.compute_explosions(g_bombs);
                break;
            } else {
                cerr << ":debug: bomb " << bomb.description()
                     << " is not accesible" << endl;
            }
        }
    }

    if (fire_in_the_hole) {
        vector<Location> locations = coords_around_2(g_me.get_x(), g_me.get_y(), true);
        for (auto &l : locations) {
            // if we've just placed a bomb, current location won't be empty
            // so we need the special condition
            if ((g_world.is_empty(l.first, l.second) ||
                 (l.first == g_me.get_x() && l.second == g_me.get_y()))
                && g_world.is_accesible(l.first, l.second)
                && g_world.get_explosion_timeout(l.first, l.second) > 3) {
                ret = l;
                break;
            }
        }

        cerr << ":debug: safe location: " << ret.first << ", " << ret.second
             << ", current loc: " << g_me.get_x() << ", " << g_me.get_y() << endl;
    }
    return fire_in_the_hole;
}


bool have_exit_point_from_target(const Target &t)
{
    bool loc_safe = false;
    if (t.type == 0) { // bomb type
        for (int i = 0; i < g_world.width() && !loc_safe; i++) {
            for (int j = 0; j < g_world.height(); j++) {
                if (g_world.is_accesible(i, j)) {
                    Bomb b(0, t.location.first, t.location.second, 2,
                           g_me.bomb_range());
                    if (location_safe_for_bomb_p(make_pair(i, j), b)) {
                        loc_safe = true; // we have at least a safe location
                        break;
                    }
                }
            }
        }
    } else {
        loc_safe = true;
    }
    return loc_safe;
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

    // take all targets and check if there are safe points from them
    while (targets.size() > 0) {
        Target &t = targets[targets.size() - 1];
        if (have_exit_point_from_target(t)) {
            break;
        }
        targets.pop_back();
    }

    if (targets.size() > 0) {
        result = targets[targets.size() - 1];
        cerr << ":debug: best target: " << result.location.first << ", "
             << result.location.second << ", type=" << result.type << ", cost="
             << result.cost << endl;
    }
    return result;
}


int my_bombs()
{
    int counter = 0;
    for (auto &b : g_bombs) {
        if (b.get_owner() == g_me.id()) {
            counter++;
        }
    }
    return counter;
}


void game_loop(int width, int height, int my_id)
{
    Target target;
    static bool first = true;
    static Location previous_loc = make_pair(-1, -1);
    while (1) {
        read_data(width, height, my_id);
        g_world.compute_access_zone(g_me.get_x(), g_me.get_y());

        Location cur_loc = make_pair(g_me.get_x(), g_me.get_y());

        // first try to protect from explosions
        Location emergency_location;
        bool emergency_target = false;
        if (check_iminent_explosion(emergency_location)) {
            // preparing for explosion
            if (emergency_location.first != -1) {
                target.location = emergency_location;
                target.type = 1;
                emergency_target = true;
            }
        }

        // check own explosion and look again for target
        static bool search_again = false;
        if (search_again && !emergency_target) {
            int xx = g_me.get_x();
            int yy = g_me.get_y();
            target.clear();
            search_again = false;
            // first search near me
            int cost = 0;
            int target_type = 0;
            compute_cost(xx, yy, xx, yy, cost, target_type);
            if (cost > k_bonus_for_close_cell) {
                Target test_target(make_pair(g_me.get_x(), g_me.get_y()), 0);
                if (have_exit_point_from_target(test_target)) {
                    // found at least a box - good enough
                    cerr << ":debug: box(es) in range - good enough" << endl;
                    target = test_target;
                }
            }
        }
        for (auto bomb : g_bombs) {
            if (bomb.timeout() == 1) {
                // next turn abort the current target
                search_again = true;
                break;
            }
        }

        bool command_executed = false;
        if (target.is_valid()) {
            // we have a target
            if (target.location == cur_loc) {
                cerr << ":debug: we are here" << endl;
                if (target.type == 0 &&
                    (g_me.bombs_available() &&
                     (my_bombs() == 0  || g_world.vital_space() > 12))) {
                    cout << "BOMB " << cur_loc.first << " " << cur_loc.second << endl;
                    command_executed = true;
                }
                if (emergency_target) {
                    // stay
                    cout << "MOVE " << cur_loc.first << " " << cur_loc.second << endl;
                    command_executed = true;
                }
                target.type = -1;
            } else {
                if (cur_loc == previous_loc && !emergency_target) {
                    cerr << "Something is wrong, we cannot move. Abort" << endl;
                    target.type = -1;
                } else {
                    cout << "MOVE " << target.location.first << " "
                         << target.location.second << " b" << endl;
                    command_executed = true;
                }
            }
        }
        if (!target.is_valid() && !command_executed) {
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
        previous_loc = cur_loc;
    } // end while(1)
}


// unit tests declarations
void test1();


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

    //  Unit tests.
    /*
    test1();
    */
}


// unit tests

void test1()
{
    World world(8, 8);
    vector<Bomb> bombs;

    bombs.push_back(Bomb(0, 2, 2, 8));
    world.matrix()[2][2] = EntityBombEnemy;

    bombs.push_back(Bomb(0, 2, 0, 3));
    world.matrix()[2][0] = EntityBombEnemy;

    world.compute_explosions(bombs);
    world.print_explosion_matrix();
}
