#include <iostream>

#include "MenuParserFactory.hpp"

#define REQUIRE(condition)                                                     \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "FAILED: " #condition << std::endl;                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

using namespace cursedmenu;

int main() {
    MenuParserFactory factory;

    REQUIRE(factory.getParserForFile("menu.json") != nullptr);
    REQUIRE(factory.getParserForFile("menu.yaml") != nullptr);
    REQUIRE(factory.getParserForFile("menu.yml") != nullptr);
    REQUIRE(factory.getParserForFile("menu.cmd") != nullptr);
    REQUIRE(factory.getParserForFile("menu.txt") == nullptr);
    REQUIRE(factory.getParserForFile("menu.invalid") == nullptr);

    std::cout << "All MenuParserFactory tests passed." << std::endl;

    return 0;
}
