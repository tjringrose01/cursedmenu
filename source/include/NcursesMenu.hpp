#ifndef CURSEDMENU_NCURSES_MENU_HPP
#define CURSEDMENU_NCURSES_MENU_HPP

#include <string>
#include <vector>

#include <curses.h>
#include <menu.h>

class CursedMenu;

namespace cursedmenu {

class NcursesMenu {
public:
    NcursesMenu(WINDOW* parentWindow, const CursedMenu& menu);
    ~NcursesMenu() noexcept;

    NcursesMenu(const NcursesMenu&) = delete;
    NcursesMenu& operator=(const NcursesMenu&) = delete;

    MENU* get() const noexcept;
    void post() const noexcept;
    void driver(int request) const noexcept;
    const char* currentItemName() const noexcept;
    const char* currentItemDescription() const noexcept;
    void nudgeSelection() const noexcept;

private:
    MENU* menu;
    WINDOW* subWindow;
    std::vector<ITEM*> items;
    std::vector<std::string> itemNames;
    std::vector<std::string> itemDescriptions;
};

} // namespace cursedmenu

#endif // CURSEDMENU_NCURSES_MENU_HPP
