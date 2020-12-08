#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

typedef std::tuple<std::string, int> CInstruction;
typedef std::vector<CInstruction> CProgram;

void run(const CProgram& program, int check, int& ip, int& acc)
{
    std::set<int> visited;

    ip = 0;
    acc = 0;

    while (true) {
        if (ip == program.size()) {
            break;
        } else if (visited.find(ip) != visited.end()) {
            break;
        } else {
            visited.insert(ip);
        }

        std::string command = std::get<0>(program[ip]);
        int num = std::get<1>(program[ip]);

        if (ip == check) {
            if (command == "nop") {
                command = "jmp";
            } else if (command == "jmp") {
                command = "nop";
            } else {
                printf("unexpected!  at ip %d, command is '%s'\n", ip, command.c_str());
            }
        }

        if (command == "nop") {
            ++ip;
        } else if (command == "acc") {
            acc += num;
            ++ip;
        } else if (command == "jmp") {
            ip += num;
        }
    }
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    CProgram program;

    while (std::getline(is, str)) {
        char instruction[256] = "";
        int count = 0;

        std::sscanf(str.c_str(), "%s %d\n", instruction, &count);
        program.push_back(std::make_tuple(std::string(instruction), count));
    }

    int ip = 0;
    int acc = 0;
    run(program, -1, ip, acc);

    printf("Infinite loop found at ip = %d: acc = %d\n", ip, acc);

    for (int check = 0; check < program.size(); check++) {
        if (std::get<0>(program[check]) == "acc") {
            continue;
        }

        ip = 0;
        acc = 0;
        run(program, check, ip, acc);
        if (ip == program.size()) {
            printf("Found the fix at ip = %d: acc = %d\n", ip, acc);
            break;
        }
    }

    return 0;
}
