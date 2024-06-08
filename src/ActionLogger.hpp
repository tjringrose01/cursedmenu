/**
 *  ActionLogger.hpp ActionLogger Class Definition
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

#ifndef __ActionLogger__
#define __ActionLogger__

#include <string>

#define COMING 1
#define GOING 0

class ActionLogger {
    private:
        bool debugMode;
        std::string userId;
        std::string logFile;
        int pid;
        std::string getSysTime();
        std::string getUserId();
        int getPid();

    public:
        ActionLogger();
        ActionLogger(bool debugMode, std::string logFile);
        bool debugModeIsOn();
        bool getDebugMode();
        void log(std::string logText);
        void logCmd(std::string logText);
        void logMenu(bool coming, std::string menuTitle);
        std::string toString();
};

#endif
