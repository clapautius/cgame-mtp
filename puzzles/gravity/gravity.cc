/**
 * https://www.codingame.com/training/community/gravity
 **/
#include <iostream>
#include <string>
#include <sstream>

using namespace std;


int main()
{
    int width;
    int height;
    cin >> width >> height; cin.ignore();
    char map[height][width];
    for (int i = 0; i < height; i++) {
        string line;
        cin >> line; cin.ignore();
        for (int j = 0; j < width; map[i][j] = line[j], j++);
    }

    for (int x = 0; x < width; x++) {
        // go up and find first '.'
        int h_empty = height - 1;
        while (map[h_empty][x] == '#') {
            h_empty--;
        }
        // find '#' elements and move them
        for (int h = h_empty - 1; h >= 0; h--) {
            // move from [h][x] to [h_empty][x]
            if (map[h][x] == '#') {
                map[h][x] = '.';
                map[h_empty--][x] = '#';
            }
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cout << map[y][x];
        }
        cout << endl;
    }
}
