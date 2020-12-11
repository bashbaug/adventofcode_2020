#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);

    std::vector<int> adapters;
    std::string str;

    adapters.push_back(0);

    while (std::getline(is, str)) {
        int num = 0;
        std::sscanf(str.c_str(), "%d", &num);
        adapters.push_back(num);
    }

    std::sort(adapters.begin(), adapters.end());
    adapters.push_back(adapters.back() + 3);

    int jolts = 0;
    int ones = 0;
    int threes = 0;

    for (auto adapter : adapters ) {
        if (adapter == 0) {
            continue;
        }
        else if (adapter - jolts == 1) {
            ones++;
        } else if (adapter - jolts == 3) {
            threes++;
        } else {
            printf("weird: adapter = %d, jolts = %d\n", adapter, jolts);
        }

        jolts = adapter;
    }

    std::cout << "Device adapter rated for " << jolts << "\n";
    std::cout << "ones = " << ones << ", threes = " << threes << ", ones x threes = " << ones * threes << "\n";

    std::vector<uint64_t> solutions(jolts + 1);
    std::fill(solutions.begin(), solutions.end(), 0);

    solutions[jolts] = 1;
    int index = adapters.size() - 2;
    do {
        int test = adapters[index];
        solutions[test] =
            solutions[test + 1] +
            solutions[test + 2] +
            solutions[test + 3];
    } while(index-- > 0);

    std::cout << "Found " << solutions[0] << " solutions\n";

    return 0;
}

// 0, 3, 4, 5, 6, 7, 10

    // 10 = 1 (end)
    // 7 = 1
        // 7 to 10 = 1
    // 6 = 1
        // 6 to 7
    // 5 = two possibilities
        // 5 to 6
        // 5 to 7
    // 4 = 4
        // 4 to 5 = 2
        // 4 to 6 = 1
        // 4 to 7 = 1
    // 3 = 7
        // 3 to 4 = 4
        // 3 to 5 = 2
        // 3 to 6 = 1
    // 0 = 7
        // 0 to 3 = 7

// 0, 3, 4, 5, 6, 7, 8, 9, 12

    // 12 = 1 (end)
    // 9 = 1
        // 9 to 12 = 1
    // 8 = 1
        // 8 to 9 = 1
    // 7 = 2
        // 7 to 8
        // 7 to 9
    // 6 = 4
        // 7 = 2
        // 8 = 1
        // 9 = 1
    // 5 = 7
        // 6 = 4
        // 7 = 2
        // 8 - 1
    // 4 = 13
        // 5 = 7
        // 6 = 4
        // 7 = 2
    // 3 = 24
        // 4 = 13
        // 5 = 7
        // 6 = 4
    // 0 = 24
        // 3 = 24
