#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool occupied(int row, int col, const std::vector<std::string>& seats)
{
    if (row < 0) {
        return false;
    }
    if (row >=  seats.size()) {
        return false;
    }
    if (col < 0) {
        return false;
    }
    if (col >= seats[row].length()) {
        return false;
    }
    return seats[row].at(col) == '#';
}

bool step(std::vector<std::string>& seats)
{
    std::vector<std::string> next = seats;
    for (int row = 0; row < seats.size(); row++) {
        std::string& str = seats[row];
        for (int col = 0; col < str.length(); col++) {
            if (seats[row].at(col) != '.') {
                int count =
                    (occupied(row - 1, col - 1, seats) ? 1 : 0) +
                    (occupied(row - 1, col,     seats) ? 1 : 0) +
                    (occupied(row - 1, col + 1, seats) ? 1 : 0) +
                    (occupied(row,     col - 1, seats) ? 1 : 0) +
                    (occupied(row,     col + 1, seats) ? 1 : 0) +
                    (occupied(row + 1, col - 1, seats) ? 1 : 0) +
                    (occupied(row + 1, col,     seats) ? 1 : 0) +
                    (occupied(row + 1, col + 1, seats) ? 1 : 0);

                //printf("at row = %d, col = %d: count = %d\n", row, col, count);
                if (count == 0) {
                    next[row].at(col) = '#';
                } else if (count >= 4) {
                    next[row].at(col) = 'L';
                }
            }
        }
    }

    if (next != seats) {
        seats = next;
        return true;
    }

    return false;
}

bool checkdir(int row, int col, int dr, int dc, const std::vector<std::string>& seats)
{
    row += dr;
    col += dc;
    do {
        if (row < 0) {
            return false;
        }
        if (row >=  seats.size()) {
            return false;
        }
        if (col < 0) {
            return false;
        }
        if (col >= seats[row].length()) {
            return false;
        }
        if (seats[row].at(col) == 'L') {
            return false;
        }
        if (seats[row].at(col) == '#') {
            return true;
        }
        row += dr;
        col += dc;
    } while(true);
    return false;
}

bool step2(std::vector<std::string>& seats)
{
    std::vector<std::string> next = seats;
    for (int row = 0; row < seats.size(); row++) {
        std::string& str = seats[row];
        for (int col = 0; col < str.length(); col++) {
            if (seats[row].at(col) != '.') {
                int count =
                    (checkdir(row, col, -1, -1, seats) ? 1 : 0) +
                    (checkdir(row, col, -1,  0, seats) ? 1 : 0) +
                    (checkdir(row, col, -1,  1, seats) ? 1 : 0) +
                    (checkdir(row, col,  0, -1, seats) ? 1 : 0) +
                    (checkdir(row, col,  0,  1, seats) ? 1 : 0) +
                    (checkdir(row, col,  1, -1, seats) ? 1 : 0) +
                    (checkdir(row, col,  1,  0, seats) ? 1 : 0) +
                    (checkdir(row, col,  1,  1, seats) ? 1 : 0);

                //printf("at row = %d, col = %d: count = %d\n", row, col, count);
                if (count == 0) {
                    next[row].at(col) = '#';
                } else if (count >= 5) {
                    next[row].at(col) = 'L';
                }
            }
        }
    }

    if (next != seats) {
        seats = next;
        return true;
    }

    return false;
}

int count(const std::vector<std::string>& seats)
{
    int num = 0;
    for (const auto& str : seats) {
        for (const auto c : str) {
            if (c == '#') {
                num++;
            }
        }
    }
    return num;
}

void print(const std::vector<std::string>& seats)
{
    for (const auto& str : seats) {
        printf("%s\n", str.c_str());
    }
    printf("\n\n");
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);

    std::vector<std::string> seats;
    std::string str;

    while (std::getline(is, str)) {
        seats.push_back(str);
    }

    do {
        //print(seats);
    } while (step2(seats));

    int num = count(seats);
    printf("steady state = %d seats\n", num);

    return 0;
}
