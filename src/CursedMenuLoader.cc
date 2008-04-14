/**
 *  CursedMenuLoader.cc - CursedMenuLoader Class Implementation - This
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
 *         ii. When loadConfig() returns with the array of MenuConfig's, this
 *             is appended to the current array(which should be empty).
 *        iii. Then this instance of loadConfig() is returned with the
 *             appended array.
 *      c. When loadConfig() returns with the array of MenuConfig's, this
 *         is appended to the current array(We now have 1 menus).
 *      d. Then this instance of loadConfig() is returned with the
 *         appended array.
 *  3. When loadConfig() returns with the array of MenuConfig's, this
 *     is appended to the current array(We now have 2 menus).
 *  4. Then this instance of loadConfig() is returned with the
 *     appended array.
 *  5. The Main Menu completes it's processing and is added to the end
 *     of the vector.
 * 
 * ---------------------------------------------------------------------------
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
#include "CursedMenuLoader.h"
#include "MenuConfig.hh"
#include "CursedMenuItem.hh"
#include "debug.hh"

using namespace std;
typedef string String;

#define PROGRAM "CursedMenuLoader"

vector<MenuConfig> CursedMenuLoader::loadConfig(const String configFile, const bool debugFlag) {

    cout << "CursedMenuLoader::loadConfig(" << configFile << ", " << debugFlag << ");" << endl;
    vector<MenuConfig> menus;
    vector<MenuConfig> tmp_menus;

    MenuConfig* curMenu = new MenuConfig();

    //Removed for compatability on older g++ compilers
    //menus.clear();
    menus.reserve(25);

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
                    curMenu->setMenuTitle(buffer.substr(i+1, buffer.length()-i-1));
                    if (debugFlag)
                    {
                        debug(PROGRAM, 2, "menuTitle = " + curMenu->getMenuTitle());
                    }
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
 
                    if ( color == "BLACK" )
                        curMenu->setForeColor(COLOR_BLACK);
                    else if ( color == "RED" )
                        curMenu->setForeColor(COLOR_RED);
                    else if ( color == "GREEN" )
                        curMenu->setForeColor(COLOR_GREEN);
                    else if ( color == "BLUE" )
                        curMenu->setForeColor(COLOR_BLUE);
                    else if ( color == "YELLOW" )
                        curMenu->setForeColor(COLOR_YELLOW);
                    else if ( color == "MAGENTA" )
                        curMenu->setForeColor(COLOR_MAGENTA);
                    else if ( color == "CYAN" )
                        curMenu->setForeColor(COLOR_CYAN);
                    else if ( color == "WHITE" )
                        curMenu->setForeColor(COLOR_WHITE);
                    else
                        curMenu->setForeColor(COLOR_WHITE);
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

                    if ( color == "BLACK" )
                        curMenu->setBackColor(COLOR_BLACK);
                    else if ( color == "RED" )
                        curMenu->setBackColor(COLOR_RED);
                    else if ( color == "GREEN" )
                        curMenu->setBackColor(COLOR_GREEN);
                    else if ( color == "BLUE" )
                        curMenu->setBackColor(COLOR_BLUE);
                    else if ( color == "YELLOW" )
                        curMenu->setBackColor(COLOR_YELLOW);
                    else if ( color == "MAGENTA" )
                        curMenu->setBackColor(COLOR_MAGENTA);
                    else if ( color == "CYAN" )
                        curMenu->setBackColor(COLOR_CYAN);
                    else if ( color == "WHITE" )
                        curMenu->setBackColor(COLOR_WHITE);
                    else
                        curMenu->setBackColor(COLOR_BLACK);
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
                    // Parse out the exec and look for sub menu to parse
                    i = exec.find("MenuSub ");
                    if (i != String::npos)
                    {
                        tmp_menus = loadConfig(exec.substr(8), debugFlag);
                        menus.insert(menus.end(), tmp_menus.begin(), tmp_menus.end());
                        tmp_menus.clear();
                    }
					    continue;
					} else {
                        i = buffer.find("ItemEnd");
                        if ( i != String::npos ) {
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
            }
        }
        menus.push_back(*curMenu);

        file_in.close();
        if (debugFlag) debug(PROGRAM, 2, "close config file");

    } else {
        cerr << "Unable to open file: " << configFile << endl;
    }

    return(menus);
}
