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

#include <ostream>
#include <sstream>
#include <string>

#include "CursedMenuItem.hpp"

CursedMenuItem::CursedMenuItem(
    const std::string& name,
    const std::string& desc,
    const std::string& exec)
    : name(name),
      description(desc),
      command(exec),
      postCommandPause(false) {
}

CursedMenuItem::CursedMenuItem(
    const std::string& name,
    const std::string& desc,
    const std::string& exec,
    const bool postCommandPause)
    : name(name),
      description(desc),
      command(exec),
      postCommandPause(postCommandPause) {
}

CursedMenuItem::~CursedMenuItem() {
}

const std::string& CursedMenuItem::getName() const {
    return name;
}

const std::string& CursedMenuItem::getDesc() const {
    return description;
}

const std::string& CursedMenuItem::getExec() const {
    return command;
}

bool CursedMenuItem::shouldPauseAfterCommand() const {
    return postCommandPause;
}

std::string CursedMenuItem::toString() const {
    std::ostringstream sout;

    sout
        << "Name:        \"" << name << "\""
        << std::endl
        << "Description: \"" << description << "\""
        << std::endl
        << "Command:     \"" << command << "\""
        << std::endl;

    return sout.str();
}
