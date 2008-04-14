/**
 *  MenuConfig.cc - MenuConfig Class Implementation - This class is
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
#include "MenuConfig.hh"
#include "CursedMenuItem.hh"
#include "debug.hh"

using namespace std;
typedef string String;

#define PROGRAM "MenuConfig"

void MenuConfig::readConfig(const String configFile) {
    menuItems.reserve(25);

    //Removed for compatability on older g++ compilers
    //menuItems.clear();

    String buffer;
    String color;
    String temp_color;
    String name;
    String desc;
    String exec;

    bool getMenu = false;
    bool getTitl = false;
    bool getItem = false;

    ifstream file_in( configFile.c_str() );

    if ( file_in.is_open() ) {
        if (debugFlag) debug(PROGRAM, 0, "Reading config file - " + configFile);
        while (! file_in.eof() ) {
            getline ( file_in, buffer );

            if ( buffer.length() < 6 )
                continue;

            String::size_type i = buffer.find_first_not_of ( " \t\n\v" );

            if ( i != String::npos && buffer[i] == '#' )
                continue;

            /* Find MenuBegin */
            if ( ! getMenu ) {
                i = buffer.find("MenuBegin");
                if ( i != String::npos ) {
                    getMenu = true;
                    continue;
                }
            }

            i = buffer.find("MenuEnd");
            if ( i != String::npos ) {
                getMenu = false;
                continue;
            }

            if ( ! getTitl ) {
                i = buffer.find("MenuTitle");
                if ( i != String::npos ) {

                    i = buffer.find("=");
                    menuTitle = buffer.substr( i+1, buffer.length() - i - 1 );
                    if (debugFlag) debug(PROGRAM, 2, "menuTitle = " + menuTitle);
                    continue;
                }
            }

            /**
             * Colors defined in curses.h
             *
             *  COLOR_BLACK     0
             *  COLOR_RED       1
             *  COLOR_GREEN     2
             *  COLOR_YELLOW    3
             *  COLOR_BLUE      4
             *  COLOR_MAGENTA   5
             *  COLOR_CYAN      6
             *  COLOR_WHITE     7
             */

            i = buffer.find("MenuFore");
            if ( i != string::npos ) {
                //TODO read in color from config and relate to curses color

                color = buffer.substr( i+9, buffer.length() - i - 1 );

                if ( ! color.empty() ) {
                    //Using ineffecient toupper() in place of std::transform
                    // for older compiler compatability
                    temp_color = "";
                    for(int x=0; x < color.length(); x++) {
                        temp_color += (char)toupper((int)color[x]);
                    }
                    color = temp_color;
 
                    cerr << "Foreground color: \"" << color << "\"" << endl;

                    if ( color == "BLACK" )
                        foreMenuColor = COLOR_BLACK;
                    else if ( color == "RED" )
                        foreMenuColor = COLOR_RED;
                    else if ( color == "GREEN" )
                        foreMenuColor = COLOR_GREEN;
                    else if ( color == "BLUE" )
                        foreMenuColor = COLOR_BLUE;
                    else if ( color == "YELLOW" )
                        foreMenuColor = COLOR_YELLOW;
                    else if ( color == "MAGENTA" )
                        foreMenuColor = COLOR_MAGENTA;
                    else if ( color == "CYAN" )
                        foreMenuColor = COLOR_CYAN;
                    else if ( color == "WHITE" )
                        foreMenuColor = COLOR_WHITE;
                    else
                        foreMenuColor = COLOR_WHITE;
                }
                
            }

            i = buffer.find("MenuBack");
            if ( i != string::npos ) {
                //TODO read in color from config and relate to curses color
                color = buffer.substr( i+9, buffer.length() - i - 1 );

                if ( ! color.empty() ) {
                    //Using ineffecient toupper() in place of std::transform
                    // for older compiler compatability
                    temp_color = "";
                    for(int x=0; x < color.length(); x++) {
                        temp_color += (char)toupper((int)color[x]);
                    }
                    color = temp_color;

                    cerr << "Background color: \"" << color << "\"" << endl;

                    if ( color == "BLACK" )
                        backMenuColor = COLOR_BLACK;
                    else if ( color == "RED" )
                        backMenuColor = COLOR_RED;
                    else if ( color == "GREEN" )
                        backMenuColor = COLOR_GREEN;
                    else if ( color == "BLUE" )
                        backMenuColor = COLOR_BLUE;
                    else if ( color == "YELLOW" )
                        backMenuColor = COLOR_YELLOW;
                    else if ( color == "MAGENTA" )
                        backMenuColor = COLOR_MAGENTA;
                    else if ( color == "CYAN" )
                        backMenuColor = COLOR_CYAN;
                    else if ( color == "WHITE" )
                        backMenuColor = COLOR_WHITE;
                    else
                        backMenuColor = COLOR_BLACK;
                }
            }

            if ( ! getItem ) {
                i = buffer.find("ItemName");
                if ( i != String::npos ) {
                    i = buffer.find("=");
                    name = buffer.substr( i+1, buffer.length() - i - 1);
					getItem = true;
                    if (debugFlag) debug(PROGRAM, 2, "found item = " + name);
					continue;
				}
			} else {
                i = buffer.find("ItemDesc");
                if ( i != String::npos ) {
                    i = buffer.find("=");
                    desc = buffer.substr( i+1, buffer.length() - i - 1);
                    if (debugFlag) debug(PROGRAM, 2, "found desc = " + desc);
					continue;
				} else {
                    i = buffer.find("ItemExec");
                    if ( i != String::npos ) {
                        i = buffer.find("=");
                        exec = buffer.substr( i+1, buffer.length() - i - 1);
                    if (debugFlag) debug(PROGRAM, 2, "found exec = " + exec);
					    continue;
					} else {
                        i = buffer.find("ItemEnd");
                        if ( i != String::npos ) {
                            getItem = false;
                            /* store name, desc, and exec in item object */
                            //menuItems.push_back( CursedMenuItem(name,desc,exec) );
                            addItem( CursedMenuItem(name,desc,exec) );
      
                            name = "";
                            desc = "";
                            exec = "";

                            continue;
                        }
                    }
                }
            }
        }

        file_in.close();
        if (debugFlag) debug(PROGRAM, 2, "close config file");

    } else {
        cerr << "Unable to open file: " << configFile << endl;
    }
}

