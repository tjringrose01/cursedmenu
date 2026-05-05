#ifndef CURSEDMENU_JSON_MENU_PARSER_HPP
#define CURSEDMENU_JSON_MENU_PARSER_HPP

#include "MenuParser.hpp"

namespace cursedmenu {

/**
 * Parser implementation for JSON menu files.
 */
class JsonMenuParser final : public MenuParser {
public:
    bool supportsFile(const std::filesystem::path& path) const override;

    MenuParseResult parseFile(
        const std::filesystem::path& path) const override;
};

} // namespace cursedmenu

#endif // CURSEDMENU_JSON_MENU_PARSER_HPP
