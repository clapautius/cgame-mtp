#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "cgame-common-tests.h" // :grep-out:

using namespace std;
using namespace cgame;

// unit tests and other debug tests declarations
bool unit_tests();

int cgame_main()
{
    int width;
    int height;
    cin >> width >> height; cin.ignore();
    for (int i = 0; i < height; i++) {
        string line;
        getline(cin, line);
    }
    int startX;
    int startY;
    cin >> startX >> startY; cin.ignore();
    int targetX;
    int targetY;
    cin >> targetX >> targetY; cin.ignore();
    int switchCount;
    cin >> switchCount; cin.ignore();
    for (int i = 0; i < switchCount; i++) {
        int switchX;
        int switchY;
        int blockX;
        int blockY;
        int initialState; // 1 if blocking, 0 otherwise
        cin >> switchX >> switchY >> blockX >> blockY >> initialState; cin.ignore();
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << "RRRRDDDDRRRRUUUULRUUUUUUUUUUUUUDDUDDDDDDDDDDDDDDDDLLLLUUUUUUUULLUULLRRDDRRDDDDDDDDLLLLLLUULLUUUULLUUUUUUL" << endl;
    return 0;
}
