#include <bitset>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    int sum_any = 0;
    int sum_all = 0;
    while (std::getline(is, str)) {
        uint32_t any = 0;
        uint32_t all = UINT_MAX;
        do {
            std::bitset<26> local;
            for (auto c : str) {
                local.set(c - 'a');
            }
            any |= (uint32_t)local.to_ulong();
            all &= (uint32_t)local.to_ulong();
            std::getline(is, str);
        }
        while (is.good() && !str.empty());

        std::bitset<26> anyset(any);
        sum_any += (int)anyset.count();

        std::bitset<26> allset(all);
        sum_all += (int)allset.count();
    }

    printf("sum of any counts is: %d\n", sum_any);
    printf("sum of all counts is: %d\n", sum_all);
}
