/**
 *  MenuConfig.hh - MenuConfig Class Definition - This class is responsible
 *                  for reading in the configureation file and storing it's
 *                  informtion within it's class members.
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

#ifndef __MenuConfig__
#define __MenuConfig__

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MenuItem.hh"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

using namespace std;

typedef string String;

class MenuConfig {
private:
    /* Private Members */
    String menuName;
    String menuTitle;

    int foreMenuColor;
    int backMenuColor;

    vector<MenuItem> menuItems;
    bool debugFlag;

    /* Private Methods */
    void readConfig(String configFile);

public:
    /* Public Members */

    /* Public Methods */
    MenuConfig();
    MenuConfig(String configFile);
    MenuConfig(bool debugFlag, String configFile);
    ~MenuConfig();
    int getNumOfItems();
    String toString();
    MenuItem getItem( int index );
    MenuItem getItem( String name );
    void addItem( MenuItem item );
    void setDebugFlag(bool debugFlag);
    int getForeColor();
    int getBackColor();
    int getMenuCenterX();

    void setForeColor(int foreColor);
    void setBackColor(int backColor);

    String getMenuTitle();
    void setMenuTitle(String menuTitle);

    String getMenuName();
    void setMenuName(String menuName);
};

#endif
