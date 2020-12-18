#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct State
{
    void initialize(int w, int h, int d, int z)
    {
        m_w = w;
        m_h = h;
        m_d = d;
        m_z = z;

        m_board.resize(m_w * m_h * m_d * m_z);
        std::fill(m_board.begin(), m_board.end(), 0);
    }

    int get(int w, int h, int d, int z) const
    {
        int state = 0;
        if (w >= 0 && h >= 0 && d >= 0 && z >= 0 &&
            w < m_w && h < m_h && d < m_d && z < m_z) {
            int index =
                z * m_w * m_h * m_d +
                d * m_w * m_h +
                h * m_w +
                w;
            state = m_board[index] ? 1 : 0;
        }
        return state;
    }

    void set(int w, int h, int d, int z, int state)
    {
        if (w >= 0 && h >= 0 && d >= 0 && z >= 0 &&
            w < m_w && h < m_h && d < m_d && z < m_z) {
            int index =
                z * m_w * m_h * m_d +
                d * m_w * m_h +
                h * m_w +
                w;
            m_board[index] = state ? 1 : 0;
        }
    }

    int count()
    {
        int num = 0;
        for (auto v : m_board) {
            num += v;
        }
        return num;
    }

    void print() const
    {
        int index = 0;
        for (int z = 0; z < m_z; z++) {
            for (int d = 0; d < m_d; d++) {
                printf("\nz=%d, w=%d:\n", d - m_d/2, z - m_z/2);
                for (int h = 0; h < m_h; h++) {
                    for (int w = 0; w < m_w; w++) {
                        if (m_board[index++] == 1) {
                            putchar('#');
                        } else {
                            putchar('.');
                        }
                    }
                    putchar('\n');
                }
            }
        }
    }

    State getnext(bool hyperslice) const
    {
        State next;
        next.initialize(m_w + 2, m_h + 2, m_d + 2, m_z + 2);

        for (int z = -1; z < m_z + 1; z++) {
            for (int d = -1; d < m_d + 1; d++) {
                for (int h = -1; h < m_h + 1; h++) {
                    for (int w = -1; w < m_w + 1; w++) {
                        int neighbors = 0;
                        for (int zz = -1; zz < 2; zz++) {
                            for (int dd = -1; dd < 2; dd++) {
                                for (int hh = -1; hh < 2; hh++) {
                                    for (int ww = -1; ww < 2; ww++) {
                                        neighbors += get(w + ww, h + hh, d + dd, z + zz);
                                    }
                                }
                            }
                        }
                        int state = get(w, h, d, z);
                        neighbors -= state;
                        if (neighbors == 2 && state) {
                            // remain active
                            next.set(w+1, h+1, d+1, z+1, 1);
                        } else if (neighbors == 3) {
                            // activate
                            next.set(w+1, h+1, d+1, z+1, 1);
                        } else {
                            // deactivate
                            next.set(w+1, h+1, d+1, z+1, 0);
                        }
                    }
                }
            }
        }

        return next;
    }

    int m_w;
    int m_h;
    int m_d;
    int m_z;

    // W X H X D x Z
    std::vector<int> m_board;
};

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    State current;
    std::getline(is, str);
    
    current.initialize((int)str.length(), (int)str.length(), 1, 1);
    int h = 0;
    do {
        int w = 0;
        for (auto c : str) {
            int state = 0;
            if (c == '#') {
                state = 1;
            }
            current.set(w++, h, 0, 0, state);
        }
        h++;
    } while(std::getline(is, str));

    for (int cycle = 0; cycle < 6; cycle++) {
        printf("Cycle = %d\n\n", cycle);
        current.print();

        State next = current.getnext(true);
        current = next;
    }

    printf("Final state:\n\n");
    current.print();

    printf("After six cycles, num active is: %d\n", current.count());

    return 0;
}

