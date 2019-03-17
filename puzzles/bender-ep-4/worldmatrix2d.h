#ifndef WORLDMATRIX2D_H
#define WORLDMATRIX2D_H

#include <vector>

#include "cgame-common.h" // :grep-out:

class WorldMatrix2D
{
public:

    WorldMatrix2D(unsigned width, unsigned height);

    unsigned width() const
    {
        return m_width;
    }

    unsigned height() const
    {
        return m_height;
    }

    /**
     * @return -1 if coordinates are invalid,
     * or the value in the cell otherwise.
     */
    int at(unsigned x, unsigned y) const;

    /**
     * If the coordinates are invalid, the result is undefined.
     */
    void set_at(unsigned x, unsigned y, int value);

private:

    std::vector<std::vector<int>> m_world_matrix;

    unsigned m_width;

    unsigned m_height;
};

#endif
