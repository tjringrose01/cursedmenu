#include <iostream>

#include "CursedMenu.hpp"
#include "CursedMenuExceptions.hpp"
#include "CursedMenuItem.hpp"

#define REQUIRE(condition)                                                     \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "FAILED: " #condition << std::endl;                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

int main() {
    {
        CursedMenu menu;
        menu.addItem(CursedMenuItem("Item1", "Desc", "echo ok"));

        REQUIRE(menu.getItem(0).getName() == "Item1");
        REQUIRE(menu.getItem("Item1").getExec() == "echo ok");
    }

    {
        CursedMenu menu;
        menu.addItem(CursedMenuItem("Item1", "Desc", "echo ok"));

        bool threw = false;
        try {
            (void)menu.getItem(99);
        } catch (const cursedmenu::RuntimeException&) {
            threw = true;
        }
        REQUIRE(threw);
    }

    {
        CursedMenu menu;
        menu.addItem(CursedMenuItem("Item1", "Desc", "echo ok"));

        bool threw = false;
        try {
            (void)menu.getItem("Missing");
        } catch (const cursedmenu::RuntimeException&) {
            threw = true;
        }
        REQUIRE(threw);
    }

    {
        CursedMenu emptyMenu;

        bool threw = false;
        try {
            (void)emptyMenu.getItem(0);
        } catch (const cursedmenu::RuntimeException&) {
            threw = true;
        }
        REQUIRE(threw);
    }

    std::cout << "All CursedMenu exception tests passed." << std::endl;
    return 0;
}
