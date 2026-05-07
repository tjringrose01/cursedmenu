#include <iostream>

#include "CursedMenuExceptions.hpp"
#include "CursedMenuLoader.hpp"

#define REQUIRE(condition)                                                     \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "FAILED: " #condition << std::endl;                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

int main() {
    bool threw = false;

    try {
        (void)CursedMenuLoader::load("tests/testdata/does-not-exist.cmd", false);
    } catch (const cursedmenu::MenuLoadException&) {
        threw = true;
    }

    REQUIRE(threw);

    std::cout << "All CursedMenuLoader exception tests passed." << std::endl;
    return 0;
}
