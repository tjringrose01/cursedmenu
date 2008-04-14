/**
 * CursedMenuItem.hh - CursedMenuItem Class Definition - This class is
 *                     responsible for storing an item that will be
 *                     accessable from the menu.
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

#ifndef __CursedMenuItem__
#define __CursedMenuItem__

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
typedef string String;

class CursedMenuItem {
private:
    /* Private Members */
    String name;
    String description;
    String command;
    bool   postCommandPause;

    /* Private Methods */

public:
    /* Public Members */

    /* Public Methods */
    CursedMenuItem(const String name, const String description, const String command);
    CursedMenuItem(const String name, const String description, const String command, const bool postCommandPause);
    ~CursedMenuItem();
    String getName();
    String getDesc();
    String getExec();
    String toString();
};

#endif
