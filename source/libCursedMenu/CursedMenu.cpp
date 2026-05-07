/**
 *  CursedMenu.cpp - CursedMenu Class Implementation - This class is
 *                  responsible for reading in the configuration file and
 *                  storing it's informtion within it's class members
 */

#include <curses.h>

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "CursedMenu.hpp"
#include "CursedMenuExceptions.hpp"
#include "CursedMenuItem.hpp"
#include "CursedMenuLoader.hpp"
#include "debug.hpp"

#define PROGRAM "CursedMenu"

CursedMenu::CursedMenu(
    const bool debugFlag,
    const std::string& configFile) {
    setDebugFlag(debugFlag);

    CursedMenu tmpMenu =
        CursedMenuLoader::load(configFile, debugFlag);

    *this = tmpMenu;
}

CursedMenu::CursedMenu() {
    setDebugFlag(false);
}

CursedMenu::CursedMenu(const std::string& configFile) {
    setDebugFlag(false);

    CursedMenu tmpMenu =
        CursedMenuLoader::load(configFile, debugFlag);

    *this = tmpMenu;
}

int CursedMenu::getNumOfItems() const noexcept {
    return static_cast<int>(menuItems.size());
}

CursedMenuItem const& CursedMenu::getItem(const int index) const {
    if (index >= 0 &&
        static_cast<size_t>(index) < menuItems.size()) {
        return menuItems.at(index);
    }

    throw cursedmenu::RuntimeException(
        "Menu item index out of bounds: index="
        + std::to_string(index)
        + ", size="
        + std::to_string(menuItems.size()));
}

CursedMenuItem const& CursedMenu::getItem(
    const std::string& name) const {
    for (const auto& item : menuItems) {
        if (item.getName() == name) {
            return item;
        }
    }

    throw cursedmenu::RuntimeException(
        "Menu item not found: name=\"" + name + "\"");
}

std::string CursedMenu::toString() const {
    std::ostringstream sout;

    unsigned int itemCount = 0;

    sout << "Title: " << menuTitle << std::endl;

    for (const auto& item : menuItems) {
        sout << "Item Number: " << itemCount++ << std::endl;
        sout << item.toString();
    }

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

void CursedMenu::setDebugFlag(const bool debugFlag) noexcept {
    this->debugFlag = debugFlag;
}

int CursedMenu::getForeColor() const noexcept {
    return foreMenuColor;
}

int CursedMenu::getBackColor() const noexcept {
    return backMenuColor;
}

int CursedMenu::getMenuCenterX() const noexcept {
    size_t lengthOfLongestItem = 0;

    for (const auto& item : menuItems) {
        if (item.getName().length() > lengthOfLongestItem) {
            lengthOfLongestItem = item.getName().length();
        }
    }

    return static_cast<int>(lengthOfLongestItem);
}

const std::string& CursedMenu::getMenuTitle() const noexcept {
    return menuTitle;
}

void CursedMenu::setMenuTitle(
    const std::string& menuTitle) {
    this->menuTitle = menuTitle;
}

const std::string& CursedMenu::getMenuName() const noexcept {
    return menuName;
}

void CursedMenu::setMenuName(
    const std::string& menuName) {
    this->menuName = menuName;
}

void CursedMenu::setForeColor(const int foreColor) noexcept {
    this->foreMenuColor = foreColor;
}

void CursedMenu::setBackColor(const int backColor) noexcept {
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
