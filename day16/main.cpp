#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using range = std::pair<int, int>;
using condition = std::pair<range, range>;
using ticket = std::vector<int>;

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

bool checkvalid(const std::string& str, const condition& c, int number)
{
    const range& r0 = c.first;
    if (number >= r0.first && number <= r0.second) {
        //printf("%d <= %d <= %d: matches %s\n", r0.first, number, r0.second, str.c_str());
        return true;
    }
    const range& r1 = c.second;
    if (number >= r1.first && number <= r1.second) {
        //printf("%d <= %d <= %d: matches %s\n", r1.first, number, r1.second, str.c_str());
        return true;
    }
    return false;
}

bool checkvalid(const std::map<std::string, condition>& conditions, int number)
{
    for (auto& c : conditions) {
        bool valid = checkvalid(c.first, c.second, number);
        if (valid) return true;
    }
    //printf("%d did not match any conditions.\n", number);
    return false;
}

bool readticket(const std::string& str, const std::map<std::string, condition>& conditions, ticket& ticket, int& count)
{
    bool valid = true;
    count = 0;

    size_t pos = 0;
    do {
        std::string token;
        pos = nextToken(str, ",\n", pos, token);

        int number = 0;
        std::sscanf(token.c_str(), "%d", &number);
        ticket.push_back(number);

        if (!checkvalid(conditions, number)) {
            count += number;
            valid = false;
        }

        if (pos == std::string::npos) break;
    } while(true);

    return valid;
}

int findindex(const std::string& str, const condition& c, const std::vector<ticket>& tickets, const std::map<int, bool>& used, int& index)
{
    printf("Finding indices for '%s'...\n", str.c_str());
    int test = 0;
    int count = 0;
    do {
        bool valid = false;
        if (used.find(test) != used.end()) continue;
        for(auto& t : tickets) {
            if (test >= t.size()) {
                return count;
            }
            int number = t[test];
            valid = checkvalid(str, c, number);
            if (valid == false) {
                break;
            }
        }
        if (valid) {
            printf("Found at %d!\n", test);
            index = test;
            ++count;
        } else {
            printf("Not %d...\n", test);
        }
    } while(++test);
    return -1;
}

void findindices(const std::map<std::string, condition>& conditions, const std::vector<ticket>& tickets, std::map<std::string, int>& indices)
{
    std::map<int, bool> used;
    for (const auto& c : conditions) {
        indices[c.first] = -1;
    }

    bool done = false;
    while (!done) {
        done = true;
        for (const auto& c : conditions) {
            if (indices[c.first] != -1) continue;

            int index;
            int count = findindex(c.first, c.second, tickets, used, index);
            if (count == 1) {
                printf("Assigning index %d to '%s'.\n", index, c.first.c_str());
                indices[c.first] = index;
                used[index] = true;
            } else {
                printf("Found %d matches for %s.  Not done yet.\n", count, c.first.c_str());
                done = false;
            }
        }
    }
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    std::map<std::string, condition> conditions;
    std::vector<ticket> tickets;

    std::map<std::string, int> positions;

    while (std::getline(is, str)) {
        if (str.empty()) break;
        size_t pos = 0;
        std::string name;
        pos = nextToken(str, ":", pos, name);

        int r0lo, r0hi, r1lo, r1hi;
        std::sscanf(str.substr(pos).c_str(), "%d-%d or %d-%d", &r0lo, &r0hi, &r1lo, &r1hi);

        printf("Read condition %s: %d-%d or %d-%d\n", name.c_str(), r0lo, r0hi, r1lo, r1hi);
        conditions[name] = std::make_pair(std::make_pair(r0lo, r0hi), std::make_pair(r1lo, r1hi));
    }

    std::getline(is, str);  // "your ticket:"
    std::getline(is, str);  // your tiket data
    ticket yours;
    int count;
    readticket(str, conditions, yours, count);

    std::getline(is, str);  // blank line
    std::getline(is, str);  // "nearby tickets:"

    int totalinvalid = 0;
    while (std::getline(is, str)) {
        std::vector<int> ticket;

        bool valid = readticket(str, conditions, ticket, count);
        if (valid) {
            tickets.push_back(ticket);
        }
        totalinvalid += count;
    }

    std::map<std::string, int> indices;
    findindices(conditions, tickets, indices);

    for (const auto& index : indices) {
        printf("%s is index %d\n", index.first.c_str(), index.second);
    }

    uint64_t product =
        (uint64_t)yours[indices["departure location"]] *
        (uint64_t)yours[indices["departure station"]] *
        (uint64_t)yours[indices["departure platform"]] *
        (uint64_t)yours[indices["departure track"]] *
        (uint64_t)yours[indices["departure date"]] *
        (uint64_t)yours[indices["departure time"]];

    printf("Total invalid: %d\n", totalinvalid);
    printf("Product: %" PRIu64 "\n", product);

    return 0;
}
