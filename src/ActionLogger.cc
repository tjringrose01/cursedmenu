/**
 * ActionLogger.cc
 *
 * ActionLogger Class Implementation - 
 *
 * Written by: Timothy Ringrose
 * Date:       01/29/2008
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
