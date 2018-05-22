#include <iostream>
#include "cgame-common.h" // :grep-out:
#include "params.h" // :grep-out:
#include "world.h" // :grep-out:
#include "bomb.h" // :grep-out:

using namespace std;

// unit tests

bool test1()
{
    static int test_no = 1;
#define TEST_EQ(X, Y) do                                                \
    { ++test_no;                                                        \
        if (X != Y) {                                                   \
            std::cerr << "test " << test_no << " fail: " << X << "," << Y << std::endl; \
            return false; };                                            \
    } while(0)

    World world(8, 8);
    vector<Bomb> bombs;
    bombs.push_back(Bomb(0, 2, 2, 8));
    world.matrix()[2][2] = EntityBombEnemy;
    bombs.push_back(Bomb(0, 2, 0, 3));
    world.matrix()[2][0] = EntityBombEnemy;
    world.compute_explosions(bombs);
    string s1 = world.get_explosion_matrix_str();
    ostringstream ostr;
    ostr << "39399999" << endl <<
      "39399999" << endl <<
      "33333999" << endl <<
      "39399999" << endl <<
      "39399999" << endl <<
      "99999999" << endl <<
      "99999999" << endl <<
      "99999999" << endl;
    string s2 = ostr.str();
    TEST_EQ(s1, s2);

    return true;
}


bool unit_tests()
{
    return test1();
}
