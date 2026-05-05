#include "JsonMenuParser.hpp"

#include <fstream>
#include <sstream>

namespace cursedmenu {

bool JsonMenuParser::supportsFile(
    const std::filesystem::path& path) const {
    return path.extension() == ".json";
}

MenuParseResult JsonMenuParser::parseFile(
    const std::filesystem::path& path) const {
    MenuParseResult result;

    std::ifstream inputFile(path);

    if (!inputFile.is_open()) {
        result.errors.push_back({
            path.string(),
            "file",
            "Unable to open JSON menu file"
        });

        return result;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();

    const std::string content = buffer.str();

    if (content.empty()) {
        result.errors.push_back({
            path.string(),
            "file",
            "JSON menu file is empty"
        });

        return result;
    }

    // Placeholder implementation until a full JSON parser is added.
    // This establishes the parser architecture and common result model.
    result.success = true;
    result.menuDefinition.version = 1;
    result.menuDefinition.rootMenu = "main";

    return result;
}

} // namespace cursedmenu
