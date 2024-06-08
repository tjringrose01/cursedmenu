/**
 *  ActionLogger.cpp - ActionLogger Class Implementation
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

#include <fstream>
#include <time.h>
#include <stdio.h>
#include "ActionLogger.hpp"

std::string ActionLogger::getSysTime() {
    char * timeStr = new char[25];
    struct tm *ptr;
    time_t tm;

    tm = time(NULL);
    ptr = localtime(&tm);

    strftime(timeStr, 25, "%Y/%m/%d %H:%M:%S", ptr);
    
    std::string returnStr = timeStr;
    return(returnStr);
}

std::string ActionLogger::getUserId() {
    std::string logname = getenv("LOGNAME");
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
    this->logFile = PACKAGE_NAME;
    this->logFile += ".log";
    this->userId = getUserId();
    this->pid = getPid();
}

ActionLogger::ActionLogger(bool debugMode, std::string logFile) {
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
void ActionLogger::log(std::string logText) {
    std::ofstream logf(logFile.c_str(), std::ios::app);

    logf << getSysTime() << " " << pid << " " << userId << ": " << logText
         << std::endl;

    logf.close();

    return;
}

std::string ActionLogger::toString() {
    std::string returnStr = "";
    return(returnStr);
}

void ActionLogger::logCmd(std::string logText) {
    std::ofstream logf(logFile.c_str(), std::ios::app);

    logf << getSysTime() << " " << pid << " " << userId << ": " 
         << "command ran: ["
         << logText
         << "]"
         << std::endl;

    logf.close();
    return;
}

void ActionLogger::logMenu(bool coming, std::string menuTitle) {
    std::ofstream logf(logFile.c_str(), std::ios::app);

    if ( coming )
        logf << getSysTime() << " " << pid << " " << userId << ": " 
             << "menu entered: ["
             << menuTitle
             << "]"
             << std::endl;
    else
        logf << getSysTime() << " " << pid << " " << userId << ": " 
             << "menu exited:  ["
             << menuTitle
             << "]"
             << std::endl;

    logf.close();
    return;
}
