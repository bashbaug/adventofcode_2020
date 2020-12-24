#include <algorithm>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

using Cups = std::list<int>;

Cups::iterator nextcup(Cups& cups, Cups::iterator it)
{
    it++;
    if (it == cups.end()) {
        return cups.begin();
    }
    return it;
}

Cups::iterator finddestination(Cups& cups, int values[3], Cups::iterator current)
{
    Cups::iterator ret;
    int dcup = *current - 1;
    do {
        if (dcup < 1) {
            dcup = (int)(cups.size() + 3);
        }
        if (values[0] != dcup && values[1] != dcup && values[2] != dcup) {
            return std::find(cups.begin(), cups.end(), dcup);
        } else {
            dcup--;
        }
    } while (true);
    return cups.end();
}

void printcups(const std::string& label, const Cups& cups, const Cups::iterator current)
{
    printf("%s: ", label.c_str());
    for (auto cup : cups) {
        if (current != cups.end() && cup == *current) {
            printf("(%d) ", cup);
        } else {
            printf("%d ", cup);
        }
    }
    putchar('\n');
}

template<bool print>
int iterate(Cups& cups, Cups::iterator& current, int move)
{
    ++move;
    printf("\n-- move %d --\n", move);
    if (print) {
        printcups("cups", cups, current);
    }

    int values[3];
    Cups::iterator next = nextcup(cups, current);
    for (int i = 0; i < 3; i++) {
        Cups::iterator temp = nextcup(cups, next);
        values[i] = *next;
        cups.erase(next);
        next = temp;
    }
    if (print) {
        printf("pick up: %d, %d, %d\n", values[0], values[1], values[2]);
    }

    Cups::iterator dest = finddestination(cups, values, current);
    if (print) {
        printf("destination: %d\n", *dest);
    }

    ++dest;
    cups.insert(dest, values[0]);
    cups.insert(dest, values[1]);
    cups.insert(dest, values[2]);
    current = nextcup(cups, current);

    return move;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    Cups orig;
    std::getline(is, str);

    for (auto c : str) {
        orig.push_back(c - '0');
    }

    {
        Cups cups = orig;

        Cups::iterator current = cups.begin();
        int move = 0;
        do {
            move = iterate<true>(cups, current, move);
        } while (move < 100);

        printf("\n-- final --\n");
        printcups("cups", cups, current);

        {
            Cups::iterator p = std::find(cups.begin(), cups.end(), 1);
            while(*(p = nextcup(cups, p)) != 1) {
                printf("%d", *p);
            }
            printf("\n");
        }
    }

    {
        Cups cups = orig;
        int next = (int)cups.size();
        while (cups.size() < 1000000) {
            cups.push_back(++next);
        }

        Cups::iterator current = cups.begin();
        int move = 0;
        do {
            move = iterate<false>(cups, current, move);
        } while (move < 10000000);

        Cups::iterator p = std::find(cups.begin(), cups.end(), 1);
        int first = *(++p);
        int second = *(++p);
        int64_t product = first * second;
        printf("Next two cups are: %d and %d, product is %" PRId64 "\n", first, second, product);
    }

    return 0;
}
