#include "worldmatrix2d.h"

using std::vector;

WorldMatrix2D::WorldMatrix2D(unsigned width, unsigned height)
  : m_width(width), m_height(height)
{
    for (unsigned int i = 0; i < m_height; i++) {
        vector<int> line;
        line.resize(m_width);
        m_world_matrix.push_back(line);
    }
}


int WorldMatrix2D::at(unsigned x, unsigned y) const
{
    int ret_val = -1;
    if (x < 0 || y < 0 || x > (m_width - 1) || y > (m_height -1)) {
        return ret_val;
    }
    return m_world_matrix[y][x];
}


void WorldMatrix2D::set_at(unsigned x, unsigned y, int value)
{
    if (!(x < 0 || y < 0 || x > (m_width - 1) || y > (m_height -1))) {
        m_world_matrix[y][x] = value;
    }
}
