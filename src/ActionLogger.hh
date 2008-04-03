/**
 * ActionLogger.hh
 *
 * ActionLogger Class Definition - 
 *
 * Written by: Timothy Ringrose
 * Date:       01/29/2008
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
