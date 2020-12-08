#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

typedef std::tuple<int, std::string> CBag;
typedef std::set<CBag> CContents;
typedef std::map<std::string, CContents> CBags;

bool find(CBags& bags, CContents& contains, const std::string& key) {
    for (auto& bag : contains) {
        const std::string& type = std::get<1>(bag);
        if (type == key) {
            return true;
        }
        if (find(bags, bags[type], key)) {
            return true;
        }
    }
    return false;
}

int findContents(CBags& bags, CContents& contains) {
    int total = 1;
    for (auto& bag : contains) {
        const int count = std::get<0>(bag);
        const std::string& type = std::get<1>(bag);
        total += count * findContents(bags, bags[type]);
    }
    return total;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    CBags bags;

    int sum_any = 0;
    int sum_all = 0;
    while (std::getline(is, str)) {
        size_t bfirst = str.find(" bag");
        std::string bagtype = str.substr(0, bfirst);
        printf("Bag Color: %s\n", bagtype.c_str());

        CContents& bag = bags[bagtype];

        size_t nfirst = str.find_first_of("012345789", bfirst);
        bfirst = str.find_first_not_of("0123456789 ", nfirst);
        while (nfirst != std::string::npos) {
            std::string num = str.substr(nfirst, bfirst-nfirst);
            int count = 0;
            std::sscanf(num.c_str(), "%d", &count);

            size_t next = str.find(" bag", bfirst);
            std::string contains = str.substr(bfirst, next-bfirst);

            printf("    contains: %d x %s\n", count, contains.c_str());
            bag.insert(std::make_tuple(count, contains));

            nfirst = str.find_first_of("012345789", bfirst);
            bfirst = str.find_first_not_of("0123456789 ", nfirst);
        }
    }

    const std::string key("shiny gold");
    int total = 0;
    for (auto bag : bags) {
        bool found = find(bags, bag.second, key);
        if (found) {
            printf("bag %s can hold %s\n", bag.first.c_str(), key.c_str());
            total++;
        }
    }

    printf("Found %d possible bags that can hold %s.\n", total, key.c_str());

    total = findContents(bags, bags[key]) - 1;
    printf("One %s bag can hold %d other bags.\n", key.c_str(), total);

    return 0;
}
