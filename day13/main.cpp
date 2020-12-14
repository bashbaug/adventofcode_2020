#include <algorithm>
#include <cinttypes>
#include <climits>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
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

int64_t findEarliest_iterate(int64_t start, int64_t step, int bus, int delta)
{
    int64_t earliest = start;
    while ((earliest + delta) % bus != 0) {
        earliest += step;
    };
    return earliest;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    int target = 0;
    std::getline(is, str);
    std::sscanf(str.c_str(), "%d", &target);

    std::getline(is, str);

    int minwait = INT_MAX;
    int minbus = 0;

    std::vector<std::tuple<int,int>> busses;
    size_t pos = 0;
    int index = 0;
    do {
        std::string token;
        pos = nextToken(str, ",", pos, token);
        if (token == "x") {
            if (pos == std::string::npos) break;
            ++index;
            continue;
        }

        int bus = 0;
        std::sscanf(token.c_str(), "%d", &bus);

        int wait = bus - (target % bus);
        printf("Found bus %d, delta = %d: wait = %d\n", bus, index, wait);
        busses.push_back(std::make_tuple(bus, index));
        ++index;

        if (wait < minwait) {
            minwait = wait;
            minbus = bus;
        }

        if (pos == std::string::npos) break;
    } while(true);

    printf("Earliest bus is %d: wait is %d: product is %d\n", minbus, minwait, minbus * minwait);

    std::sort(busses.begin(), busses.end(), std::greater<>());

    int64_t earliest = 0;
    int64_t step = 1;
    index = 0;
    while (index < busses.size()) {
        int bus = std::get<0>(busses[index]);
        int delta = std::get<1>(busses[index]);
        printf("Testing bus %d: start time = %" PRId64 ", step = %" PRId64 "\n", bus, earliest, step);
        earliest = findEarliest_iterate(earliest, step, bus, delta);
        step *= bus;
        ++index;
    }

    printf("Start time is %" PRId64 "\n", earliest);

    return 0;
}
