#ifndef CURSEDMENU_LEGACY_CMD_MENU_PARSER_HPP
#define CURSEDMENU_LEGACY_CMD_MENU_PARSER_HPP

#include "MenuParser.hpp"

namespace cursedmenu {

/**
 * Parser implementation for the legacy .cmd menu file format.
 */
class LegacyCmdMenuParser final : public MenuParser {
public:
    bool supportsFile(const std::filesystem::path& path) const override;

    MenuParseResult parseFile(
        const std::filesystem::path& path) const override;
};

} // namespace cursedmenu

#endif // CURSEDMENU_LEGACY_CMD_MENU_PARSER_HPP
