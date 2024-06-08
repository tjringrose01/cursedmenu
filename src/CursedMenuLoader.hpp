/**
 *  CursedMenuLoader.hpp - CursedMenuLoader Class Definition - This class is
 *                  responsible for reading in the configuration file and
 *                  returning it's informtion within class members.
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

#ifndef __CURSEDMENULOADER__
#define __CURSEDMENULOADER__

#include <vector>
#include <string>
#include "CursedMenu.hpp"

class CursedMenuLoader
{
    private:
        /* Private Members */

        /* Private Methods */

    public:
        /* Public Members */

        /* Public Methods */
        static CursedMenu load(const std::string configFile, const bool debugFlag);
        static std::vector<CursedMenu> loadConfig(const std::string configFile, const bool debugFlag);
};

#endif // __CURSEDMENULOADER__
