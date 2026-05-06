#ifndef CURSEDMENU_MENU_PARSE_RESULT_HPP
#define CURSEDMENU_MENU_PARSE_RESULT_HPP

#include <string>
#include <vector>

#include "MenuDefinition.hpp"

namespace cursedmenu {

/**
 * Represents a parser validation or load error.
 */
struct MenuParseError {
    std::string file;
    std::string location;
    std::string message;
};

/**
 * Represents the result of parsing a menu definition file.
 */
struct MenuParseResult {
    bool success = false;
    MenuDefinition menuDefinition;
    std::vector<MenuParseError> errors;
};

} // namespace cursedmenu

#endif // CURSEDMENU_MENU_PARSE_RESULT_HPP
