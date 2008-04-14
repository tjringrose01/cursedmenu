/**
 * CursedMenuItem.cc - CursedMenuItem Class Implementation - This class is
 *                     responsible for storing an item that will be accessable
 *                     from the menu.
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

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CursedMenuItem.hh"

using namespace std;
typedef string String;

/* Private Members:
 *     String name;
 *     String description;
 *     String command;
 */

CursedMenuItem::CursedMenuItem(const String name, const String desc, const String exec) {
    this->name = name;
    this->description = desc;
    this->command = exec;
    this->postCommandPause = false;
}

CursedMenuItem::CursedMenuItem(const String name, const String desc, const String exec, const bool postCommandPause) {
    this->name = name;
    this->description = desc;
    this->command = exec;
    this->postCommandPause = postCommandPause;
}

CursedMenuItem::~CursedMenuItem() {}

String CursedMenuItem::getName() {
    return( this->name );
}

String CursedMenuItem::getDesc() {
    return( this->description);
}

String CursedMenuItem::getExec() {
    return( this->command);
}

String CursedMenuItem::toString() {
    ostringstream sout;
    sout << "Name:        \"" << name << "\"" << endl
         << "Description: \"" << description << "\"" << endl
         << "Command:     \"" << command << "\"" << endl;

    return(sout.str());
}
