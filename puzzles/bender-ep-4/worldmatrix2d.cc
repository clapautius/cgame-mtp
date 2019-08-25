#include "worldmatrix2d.h"

using std::vector;
using cgame::Location;

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


void WorldMatrix2D::fill(int value)
{
    for (unsigned i = 0; i < m_height; i++)
        for (unsigned j = 0; j < m_width; j++)
            m_world_matrix[i][j] = value;
}


void WorldMatrix2D::find_path_dfs_between_rec(
  std::function<bool (Location)> is_location_free,
  const Location &start, const Location &end, int max_depth,
  WorldMatrix2D &helper_matrix, vector<Location> &path)
{

}


std::vector<Location> WorldMatrix2D::find_path_dfs_between(
  unsigned width, unsigned height,
  std::function<bool (Location)> is_location_free,
  const Location &start, const Location &end, int max_depth)
{
    vector<Location> ret;
    // check if the start position is valid
    if (!location_valid_p(start, width, height)) {
        return ret;
    }
    WorldMatrix2D helper_matrix(width, height);
    helper_matrix.fill(-1);

}
