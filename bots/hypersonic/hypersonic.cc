#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

using Location = pair<int, int>;

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
        cerr << ":debug: new params for type " << m_type << ": " << i1 << "," << i2
             << "," << i3 << "," << i4 << endl;
        m_x = i1;
        m_y = i2;
        m_bombs_avail = i3;
        m_range = i4;
    }

    bool bomb_available() const
    {
        return m_bombs_avail > 0;
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

private:

    int m_owner;

    int m_x;

    int m_y;

    int m_time_remaining;

    int m_range;
};


Player g_me(0) ;
Player g_enemy(1); // :fixme: make an array or something

enum EntityType
{
    EntityEmpty,
    EntityBox,
    EntityBomb,
    EntityMark
};


vector<vector<EntityType> > g_matrix;
int g_width = 0;
int g_height = 0;

void read_data(int width, int height, int my_id)
{
    for (int i = 0; i < height; i++) {
        string row;
        getline(cin, row);
/*
        cerr << row << endl; // :debug:
*/
        for (int j = 0; j < width; j++) {
            if (row[j] == '.') {
                g_matrix[j][i] = EntityEmpty;
            } else if (row[j] == '0' || row[j] == '1' || row[j] == '2' || row[j] == '3') {
                g_matrix[j][i] = EntityBox;
            } else {
                // this should not happen but hope for the best
                g_matrix[j][i] = EntityEmpty;
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
        }
        // :fixme: ignore the bombs for now
    }
    g_width = width;
    g_height = height;
}


vector<pair<int, int> > coords_around(int x, int y)
{
    vector<pair<int, int> > result;
    if (x > 0) {
        result.push_back(make_pair(x - 1, y));
    }
    if (x < g_matrix.size() - 2) {
        result.push_back(make_pair(x + 1, y));
    }
    if (y > 0) {
        result.push_back(make_pair(x, y - 1));
    }
    if (y < g_matrix[0].size() - 2) {
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
        if (g_matrix[coord.first][coord.second] == EntityBox) {
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
    return (x >= 0 && x < g_width && y >= 0 && y < g_height);
}


void compute_cost(int cur_x, int cur_y, int x, int y, int &cost, int &target_type)
{
    cerr << ":debug: cost(" << x << ", " << y << "): "; // :debug:
    if (g_matrix[x][y] == EntityBox) {
        // we cannot go there, ignore
        target_type = -1;
        cerr << "not empty" << endl; // :debug:
        return;
    }
    int distance = distance_between(make_pair(cur_x, cur_y), make_pair(x, y));
    int boxes = 0;
    for (int i = x - g_me.bomb_range() + 1; i < x + g_me.bomb_range(); i++) {
        if (location_valid_p(i, y) && g_matrix[i][y] == EntityBox) {
            ++boxes;
            break;
        }
    }
    for (int i = y - g_me.bomb_range() + 1; i < y + g_me.bomb_range(); i++) {
        if (location_valid_p(x, i) && g_matrix[x][i] == EntityBox) {
            ++boxes;
            break;
        }
    }
    // minimum factor for a box should be 25, otherwise sometimes it doesn't worth going
    // for boxes because they are too far - we want to avoid that
    cost = boxes * 25 - distance;
    target_type = 0;

    // :debug:
    cerr << "boxes=" << boxes << ", distance=" << distance << ", cost=" << cost << endl;
}


Target compute_next_target(bool ignore_current_position)
{
    vector<Target> targets;
    Target result;
    for (int i = 0; i < g_height; i++) {
        for (int j = 0; j < g_width; j++) {
            if (!(ignore_current_position && j == g_me.get_x() && i == g_me.get_y())) {
                int target_type = -1;
                int cost;
                compute_cost(g_me.get_x(), g_me.get_y(), j, i, cost, target_type);
                if (target_type != -1) {
                    targets.push_back(Target(make_pair(j, i), target_type, cost));
                }
            }
        }
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


void game_loop_level2(int width, int height, int my_id)
{
    Target target;
    static bool first = true;
    while (1) {
        read_data(width, height, my_id);
        Location cur_loc = make_pair(g_me.get_x(), g_me.get_y());
        if (target.is_valid()) {
            // we have a target
            if (target.location == cur_loc) {
                cerr << ":debug: we are here" << endl;
                if (target.type == 0) {
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


void game_loop_level1(int width, int height, int my_id)
{
    static pair<int, int> target;
    static bool first = true;
    while (1) {
        read_data(width, height, my_id);
        // first move to the center of the matrix
        if (first) {
            for (int i = 4; i < 7; i++) {
                if (g_matrix[5][i] != EntityBox) {
                    target = make_pair(5, i);
                }
            }
            first = false;
        }
        if (target.first != -1) {
            // we have a target
            if (near_coord_p(g_me.get_x(), g_me.get_y(), target.first, target.second)) {
                // we arrived near a target
                target.first = -1;
                if (g_me.bomb_available()) {
                    cout << "BOMB " << g_me.get_x() << " " << g_me.get_y() << endl;
                } else {
                    // just stay here
                    cerr << ":debug: no bomb available" << endl;
                    cout << "MOVE " << g_me.get_x() << " " << g_me.get_y() << endl;
                }
            } else {
                // keep moving
                cout << "MOVE " << target.first << " " << target.second << endl;
            }
        } else {
            vector<vector<EntityType>> new_matrix = g_matrix;
            pair<int, int> box = find_box(new_matrix, g_me.get_x(), g_me.get_y());
            cerr << ":debug: found box at " << box.first << ", " << box.second << endl;
            if (box.first == -1) {
                // no box, just stay here
                cerr << ":debug: no box found (strange)" << endl;
                cout << "MOVE " << g_me.get_x() << " " << g_me.get_y() << endl;
            } else {
                target = box;
                cout << "MOVE " << box.first << " " << box.second << endl;
            }
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
    g_matrix.resize(width);
    for (int i = 0; i < width; i++) {
        g_matrix[i].resize(height);
    }

    game_loop_level2(width, height, myId);
}
