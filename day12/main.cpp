#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void move1(char command, int value, int& heading, int& x, int& y)
{
    if (command == 'F') {
        switch(heading) {
        case 0: command = 'N'; break;
        case 1: command = 'E'; break;
        case 2: command = 'S'; break;
        case 3: command = 'W'; break;
        default:
            printf("unexpected: heading = %d\n", heading);
            break;
        }
    }

    switch(command) {
    case 'N': y += value; break;
    case 'S': y -= value; break;
    case 'E': x += value; break;
    case 'W': x -= value; break;
    case 'L':
        heading -= (value / 90);
        heading &= (4 - 1);
        break;
    case 'R':
        heading += (value / 90);
        heading &= (4 - 1);
        break;
    default:
        printf("unexpected: command = '%c'\n", command);
        break;
    }
}

void move2(char command, int value, int& x, int& y, int& wpx, int& wpy)
{
    int temp = 0;

    switch(command) {
    case 'N': wpy += value; break;
    case 'S': wpy -= value; break;
    case 'E': wpx += value; break;
    case 'W': wpx -= value; break;
    case 'L':
        value = -value;
        // fall through
    case 'R':
        value /= 90;
        value &= (4 - 1);
        switch (value) {
        case 0: break;  // nothing to do
        case 1:
            temp = wpx;
            wpx = wpy;
            wpy = -temp;
            break;
        case 2:
            wpx = -wpx;
            wpy = -wpy;
            break;
        case 3:
            temp = wpx;
            wpx = -wpy;
            wpy = temp;
            break;
        default:
            printf("unexpected: rotate = %d\n", value);
        }
        break;
    case 'F':
        x += wpx * value;
        y += wpy * value;
        break;
    default:
        printf("unexpected: command = '%c'\n", command);
        break;
    }
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);

    int heading = 1;    // 0 = N, 1 = E, 2 = S, 3 = W
    int x = 0;  // E = +, W = -
    int y = 0;  // N = +, S = -
    int wpx = 10;
    int wpy = 1;

    std::string str;
    while (std::getline(is, str)) {
        char command;
        int value;
        std::sscanf(str.c_str(), "%c%d", &command, &value);

        //move1(command, value, heading, x, y);
        move2(command, value, x, y, wpx, wpy);

        printf("x = %d, y = %d\n", x, y);
    }

    x = (x < 0) ? -x : x;
    y = (y < 0) ? -y : y;

    printf("x = %d, y = %d, x + y = %d\n", x, y, x + y);

    return 0;
}
