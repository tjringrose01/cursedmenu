#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

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
    {
        bool threw = false;
        try {
            (void)CursedMenuLoader::load(
                "tests/testdata/does-not-exist.cmd",
                false);
        } catch (const cursedmenu::MenuLoadException&) {
            threw = true;
        }
        REQUIRE(threw);
    }

    {
        bool threw = false;
        try {
            (void)CursedMenuLoader::load(
                "tests/testdata/recursive-a.cmd",
                false);
        } catch (const cursedmenu::MenuLoadException&) {
            threw = true;
        }
        REQUIRE(threw);
    }

    {
        const std::string tempPath = "tests/testdata/oversized-line.cmd";
        {
            std::ofstream output(tempPath);
            output << "MenuBegin\n";
            output << "MenuTitle = ";
            output << std::string(5000, 'A') << "\n";
            output << "MenuEnd\n";
        }

        bool threw = false;
        try {
            (void)CursedMenuLoader::load(tempPath, false);
        } catch (const cursedmenu::MenuLoadException&) {
            threw = true;
        }
        std::filesystem::remove(tempPath);
        REQUIRE(threw);
    }

    std::cout << "All CursedMenuLoader exception tests passed." << std::endl;
    return 0;
}
