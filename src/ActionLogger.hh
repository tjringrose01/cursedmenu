/**
 *  ActionLogger.hh ActionLogger Class Definition
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

#ifndef __ActionLogger__
#define __ActionLogger__

#include <string>

#define COMING 1
#define GOING 0

using namespace std;
typedef string String;

class ActionLogger {
    private:
        bool debugMode;
        String userId;
        String logFile;
        int pid;
        String getSysTime();
        String getUserId();
        int getPid();

    public:
        ActionLogger();
        ActionLogger(bool debugMode, String logFile);
        bool debugModeIsOn();
        bool getDebugMode();
        void log(String logText);
        void logCmd(String logText);
        void logMenu(bool coming, String menuTitle);
        String toString();
};

#endif
