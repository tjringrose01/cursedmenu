#include <iostream>

#include "MenuFileResolver.hpp"

#define REQUIRE(condition)                                                     \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "FAILED: " #condition << std::endl;                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

int main() {
    REQUIRE(
        cursedmenu::resolveMenuFilePath("default.json")
        == "examples/default.json");

    REQUIRE(
        cursedmenu::resolveMenuFilePath("default.cmd")
        == "source/cursedmenu/default.cmd");

    REQUIRE(
        cursedmenu::resolveMenuFilePath("does-not-exist.json")
        == "does-not-exist.json");

    std::cout << "All MenuFileResolver tests passed." << std::endl;
    return 0;
}
