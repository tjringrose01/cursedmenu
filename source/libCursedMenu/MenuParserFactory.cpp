#include "MenuParserFactory.hpp"

#include "JsonMenuParser.hpp"
#include "LegacyCmdMenuParser.hpp"
#include "YamlMenuParser.hpp"

namespace cursedmenu {

MenuParserFactory::MenuParserFactory() {
    parsers.push_back(std::make_unique<JsonMenuParser>());
    parsers.push_back(std::make_unique<YamlMenuParser>());
    parsers.push_back(std::make_unique<LegacyCmdMenuParser>());
}

const MenuParser* MenuParserFactory::getParserForFile(
    const std::filesystem::path& path) const {
    for (const auto& parser : parsers) {
        if (parser->supportsFile(path)) {
            return parser.get();
        }
    }

    return nullptr;
}

} // namespace cursedmenu
