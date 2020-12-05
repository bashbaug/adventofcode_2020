#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    std::vector<int> ids;

    while (std::getline(is, str)) {
        int id = 0;
        for (auto c : str) {
            id = id * 2;
            if (c == 'B' || c == 'R') {
                id = id + 1;
            }
        }
        printf("Seat ID for %s is %d.\n", str.c_str(), id);
        ids.push_back(id);
    }

    std::sort(ids.begin(), ids.end());

    printf("Highest seat id is %d.\n", ids.back());

    for (int index = 0; index < ids.size(); index++) {
        if (ids[index] + 1 != ids[index + 1]) {
            printf("Your seat ID is %d.\n", ids[index] + 1);
            break;
        }
    }

    return 0;
}
