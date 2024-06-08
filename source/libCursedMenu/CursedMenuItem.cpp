/**
 * CursedMenuItem.cpp - CursedMenuItem Class Implementation - This class is
 *                     responsible for storing an item that will be accessable
 *                     from the menu.
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

#include <string>
#include <ostream>
#include <sstream>
#include "CursedMenuItem.hpp"

/* Private Members:
 *     std::string name;
 *     std::string description;
 *     std::string command;
 */

CursedMenuItem::CursedMenuItem(const std::string name, const std::string desc, const std::string exec) {
    this->name = name;
    this->description = desc;
    this->command = exec;
    this->postCommandPause = false;
}

CursedMenuItem::CursedMenuItem(const std::string name, const std::string desc, const std::string exec, const bool postCommandPause) {
    this->name = name;
    this->description = desc;
    this->command = exec;
    this->postCommandPause = postCommandPause;
}

CursedMenuItem::~CursedMenuItem() {}

std::string CursedMenuItem::getName() {
    return( this->name );
}

std::string CursedMenuItem::getDesc() {
    return( this->description);
}

std::string CursedMenuItem::getExec() {
    return( this->command);
}

std::string CursedMenuItem::toString() {
    std::ostringstream sout;
    sout << "Name:        \"" << name << "\"" << std::endl
         << "Description: \"" << description << "\"" << std::endl
         << "Command:     \"" << command << "\"" << std::endl;

    return(sout.str());
}
