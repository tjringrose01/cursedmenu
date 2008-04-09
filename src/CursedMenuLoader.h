/**
 *  CursedMenuLoader.h - CursedMenuLoader Class Definition - This class is
 *                  responsible for reading in the configuration file and
 *                  returning it's informtion within class members.
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

#ifndef __CursedMenuLoader_h__
#define __CursedMenuLoader_h__

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MenuItem.hh"
#include "MenuConfig.hh"

using namespace std;

typedef string String;

class CursedMenuLoader {
private:
    /* Private Members */

    /* Private Methods */

public:
    /* Public Members */

    /* Public Methods */
    static vector<MenuConfig> loadConfig(const String configFile, const bool debugFlag);
};

#endif