MenuConfig::MenuConfig(const bool debugFlag, const String configFile) {
    setDebugFlag(debugFlag);
    readConfig(configFile);
}

MenuConfig::MenuConfig() {
    setDebugFlag(false);
}

MenuConfig::MenuConfig(const String configFile) {
    setDebugFlag(false);
    readConfig(configFile);
}

/**
 * Return the number of items in the config arrays.
 */
int MenuConfig::getNumOfItems() {
    return(menuItems.size());
}

/**
 * Housekeeping
 */
MenuConfig::~MenuConfig() {
}

CursedMenuItem MenuConfig::getItem( int index ) {
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

CursedMenuItem MenuConfig::getItem( String name ) {
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
String MenuConfig::toString() {

    if (debugFlag) cerr << "Entered MenuConfig::toString()" << endl;
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
void MenuConfig::addItem( CursedMenuItem item ) {
    vector<CursedMenuItem>::iterator it;

    for( it = menuItems.begin(); it != menuItems.end(); it++ ) {
        if ( it->getName() == item.getName() ) {
            return;
        }
    }

    menuItems.push_back( item );
    return;
}

void MenuConfig::setDebugFlag(bool debugFlag) {
    this->debugFlag = debugFlag;
    return;
}

int MenuConfig::getForeColor() {
    return(foreMenuColor);
}

int MenuConfig::getBackColor() {
    return(backMenuColor);
}

int MenuConfig::getMenuCenterX() {
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
String MenuConfig::getMenuTitle() {
    return(menuTitle);
}

/**
 * Sets class member menuTitle.
 */
void MenuConfig::setMenuTitle(String menuTitle) {
    this->menuTitle = menuTitle;
}

/**
 * Returns the name of this menu.
 */
String MenuConfig::getMenuName() {
    return(menuName);
}

/**
 * Sets class member menuName.
 */
void MenuConfig::setMenuName(String menuName) {
    this->menuName = menuName;
}

void MenuConfig::setForeColor(int foreColor) {
    this->foreMenuColor = foreColor;
}

void MenuConfig::setBackColor(int backColor) {
    this->backMenuColor = backColor;
}
