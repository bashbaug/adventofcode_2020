#include <fstream>
#include <iostream>
#include <string>

void debug(const std::string& str, const std::string& label, int xpos)
{
    bool match = str.at(xpos) == '#';

    std::string test = str;
    test.at(xpos) = match ? 'X' : 'O';
    printf("      %s : %s (%s = %d)\n", test.c_str(), label.c_str(), match ? "Match!  " : "No Match", xpos);
}

//#define DEBUG(_str, _var)   debug(_str, #_var, _var);
#define DEBUG(_str, _var)

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    int xpos_1_1 = 0; size_t trees_1_1 = 0;
    int xpos_3_1 = 0; size_t trees_3_1 = 0;
    int xpos_5_1 = 0; size_t trees_5_1 = 0;
    int xpos_7_1 = 0; size_t trees_7_1 = 0;

    int xpos_1_2 = 0; size_t trees_1_2 = 0;
    int ypos_1_2 = 0;

    while (std::getline(is, str)) {
        xpos_1_1 = xpos_1_1 % str.length();
        xpos_3_1 = xpos_3_1 % str.length();
        xpos_5_1 = xpos_5_1 % str.length();
        xpos_7_1 = xpos_7_1 % str.length();
        xpos_1_2 = xpos_1_2 % str.length();

        //printf("\nstr = %s\n", str.c_str());

        DEBUG(str, xpos_1_1);
        if (str.at(xpos_1_1) == '#') {
            ++trees_1_1;
        }
        DEBUG(str, xpos_3_1);
        if (str.at(xpos_3_1) == '#') {
            ++trees_3_1;
        }
        DEBUG(str, xpos_5_1);
        if (str.at(xpos_5_1) == '#') {
            ++trees_5_1;
        }
        DEBUG(str, xpos_7_1);
        if (str.at(xpos_7_1) == '#') {
            ++trees_7_1;
        }
        if (ypos_1_2 % 2 == 0) {
            DEBUG(str, xpos_1_2);
            if (str.at(xpos_1_2) == '#') {
                ++trees_1_2;
            }
            xpos_1_2 += 1;
        }

        xpos_1_1 += 1;
        xpos_3_1 += 3;
        xpos_5_1 += 5;
        xpos_7_1 += 7;
        ypos_1_2 += 1;
    }

    printf("Part 1: Found %zd trees.\n", trees_3_1);

    printf("Part 2: Trees are %zd, %zd, %zd, %zd, %zd\n", trees_1_1, trees_3_1, trees_5_1, trees_7_1, trees_1_2);
    printf("Part 2: Product of trees is %zd\n", trees_1_1 * trees_3_1 * trees_5_1 * trees_7_1 * trees_1_2);
}
