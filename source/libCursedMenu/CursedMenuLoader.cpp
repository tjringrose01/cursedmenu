/**
 *  CursedMenuLoader.cpp - CursedMenuLoader Class Implementation - This
 *                        class is responsible for reading in the
 *                        configuration file and returning it's informtion
 *                        within class members.
 *
 *  Caution - This class uses recursion!!!
 *
 *  Load Menu Logic:
 *  1. Initial call to loadConfig() is made.
 *  2. Before the Main Menu has completed, a sub menu is detected.
 *      a. A recursive call to loadConfig() is made for the sub menu file
 *      b. Before the sub menu has completed, a sub menu is detected
 *          i. A recursive call to loadConfig() is made for the sub menu file
 *         ii. When loadConfig() returns with the array of CursedMenu's, this
 *             is appended to the current array(which should be empty).
 *        iii. Then this instance of loadConfig() is returned with the
 *             appended array.
 *      c. When loadConfig() returns with the array of CursedMenu's, this
 *         is appended to the current array(We now have 1 menus).
 *      d. Then this instance of loadConfig() is returned with the
 *         appended array.
 *  3. When loadConfig() returns with the array of CursedMenu's, this
 *     is appended to the current array(We now have 2 menus).
 *  4. Then this instance of loadConfig() is returned with the
 *     appended array.
 *  5. The Main Menu completes it's processing and is added to the end
 *     of the vector.
 *
 * ---------------------------------------------------------------------------
 *
 *  Copyright 2007, 2008, 2024 Timothy Ringrose
 *
 *  This file is part of cursedmenu.
 *
 *  cursedmenu is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  cursedmenu is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with cursedmenu.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cctype>
#include <curses.h>

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "CursedMenu.hpp"
#include "CursedMenuItem.hpp"
#include "CursedMenuLoader.hpp"
#include "debug.hpp"

#define PROGRAM "CursedMenuLoader"

namespace {

std::string trimLeadingSpaces(std::string value) {
    const auto firstNonSpace = value.find_first_not_of(' ');

    if (firstNonSpace == std::string::npos) {
        return "";
    }

    return value.substr(firstNonSpace);
std::string resolveConfigPath(
    const std::string& configFile,
    const std::filesystem::path& parentConfigPath)
{
    namespace fs = std::filesystem;
    fs::path candidate(configFile);

    if (fs::exists(candidate)) {
        return candidate.string();
    }

    if (!candidate.is_absolute() && !parentConfigPath.empty()) {
        const fs::path relativeToParent =
            parentConfigPath.parent_path() / candidate;
        if (fs::exists(relativeToParent)) {
            return relativeToParent.string();
        }
    }

    return configFile;
}

} // namespace

CursedMenu CursedMenuLoader::load(const std::string configFile, const bool debugFlag)
{
    std::vector<CursedMenu> menus = loadConfig(configFile, debugFlag);
    if (menus.empty()) {
        return CursedMenu();
    }

    return menus.back();
}

std::string toUpperAscii(const std::string& value) {
    std::string result;
    result.reserve(value.length());

    for (const unsigned char character : value) {
        result += static_cast<char>(std::toupper(character));
    }

    return result;
}

int foregroundColorFromName(const std::string& colorName) {
    const std::string color = toUpperAscii(colorName);
    const std::string resolvedConfigFile =
        resolveConfigPath(configFile, std::filesystem::path());
    const std::filesystem::path resolvedConfigPathObj(resolvedConfigFile);

    std::cout << "CursedMenuLoader::loadConfig(" << resolvedConfigFile << ", " << debugFlag << ");" << std::endl;
    std::vector<CursedMenu> menus;
    std::vector<CursedMenu> tmp_menus;

    if (color == "BLACK") {
        return COLOR_BLACK;
    }

    if (color == "RED") {
        return COLOR_RED;
    }

    if (color == "GREEN") {
        return COLOR_GREEN;
    }

    if (color == "BLUE") {
        return COLOR_BLUE;
    }

    if (color == "YELLOW") {
        return COLOR_YELLOW;
    }

    if (color == "MAGENTA") {
        return COLOR_MAGENTA;
    }

    if (color == "CYAN") {
        return COLOR_CYAN;
    }

    return COLOR_WHITE;
}

int backgroundColorFromName(const std::string& colorName) {
    const std::string color = toUpperAscii(colorName);

    if (color == "BLACK") {
        return COLOR_BLACK;
    }

    if (color == "RED") {
        return COLOR_RED;
    }

    if (color == "GREEN") {
        return COLOR_GREEN;
    }

    if (color == "BLUE") {
        return COLOR_BLUE;
    }

    if (color == "YELLOW") {
        return COLOR_YELLOW;
    }

    if (color == "MAGENTA") {
        return COLOR_MAGENTA;
    }

    if (color == "CYAN") {
        return COLOR_CYAN;
    }

    if (color == "WHITE") {
        return COLOR_WHITE;
    }

    return COLOR_BLACK;
}

bool containsToken(
    const std::string& buffer,
    const std::string& token) {
    return buffer.find(token) != std::string::npos;
}

std::string valueAfterEquals(const std::string& buffer) {
    const auto equalsPosition = buffer.find('=');

    if (equalsPosition == std::string::npos) {
        return "";
    }

    return trimLeadingSpaces(buffer.substr(equalsPosition + 1));
}

} // namespace

CursedMenu CursedMenuLoader::load(
    const std::string& configFile,
    const bool debugFlag) {
    const std::vector<CursedMenu> menus = loadConfig(configFile, debugFlag);

    return menus.at(menus.size() - 1);
}

std::vector<CursedMenu> CursedMenuLoader::loadConfig(
    const std::string& configFile,
    const bool debugFlag) {
    std::cout
        << "CursedMenuLoader::loadConfig("
        << configFile
        << ", "
        << debugFlag
        << ");"
        << std::endl;

    std::vector<CursedMenu> menus;
    menus.reserve(25);

    CursedMenu currentMenu;

    std::string buffer;
    std::string name;
    std::string desc;
    std::string exec;

    bool getMenu = false;
    bool getTitl = false;
    bool getItem = false;

    std::ifstream fileInput(configFile.c_str());

    if (!fileInput.is_open()) {
        std::cerr << "Unable to open file: " << configFile << std::endl;
        return menus;
    }

    if (debugFlag) {
        debug(PROGRAM, 0, "Reading config file - " + configFile);
    }
    std::ifstream file_in( resolvedConfigFile.c_str() );

    if ( file_in.is_open() ) {
        if (debugFlag) debug(PROGRAM, 0, "Reading config file - " + resolvedConfigFile);
        while (! file_in.eof() ) {
            getline ( file_in, buffer );

    while (std::getline(fileInput, buffer)) {
        if (buffer.length() < 6) {
            continue;
        }

        const auto firstContent = buffer.find_first_not_of(" \t\n\v");

        if (firstContent != std::string::npos &&
            buffer[firstContent] == '#') {
            continue;
        }

        if (!getMenu && containsToken(buffer, "MenuBegin")) {
            getMenu = true;
            continue;
        }

        if (containsToken(buffer, "MenuEnd")) {
            getMenu = false;
            continue;
        }

        if (!getTitl && containsToken(buffer, "MenuTitle")) {
            currentMenu.setMenuTitle(valueAfterEquals(buffer));

            if (debugFlag) {
                debug(
                    PROGRAM,
                    2,
                    "menuTitle = " + currentMenu.getMenuTitle());
            }

            continue;
        }

        if (containsToken(buffer, "MenuFore")) {
            const std::string color = valueAfterEquals(buffer);

            if (!color.empty()) {
                currentMenu.setForeColor(foregroundColorFromName(color));
            }

            continue;
        }

        if (containsToken(buffer, "MenuBack")) {
            const std::string color = valueAfterEquals(buffer);

            if (!color.empty()) {
                currentMenu.setBackColor(backgroundColorFromName(color));
            }

            continue;
        }

        if (!getItem) {
            if (containsToken(buffer, "ItemName")) {
                name = valueAfterEquals(buffer);
                getItem = true;

                if (debugFlag) {
                    debug(PROGRAM, 2, "found item = " + name);
                }

                continue;
            }
        } else {
            if (containsToken(buffer, "ItemDesc")) {
                desc = valueAfterEquals(buffer);

                if (debugFlag) {
                    debug(PROGRAM, 2, "found desc = " + desc);
                }

                continue;
            }

            if (containsToken(buffer, "ItemExec")) {
                exec = valueAfterEquals(buffer);

                if (debugFlag) {
                    debug(PROGRAM, 2, "found exec = " + exec);
            if ( ! getItem ) {
                i = buffer.find("ItemName");
                if ( i != std::string::npos ) {
                    i = buffer.find("=");
                    name = buffer.substr( i+1, buffer.length() - i - 1);
                    while (name.substr(0,1).compare(" ") == 0)
                    {
                        name = name.substr(1);
                    }
					getItem = true;
                    if (debugFlag) debug(PROGRAM, 2, "found item = " + name);
					continue;
				}
			} else {
                i = buffer.find("ItemDesc");
                if ( i != std::string::npos ) {
                    i = buffer.find("=");
                    desc = buffer.substr( i+1, buffer.length() - i - 1);
                    while (desc.substr(0,1).compare(" ") == 0)
                    {
                        desc = desc.substr(1);
                    }
                    if (debugFlag) debug(PROGRAM, 2, "found desc = " + desc);
					continue;
				} else {
                    i = buffer.find("ItemExec");
                    if (i != std::string::npos) {
                        i = buffer.find("=");
                        exec = buffer.substr(i+1, buffer.length() - i - 1);
                        while (exec.substr(0,1).compare(" ") == 0)
                        {
                            exec = exec.substr(1);
                        }
                       

                    if (debugFlag) debug(PROGRAM, 2, "found exec = " + exec);
                    // Parse out the exec and look for sub menu to parse
                    i = exec.find("MenuSub ");
                    if (i != std::string::npos)
                    {
                        std::string subMenuConfigFile = exec.substr(8);
                        while (!subMenuConfigFile.empty()
                            && subMenuConfigFile.front() == ' ') {
                            subMenuConfigFile.erase(0, 1);
                        }

                        subMenuConfigFile =
                            resolveConfigPath(
                                subMenuConfigFile,
                                resolvedConfigPathObj);

                        tmp_menus = loadConfig(subMenuConfigFile, debugFlag);
                        menus.insert(menus.end(), tmp_menus.begin(), tmp_menus.end());
                        tmp_menus.clear();
                    }
					    continue;
					} else {
                        i = buffer.find("ItemEnd");
                        if ( i != std::string::npos ) {
                            getItem = false;
                            /* store name, desc, and exec in item object */
                            curMenu->addItem(CursedMenuItem(name,desc,exec));
      
                            name = "";
                            desc = "";
                            exec = "";

                            continue;
                        }
                    }
                }

                const auto submenuPosition = exec.find("MenuSub ");

                if (submenuPosition != std::string::npos) {
                    std::vector<CursedMenu> subMenus =
                        loadConfig(
                            exec.substr(submenuPosition + 8),
                            debugFlag);

                    menus.insert(
                        menus.end(),
                        subMenus.begin(),
                        subMenus.end());
                }

                continue;
            }

            if (containsToken(buffer, "ItemEnd")) {
                getItem = false;
                currentMenu.addItem(CursedMenuItem(name, desc, exec));

                name.clear();
                desc.clear();
                exec.clear();

                continue;
            }
        }
    }

    menus.push_back(currentMenu);

    if (debugFlag) {
        debug(PROGRAM, 2, "close config file");
    } else {
        std::cerr << "Unable to open file: " << resolvedConfigFile << std::endl;
    }

    return menus;
}
