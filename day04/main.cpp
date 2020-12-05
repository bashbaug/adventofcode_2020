#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

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

bool check_ecl(const std::string& value) {
    // ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
    if (value != "amb" &&
        value != "blu" &&
        value != "brn" &&
        value != "gry" &&
        value != "grn" &&
        value != "hzl" &&
        value != "oth") {
        printf("ecl invalid, no match: %s\n", value.c_str());
        return false;
    }
    printf("ecl valid: %s\n", value.c_str());
    return true;
}
bool check_pid(const std::string& value) {
    // pid (Passport ID) - a nine-digit number, including leading zeroes.
    if (value.length() != 9) {
        return false;
    }
    if (value.find_first_not_of("01234567890") != std::string::npos) {
        return false;
    }
    printf("pid valid: %s\n", value.c_str());
    return true;
}
bool check_eyr(const std::string& value) {
    // eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
    if (value.length() != 4) {
        printf("eyr invalid, too short: %s\n", value.c_str());
        return false;
    }
    if (value.find_first_not_of("01234567890") != std::string::npos) {
        printf("eyr invalid, bad chars: %s\n", value.c_str());
        return false;
    }
    int year = 0;
    std::sscanf(value.c_str(), "%d", &year);
    if (year >= 2020 && year <= 2030) {
        printf("eyr valid: %s\n", value.c_str());
        return true;
    }
    printf("eyr invalid, out of range: %s\n", value.c_str());
    return false;
}
bool check_hcl(const std::string& value) {
    // hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
    if (value.length() != 7) {
        printf("hcl invalid, too short: %s\n", value.c_str());
        return false;
    }
    if (value.at(0) != '#') {
        printf("hcl invalid, wrong first char: %s\n", value.c_str());
        return false;
    }
    if (value.substr(1).find_first_not_of("01234567890abcdef") != std::string::npos) {
        printf("hcl invalid, bad chars: %s\n", value.c_str());
    }
    printf("hcl valid: %s\n", value.c_str());
    return true;
}
bool check_byr(const std::string& value) {
    // byr (Birth Year) - four digits; at least 1920 and at most 2002.
    if (value.length() != 4) {
        printf("byr invalid, too short: %s\n", value.c_str());
        return false;
    }
    if (value.find_first_not_of("01234567890") != std::string::npos) {
        printf("byr invalid, bad chars: %s\n", value.c_str());
        return false;
    }
    int year = 0;
    std::sscanf(value.c_str(), "%d", &year);
    if (year >= 1920 && year <= 2002) {
        printf("byr valid: %s\n", value.c_str());
        return true;
    }
    printf("byr invalid, out of range %d: %s\n", year, value.c_str());
    return false;
}
bool check_iyr(const std::string& value) {
    // iyr (Issue Year) - four digits; at least 2010 and at most 2020.
    if (value.length() != 4) {
        printf("iyr invalid, too short: %s\n", value.c_str());
        return false;
    }
    if (value.find_first_not_of("01234567890") != std::string::npos) {
        printf("iyr invalid, bad chars: %s\n", value.c_str());
        return false;
    }
    int year = 0;
    std::sscanf(value.c_str(), "%d", &year);
    if (year >= 2010 && year <= 2020) {
        printf("iyr valid: %s\n", value.c_str());
        return true;
    }
    printf("iyr invalid, out of range %d: %s\n", year, value.c_str());
    return false;
}
bool check_hgt(const std::string& value) {
    // hgt (Height) - a number followed by either cm or in:
    //    If cm, the number must be at least 150 and at most 193.
    //    If in, the number must be at least 59 and at most 76.

    size_t pos = value.find("cm");
    if (pos != std::string::npos && value.substr(pos) == "cm") {
        int n = 0;
        std::sscanf(value.substr(0, pos).c_str(), "%d", &n);
        if (n >= 150 && n <= 193) {
            printf("hgt valid: %s -> %d cm\n", value.c_str(), n);
            return true;
        } else {
            printf("hgt invalid: out of range: %s\n", value.c_str());
        }
    }

    pos = value.find("in");
    if (pos != std::string::npos && value.substr(pos) == "in") {
        int n = 0;
        std::sscanf(value.substr(0, pos).c_str(), "%d", &n);
        if (n >= 59 && n <= 76) {
            printf("hgt valid: %s -> %d in\n", value.c_str(), n);
            return true;
        } else {
            printf("hgt invalid: out of range: %s\n", value.c_str());
        }
    }

    printf("hgt invalid: %s\n", value.c_str());
    return false;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    int valid = 0;
    while (std::getline(is, str)) {
        bool has_ecl = false; bool valid_ecl = false;
        bool has_pid = false; bool valid_pid = false;
        bool has_eyr = false; bool valid_eyr = false;
        bool has_hcl = false; bool valid_hcl = false;
        bool has_byr = false; bool valid_byr = false;
        bool has_iyr = false; bool valid_iyr = false;
        bool has_hgt = false; bool valid_hgt = false;

        do {
            std::string key;
            std::string value;
            size_t pos = 0;
            do {
                pos = nextToken(str, ": \n", pos, key);
                pos = nextToken(str, ": \n", pos, value);

                printf("\nkey = %s, value = %s\n", key.c_str(), value.c_str());

                if (key == "ecl") { has_ecl = true; valid_ecl = check_ecl(value); }
                if (key == "pid") { has_pid = true; valid_pid = check_pid(value); }
                if (key == "eyr") { has_eyr = true; valid_eyr = check_eyr(value); }
                if (key == "hcl") { has_hcl = true; valid_hcl = check_hcl(value); }
                if (key == "byr") { has_byr = true; valid_byr = check_byr(value); }
                if (key == "iyr") { has_iyr = true; valid_iyr = check_iyr(value); }
                if (key == "hgt") { has_hgt = true; valid_hgt = check_hgt(value); }
            }
            while (pos != std::string::npos);

            std::getline(is, str);
        }
        while (is.good() && !str.empty());

        bool check =
            has_ecl && valid_ecl &&
            has_pid && valid_pid &&
            has_eyr && valid_eyr &&
            has_hcl && valid_hcl &&
            has_byr && valid_byr &&
            has_iyr && valid_iyr &&
            has_hgt && valid_hgt;

        if (check) {
            ++valid;
        }
    }

    printf("Found %d valid passports.\n", valid);
}
