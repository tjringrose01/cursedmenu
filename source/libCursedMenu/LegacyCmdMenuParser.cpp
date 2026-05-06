#include "LegacyCmdMenuParser.hpp"

#include <fstream>

namespace cursedmenu {

bool LegacyCmdMenuParser::supportsFile(
    const std::filesystem::path& path) const {
    return path.extension() == ".cmd";
}

MenuParseResult LegacyCmdMenuParser::parseFile(
    const std::filesystem::path& path) const {
    MenuParseResult result;

    std::ifstream inputFile(path);

    if (!inputFile.is_open()) {
        result.errors.push_back({
            path.string(),
            "file",
            "Unable to open legacy .cmd menu file"
        });

        return result;
    }

    // Placeholder implementation while the existing parser logic is migrated
    // into the new parser architecture.
    result.success = true;
    result.menuDefinition.version = 1;
    result.menuDefinition.rootMenu = "main";

    return result;
}

} // namespace cursedmenu
