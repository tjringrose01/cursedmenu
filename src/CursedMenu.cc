/**
 *  CursedMenu.cc - CursedMenu Class Implementation - This class is
 *                  responsible for reading in the configuration file and
 *                  storing it's informtion within it's class members
 *
 *  Copyright 2007, 2008 Timothy Ringrose
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

#include <vector>
#include <sstream>
#include <string>
#include <cctype> //std::toupper
#include <curses.h>
#include "CursedMenu.hh"
#include "CursedMenuItem.hh"
#include "CursedMenuLoader.h"
#include "debug.hh"

using namespace std;
typedef string String;

#define PROGRAM "CursedMenu"

CursedMenu::CursedMenu(const bool debugFlag, const String configFile) {
    setDebugFlag(debugFlag);
    CursedMenu tmpMenu = CursedMenuLoader::load(configFile, debugFlag);
    *this = tmpMenu;
}

CursedMenu::CursedMenu() {
    setDebugFlag(false);
}

CursedMenu::CursedMenu(const String configFile) {
    setDebugFlag(false);
    CursedMenu tmpMenu = CursedMenuLoader::load(configFile, debugFlag);
    *this = tmpMenu;
}

/**
 * Return the number of items in the config arrays.
 */
int CursedMenu::getNumOfItems() {
    return(menuItems.size());
}

/**
 * Housekeeping
 */
CursedMenu::~CursedMenu() {
}

CursedMenuItem CursedMenu::getItem( int index ) {
    int i = 0;

    vector<CursedMenuItem>::iterator it;

    for( it = menuItems.begin(); it != menuItems.end(); it++ ) {
        if ( index == i )
            return( *it );
        else 
            ++i;
    }

    cerr << "Error: item out of bounds." << endl;
    return( menuItems[menuItems.size() - 1] );
}

CursedMenuItem CursedMenu::getItem( String name ) {
    vector<CursedMenuItem>::iterator it;

    for( it = menuItems.begin(); it != menuItems.end(); it++ ) {
        if ( it->getName() == name )
            return( *it );
    }

    cerr << "Error: item out of bounds." << endl;
    return( menuItems[menuItems.size() - 1] );
}

/**
 * Method to try and show te object in string form.
 */
String CursedMenu::toString() {

    if (debugFlag) cerr << "Entered CursedMenu::toString()" << endl;
    ostringstream sout;

    unsigned int itemCount = 0;

    sout << "Menu: ==================================================" << endl;
    sout << "Title: " << menuTitle << endl;

    vector<CursedMenuItem>::iterator it;
   
    for( it = menuItems.begin(); it != menuItems.end(); it++ ) {
        sout << "-----------------" << endl;
        sout << "Item Number: " << itemCount++ << endl;
        sout << it->toString();
    }

    sout << "========================================================" << endl;

    return(sout.str());
}

/**
 * Method to add an additional item to the vector
 */
void CursedMenu::addItem( CursedMenuItem item ) {
    vector<CursedMenuItem>::iterator it;

    for( it = menuItems.begin(); it != menuItems.end(); it++ ) {
        if ( it->getName() == item.getName() ) {
            return;
        }
    }

    menuItems.push_back( item );
    return;
}

void CursedMenu::setDebugFlag(bool debugFlag) {
    this->debugFlag = debugFlag;
    return;
}

int CursedMenu::getForeColor() {
    return(foreMenuColor);
}

int CursedMenu::getBackColor() {
    return(backMenuColor);
}

int CursedMenu::getMenuCenterX() {
    vector<CursedMenuItem>::iterator it;
    unsigned int lengthOfLongestItem = 0;

    for( it = menuItems.begin(); it != menuItems.end(); it++ ) {
        if ( it->getName().length() > lengthOfLongestItem )
            lengthOfLongestItem = it->getName().length();
    }

    return(lengthOfLongestItem);
}

/**
 * Returns the title of this menu.
 */
String CursedMenu::getMenuTitle() {
    return(menuTitle);
}

/**
 * Sets class member menuTitle.
 */
void CursedMenu::setMenuTitle(String menuTitle) {
    this->menuTitle = menuTitle;
}

/**
 * Returns the name of this menu.
 */
String CursedMenu::getMenuName() {
    return(menuName);
}

/**
 * Sets class member menuName.
 */
void CursedMenu::setMenuName(String menuName) {
    this->menuName = menuName;
}

void CursedMenu::setForeColor(int foreColor) {
    this->foreMenuColor = foreColor;
}

void CursedMenu::setBackColor(int backColor) {
    this->backMenuColor = backColor;
}

CursedMenu& CursedMenu::operator=(const CursedMenu& cm) {
    if (this != &cm) // make sure not same object
    {
        menuName = cm.menuName;
        menuTitle = cm.menuTitle;
        foreMenuColor = cm.foreMenuColor;
        backMenuColor = cm.backMenuColor;
        debugFlag = cm.debugFlag;

        menuItems.clear();
        for (int x=0; x < cm.menuItems.size(); x++)
        {
            menuItems.push_back(cm.menuItems.at(x));
        }
    }
    return *this;    // Return ref for multiple assignment
}
