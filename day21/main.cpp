#include <algorithm>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using Ingredients = std::list<std::string>;
using Allergens = std::list<std::string>;

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

struct IAPair
{
    std::string ingredient;
    std::string allergen;
};

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    std::vector<Ingredients> allIngredients;
    std::vector<Allergens> allAllergens;

    while (std::getline(is, str)) {
        Ingredients ingredients;
        Allergens allergens;

        std::string token;
        size_t pos = 0;
        while (pos != std::string::npos) {
            pos = nextToken(str, " (", pos, token);
            if (token == "contains") {
                break;
            }
            if (!token.empty()) {
                printf("Got ingredient: %s\n", token.c_str());
                ingredients.push_back(token);
            }
        }
        while (pos != std::string::npos) {
            pos = nextToken(str, " ,)", pos, token);
            if (pos != std::string::npos && !token.empty()) {
                printf("Got allergen: %s\n", token.c_str());
                allergens.push_back(token);
            }
        }

        printf("\n\n");

        ingredients.sort();
        allergens.sort();

        allIngredients.push_back(ingredients);
        allAllergens.push_back(allergens);
    }

    std::vector<IAPair> cdil;

    size_t numsets = allAllergens.size();
    int count = 0;

    bool done;
    do {
        done = true;

        for (int i = 0; i < numsets; i++) {
            Ingredients& ti = allIngredients[i];
            Allergens& ta = allAllergens[i];
            if (ta.size() == 1) {
                printf("Testing allergen: %s\n", ta.front().c_str());

                Ingredients ii = ti;
                for (int o = 0; o < numsets; o++) {
                    Ingredients& oi = allIngredients[o];
                    Allergens& oa = allAllergens[o];

                    if (std::find(oa.begin(), oa.end(), ta.front()) != oa.end()) {
                        Ingredients temp;
                        std::set_intersection(
                            ii.begin(), ii.end(),
                            oi.begin(), oi.end(),
                            std::back_inserter(temp));
                        ii = temp;
                    }
                }
                printf("Intersected set size is: %d\n", (int)ii.size());
                if (ii.size() == 1) {
                    printf("Deduced that ingredient %s has allergen %s!\n",
                        ii.front().c_str(),
                        ta.front().c_str());
                    cdil.push_back({ii.front(), ta.front()});

                    std::string allergen = ta.front();

                    for (auto& ri : allIngredients) {
                        ri.remove(ii.front());
                    }
                    for (auto& ra : allAllergens) {
                        ra.remove(allergen);
                    }

                    done = false;
                    break;
                }
            }
        }

        count = 0;
        for (int i =0; i < numsets; i++) {
            const Ingredients& ti = allIngredients[i];
            const Allergens& ta = allAllergens[i];

            for (const auto& tis : ti) {
                printf("%s ", tis.c_str());
                ++count;
            }

            printf("(contains: ");
            for (const auto& tas : ta) {
                printf("%s ", tas.c_str());
            }
            printf(")\n");
        }
        printf("\n\n");
    } while (!done);

    printf("Ingredients remaining: %d\n", count);

    std::sort(cdil.begin(), cdil.end(), [](const IAPair& a, const IAPair& b) { return a.allergen < b.allergen; });
    for(const auto& p : cdil) {
        printf("%s,", p.ingredient.c_str());
    }
    printf("\n");

    return 0;
}
