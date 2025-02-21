/**
 *  debug.cpp - Debugging related functions.
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

#include <iostream>
#include <string>
#include "debug.hpp"

void debug( std::string program, int indent, std::string message ) {
    std::cerr << program << ": ";
    for ( int i = 0; i < indent; i++ )
        std::cerr << " ";
    std::cerr << message << std::endl;
}
