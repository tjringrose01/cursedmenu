#include "NcursesMenu.hpp"

#include "CursedMenu.hpp"

namespace cursedmenu {

NcursesMenu::NcursesMenu(
    WINDOW* parentWindow,
    const CursedMenu& menuModel)
    : menu(nullptr),
      subWindow(nullptr) {
    const int itemCount = menuModel.getNumOfItems();
    items.reserve(itemCount);
    itemNames.reserve(itemCount);
    itemDescriptions.reserve(itemCount);

    for (int index = 0; index < itemCount; ++index) {
        const auto& item = menuModel.getItem(index);
        itemNames.push_back(item.getName());
        itemDescriptions.push_back(item.getDesc());

        items.push_back(new_item(
            itemNames.back().c_str(),
            itemDescriptions.back().c_str()));
    }

    items.push_back(nullptr);

    menu = new_menu(items.data());

    set_menu_format(menu, LINES - 2, 0);
    menu_opts_on(menu, O_ROWMAJOR);
    menu_opts_off(menu, O_SHOWDESC);
    menu_opts_off(menu, O_NONCYCLIC);

    set_menu_win(menu, parentWindow);

    const int menuCenter =
        (COLS / 2) - (menuModel.getMenuCenterX() / 2);
    subWindow = derwin(parentWindow, 0, 0, 6, menuCenter);
    set_menu_sub(menu, subWindow);

    set_menu_fore(menu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_back(menu, COLOR_PAIR(1));
}

NcursesMenu::~NcursesMenu() noexcept {
    if (menu != nullptr) {
        unpost_menu(menu);
        free_menu(menu);
    }

    for (auto* item : items) {
        if (item != nullptr) {
            free_item(item);
        }
    }

    if (subWindow != nullptr) {
        delwin(subWindow);
    }
}

MENU* NcursesMenu::get() const noexcept {
    return menu;
}

void NcursesMenu::post() const noexcept {
    post_menu(menu);
}

void NcursesMenu::driver(const int request) const noexcept {
    menu_driver(menu, request);
}

const char* NcursesMenu::currentItemName() const noexcept {
    return item_name(current_item(menu));
}

const char* NcursesMenu::currentItemDescription() const noexcept {
    return item_description(current_item(menu));
}

void NcursesMenu::nudgeSelection() const noexcept {
    menu_driver(menu, REQ_UP_ITEM);
    menu_driver(menu, REQ_DOWN_ITEM);
}

} // namespace cursedmenu
