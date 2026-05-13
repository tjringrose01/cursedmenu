#include <filesystem>
#include <fstream>
#include <iostream>

#include "CursedMenuExceptions.hpp"
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

    REQUIRE(requiresException(testFile("invalid-json-format.json")));
    REQUIRE(requiresException(testFile("missing-root-menu.json")));
    REQUIRE(requiresException(testFile("duplicate-menu-id.json")));
    REQUIRE(requiresException(testFile("missing-submenu-target.json")));
    REQUIRE(requiresException(testFile("submenu-cycle.json")));
    REQUIRE(requiresException(testFile("invalid-colors.json")));
    REQUIRE(requiresException(testFile("empty-values.json")));

    {
        auto result = parser.parseFile(testFile("long-values.json"));

        REQUIRE(result.success);
        REQUIRE(result.errors.empty());
    }

    REQUIRE(requiresException(testFile("multiple-actions.json")));
    REQUIRE(requiresException(testFile("unsupported-action.json")));

    {
        const auto tempPath = testFile("oversized-field.json");
        {
            std::ofstream output(tempPath);
            output
                << "{\n"
                << "  \"version\": 1,\n"
                << "  \"rootMenu\": \"main\",\n"
                << "  \"menus\": [\n"
                << "    {\n"
                << "      \"id\": \"main\",\n"
                << "      \"title\": \"Main\",\n"
                << "      \"items\": [\n"
                << "        {\n"
                << "          \"name\": \"" << std::string(5000, 'N') << "\",\n"
                << "          \"command\": \"echo hi\"\n"
                << "        }\n"
                << "      ]\n"
                << "    }\n"
                << "  ]\n"
                << "}\n";
        }

        REQUIRE(requiresException(tempPath));
        std::filesystem::remove(tempPath);
    }

    std::cout << "All JsonMenuParser tests passed." << std::endl;

    return 0;
}
