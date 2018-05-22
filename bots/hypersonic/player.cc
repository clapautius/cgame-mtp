#include <iostream>

#include "player.h" // :grep-out:

using namespace std;


Player::Player(int id, int x, int y, int bombs_avail)
  : m_id(id), m_x(x), m_y(y), m_prev_x(-1), m_prev_y(-1),
    m_bombs_avail(bombs_avail), m_bombs_max(bombs_avail), m_range(0),
    m_direction(DirectionUnknown)
{
    cerr << "new player " << m_id << ": " << m_x << "," << m_y
         << ", bombs.av.=" << m_bombs_avail << ", range=" << m_range << endl;
}


void Player::set_params(int id, int x, int y, int i3, int i4, bool is_me)
{
    m_id = id;
    cerr << "new params for " << (is_me ? "me " : "player ") << m_id << ": "
         << x << "," << y << ", bombs.av.=" << i3 << ", range=" << i4 << endl;
    m_prev_x = m_x;
    m_x = x;
    m_prev_y = m_y;
    m_y = y;
    m_bombs_avail = i3;
    m_range = i4;
    if (m_bombs_avail > m_bombs_max) {
        m_bombs_max = m_bombs_avail;
    }
    if (m_x == m_prev_x) {
        if (m_y == m_prev_y) {
            m_direction = DirectionStationary;
        } else if (m_y > m_prev_y) {
            m_direction = DirectionDown;
        } else {
            m_direction = DirectionUp;
        }
    } else {
        if (m_x == m_prev_x) {
            m_direction = DirectionStationary;
        } else if (m_x > m_prev_x) {
            m_direction = DirectionRight;
        } else {
            m_direction = DirectionLeft;
        }
    }
}
