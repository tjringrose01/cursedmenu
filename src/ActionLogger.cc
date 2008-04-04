/**
 *  ActionLogger.cc - ActionLogger Class Implementation
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

#include <fstream>
#include <string>
#include <time.h>
#include <stdio.h>
#include "ActionLogger.hh"

using namespace std;
typedef string String;

String ActionLogger::getSysTime() {
    char * timeStr = new char[25];
    struct tm *ptr;
    time_t tm;

    tm = time(NULL);
    ptr = localtime(&tm);

    strftime(timeStr, 25, "%Y/%m/%d %H:%M:%S", ptr);
    
    String returnStr = timeStr;
    return(returnStr);
}

String ActionLogger::getUserId() {
    String logname = getenv("LOGNAME");
    if ( logname.size() <= 0 )
        logname = "unknown";

    return(logname);
}

int ActionLogger::getPid() {
    return((int) getpid());
}

/**
 * Default constructor.
 */
ActionLogger::ActionLogger() {
    this->debugMode = false;
    this->logFile = "curmenu.log";
    this->userId = getUserId();
    this->pid = getPid();
}

ActionLogger::ActionLogger(bool debugMode, String logFile) {
    this->debugMode = debugMode;
    this->logFile = logFile;
    this->userId = getUserId();
    this->pid = getPid();
}

bool ActionLogger::debugModeIsOn() {
    return(debugMode);
}

bool ActionLogger::getDebugMode() {
    return(debugMode);
}

/**
 * Log straight text to logfile.
 * Note: For menu calls and program execution, please use logMnu and logCmd
 */
void ActionLogger::log(String logText) {
    ofstream logf(logFile.c_str(), ios::app);

    logf << getSysTime() << " " << pid << " " << userId << ": " << logText
         << endl;

    logf.close();

    return;
}

String ActionLogger::toString() {
    String returnStr = "";
    return(returnStr);
}

void ActionLogger::logCmd(String logText) {
    ofstream logf(logFile.c_str(), ios::app);

    logf << getSysTime() << " " << pid << " " << userId << ": " 
         << "command ran: ["
         << logText
         << "]"
         << endl;

    logf.close();
    return;
}

void ActionLogger::logMenu(bool coming, String menuTitle) {
    ofstream logf(logFile.c_str(), ios::app);

    if ( coming )
        logf << getSysTime() << " " << pid << " " << userId << ": " 
             << "menu entered: ["
             << menuTitle
             << "]"
             << endl;
    else
        logf << getSysTime() << " " << pid << " " << userId << ": " 
             << "menu exited:  ["
             << menuTitle
             << "]"
             << endl;

    logf.close();
    return;
}
