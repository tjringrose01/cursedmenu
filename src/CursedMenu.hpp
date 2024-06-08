/**
 *  CursedMenu.hpp - CursedMenu Class Definition - This class is responsible
 *                  for reading in the configureation file and storing it's
 *                  informtion within it's class members.
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

#ifndef __CursedMenu__
#define __CursedMenu__

#include <vector>
#include <string>
#include "CursedMenuItem.hpp"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

class CursedMenu {
private:
    /* Private Members */
    std::string menuName;
    std::string menuTitle;

    int foreMenuColor;
    int backMenuColor;

    std::vector<CursedMenuItem> menuItems;
    bool debugFlag;

    /* Private Methods */

public:
    /* Public Members */

    /* Public Methods */
    CursedMenu();
    CursedMenu(std::string configFile);
    CursedMenu(bool debugFlag, std::string configFile);
    ~CursedMenu();
    int getNumOfItems();
    std::string toString();
    CursedMenuItem getItem( int index );
    CursedMenuItem getItem( std::string name );
    void addItem( CursedMenuItem item );
    void setDebugFlag(bool debugFlag);
    int getForeColor();
    int getBackColor();
    int getMenuCenterX();

    void setForeColor(int foreColor);
    void setBackColor(int backColor);

    std::string getMenuTitle();
    void setMenuTitle(std::string menuTitle);

    std::string getMenuName();
    void setMenuName(std::string menuName);
    CursedMenu& operator=(const CursedMenu& cm);
};

#endif
