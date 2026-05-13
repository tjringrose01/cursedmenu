#include <filesystem>
#include <iostream>

#include "CursedMenuExceptions.hpp"
#include "YamlMenuParser.hpp"

#define REQUIRE(condition)                                                     \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "FAILED: " #condition << std::endl;                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

using namespace cursedmenu;

namespace {

std::filesystem::path testFile(const std::string& name) {
    return std::filesystem::path("tests/testdata") / name;
}

} // namespace

int main() {
    YamlMenuParser parser;

    REQUIRE(parser.supportsFile("menu.yaml"));
    REQUIRE(parser.supportsFile("menu.yml"));
    REQUIRE(!parser.supportsFile("menu.json"));

    {
        auto result = parser.parseFile(testFile("valid-default.yaml"));
        REQUIRE(result.success);
        REQUIRE(result.errors.empty());
        REQUIRE(result.menuDefinition.rootMenu == "main");
        REQUIRE(result.menuDefinition.menus.size() == 2);
    }

    {
        auto result = parser.parseFile(testFile("settings-aliases.yaml"));
        REQUIRE(result.success);
        REQUIRE(result.errors.empty());
        REQUIRE(result.menuDefinition.debug);
        REQUIRE(result.menuDefinition.pauseAfterExecution);
        REQUIRE(!result.menuDefinition.showDate);
        REQUIRE(result.menuDefinition.showTime);
    }

    {
        auto result = parser.parseFile(testFile("yaml-single-quote-command.yaml"));
        REQUIRE(result.success);
        REQUIRE(result.menuDefinition.menus.at(0).items.at(0).action.value
            == "printf 'single-quoted value'");
    }

    {
        auto result = parser.parseFile(testFile("yaml-double-quote-command.yaml"));
        REQUIRE(result.success);
        REQUIRE(result.menuDefinition.menus.at(0).items.at(0).action.value
            == "printf \"double-quoted value\"");
    }

    auto requiresException = [&parser](const std::filesystem::path& path) {
        try {
            (void)parser.parseFile(path);
            return false;
        } catch (const ParserException&) {
            return true;
        } catch (const ValidationException&) {
            return true;
        }
    };

    REQUIRE(requiresException(testFile("yaml-malformed.yaml")));
    REQUIRE(requiresException(testFile("yaml-unknown-field.yaml")));
    REQUIRE(requiresException(testFile("yaml-missing-submenu-target.yaml")));
    REQUIRE(requiresException(testFile("yaml-multiline-command.yaml")));

    std::cout << "All YamlMenuParser tests passed." << std::endl;
    return 0;
}
