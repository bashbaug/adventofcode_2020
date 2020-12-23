#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <tuple>

std::string deck2str(const std::list<int>& deck)
{
    std::string ret;
    for (auto card : deck) {
        ret += std::to_string(card);
        ret += ", ";
    }
    return ret;
}

void game(std::list<int>& player1, std::list<int>& player2, int& gamecount)
{
    int localgamecount = ++gamecount;
    printf("=== Game %d ===\n", localgamecount);

    int roundcount = 0;
    do {
        printf("-- Round %d (Game %d) --\n", ++roundcount, localgamecount);
        int p1card = player1.front(); player1.pop_front();
        int p2card = player2.front(); player2.pop_front();

        printf("Player 1's deck: %s\n", deck2str(player1).c_str());
        printf("Player 2's deck: %s\n", deck2str(player1).c_str());
        printf("Player 1 plays: %d\n", p1card);
        printf("Player 2 plays: %d\n", p2card);

        if (p1card > p2card) {
            printf("Player 1 wins round %d of game %d!\n\n", roundcount, localgamecount);
            player1.push_back(p1card);
            player1.push_back(p2card);
        } else {
            printf("Player 2 wins round %d of game %d!\n\n", roundcount, localgamecount);
            player2.push_back(p2card);
            player2.push_back(p1card);
        }
    } while (!player1.empty() && !player2.empty());

    printf("The winner of game %d is player %d!\n\n", localgamecount, !player1.empty() ? 1 : 2);
}

bool gamerecursive(std::list<int>& player1, std::list<int>& player2, int& gamecount)
{
    int localgamecount = ++gamecount;
    printf("=== Game %d ===\n", localgamecount);

    std::set<std::tuple<std::list<int>, std::list<int>>> seen;

    int roundcount = 0;
    do {
        printf("-- Round %d (Game %d) --\n", ++roundcount, localgamecount);
        int p1card = player1.front(); player1.pop_front();
        int p2card = player2.front(); player2.pop_front();

        printf("Player 1's deck: %s\n", deck2str(player1).c_str());
        printf("Player 2's deck: %s\n", deck2str(player2).c_str());
        printf("Player 1 plays: %d\n", p1card);
        printf("Player 2 plays: %d\n", p2card);

        auto state = std::make_tuple(player1, player2);
        if (seen.find(state) != seen.end()) {
            printf("Player 1 wins due to infinite recursion check.\n");
            return true;    // player 1
        } else {
            seen.insert(state);
        }

        bool p1wins;
        if (player1.size() >= p1card && player2.size() >= p2card) {
            std::list<int> newplayer1;
            std::list<int>::iterator p1it = player1.begin();
            while (newplayer1.size() < p1card) {
                newplayer1.push_back(*p1it++);
            }

            std::list<int> newplayer2;
            std::list<int>::iterator p2it = player2.begin();
            while (newplayer2.size() < p2card) {
                newplayer2.push_back(*p2it++);
            }

            printf("Playing a sub-game to determine the winner...\n\n");
            p1wins = gamerecursive(newplayer1, newplayer2, gamecount);
            printf("... anyway back to game %d.\n", localgamecount);
        } else {
            p1wins = (p1card > p2card);
        }

        if (p1wins) {
            printf("Player 1 wins round %d of game %d!\n\n", roundcount, localgamecount);
            player1.push_back(p1card);
            player1.push_back(p2card);
        } else {
            printf("Player 2 wins round %d of game %d!\n\n", roundcount, localgamecount);
            player2.push_back(p2card);
            player2.push_back(p1card);
        }
    } while (!player1.empty() && !player2.empty());

    printf("The winner of game %d is player %d!\n\n", localgamecount, !player1.empty() ? 1 : 2);
    return !player1.empty();
}

int calculatetotal(const std::list<int>& cards)
{
    int value = (int)cards.size();
    int total = 0;
    for (auto card : cards) {
        printf("Card %d has value %d.\n", card, value);
        total += card * value;
        printf("Total is: %d\n", total);
        value--;
    }
    return total;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    std::list<int> player1;
    std::list<int> player2;

    std::getline(is, str);  // Player 1

    while (std::getline(is, str)) {
        if (str.empty()) break;

        int card = 0;
        std::sscanf(str.c_str(), "%d", &card);
        printf("Adding card %d (%s) to player 1's deck.\n", card, str.c_str());
        player1.push_back(card);
    }

    std::getline(is, str);  // Player 2

    while (std::getline(is, str)) {
        if (str.empty()) break;

        int card = 0;
        std::sscanf(str.c_str(), "%d", &card);
        printf("Adding card %d (%s) to player 2's deck.\n", card, str.c_str());
        player2.push_back(card);
    }

    int phase1total = 0;
    int phase2total = 0;

    {
        std::list<int> p1temp = player1;
        std::list<int> p2temp = player2;

        int gamecount = 0;
        game(p1temp, p2temp, gamecount);
        phase1total = calculatetotal( p1temp.empty() ? p2temp : p1temp );
    }

    {
        std::list<int> p1temp = player1;
        std::list<int> p2temp = player2;

        int gamecount = 0;
        gamerecursive(p1temp, p2temp, gamecount);
        phase2total = calculatetotal( p1temp.empty() ? p2temp : p1temp );
    }

    fprintf(stderr, "Phase1: The winning player's score is: %d\n", phase1total);
    fprintf(stderr, "Phase2: The winning player's score is: %d\n", phase2total);

    return 0;
}
