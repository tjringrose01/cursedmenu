/**
 *  CursedMenu.hh - CursedMenu Class Definition - This class is responsible
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

#ifndef __CursedMenu__
#define __CursedMenu__

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CursedMenuItem.hh"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

using namespace std;

typedef string String;

class CursedMenu {
private:
    /* Private Members */
    String menuName;
    String menuTitle;

    int foreMenuColor;
    int backMenuColor;

    vector<CursedMenuItem> menuItems;
    bool debugFlag;

    /* Private Methods */

public:
    /* Public Members */

    /* Public Methods */
    CursedMenu();
    CursedMenu(String configFile);
    CursedMenu(bool debugFlag, String configFile);
    ~CursedMenu();
    int getNumOfItems();
    String toString();
    CursedMenuItem getItem( int index );
    CursedMenuItem getItem( String name );
    void addItem( CursedMenuItem item );
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
    CursedMenu& operator=(const CursedMenu& cm);
};

#endif
