#ifndef HYPERSONIC_WORLD_H
#define HYPERSONIC_WORLD_H

#include <vector>
#include <string>
#include <sstream>
#include "utils.h" // :grep-out:

class Bomb;

class World
{
public:

    World(int width = 1, int height = 1)
      : m_access_computed(false), m_explosions_computed(false)
    {
        set_world_size(width, height);
    }

    void clear()
    {
        m_access_computed = false;
        m_explosions_computed = false;
    }

    /**
     * This is to avoid changing a lot of code.
     */
    std::vector<std::vector<EntityType>> & matrix()
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
     * Also computes distances from the start position to all other cells.
     */
    void compute_access_zone(int start_x, int start_y,
                             int my_corner_x, int my_corner_y);

    void compute_explosion_access_zone(int start_x, int start_y);

    void compute_explosions(std::vector<Bomb> &world_bombs);

    bool is_accesible(int x, int y) const;

    int distance_to(int x, int y) const
    {
        return m_access_matrix[x][y];
    }

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
    std::string get_explosion_matrix_str() const
    {
        std::ostringstream ostr;
        for (int i = 0; i < width(); i++) {
            for (int j = 0; j < height(); j++) {
                ostr << m_explosion_matrix[i][j];
            }
            ostr << std::endl;
        }
        return ostr.str();
    }

    void set_no_boxes(int boxes)
    {
        m_boxes = boxes;
    }

    int boxes() const
    {
        return m_boxes;
    }

    bool is_closed_area(int x, int y) const;

private:

    void compute_access_zone_rec(int x, int y);

    void compute_explosion_access_zone_rec(int start_x, int start_y);

    std::vector<std::vector<EntityType> > m_matrix;
    std::vector<std::vector<int> > m_access_matrix;
    std::vector<std::vector<int> > m_closed_areas_matrix;
    std::vector<std::vector<EntityType> > m_explosion_access_matrix;
    std::vector<std::vector<int> > m_explosion_matrix;

    int m_vital_space;

    int m_boxes;

    int m_access_computed;

    int m_explosions_computed;
};

#endif
