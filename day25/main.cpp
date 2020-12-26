#include <cinttypes>
#include <fstream>
#include <iostream>
#include <string>

int transform(int subjectnumber, int value)
{
    value = ((int64_t)value * subjectnumber) % 20201227;
    return value;
}

int findloopsize(const int key)
{
    const int subjectnumber = 7;
    int loopsize = 0;
    int value = 1;
    do {
        ++loopsize;
        value = transform(subjectnumber, value);
    } while (value != key);
    return loopsize;
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    int cardpublickey = 0;
    std::getline(is, str);
    sscanf(str.c_str(), "%d", &cardpublickey);

    int doorpublickey = 0;
    std::getline(is, str);
    sscanf(str.c_str(), "%d", &doorpublickey);

    printf("Card public key: %d\n", cardpublickey);
    printf("Door public key: %d\n", doorpublickey);

    int cardloopsize = findloopsize(cardpublickey);
    int doorloopsize = findloopsize(doorpublickey);

    printf("Card loop size: %d\n", cardloopsize);
    printf("Door loop size: %d\n", doorloopsize);

    int value = 1;
    for (int i = 0; i < cardloopsize; i++) {
        value = transform(doorpublickey, value);
    }

    printf("Transforming door public key by card loop size: %d\n", (int)value);

    value = 1;
    for (int i = 0; i < doorloopsize; i++) {
        value = transform(cardpublickey, value);
    }

    printf("Transforming card public key by door loop size: %d\n", (int)value);

    return 0;
}
