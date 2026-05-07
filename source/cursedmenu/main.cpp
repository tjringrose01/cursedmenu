/**
 * main.cpp - cursedmenu entry point
 *
 * Modernization note:
 * Interactive ncurses runtime ownership now resides in
 * cursedmenu::CursedMenuRunner.
 */

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "ActionLogger.hpp"
#include "CursedMenu.hpp"
#include "CursedMenuExceptions.hpp"
#include "CursedMenuLoader.hpp"
#include "CursedMenuRunner.hpp"
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
    bool& performMenuCheck) {
    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];

        if (argument == "--help" || argument == "-h") {
            displayUsage();
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

std::string resolveMenuFilePath(const std::string& requestedMenuFile) {
    namespace fs = std::filesystem;

    if (requestedMenuFile.empty()) {
        return requestedMenuFile;
    }

    const fs::path directPath(requestedMenuFile);
    if (fs::exists(directPath)) {
        return requestedMenuFile;
    }

    if (directPath.has_parent_path()) {
        return requestedMenuFile;
    }

    const fs::path sourceMenuPath =
        fs::path("source") / "cursedmenu" / requestedMenuFile;

    if (fs::exists(sourceMenuPath)) {
        return sourceMenuPath.string();
    }

    return requestedMenuFile;
}

} // namespace

int main(int argc, char** argv) {
    try {
        ActionLogger logger;

        logger.log("cursedmenu begin");

        ensureEnvironmentVariable(
            "TERMINFO",
            "/usr/share/terminfo");

        std::string menuFile = "default.cmd";
        bool performMenuCheck = false;
        bool debugIsOn = false;

        const int parseResult = parseArgs(
            argc,
            argv,
            menuFile,
            performMenuCheck);

        menuFile = resolveMenuFilePath(menuFile);

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

        menus.push(CursedMenu(debugIsOn, menuFile));

        if (menus.top().getNumOfItems() == 0) {
            throw cursedmenu::MenuLoadException(
                "No menu items found in: " + menuFile);
        }

        cursedmenu::CursedMenuRunner runner(logger);

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
