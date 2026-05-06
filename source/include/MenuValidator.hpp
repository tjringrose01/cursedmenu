#ifndef CURSEDMENU_MENU_VALIDATOR_HPP
#define CURSEDMENU_MENU_VALIDATOR_HPP

#include <string>
#include <vector>

#include "MenuDefinition.hpp"
#include "MenuParseResult.hpp"

namespace cursedmenu {

/**
 * Performs shared semantic validation for parsed menu definitions.
 *
 * Parsers should handle format-specific syntax errors. This validator handles
 * rules that apply after a file has been converted into the common menu model.
 */
class MenuValidator {
public:
    /**
     * Validates a menu definition and returns all discovered errors.
     */
    std::vector<MenuParseError> validate(
        const MenuDefinition& menuDefinition,
        const std::string& fileName) const;

private:
    bool isKnownColor(const std::string& colorName) const;
};

} // namespace cursedmenu

#endif // CURSEDMENU_MENU_VALIDATOR_HPP
