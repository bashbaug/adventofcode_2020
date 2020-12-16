#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

size_t nextToken(const std::string& str, const std::string& delims, size_t pos, std::string& token)
{
    size_t newpos = str.find_first_of(delims, pos);
    if (newpos == std::string::npos) {
        token = str.substr(pos);
    } else {
        token = str.substr(pos, newpos - pos);
        ++newpos;
    }

    return newpos;
}

#define MAX_TURN 30000000
//#define USE_MAP
#define USE_UNORDERED_MAP

// std::map is about 14 seconds
// std::unordered_map is about 4 seconds
// std::vector is about 1 second

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    int turn = 0;
    int lastnumber = 0;
    std::getline(is, str);

#if defined(USE_MAP)
    std::map<int, int> lastseenmap;
#elif defined(USE_UNORDERED_MAP)
    std::unordered_map<int, int> lastseenmap;
#else
    std::vector<int> lastseenmap(MAX_TURN);
    std::fill(lastseenmap.begin(), lastseenmap.end(), 0);
#endif

    size_t pos = 0;
    do {
        std::string token;
        pos = nextToken(str, ",\n", pos, token);

        std::sscanf(token.c_str(), "%d", &lastnumber);
        printf("Read: %d\n", lastnumber);

        ++turn;
        lastseenmap[lastnumber] = turn;

        if (pos == std::string::npos) break;
    } while(true);

    int v2020 = 0;
    int v30000000 = 0;

    while(turn != MAX_TURN) {
        int lastturn = lastseenmap[lastnumber];
        lastseenmap[lastnumber] = turn;

        lastnumber = 0;
        if (lastturn != turn && lastturn != 0) {
            lastnumber = turn - lastturn;
        }

        ++turn;

        if (turn == 2020) {
            v2020 = lastnumber;
        }
        if (turn == 30000000) {
            v30000000 = lastnumber;
        }
    }

    printf("The 2020th number_old is: %d\n", v2020);
    printf("The 30000000th number_old is: %d\n", v30000000);

    printf("The size of the map is: %d\n", (int)lastseenmap.size());

    return 0;
}
