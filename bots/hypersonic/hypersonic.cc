#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;


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
            } else if (row[j] == '0') {
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


void game_loop(int width, int height, int my_id)
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

    game_loop(width, height, myId);
}
