/**
 *  CursedMenu.cpp - CursedMenu Class Implementation - This class is
 *                  responsible for reading in the configuration file and
 *                  storing it's informtion within it's class members
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

#include <curses.h>

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "CursedMenu.hpp"
#include "CursedMenuItem.hpp"
#include "CursedMenuLoader.hpp"
#include "debug.hpp"

#define PROGRAM "CursedMenu"

CursedMenu::CursedMenu(
    const bool debugFlag,
    const std::string configFile) {
    setDebugFlag(debugFlag);

    CursedMenu tmpMenu =
        CursedMenuLoader::load(configFile, debugFlag);

    *this = tmpMenu;
}

CursedMenu::CursedMenu() {
    setDebugFlag(false);
}

CursedMenu::CursedMenu(const std::string configFile) {
    setDebugFlag(false);

    CursedMenu tmpMenu =
        CursedMenuLoader::load(configFile, debugFlag);

    *this = tmpMenu;
}

int CursedMenu::getNumOfItems() const {
    return static_cast<int>(menuItems.size());
}

CursedMenu::~CursedMenu() {
}

CursedMenuItem CursedMenu::getItem(const int index) const {
    if (index >= 0 &&
        static_cast<size_t>(index) < menuItems.size()) {
        return menuItems.at(index);
    }

    std::cerr << "Error: item out of bounds." << std::endl;

    return menuItems.back();
}

CursedMenuItem CursedMenu::getItem(
    const std::string& name) const {
    for (const auto& item : menuItems) {
        if (item.getName() == name) {
            return item;
        }
    }

    std::cerr << "Error: item out of bounds." << std::endl;

    return menuItems.back();
}

std::string CursedMenu::toString() const {
    if (debugFlag) {
        std::cerr
            << "Entered CursedMenu::toString()"
            << std::endl;
    }

    std::ostringstream sout;

    unsigned int itemCount = 0;

    sout
        << "Menu: =================================================="
        << std::endl;

    sout << "Title: " << menuTitle << std::endl;

    for (const auto& item : menuItems) {
        sout << "-----------------" << std::endl;
        sout << "Item Number: " << itemCount++ << std::endl;
        sout << item.toString();
    }

    sout
        << "========================================================"
        << std::endl;

    return sout.str();
}

void CursedMenu::addItem(const CursedMenuItem& item) {
    for (const auto& existingItem : menuItems) {
        if (existingItem.getName() == item.getName()) {
            return;
        }
    }

    menuItems.push_back(item);
}

void CursedMenu::setDebugFlag(const bool debugFlag) {
    this->debugFlag = debugFlag;
}

int CursedMenu::getForeColor() const {
    return foreMenuColor;
}

int CursedMenu::getBackColor() const {
    return backMenuColor;
}

int CursedMenu::getMenuCenterX() const {
    size_t lengthOfLongestItem = 0;

    for (const auto& item : menuItems) {
        if (item.getName().length() > lengthOfLongestItem) {
            lengthOfLongestItem = item.getName().length();
        }
    }

    return static_cast<int>(lengthOfLongestItem);
}

const std::string& CursedMenu::getMenuTitle() const {
    return menuTitle;
}

void CursedMenu::setMenuTitle(
    const std::string& menuTitle) {
    this->menuTitle = menuTitle;
}

const std::string& CursedMenu::getMenuName() const {
    return menuName;
}

void CursedMenu::setMenuName(
    const std::string& menuName) {
    this->menuName = menuName;
}

void CursedMenu::setForeColor(const int foreColor) {
    this->foreMenuColor = foreColor;
}

void CursedMenu::setBackColor(const int backColor) {
    this->backMenuColor = backColor;
}

CursedMenu& CursedMenu::operator=(const CursedMenu& cm) {
    if (this != &cm) {
        menuName = cm.menuName;
        menuTitle = cm.menuTitle;
        foreMenuColor = cm.foreMenuColor;
        backMenuColor = cm.backMenuColor;
        debugFlag = cm.debugFlag;
        menuItems = cm.menuItems;
    }

    return *this;
}
