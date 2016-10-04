/*
 * https://www.codingame.com/ide/5824534481e9ac37af20406dc1bf57b8738a277
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// uncomment to get lots of debug messages on cerr
//#define DEBUG

bool unit_tests();

string get_mimetype(const map<string, string> &mimetypes, const string &fname)
{
    size_t pos = fname.find_last_of('.');
    if (pos != string::npos) {
        string ext = fname.substr(pos + 1);
        transform(ext.begin(), ext.end(), ext.begin(), ::toupper);
#ifdef DEBUG
        cerr << ":debug: ext=" << ext << endl;
#endif
        if (mimetypes.count(ext) > 0) {
            return mimetypes.at(static_cast<const string>(ext));
        }
    }
    return "UNKNOWN";
}


/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main(int argc, char *argv[])
{
    if (argc > 1 && string(argv[1]) == "--tests") {
        exit(unit_tests() ? 0 : 1);
    }

    map<string, string> mimetypes;
    int N; // Number of elements which make up the association table.
    cin >> N; cin.ignore();
    int Q; // Number Q of file names to be analyzed.
    cin >> Q; cin.ignore();
    for (int i = 0; i < N; i++) {
        string ext; // file extension
        string mt; // MIME type.
        cin >> ext >> mt; cin.ignore();
        transform(ext.begin(), ext.end(), ext.begin(), ::toupper);
        mimetypes[ext] = mt;
    }
    for (int i = 0; i < Q; i++) {
        string fname; // One file name per line.
        getline(cin, fname);
        cout << get_mimetype(mimetypes, fname) << endl;
    }
}


bool unit_tests()
{
#define TEST_EQ(X, Y) do                                                \
    { if (get_mimetype(mimetypes, X) != Y) {                            \
            cerr << "fail: " << X << "," << Y << endl; return false; }; \
    } while(0)

    map<string, string> mimetypes;
    mimetypes["PNG"] = "png-type";
    TEST_EQ("a.b", "UNKNOWN");
    TEST_EQ("mmm", "UNKNOWN");
    TEST_EQ("a.pNg", "png-type");
    TEST_EQ("", "UNKNOWN");
    return true;
}
