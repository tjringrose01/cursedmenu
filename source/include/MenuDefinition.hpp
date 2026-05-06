#ifndef CURSEDMENU_MENU_DEFINITION_HPP
#define CURSEDMENU_MENU_DEFINITION_HPP

#include <string>
#include <vector>

namespace cursedmenu {

/**
 * Describes the type of action a menu item performs.
 */
enum class MenuActionType {
    Command,
    Submenu,
    Exit
};

/**
 * Represents a single menu item action in the common runtime menu model.
 */
struct MenuAction {
    MenuActionType type = MenuActionType::Command;
    std::string value;
};

/**
 * Represents a selectable item in a menu.
 */
struct MenuItem {
    std::string name;
    std::string description;
    MenuAction action;
};

/**
 * Represents a named menu independent of the source file format.
 */
struct Menu {
    std::string id;
    std::string title;
    std::string foreground;
    std::string background;
    std::vector<MenuItem> items;
};

/**
 * Represents an entire menu definition independent of JSON, legacy .cmd,
 * or any future menu file format.
 */
struct MenuDefinition {
    int version = 1;
    bool debug = false;
    bool pauseAfterExecution = false;
    std::string rootMenu;
    std::vector<Menu> menus;
};

} // namespace cursedmenu

#endif // CURSEDMENU_MENU_DEFINITION_HPP
