/**
 * MenuConfig.hh
 *
 * MenuConfig Class Definition - This class is responsible for reading in the 
 * configureation file and storing it's informtion within it's class members.
 *
 * Written by: Timothy Ringrose
 * Date:       08/07/2007
 */

#ifndef __MenuConfig__
#define __MenuConfig__

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MenuItem.hh"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

using namespace std;

typedef string String;

class MenuConfig {
private:
    /* Private Members */
    String menuName;
    String menuTitle;

    int foreMenuColor;
    int backMenuColor;

    vector<MenuItem> menuItems;
    bool debugFlag;

    /* Private Methods */
    void readConfig(String configFile);

public:
    /* Public Members */

    /* Public Methods */
    MenuConfig();
    MenuConfig(String configFile);
    MenuConfig(bool debugFlag, String configFile);
    ~MenuConfig();
    int getNumOfItems();
    String getMenuTitle();
    String toString();
    MenuItem getItem( int index );
    MenuItem getItem( String name );
    void addItem( MenuItem item );
    void setDebugFlag(bool debugFlag);
    int getForeColor();
    int getBackColor();
    int getMenuCenterX();
};

#endif
