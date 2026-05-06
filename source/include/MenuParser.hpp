#ifndef CURSEDMENU_MENU_PARSER_HPP
#define CURSEDMENU_MENU_PARSER_HPP

#include <filesystem>

#include "MenuParseResult.hpp"

namespace cursedmenu {

/**
 * Common parser abstraction for all supported menu file formats.
 */
class MenuParser {
public:
    virtual ~MenuParser() noexcept = default;

    /**
     * Returns true if this parser supports the provided file path.
     */
    [[nodiscard]] virtual bool supportsFile(
        const std::filesystem::path& path) const noexcept = 0;

    /**
     * Parses the provided menu definition file.
     */
    [[nodiscard]] virtual MenuParseResult parseFile(
        const std::filesystem::path& path) const = 0;
};

} // namespace cursedmenu

#endif // CURSEDMENU_MENU_PARSER_HPP
