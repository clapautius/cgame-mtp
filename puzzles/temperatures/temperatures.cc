/**
 * https://www.codingame.com/training/easy/temperatures
 **/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int n; // the number of temperatures to analyse
    cin >> n; cin.ignore();
    string temps; // the n temperatures expressed as integers ranging from -273 to 5526
    getline(cin, temps);

    int result = 5527;
    if (n == 0) {
        result = 0;
    } else {
        std::istringstream istr(temps);
        while(istr.good()) {
            int val;
            istr >> val;
            if (abs(val) < abs(result)) {
                result = val;
            } else if (result == -1 * val) {
                result = fabs(result);
            }
        }
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << result << endl;
}
