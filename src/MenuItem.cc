/**
 * MenuItem.cc
 *
 * MenuItem Class Implementation - This class is responsible for storing an item
 * that will be accessable from the menu.
 *
 * Written by: Timothy Ringrose
 * Date:       01/09/2008
 */

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MenuItem.hh"

using namespace std;
typedef string String;

/* Private Members:
 *     String name;
 *     String description;
 *     String command;
 */

MenuItem::MenuItem(const String name, const String desc, const String exec) {
    this->name = name;
    this->description = desc;
    this->command = exec;
    this->postCommandPause = false;
}

MenuItem::MenuItem(const String name, const String desc, const String exec, const bool postCommandPause) {
    this->name = name;
    this->description = desc;
    this->command = exec;
    this->postCommandPause = postCommandPause;
}

MenuItem::~MenuItem() {}

String MenuItem::getName() {
    return( this->name );
}

String MenuItem::getDesc() {
    return( this->description);
}

String MenuItem::getExec() {
    return( this->command);
}

String MenuItem::toString() {
    ostringstream sout;
    sout << "Name:        \"" << name << "\"" << endl
         << "Description: \"" << description << "\"" << endl
         << "Command:     \"" << command << "\"" << endl;

    return(sout.str());
}
