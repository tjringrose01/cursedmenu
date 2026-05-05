#ifndef CURSEDMENU_MENU_PARSER_FACTORY_HPP
#define CURSEDMENU_MENU_PARSER_FACTORY_HPP

#include <memory>
#include <vector>

#include "MenuParser.hpp"

namespace cursedmenu {

/**
 * Creates and manages parser implementations for supported menu formats.
 */
class MenuParserFactory {
public:
    MenuParserFactory();

    /**
     * Returns a parser that supports the provided file path.
     */
    const MenuParser* getParserForFile(
        const std::filesystem::path& path) const;

private:
    std::vector<std::unique_ptr<MenuParser>> parsers;
};

} // namespace cursedmenu

#endif // CURSEDMENU_MENU_PARSER_FACTORY_HPP
