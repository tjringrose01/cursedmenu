#ifndef CURSEDMENU_YAML_MENU_PARSER_HPP
#define CURSEDMENU_YAML_MENU_PARSER_HPP

#include "MenuParser.hpp"

namespace cursedmenu {

class YamlMenuParser final : public MenuParser {
public:
    bool supportsFile(const std::filesystem::path& path) const noexcept override;

    MenuParseResult parseFile(
        const std::filesystem::path& path) const override;
};

} // namespace cursedmenu

#endif // CURSEDMENU_YAML_MENU_PARSER_HPP
