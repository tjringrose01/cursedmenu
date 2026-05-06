#include <filesystem>
#include <iostream>

#include "JsonMenuParser.hpp"

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
    JsonMenuParser parser;

    REQUIRE(parser.supportsFile("menu.json"));
    REQUIRE(!parser.supportsFile("menu.cmd"));

    {
        auto result = parser.parseFile(testFile("valid-default.json"));

        REQUIRE(result.success);
        REQUIRE(result.errors.empty());
        REQUIRE(result.menuDefinition.rootMenu == "main");
        REQUIRE(result.menuDefinition.menus.size() == 2);
    }

    {
        auto result = parser.parseFile(testFile("invalid-json-format.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("missing-root-menu.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("duplicate-menu-id.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("missing-submenu-target.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("submenu-cycle.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("invalid-colors.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("empty-values.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("long-values.json"));

        REQUIRE(result.success);
        REQUIRE(result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("multiple-actions.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    {
        auto result = parser.parseFile(testFile("unsupported-action.json"));

        REQUIRE(!result.success);
        REQUIRE(!result.errors.empty());
    }

    std::cout << "All JsonMenuParser tests passed." << std::endl;

    return 0;
}
