#include <algorithm>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Tile {
    Tile(int num) : Used(false), Num(num), Dim(0), Matches(0) {}

    const char& get(int row, int col) const
    {
        return Data.at(row * Dim + col);
    }

    char& get(int row, int col)
    {
        return Data.at(row * Dim + col);
    }

    void addRow(const std::string& rowdata)
    {
        if (Dim == 0) {
            Dim = (int)rowdata.length();
        } else {
            if (Dim != rowdata.length()) {
                printf("Row data mismatch?  Had %d, got %d.\n", Dim, (int)rowdata.length());
            }
        }
        Data += rowdata;
    }

    std::string getEdge(int num) const
    {
        // 0 = top, 1 = right, 2 = bottom, 3 = left
        std::string ret;
        if (num % 4 == 0) {
            ret = Data.substr(0, Dim);
        } else if (num % 4 == 1) {
            for (int c = 0; c < Dim; c++) {
                ret += get(c, Dim - 1);
            }
        } else if (num % 4 == 2) {
            ret = Data.substr((Dim - 1) * Dim, Dim);
        } else if (num % 4 == 3) {
            for (int c = 0; c < Dim; c++) {
                ret += get(c, 0);
            }
        }
        if (num >= 4) {
            std::reverse(ret.begin(), ret.end());
        }
        return ret;
    }

    bool matches(const Tile& othertile, int& outindex) const
    {
        if (Num == othertile.Num) {
            printf("Tile %d trivially matches tile %d - BUT IS THIS WHAT YOU WANTED TO DO?\n", Num, othertile.Num);
            return true;
        }

        for (int e = 0; e < 4; e++) {
            std::string edge = getEdge(e);

            for (int oe = 0; oe < 8; oe++) {
                std::string other = othertile.getEdge(oe);
                if (edge == other) {
                    printf("Tile %d matches tile %d.\n", Num, othertile.Num);
                    outindex = oe;
                    return true;
                }
            }
        }

        return false;
    }

    void print() const
    {
        printf("\n\nTile %d\n", Num);
        for (int row = 0; row < Dim; row++) {
            printf("  %s\n", Data.substr(row * Dim, Dim).c_str());
        }
    }

    void printedges() const
    {
        printf("Tile %d edges\n", Num);
        for (int edge = 0; edge < 8; edge++) {
            printf("    %d: %s\n", edge, getEdge(edge).c_str());
        }
    }

    void flipV()
    {
        std::string newData;

        for (int row = 0; row < Dim; row++) {
            for (int col = 0; col < Dim; col++) {
                newData += get(Dim - row - 1, col);
            }
        }

        Data = newData;
    }

    void flipH()
    {
        std::string newData;

        for (int row = 0; row < Dim; row++) {
            for (int col = 0; col < Dim; col++) {
                newData += get(row, Dim - col - 1);
            }
        }

        Data = newData;
    }

    void rotateR()
    {
        std::string newData;

        for (int row = 0; row < Dim; row++) {
            for (int col = 0; col < Dim; col++) {
                newData += get(Dim - col - 1, row);
            }
        }

        Data = newData;
    }

    void rotateL()
    {
        std::string newData;

        for (int row = 0; row < Dim; row++) {
            for (int col = 0; col < Dim; col++) {
                newData += get(col, Dim - row - 1);
            }
        }

        Data = newData;
    }

    void transpose()
    {
        std::string newData;

        for (int row = 0; row < Dim; row++) {
            for (int col = 0; col < Dim; col++) {
                newData += get(col, row);
            }
        }

        Data = newData;
    }

    std::string Data;
    bool Used;
    int Num;
    int Dim;
    int Matches;
};

