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

    /**
     * @todo :fixme: add this to a special constructor.
     */
    void fill(int value);

    /**
     * @return the path between start and end or an empty vector if no such path was
     * found.
     */
    static std::vector<cgame::Location> find_path_dfs_between(
      unsigned width, unsigned height,
      std::function<bool (cgame::Location)> is_location_free,
      const cgame::Location &start, const cgame::Location &end, int max_depth);

    static bool location_valid_p(const cgame::Location &loc, unsigned width, unsigned height)
    {
        return (loc.x() >= 0 && loc.x() < width && loc.y() >= 0 && loc.y() < height);
    }

private:

    std::vector<std::vector<int>> m_world_matrix;

    unsigned m_width;

    unsigned m_height;

    void find_path_dfs_between_rec(
      std::function<bool (cgame::Location)> is_location_free,
      const cgame::Location &start, const cgame::Location &end, int max_depth,
      WorldMatrix2D &helper_matrix, std::vector<cgame::Location> &path);

};

#endif
