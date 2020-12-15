#include <cinttypes>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>

uint64_t next(uint64_t mask, uint64_t index)
{
    uint64_t newMask = 0;

    uint64_t checkMask = 1;
    uint64_t checkIndex = 1;
    for (int bit = 0; bit < 64; bit++) {
        if ((mask & checkMask) != 0) {
            if ((index & checkIndex) != 0) {
                newMask |= checkMask;
                index &= ~checkIndex;
            }
            checkIndex <<= 1;
        }
        checkMask <<= 1;
    }
    if (index == 0) {
        return newMask;
    }
    return 0;
}

int main(int argc, char** argv)
{
#if 0
    std::ifstream is(argv[1]);
    std::string str;

    uint64_t andMask = 0;
    uint64_t orMask = 0;

    std::map<uint64_t, uint64_t> memory;
    while (std::getline(is, str)) {
        if (str.substr(0, 4) == "mask") {
            andMask = 0;
            orMask = 0;

            size_t maskpos = str.find_first_of("X01");
            str = str.substr(maskpos);
            for (auto c : str) {
                andMask <<= 1;
                orMask <<= 1;
                if (c == '0') {
                    andMask &= ~1;
                } else {
                    andMask |= 1;
                }
                if (c == '1') {
                    orMask |= 1;
                }
            }

            printf("AND mask is %" PRIu64 " (%09" PRIx64 ")\n", andMask, andMask);
            printf("OR  mask is %" PRIu64 " (%09" PRIx64 ")\n", orMask, orMask);
        } else {
            uint64_t address;
            uint64_t value;
            sscanf(str.c_str(), "mem[%" SCNu64 "] = %" SCNu64 "\n", &address, &value);

            printf("mem[%" PRIu64 "] = %" PRIu64 "\n", address, value);

            memory[address] = ( value & andMask ) | orMask;
            printf("-> %" PRIu64 "\n", memory[address]);
        }
    }

    uint64_t sum = 0;
    for (auto v : memory) {
        sum += v.second;
    }

    printf("Sum is: %" PRIu64 "\n", sum);
#else
    std::ifstream is(argv[1]);
    std::string str;

    uint64_t orMask = 0;
    uint64_t fltMask = 0;

    std::map<uint64_t, uint64_t> memory;
    while (std::getline(is, str)) {
        if (str.substr(0, 4) == "mask") {
            orMask = 0;
            fltMask = 0;

            size_t maskpos = str.find_first_of("X01");
            str = str.substr(maskpos);
            for (auto c : str) {
                orMask <<= 1;
                fltMask <<= 1;
                if (c == '1') {
                    orMask |= 1;
                }
                if (c == 'X') {
                    fltMask |= 1;
                }
            }

            printf("OR  mask is %" PRIu64 " (%09" PRIx64 ")\n", orMask, orMask);
            printf("FLT mask is %" PRIu64 " (%09" PRIx64 ")\n", fltMask, fltMask);
        } else {
            uint64_t address;
            uint64_t value;
            sscanf(str.c_str(), "mem[%" SCNu64 "] = %" SCNu64 "\n", &address, &value);
            printf("mem[%" PRIu64 "] = %" PRIu64 "\n", address, value);

            address &= ~fltMask;
            address |= orMask;

            uint64_t floatvalue = 0;
            uint64_t index = 0;
            do {
                uint64_t fltAddress = address | floatvalue;
                memory[fltAddress] = value;
                printf("-> mem[%" PRIu64 "] = %" PRIu64 " (floatvalue = %09" PRIx64 ")\n", fltAddress, value, floatvalue);
            } while (floatvalue = next(fltMask, ++index));
        }
    }

    uint64_t sum = 0;
    for (auto v : memory) {
        sum += v.second;
    }

    printf("Sum is: %" PRIu64 "\n", sum);
#endif

    return 0;
}
