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

int CursedMenu::getNumOfItems() {
    return menuItems.size();
}

CursedMenu::~CursedMenu() {
}

CursedMenuItem CursedMenu::getItem(int index) {
    int i = 0;

    std::vector<CursedMenuItem>::iterator it;

    for (it = menuItems.begin(); it != menuItems.end(); it++) {
        if (index == i) {
            return *it;
        }

        ++i;
    }

    std::cerr << "Error: item out of bounds." << std::endl;

    return menuItems[menuItems.size() - 1];
}

CursedMenuItem CursedMenu::getItem(std::string name) {
    std::vector<CursedMenuItem>::iterator it;

    for (it = menuItems.begin(); it != menuItems.end(); it++) {
        if (it->getName() == name) {
            return *it;
        }
    }

    std::cerr << "Error: item out of bounds." << std::endl;

    return menuItems[menuItems.size() - 1];
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

    std::vector<CursedMenuItem>::const_iterator it;

    for (it = menuItems.begin(); it != menuItems.end(); it++) {
        sout << "-----------------" << std::endl;
        sout << "Item Number: " << itemCount++ << std::endl;
        sout << it->toString();
    }

    sout
        << "========================================================"
        << std::endl;

    return sout.str();
}

void CursedMenu::addItem(CursedMenuItem item) {
    std::vector<CursedMenuItem>::iterator it;

    for (it = menuItems.begin(); it != menuItems.end(); it++) {
        if (it->getName() == item.getName()) {
            return;
        }
    }

    menuItems.push_back(item);
}

void CursedMenu::setDebugFlag(bool debugFlag) {
    this->debugFlag = debugFlag;
}

int CursedMenu::getForeColor() {
    return foreMenuColor;
}

int CursedMenu::getBackColor() {
    return backMenuColor;
}

int CursedMenu::getMenuCenterX() {
    std::vector<CursedMenuItem>::iterator it;

    unsigned int lengthOfLongestItem = 0;

    for (it = menuItems.begin(); it != menuItems.end(); it++) {
        if (it->getName().length() > lengthOfLongestItem) {
            lengthOfLongestItem = it->getName().length();
        }
    }

    return lengthOfLongestItem;
}

std::string CursedMenu::getMenuTitle() {
    return menuTitle;
}

void CursedMenu::setMenuTitle(std::string menuTitle) {
    this->menuTitle = menuTitle;
}

std::string CursedMenu::getMenuName() {
    return menuName;
}

void CursedMenu::setMenuName(std::string menuName) {
    this->menuName = menuName;
}

void CursedMenu::setForeColor(int foreColor) {
    this->foreMenuColor = foreColor;
}

void CursedMenu::setBackColor(int backColor) {
    this->backMenuColor = backColor;
}

CursedMenu& CursedMenu::operator=(const CursedMenu& cm) {
    if (this != &cm) {
        menuName = cm.menuName;
        menuTitle = cm.menuTitle;
        foreMenuColor = cm.foreMenuColor;
        backMenuColor = cm.backMenuColor;
        debugFlag = cm.debugFlag;

        menuItems.clear();

        for (size_t x = 0; x < cm.menuItems.size(); x++) {
            menuItems.push_back(cm.menuItems.at(x));
        }
    }

    return *this;
}