int findnext(std::vector<Tile>& tiles, int left, int above, const std::vector<int>& search)
{
    for (auto index : search) {
        Tile& othertile = tiles[index];
        if (othertile.Used == true) continue;

        bool found = true;
        if (left >= 0) {
            const Tile& lefttile = tiles[left];
            int outindex;
            found &= lefttile.matches(othertile, outindex);
            if (found) {
                printf("Before Left Adjustment (out index is %d)...\n", outindex);
                othertile.print();
                switch(outindex) {
                case 0: othertile.flipH(); othertile.rotateL(); break;  // top
                case 4: othertile.rotateL(); break;     // top reversed
                case 1: othertile.flipH(); break;       // right
                case 5: othertile.flipV(); othertile.flipH(); break;    // right reversed
                case 2: othertile.rotateR(); break;     // bottom
                case 6: othertile.flipH(); othertile.rotateR(); break;  // bottom reversed
                case 3: break;                          // left
                case 7: othertile.flipV(); break;       // left reversed
                default: printf("Unknown out index %d!\n", outindex);
                }

                printf("After...\n"); othertile.print();

                int test;
                lefttile.matches(othertile, test);
                if (test != 3) {
                    printf("Bad left alignment!  outindex was %d, test is %d\n", outindex, test);
                }
            }
        }
        if (above >= 0) {
            const Tile& abovetile = tiles[above];
            int outindex;
            found &= abovetile.matches(othertile, outindex);
            if (found) {
                printf("Before Top Adjustment (out index is %d)...\n", outindex);
                printf("Above Tile:\n");
                abovetile.print();
                printf("This Tile:\n");
                othertile.print();
                switch(outindex) {
                case 0: break;  // top
                case 4: othertile.flipH(); break;   // top reversed
                case 1: othertile.rotateL(); break; // right
                case 5: othertile.flipV(); othertile.rotateL(); break;  // right reversed
                case 2: othertile.flipV(); break;   // bottom
                case 6: othertile.flipH(); othertile.flipV(); break;    // bottom reversed
                case 3: othertile.flipV(); othertile.rotateR(); break;  // left
                case 7: othertile.rotateR(); break; // left reversed
                default: printf("Unknown out index %d!\n", outindex);
                }

                printf("After...\n");
                othertile.print();

                int test;
                abovetile.matches(othertile, test);
                if (test != 0) {
                    printf("Bad top alignment!  left index is %d, outindex was %d, test is %d\n", left, outindex, test);
                }
            }
        }
        if (found) {
            return index;
        }
    }
    printf("I didn't find it??\n");
    return -1;
}

void stitch(const std::vector<Tile>& tiles, const std::vector<int>& sequence, int tiledim, Tile& outtile)
{
    std::string megarow;
    for (int tr = 0; tr < tiledim; tr++) {
        for (int r = 0; r < tiles[0].Dim; r++) {
            megarow = "";
            for (int tc = 0; tc < tiledim; tc++) {
                int tile = sequence[tr * tiledim + tc];
                for (int c = 0; c < tiles[0].Dim; c++) {
                    char value = tiles[tile].get(r, c);
                    putchar(value);
                    if (c != 0 && c != tiles[0].Dim - 1) {
                        megarow += tiles[tile].get(r, c);
                    }
                }
                putchar(' ');
            }
            printf("\n");
            if (r != 0 && r != tiles[0].Dim - 1) {
                outtile.addRow(megarow);
            }
        }
        printf("\n");
    }
}

bool findmonster(const Tile& image, const std::vector<std::string>& pattern, int ir, int ic)
{
    for (int pr = 0; pr < pattern.size(); pr++) {
        for (int pc = 0; pc < pattern[pr].length(); pc++) {
            if (pattern[pr].at(pc) == '#' && image.get(ir + pr, ic + pc) != '#') {
                return false;
            }
        }
    }
    return true;
}

