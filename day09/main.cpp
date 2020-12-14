#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    int preamble = strtol(argv[2], NULL, 10);

    int missing = 0;

    std::vector<int> sequence;
    std::string str;

    while (std::getline(is, str)) {
        int num = 0;
        std::sscanf(str.c_str(), "%d", &num);
        sequence.push_back(num);
    }

    for (int ti = preamble; ti < sequence.size(); ti++) {
        int target = sequence[ti];
        bool found = false;
        for (int v0i = ti - preamble; v0i < ti; v0i++) {
            for (int v1i = v0i + 1; v1i < ti; v1i++) {
                int sum = sequence[v0i] + sequence[v1i];
                if (sum == target) {
                    found = true;
                }
            }
        }
        if (found == false) {
            printf("Missing number: %d\n", target);
            missing = target;
        }
    }

    for (int s = 0; s < sequence.size(); s++) {
        int sum = sequence[s];
        for (int e = s + 1; e < sequence.size(); e++) {
            sum += sequence[e];
            if (sum == missing) {
                int smallest = INT_MAX;
                int largest = INT_MIN;
                for (int i = s; i <= e; i++) {
                    smallest = sequence[i] < smallest ? sequence[i] : smallest;
                    largest = sequence[i] > largest ? sequence[i] : largest;
                }
                printf("Found a sequence!  smallest = %d, largest = %d, sum = %d\n", smallest, largest, smallest + largest);
                break;
            }
            if (sum > missing) {
                break;
            }
        }
    }

    return 0;
}
