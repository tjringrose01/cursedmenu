/**
 * MenuItem.hh
 *
 * MenuItem Class Definition - This class is responsible for storing an item
 * that will be accessable from the menu.
 *
 * Written by: Timothy Ringrose
 * Date:       01/09/2008
 */

#ifndef __MenuItem__
#define __MenuItem__

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
typedef string String;

class MenuItem {
private:
    /* Private Members */
    String name;
    String description;
    String command;
    bool   postCommandPause;

    /* Private Methods */

public:
    /* Public Members */

    /* Public Methods */
    MenuItem(const String name, const String description, const String command);
    MenuItem(const String name, const String description, const String command, const bool postCommandPause);
    ~MenuItem();
    String getName();
    String getDesc();
    String getExec();
    String toString();
};

#endif