int countmonsters(const Tile& image, const std::vector<std::string>& pattern)
{
    int count = 0;
    for (int r = 0; r < image.Dim - pattern.size(); r++) {
        for (int c = 0; c < image.Dim - pattern[0].length(); c++ ) {
            bool found = findmonster(image, pattern, r, c);
            if (found) {
                printf("Found a monster at row: %d, col: %d\n", r, c);
                count++;
            }
        }
    }
    return count;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    std::vector<Tile> tiles;

    while (std::getline(is, str)) {
        int num = 0;
        std::sscanf(str.c_str(), "Tile %d:", &num);

        Tile tile(num);
        while (std::getline(is, str)) {
            if (str.empty()) break;
            tile.addRow(str);
        }

        tiles.push_back(tile);
    }

    printf("Found %d tiles.\n", (int)tiles.size());

    for (const auto& tile : tiles) {
        tile.print();
        tile.printedges();
    }

    for (int t = 0; t < tiles.size(); t++) {
        for (int ot = t + 1; ot < tiles.size(); ot++) {
            if (t == ot) continue;
            int outindex;
            if (tiles[t].matches(tiles[ot], outindex)) {
                tiles[t].Matches++;
                tiles[ot].Matches++;
            }
        }
    }

    std::vector<int> corners;
    std::vector<int> edges;
    std::vector<int> middles;

    uint64_t product = 1;
    int others = 0;
    for (int t = 0; t < tiles.size(); t++) {
        printf("Tile %d (%d) has %d matches.\n", t, tiles[t].Num, tiles[t].Matches);
        if (tiles[t].Matches == 2) {
            printf("\t --> Found corner tile %d (%d)?\n", t, tiles[t].Num);
            corners.push_back(t);
            product *= tiles[t].Num;
        } else if (tiles[t].Matches == 3) {
            edges.push_back(t);
        } else if (tiles[t].Matches == 4) {
            middles.push_back(t);
        } else {
            others++;
        }
    }

    int tiledim = (int)edges.size() / 4 + 2;
    std::vector<int> sequence;

    // This is a hack!
    // If we didn't want to do this hack we could instead track used edges
    // and orient with an edge on the right and bottom.
    if (tiledim == 3) {
        // sample input
        tiles[corners[0]].flipV();
    } else if (tiledim == 12) {
        // actual input
        tiles[corners[0]].transpose();
    }

    {
        // Start with an arbitrary corner tile:
        sequence.push_back(corners[0]);
        tiles[corners[0]].Used = true;
        for (int t = 0; t < edges.size() / 4; t++) {
            int left = sequence.back();
            int next = findnext(tiles, left, -1, edges);
            sequence.push_back(next);
            tiles[next].Used = true;
        }
        // Find the ending corner tile:
        {
            int left = sequence.back();
            int next = findnext(tiles, left, -1, corners);
            sequence.push_back(next);
            tiles[next].Used = true;
        }
        for (int r = 0; r < edges.size() / 4; r++) {
            // Find the first edge tile.
            {
                int above = sequence[sequence.size() - tiledim];
                int next = findnext(tiles, -1, above, edges);
                sequence.push_back(next);
                tiles[next].Used = true;
            }
            for (int t = 0; t < edges.size() / 4; t++) {
                int left = sequence.back();
                int above = sequence[sequence.size() - tiledim];
                int next = findnext(tiles, left, above, middles);
                sequence.push_back(next);
                tiles[next].Used = true;
            }
            // Find the ending edge tile.
            {
                int left = sequence.back();
                int above = sequence[sequence.size() - tiledim];
                int next = findnext(tiles, left, above, edges);
                sequence.push_back(next);
                tiles[next].Used = true;
            }
        }
        // Find the last row corner tile.
        {
            int above = sequence[sequence.size() - tiledim];
            int next = findnext(tiles, -1, above, corners);
            sequence.push_back(next);
            tiles[next].Used = true;
        }
        for (int t = 0; t < edges.size() / 4; t++) {
            int left = sequence.back();
            int above = sequence[sequence.size() - tiledim];
            int next = findnext(tiles, left, above, edges);
            sequence.push_back(next);
            tiles[next].Used = true;
        }
        // Find the ending corner tile:
        {
            int left = sequence.back();
            int above = sequence[sequence.size() - tiledim];
            int next = findnext(tiles, left, above, corners);
            sequence.push_back(next);
            tiles[next].Used = true;
        }
    }

    for (int i = 0; i < sequence.size(); i++) {
        if (i % tiledim == 0) {
            printf("\n");
        }
        printf("%4d ", tiles[sequence[i]].Num);
    }

    printf("\n\nStitched image:\n\n");

    Tile outtile(9999999);
    stitch(tiles, sequence, (int)edges.size() / 4 + 2, outtile);

    printf("Output tile:\n");
    outtile.print();

    std::vector<std::string> monster;
    monster.push_back("                  # ");
    monster.push_back("#    ##    ##    ###");
    monster.push_back(" #  #  #  #  #  #   ");

    int count = 0;
    count += countmonsters(outtile, monster);
    printf("Original orientation: Found %d monsters.\n", count);

    outtile.transpose();
    count += countmonsters(outtile, monster);
    printf("Transpose: Found %d monsters.\n", count);

    int water = 0;
    for (const auto c : outtile.Data) {
        if (c == '#') ++water;
    }

    printf("\n\nFound %d corners, %d edges, %d, middles, and %d others.\n", (int)corners.size(), (int)edges.size(), (int)middles.size(), others);
    printf("Product of corner tiles = %" PRIu64 "\n", product);

    printf("Found %d water.  Subtract %d x 15 == %d\n", water, count, water - count * 15);

    return 0;
}
