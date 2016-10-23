#ifndef HYPERSONIC_PLAYER_H
#define HYPERSONIC_PLAYER_H

class Player
{
public:

    enum PlayerDirection
    {
        DirectionUnknown,
        DirectionStationary,
        DirectionUp,
        DirectionDown,
        DirectionLeft,
        DirectionRight
    };

    Player(int id = -1, int x = 0, int y = 0, int bombs_avail = 0);

    int get_x() const
    {
        return m_x;
    }

    int get_y() const
    {
        return m_y;
    }

    int x() const
    {
        return m_x;
    }

    int y() const
    {
        return m_y;
    }

    int id() const
    {
        return m_id;
    }

    void set_params(int id, int x, int y, int i3, int i4, bool is_me = false);

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

    int m_prev_x;
    int m_prev_y;

    int m_bombs_avail;

    int m_bombs_max;

    int m_range;

    PlayerDirection m_direction;
};

#endif
