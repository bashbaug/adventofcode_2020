#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;
    std::vector<int> values;
    while (std::getline(is, str)) {
        values.push_back(std::stoi(str));
    }

    const size_t sz = values.size();
    for (size_t v0i = 0; v0i < sz; v0i++) {
        auto v0 = values[v0i];
        for (size_t v1i = v0i; v1i < sz; v1i++) {
            auto v1 = values[v1i];
            if (v0 + v1 == 2020) {
                std::cout << "v0 = " << v0 << " v1 = " << v1 << " v0 x v1 = " << v0 * v1 << "\n";
            }
        }
    }

    for (size_t v0i = 0; v0i < sz; v0i++) {
        auto v0 = values[v0i];
        for (size_t v1i = v0i; v1i < sz; v1i++) {
            auto v1 = values[v1i];
            for (size_t v2i = v1i; v2i < sz; v2i++) {
                auto v2 = values[v2i];
                if (v0 + v1 + v2 == 2020) {
                    std::cout << "v0 = " << v0 << " v1 = " << v1 << " v2 = " << v2 << " v0 x v1 x v2 = " << v0 * v1 * v2 << "\n";
                }
            }
        }
    }

    return 0;
}
