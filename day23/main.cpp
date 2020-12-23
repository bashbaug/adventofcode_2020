#include <algorithm>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Cups = std::vector<int>;

int finddestination(Cups& cups, int values[3], int current)
{
    int dcup = current - 1;
    do {
        if (dcup < 1) {
            dcup = (int)cups.size() - 1;
        }
        if (values[0] != dcup && values[1] != dcup && values[2] != dcup) {
            return dcup;
        } else {
            dcup--;
        }
    } while (true);
    return 0;
}

void printcups(const std::string& label, const Cups& cups, int current)
{
    printf("%s: ", label.c_str());
    int index = 0;
    do {
        index = cups[index];
        if (index == current) {
            printf("(%d) ", index);
        } else {
            printf("%d ", index);
        }
    } while (cups[index] != cups[0]);
    putchar('\n');
}

template<bool print>
int iterate(Cups& cups, int& current, int move)
{
    ++move;
    if (print) {
        printf("\n-- move %d --\n", move);
        printcups("cups", cups, current);
    }

    int values[3];
    values[0] = cups[current];
    values[1] = cups[values[0]];
    values[2] = cups[values[1]];
    if (print) {
        printf("pick up: %d, %d, %d\n", values[0], values[1], values[2]);
    }

    int dest = finddestination(cups, values, current);
    if (print) {
        printf("destination: %d\n", dest);
    }

    cups[current] = cups[values[2]];
    cups[values[2]] = cups[dest];
    cups[dest] = values[0];

    current = cups[current];

    return move;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    Cups orig;
    std::getline(is, str);

    orig.resize(str.length() + 1);

    int index = 0;
    for (auto c : str) {
        int next = c - '0';
        orig[index] = next;
        index = next;
    }
    orig[index] = orig[0];

    {
        const int moves = 100;

        Cups cups = orig;

        int current = cups[0];
        int move = 0;
        do {
            move = iterate<true>(cups, current, move);
        } while (move < moves);

        printf("\n-- final --\n");
        printcups("cups", cups, current);

        printf("\n\nPart 1 result: ");
        {
            int index = 1;
            for (int i = 0; i < cups.size() - 2; i++) {
                index = cups[index];
                printf("%d", index);
            }
            putchar('\n');
        }
    }

    {
        const int origsize = (int)orig.size();
        const int newsize = 1000000;
        const int moves = 10000000;

        Cups cups = orig;
        cups.resize(newsize + 1);

        int index = str.at(str.length() - 1) - '0';
        for (int i = origsize; i <= newsize; i++) {
            cups[index] = i;
            index = i;
        }
        cups[index] = cups[0];

        int current = cups[0];
        int move = 0;
        do {
            move = iterate<false>(cups, current, move);
        } while (move < moves);

        index = 1;
        int first = cups[index];
        int second = cups[first];
        int64_t product = (int64_t)first * (int64_t)second;
        printf("Part 2 result: Next two cups are: %d and %d, product is %" PRId64 "\n", first, second, product);
    }

    return 0;
}
