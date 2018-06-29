#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "shuttle.h" // :grep-out:
#include "world.h" // :grep-out:
#include "cgame-common-tests.h" // :grep-out:

using namespace std;
using namespace cgame;

// unit tests and other debug tests declarations
bool unit_tests();

int cgame_main()
{
    World world;
    world.read_world();

    Shuttle shuttle;
    // game loop
    while (1) {
        shuttle.read_data();
        cerr << __FUNCTION__ << shuttle.to_str() << endl;

        int target_speed = -38;
        int max_power = 4;
        if (shuttle.y() > 1800) {
            target_speed = -15;
            max_power = 3;
        }
        if (shuttle.v_speed() > target_speed + 1) {
            shuttle.decrease_power();
        } else if (shuttle.v_speed() < target_speed - 1) {
            if (shuttle.power() < max_power) {
                shuttle.increase_power();
            }
        }

        // 2 integers: rotate power. rotate is the desired rotation angle (should be 0 for level 1), power is the desired thrust power (0 to 4).
        cout << "0 " << shuttle.power() << endl;
    }
}
