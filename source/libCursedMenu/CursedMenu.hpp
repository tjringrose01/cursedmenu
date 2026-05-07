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

#ifndef __CURSEDMENU__
#define __CURSEDMENU__

#include <string>
#include <vector>

#include "CursedMenuItem.hpp"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

class CursedMenu {
private:
    std::string menuName;
    std::string menuTitle;

    int foreMenuColor;
    int backMenuColor;

    std::vector<CursedMenuItem> menuItems;
    bool debugFlag;

public:
    CursedMenu();
    explicit CursedMenu(const std::string& configFile);
    CursedMenu(bool debugFlag, const std::string& configFile);
    ~CursedMenu() noexcept = default;

    [[nodiscard]] int getNumOfItems() const noexcept;

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] const CursedMenuItem& getItem(int index) const;
    [[nodiscard]] const CursedMenuItem& getItem(const std::string& name) const;

    void addItem(const CursedMenuItem& item);

    void setDebugFlag(bool debugFlag) noexcept;

    [[nodiscard]] int getForeColor() const noexcept;
    [[nodiscard]] int getBackColor() const noexcept;
    [[nodiscard]] int getMenuCenterX() const noexcept;

    void setForeColor(int foreColor) noexcept;
    void setBackColor(int backColor) noexcept;

    [[nodiscard]] const std::string& getMenuTitle() const noexcept;
    void setMenuTitle(const std::string& menuTitle);

    [[nodiscard]] const std::string& getMenuName() const noexcept;
    void setMenuName(const std::string& menuName);

    CursedMenu& operator=(const CursedMenu& cm);
};

#endif // __CURSEDMENU__
