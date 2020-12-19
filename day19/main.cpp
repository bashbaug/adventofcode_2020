#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using Option = std::list<std::string>;
using Options = std::list<Option>;
using RuleMap = std::map<std::string, Options>;

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

bool evaluate(const int indent, const std::string& rule, RuleMap& rulemap, const std::string& message, int& matched)
{
    if (message.empty()) {
        return false;
    }

    const Options& options = rulemap[rule];
    //printf("%*cEvalulating rule '%s' on message '%s'...\n", indent, ' ', rule.c_str(), message.c_str());
    for (const auto& option : options) {
        bool valid = true;
        int num = 0;
        for (const auto& next : option) {
            if (next.front() == '"') {
                char test = next.at(1);
                valid &= message.at(num) == test;
                num++;
            } else {
                valid &= evaluate(indent + 4, next, rulemap, message.substr(num), matched);
                num = num + matched;
            }
            if (!valid) {
                break;
            }
        }
        if (valid) {
            //printf("%*c  Rule '%s' on message '%s' matched!\n", indent, ' ', rule.c_str(), message.c_str());
            matched = num;
            return true;
        }
    }
    //printf("%*c  Rule '%s' on message '%s' failed to match.\n", indent, ' ', rule.c_str(), message.c_str());

    return false;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    RuleMap rulemap;

    while (std::getline(is, str)) {
        if (str.empty()) break;
        if (str.front() == ';') continue;

        size_t pos = 0;
        std::string name;
        pos = nextToken(str, ":", pos, name);

        Options& options = rulemap[name];

        do {
            std::string optstr;
            pos = nextToken(str, "|", pos, optstr);
            if (optstr.front() == ' ') {
                optstr = optstr.substr(1);
            }
            if (optstr.back() == ' ') {
                optstr.pop_back();
            }
            {
                Option option;
                size_t pos = 0;
                do {
                    std::string next;
                    pos = nextToken(optstr, " ", pos, next);
                    //option.push_back(next);
                    option.push_front(next);
                } while (pos != std::string::npos);
                options.push_back(option);
            }
        } while (pos != std::string::npos);
    }

    size_t longest = 0;
    std::vector<std::string> messages;
    while (std::getline(is, str)) {
        if (str.front() == ';') continue;

        std::reverse(str.begin(), str.end());
        messages.push_back(str);
        if (str.length() > longest) {
            longest = str.length();
        }
    }

    printf("The longest message is %d characters.\n", (int)longest);

    {
        int count = 0;
        for (const auto& message : messages) {
            int matched = 0;
            bool valid = evaluate(0, "0", rulemap, message, matched);
            if (valid && matched == message.length()) {
                //printf("VALID (%d): %s\n", matched, message.c_str());
                ++count;
            } else {
                //printf("INVALID (%d)! %s\n", matched, message.c_str());
            }
        }
        printf("Part 1: There were %d valid messages.\n", count);
    }

    // This is for part 2:
    printf("Fixing up Rule 8...\n");
    {
        Options& rule8 = rulemap["8"];
        Option fixup;
        fixup.push_back("42");
        while (fixup.size() < longest) {
            fixup.push_back("42");
            rule8.push_front(fixup);
        }
    }
    printf("Fixing up Rule 11...\n");
    {
        Options& rule11 = rulemap["11"];
        Option fixup;
        //fixup.push_front("42"); fixup.push_back("31");
        fixup.push_front("31"); fixup.push_back("42");
        while (fixup.size() < longest) {
            //fixup.push_front("42"); fixup.push_back("31");
            fixup.push_front("31"); fixup.push_back("42");
            rule11.push_front(fixup);
        }
    }

    {
        int count = 0;
        for (const auto& message : messages) {
            int matched = 0;
            bool valid = evaluate(0, "0", rulemap, message, matched);
            if (valid && matched == message.length()) {
                //printf("VALID (%d): %s\n", matched, message.c_str());
                ++count;
            } else {
                //printf("INVALID (%d)! %s\n", matched, message.c_str());
            }
        }
        printf("Part 2: There were %d valid messages.\n", count);
    }

    return 0;
}
