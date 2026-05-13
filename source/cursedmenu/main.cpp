/**
 * main.cpp - cursedmenu entry point
 *
 * Modernization note:
 * Interactive ncurses runtime ownership now resides in
 * cursedmenu::CursedMenuRunner.
 */

#include <cstdlib>
#include <curses.h>
#include <filesystem>
#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "ActionLogger.hpp"
#include "CursedMenu.hpp"
#include "CursedMenuExceptions.hpp"
#include "CursedMenuItem.hpp"
#include "MenuDefinition.hpp"
#include "CursedMenuLoader.hpp"
#include "CursedMenuRunner.hpp"
#include "MenuFileResolver.hpp"
#include "MenuParserFactory.hpp"
#include "debug.hpp"

#define PACKAGE_NAME "cursedmenu"
#define PACKAGE_STRING "cursedmenu"

namespace {

void displayUsage() {
    std::cout
        << PACKAGE_NAME << " - " << PACKAGE_STRING << std::endl
        << std::endl
        << "usage: " << PACKAGE_NAME
        << " [-h|--help] [-m <menu file>] [-c]"
        << std::endl
        << std::endl
        << "options:" << std::endl
        << "  -h or --help    - show program usage" << std::endl
        << "  -m <menu file>  - specify the cursed menu definition"
        << std::endl
        << "  -c              - check all menu definition files"
        << std::endl;
}

int parseArgs(
    int argc,
    char** argv,
    std::string& menuFile,
    bool& performMenuCheck,
    bool& showHelp) {
    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];

        if (argument == "--help" || argument == "-h") {
            showHelp = true;
            return SUCCESS;
        }

        if (argument == "-m") {
            if (index + 1 >= argc) {
                std::cerr
                    << "Missing value for -m option"
                    << std::endl;

                return ERROR_INVALID_ARG_VALUES;
            }

            menuFile = argv[++index];
            continue;
        }

        if (argument == "-c") {
            performMenuCheck = true;
            continue;
        }

        std::cerr
            << "Unknown option: " << argument
            << std::endl;

        return ERROR_INVALID_USAGE;
    }

    return SUCCESS;
}

std::string getEnvironmentVariable(
    const std::string& variableName) {
    const char* variableValue = std::getenv(variableName.c_str());

    if (variableValue == nullptr) {
        return "";
    }

    return variableValue;
}

void ensureEnvironmentVariable(
    const std::string& variableName,
    const std::string& variableValue) {
    if (getEnvironmentVariable(variableName).empty()) {
        setenv(variableName.c_str(), variableValue.c_str(), 1);
    }
}

int colorFromName(const std::string& colorName) {
    if (colorName == "BLACK") {
        return COLOR_BLACK;
    }
    if (colorName == "RED") {
        return COLOR_RED;
    }
    if (colorName == "GREEN") {
        return COLOR_GREEN;
    }
    if (colorName == "YELLOW") {
        return COLOR_YELLOW;
    }
    if (colorName == "BLUE") {
        return COLOR_BLUE;
    }
    if (colorName == "MAGENTA") {
        return COLOR_MAGENTA;
    }
    if (colorName == "CYAN") {
        return COLOR_CYAN;
    }
    return COLOR_WHITE;
}

std::string execTextFromAction(const cursedmenu::MenuAction& action) {
    if (action.type == cursedmenu::MenuActionType::Exit) {
        return "MenuExit";
    }
    if (action.type == cursedmenu::MenuActionType::Submenu) {
        return "MenuSub " + action.value;
    }
    return action.value;
}

CursedMenu buildRuntimeMenuFromDefinition(
    const cursedmenu::Menu& parsedMenu,
    const bool debugIsOn) {
    CursedMenu runtimeMenu;
    runtimeMenu.setDebugFlag(debugIsOn);
    runtimeMenu.setMenuName(parsedMenu.id);
    runtimeMenu.setMenuTitle(parsedMenu.title);

    if (!parsedMenu.foreground.empty()) {
        runtimeMenu.setForeColor(colorFromName(parsedMenu.foreground));
    }
    if (!parsedMenu.background.empty()) {
        runtimeMenu.setBackColor(colorFromName(parsedMenu.background));
    }

    for (const auto& parsedItem : parsedMenu.items) {
        runtimeMenu.addItem(
            CursedMenuItem(
                parsedItem.name,
                parsedItem.description,
                execTextFromAction(parsedItem.action)));
    }

    return runtimeMenu;
}

} // namespace

