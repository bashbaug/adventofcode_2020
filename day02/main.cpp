#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    int valid = 0;
    int newvalid = 0;
    while (std::getline(is, str)) {
        int min = 0;
        int max = 0;
        char c = 0;
        char pw[256];

        int ret = std::sscanf(str.c_str(), "%d-%d %c: %s", &min, &max, &c, pw);

        printf("\nTesting %d-%d instances of %c for %s:\n", min, max, c, pw);

        int count = 0;
        char* test = pw;
        while (*test != '\0') {
            if (*test == c) {
                count++;
            }
            ++test;
        }

        printf("Found %d instances.\n", count);

        if (min <= count && max >= count) {
            printf("Valid!\n");
            ++valid;
        }
        else {
            printf("Invalid.\n");
        }

        size_t sz = strlen(pw);

        --min;
        --max;
        bool v0 = (min < sz && pw[min] == c);
        bool v1 = (max < sz && pw[max] == c);
        printf("First match = %s, second match = %s\n", v0 ? "yes" : "no", v1 ? "yes" : "no");
        if ((v0 || v1) && !(v0 && v1)) {
            printf("Valid!\n");
            ++newvalid;
        }
        else {
            printf("Invalid.\n");
        }
    }

    printf("Found %d valid passwords.\n", valid);
    printf("Found %d new valid passwords.\n", newvalid);

    return 0;
}
