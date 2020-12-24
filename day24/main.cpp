#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

using Tiles = std::map<std::pair<int, int>, int>;

int counttiles(const Tiles& tiles)
{
    int count = 0;
    for (const auto& tile : tiles) {
        //printf("Tile (%d, %d) has value %d.\n", tile.first.first, tile.first.second, tile.second);
        if (tile.second) {
            ++count;
        }
    }
    return count;
}

int gettile(const Tiles& tiles, int xcoord, int ycoord)
{
    Tiles::const_iterator it = tiles.find(std::make_pair(xcoord, ycoord));
    if (it == tiles.end()) {
        return 0;
    }
    return it->second;
}

int countneighbors(const Tiles& tiles, int xcoord, int ycoord)
{
    int count = 0;
    count += gettile(tiles, xcoord - 2, ycoord    ) % 2; // W
    count += gettile(tiles, xcoord - 1, ycoord + 2) % 2; // NW
    count += gettile(tiles, xcoord + 1, ycoord + 2) % 2; // NE
    count += gettile(tiles, xcoord + 2, ycoord    ) % 2; // E
    count += gettile(tiles, xcoord + 1, ycoord - 2) % 2; // SE
    count += gettile(tiles, xcoord - 1, ycoord - 2) % 2; // SW
    return count;
}

void growneighbor(const Tiles& tiles, Tiles& next, int xcoord, int ycoord)
{
    Tiles::const_iterator it = tiles.find(std::make_pair(xcoord, ycoord));
    if (it == tiles.end()) {
        int count = countneighbors(tiles, xcoord, ycoord);
        if (count == 2) {
            //printf("Growing at tile (%d, %d), setting to 1.\n", xcoord, ycoord);
            next[std::make_pair(xcoord, ycoord)] = 1;
        }
    }
}

void growneighbors(const Tiles& tiles, Tiles& next, int xcoord, int ycoord)
{
    growneighbor(tiles, next, xcoord - 2, ycoord    );
    growneighbor(tiles, next, xcoord - 1, ycoord + 2);
    growneighbor(tiles, next, xcoord + 1, ycoord + 2);
    growneighbor(tiles, next, xcoord + 2, ycoord    );
    growneighbor(tiles, next, xcoord + 1, ycoord - 2);
    growneighbor(tiles, next, xcoord - 1, ycoord - 2);
}

void iterate(const Tiles& tiles, Tiles& next)
{
    for (const auto& tile : tiles) {
        int xcoord = tile.first.first;
        int ycoord = tile.first.second;
        growneighbors(tiles, next, xcoord, ycoord);
        
        int count = countneighbors(tiles, xcoord, ycoord);
        if (count == 0 || count > 2) {
            //printf("Tile at coordinate (%d, %d) has %d neighbors, was %d now setting to 0.\n", xcoord, ycoord, count, tile.second);
            next[tile.first] = 0;
        } else if (count == 2) {
            //printf("Tile at coordinate (%d, %d) has %d neighbors, was %d setting to 1.\n", xcoord, ycoord, count, tile.second);
            next[tile.first] = 1;
        } else {
            //printf("Tile at coordinate (%d, %d) has %d neighbors, leaving %d.\n", xcoord, ycoord, count, tile.second);
            next[tile.first] = tile.second;
        }
    }
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    Tiles tiles;

    while (std::getline(is, str)) {
        int xcoord = 0;
        int ycoord = 0;

        for (int c = 0; c < str.length(); c++) {
            int v = str.at(c);
            switch(v) {
            case 'n':
                ycoord += 2;
                ++c;
                v = str.at(c);
                if (v == 'w') {
                    xcoord -= 1;
                } else if (v == 'e') {
                    xcoord += 1;
                } else {
                    printf("unexpected char after 'n'!\n");
                }
                break;
            case 's':
                ycoord -= 2;
                ++c;
                v = str.at(c);
                if (v == 'w') {
                    xcoord -= 1;
                } else if (v == 'e') {
                    xcoord += 1;
                } else {
                    printf("unexpected char after 's'!\n");
                }
                break;
            case 'e':
                xcoord += 2;
                break;
            case 'w':
                xcoord -= 2;
                break;
            default:
                printf("unexpected char '%c'!\n", v);
                break;
            }
        }

        auto key = std::make_pair(xcoord, ycoord);
        tiles[key] = 1 - tiles[key];
    }

    int count = counttiles(tiles);
    printf("Part 1: A total of %d tiles are flipped.\n", count);

    for (int day = 0; day < 100; day++) {
        Tiles next;

        iterate(tiles, next);

        tiles = next;
        count = counttiles(tiles);
        printf("Day %d: %d\n", day + 1, count);
    }

    return 0;
}