int main(int argc, char** argv) {
    try {
        ActionLogger logger;

        logger.log("cursedmenu begin");

        ensureEnvironmentVariable(
            "TERMINFO",
            "/usr/share/terminfo");

        std::string menuFile = "default.json";
        bool performMenuCheck = false;
        bool showHelp = false;
        bool debugIsOn = false;

        const int parseResult = parseArgs(
            argc,
            argv,
            menuFile,
            performMenuCheck,
            showHelp);

        if (showHelp) {
            displayUsage();
            return SUCCESS;
        }

        menuFile = cursedmenu::resolveMenuFilePath(menuFile);

        if (parseResult != SUCCESS) {
            displayUsage();
            return parseResult;
        }

        if (performMenuCheck) {
            cursedmenu::MenuParserFactory parserFactory;
            const cursedmenu::MenuParser* parser =
                parserFactory.getParserForFile(menuFile);

            if (parser == nullptr) {
                throw cursedmenu::ParserException(
                    "No parser available for file: " + menuFile);
            }

            const auto parseResult = parser->parseFile(menuFile);

            std::cout
                << "Number of menus correctly parsed: "
                << parseResult.menuDefinition.menus.size()
                << std::endl;

            return SUCCESS;
        }

        std::stack<CursedMenu> menus;
        std::function<std::optional<CursedMenu>(const std::string&)> submenuResolver;

        cursedmenu::MenuParserFactory parserFactory;
        const cursedmenu::MenuParser* parser =
            parserFactory.getParserForFile(menuFile);
        if (parser == nullptr) {
            throw cursedmenu::ParserException(
                "No parser available for file: " + menuFile);
        }

        const auto parsedResult = parser->parseFile(menuFile);
        std::unordered_map<std::string, CursedMenu> runtimeMenusById;
        runtimeMenusById.reserve(parsedResult.menuDefinition.menus.size());

        for (const auto& parsedMenu : parsedResult.menuDefinition.menus) {
            runtimeMenusById.emplace(
                parsedMenu.id,
                buildRuntimeMenuFromDefinition(parsedMenu, debugIsOn));
        }

        const auto rootMenuIterator =
            runtimeMenusById.find(parsedResult.menuDefinition.rootMenu);
        if (rootMenuIterator == runtimeMenusById.end()) {
            throw cursedmenu::MenuLoadException(
                "Root menu not found in parsed definition: "
                + parsedResult.menuDefinition.rootMenu);
        }

        submenuResolver = [runtimeMenusById](const std::string& submenuId)
            -> std::optional<CursedMenu> {
            const auto iterator = runtimeMenusById.find(submenuId);
            if (iterator == runtimeMenusById.end()) {
                return std::nullopt;
            }
            return iterator->second;
        };

        menus.push(rootMenuIterator->second);

        if (menus.top().getNumOfItems() == 0) {
            throw cursedmenu::MenuLoadException(
                "No menu items found in: " + menuFile);
        }

        cursedmenu::CursedMenuRunner runner(logger, submenuResolver);

        runner.run(menus);

        return SUCCESS;
    } catch (const cursedmenu::ValidationException& exception) {
        std::cerr
            << "validation error: " << exception.what()
            << std::endl;
        return ERROR_INVALID_FILENAME;
    } catch (const cursedmenu::ParserException& exception) {
        std::cerr
            << "parser error: " << exception.what()
            << std::endl;
        return ERROR_INVALID_FILENAME;
    } catch (const cursedmenu::MenuLoadException& exception) {
        std::cerr
            << "menu load error: " << exception.what()
            << std::endl;
        return ERROR_INVALID_FILENAME;
    } catch (const cursedmenu::RuntimeException& exception) {
        std::cerr
            << "runtime error: " << exception.what()
            << std::endl;
        return ERROR_UNKNOWN;
    } catch (const cursedmenu::CursedMenuException& exception) {
        std::cerr
            << "cursedmenu error: " << exception.what()
            << std::endl;
        return ERROR_INVALID_FILENAME;
    } catch (const std::exception& exception) {
        std::cerr
            << "unexpected error: " << exception.what()
            << std::endl;
        return ERROR_UNKNOWN;
    }
}
