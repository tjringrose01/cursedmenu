/**
 *  main.cpp - Menu Program using ncurses
 *
 *  Program Flow:
 *    1. Load the initial menu into the array
 *    2. Recursivly use runMenu()
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

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <curses.h>
#include <menu.h>
#include "CursedMenu.hh"
#include "debug.hh"
#include "ActionLogger.hh"
#include "CursedMenuLoader.h"

#define PROGRAM "cursedmenu"

/***************************************************************************
 * Function to display the proper program usage.
 ***************************************************************************/
void displayUsage() {
   std::cout << PACKAGE_NAME << " - " << PACKAGE_STRING << std::endl
        << std::endl
        << "Copyright 2007, 2008 Timothy Ringrose" << std::endl
        << std::endl
        << PACKAGE_NAME << " is free software: you can redistribute it and/or modify" << std::endl
        << "it under the terms of the GNU General Public License as published by" << std::endl
        << "the Free Software Foundation, either version 3 of the License, or" << std::endl
        << "(at your option) any later version." << std::endl
        << std::endl
        << PACKAGE_NAME << " is distributed in the hope that it will be useful," << std::endl
        << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl
        << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl
        << "GNU General Public License for more details." << std::endl
        << std::endl
        << "You should have received a copy of the GNU General Public License" << std::endl
        << "along with " << PACKAGE_NAME << ".  If not, see <http://www.gnu.org/licenses/>." << std::endl
        << std::endl
        << "usage: " << PACKAGE_NAME << "[-h or --help] | [-m <menu file>] [-c]" << std::endl
        << std::endl
        << "options:" << std::endl
        << "  -h or --help    - show program usage" << std::endl
        << "  -m <menu file>  - specify the cursed menu definition" << std::endl
        << "  -c              - check all menu definition files then validate" << std::endl
        << "                    and display all menu and menu item syntax" << std::endl
        << std::endl;
}

/**
 * Function to make sure the the command line arguments are correct
 */
int parse_args( int argc,
             char** argv,
            std::string* mVal,
              bool* cVal) {
 
    std::string arg;

    for ( int x = 1; x < argc; x++) {
        if (argv[x][0] == '-') {
            arg = argv[x];
            if (arg.compare("--help") == 0
            ||  arg.compare("-h")     == 0)
            { 
                displayUsage();
                exit(SUCCESS);
            }
            switch (argv[x][1]) {
                case 'm':   *mVal = argv[++x];
                    break;
                case 'c':   *cVal = true;
                    break;
                default:
                    std::cerr << "Error: Invalid usage" << std::endl
                         << "Unknown option \'" << arg << "\'" << std::endl;
                    displayUsage();
                    exit(ERROR_INVALID_USAGE);
            }
        }
    }

    // Check for values
    if ( mVal->empty() ) {
        std::cerr << "Error: Invalid argument value for option '-m'" << std::endl;
        displayUsage();
        exit(ERROR_INVALID_ARG_VALUES);
    }

    return(SUCCESS);
}

/**
 * Function to display the description at the bottom of the window
 */
void dispDesc(WINDOW* win, std::string desc, int pos_x, int pos_y) {
    /* Fill in all the emppty space with color */
    for ( int y = 1; y < pos_x - 1; y++ )
        mvwprintw(win, pos_y - 2, y, " ");

    mvwprintw(win, pos_y - 2, 1, desc.c_str());
}

/**
 * Return centering x position for display
 */
int xCtr( std::string str, int width ) { return((int)((width-str.length())/2)); }

/**
 * Function to display the menu title at the top of the screen.
 */
void dispMenuTitle(CursedMenu mc, WINDOW* menu_window) {
    std::string title = mc.getMenuTitle();

    if ( title.length() == 0 )
        title = "Cursed Menu";

    unsigned int centerX = xCtr(title, COLS);
    unsigned int titleLen = title.length();

    // Top Left corner
    mvwaddch(menu_window, 2, centerX - 2, ACS_ULCORNER);

    // Top Line
    wmove(menu_window, 2, centerX - 1 );
    whline(menu_window, ACS_HLINE, titleLen + 2);

    // Top Right corner
    mvwaddch(menu_window, 2, centerX + titleLen + 1, ACS_URCORNER);

    // Right side
    mvwaddch(menu_window, 3, centerX + titleLen + 1, ACS_VLINE);

    // Bottom Right corner
    mvwaddch(menu_window, 4, centerX + titleLen + 1, ACS_LRCORNER);

    // Left side
    mvwaddch(menu_window, 3, centerX - 2, ACS_VLINE);

    // Bottom Left corner
    mvwaddch(menu_window, 4, centerX - 2, ACS_LLCORNER);

    // Bottom Line
    wmove(menu_window, 4, centerX - 1 );
    whline(menu_window, ACS_HLINE, titleLen + 2);

    // Title
    mvwprintw(menu_window, 3, centerX, title.c_str());

    return;
}

/**
 * Function to clear the screen
 */
void clearScreen(WINDOW* win, int LINES, int COLS) {
    // Fill in all the emppty space with color //
    for ( int x = 1; x < LINES - 1; x++ ) {
        for ( int y = 1; y < COLS - 1; y++ ) {
            mvwprintw(win, x, y, " ");
        }
    }
}

void loadMenuColor(CursedMenu* menu) {
    // Define colors
    init_pair(1, menu->getForeColor(), menu->getBackColor());
}

void loadCurses(CursedMenu* menu) {
    // Initialize curses
    initscr();

    // Initialize color
    start_color();

    // Set colors
    loadMenuColor(menu);

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

}

void refreshWin(WINDOW* win, MENU* menu) {
    pos_menu_cursor(menu);
    refresh();
    wrefresh(win);
    return;
}

void runMenu(ActionLogger* al, std::stack<CursedMenu>* menus ) {
    //std::string logEntry = "entering menu: ";
    //logEntry += menus->top().getMenuTitle().c_str();
    al->logMenu(COMING, menus->top().getMenuTitle());

    bool debugIsOn = al->getDebugMode();
    std::string selName;

    int menuCenter;
    int userInput;
    int retVal;

    MENU* curses_menu;
    WINDOW* menu_window;
    ITEM** menu_items;

    CursedMenu currentMenu = menus->top();

    //
    // Initialize curses
    //
    loadCurses(&currentMenu);

    /* Create window and associate menu wit it */
    menu_window = newwin(LINES, COLS, 0, 0);

    wattron(menu_window, COLOR_PAIR(1));
    box(menu_window, 0, 0);
    keypad(menu_window, TRUE);

    //
    // Fill in all the empty space with color
    //
    clearScreen(menu_window, LINES, COLS);

    //
    // Paint the menu header
    //
    dispMenuTitle(currentMenu, menu_window);
    
    /**************************************/
    /* Load the items into curses members */
    /**************************************/

    /* Allocate memory space for array */
    menu_items = new ITEM*[currentMenu.getNumOfItems() + 1];

    if (debugIsOn) debug(PROGRAM, 0, "Loading menu items into curses:");
    /* Load the items into the menu_items array */
    for(int i = 0; i < currentMenu.getNumOfItems(); i++) {
        if (debugIsOn) debug(PROGRAM, 2, "menu item: " + currentMenu.getItem(i).getName());

        menu_items[i] = new_item( currentMenu.getItem(i).getName().c_str(), currentMenu.getItem(i).getDesc().c_str() );
    }

    menu_items[currentMenu.getNumOfItems()] = NULL;
    if (debugIsOn) debug(PROGRAM, 0, "All menu_items loaded");

    if (debugIsOn) debug(PROGRAM, 0, "ncurses API calls:");

    /* Create menu */
    curses_menu = new_menu((ITEM **)menu_items);
    if (debugIsOn) debug(PROGRAM, 2, "new_menu()");

    /* Make a menu to fit the menu window */
    set_menu_format(curses_menu, LINES - 2, 0);
    if (debugIsOn) debug(PROGRAM, 2, "set_menu_format()");

    /* Set the menu option bits */
    menu_opts_on(curses_menu, O_ROWMAJOR);
    menu_opts_off(curses_menu, O_SHOWDESC);
    menu_opts_off(curses_menu, O_NONCYCLIC);

    /* Assign the menu to the menu_window */
    set_menu_win(curses_menu, menu_window);
    if (debugIsOn) debug(PROGRAM, 2, "set_menu_win()");

    menuCenter = ( COLS / 2 ) - ( currentMenu.getMenuCenterX() / 2 );
    set_menu_sub(curses_menu, derwin(menu_window, 0, 0, 6, menuCenter ));
    if (debugIsOn) debug(PROGRAM, 2, "set_menu_sub()");

    /* Set fore ground and back ground of the menu */
    set_menu_fore(curses_menu, COLOR_PAIR(1) | A_REVERSE);
    if (debugIsOn) debug(PROGRAM, 2, "set_menu_fore()");
    set_menu_back(curses_menu, COLOR_PAIR(1));
    if (debugIsOn) debug(PROGRAM, 2, "set_menu_back()");

    /* Post the menu */
    post_menu(curses_menu);
    if (debugIsOn) debug(PROGRAM, 2, "post_menu()");

    dispDesc(menu_window, item_description(current_item(curses_menu)), COLS, LINES);

    refreshWin(menu_window, curses_menu);
    while( ( userInput = getch() ) ) {
        switch( userInput ) {
        case KEY_DOWN:
            menu_driver(curses_menu, REQ_DOWN_ITEM);

            dispDesc(menu_window, item_description(current_item(curses_menu)), COLS, LINES);

            pos_menu_cursor(curses_menu);
            refresh();
            wrefresh(menu_window);
            break;
        case KEY_UP:
            menu_driver(curses_menu, REQ_UP_ITEM);

            dispDesc(menu_window, item_description(current_item(curses_menu)), COLS, LINES);

            pos_menu_cursor(curses_menu);
            refresh();
            wrefresh(menu_window);
            break;
        case 10:
            selName = item_name(current_item(curses_menu));

            // Check for MenuExit
            if ( currentMenu.getItem(selName).getExec() == "MenuExit" ) {
                if (debugIsOn) debug(PROGRAM, 0, "MenuExit - Time to cleanup");

                /* Free Resources */
                if (debugIsOn) debug(PROGRAM, 2, "unpost_menu()");
                unpost_menu(curses_menu);

                if (debugIsOn) debug(PROGRAM, 2, "free_menu");
                free_menu(curses_menu);

                if (debugIsOn) debug(PROGRAM, 2, "endwin()");
                endwin();

                //std::string logEntry = "leaving menu: ";
                //logEntry += menus->top().getMenuTitle().c_str();
                al->logMenu(GOING, menus->top().getMenuTitle());

                return;
            }

            // Check for MenuSub itentifier
            if ( menus->top().getItem(selName).getExec().find("MenuSub ") != std::string::npos ) {
                // Get filename to load
                int z = 0;
                std::string temp = currentMenu.getItem(selName).getExec();
                z = temp.find("MenuSub ");

                std::string subMenuFile = temp.substr(8+z);
 
                // Add new menu from file into vector
                menus->push(CursedMenu(debugIsOn, subMenuFile));
 
                // Update iterator
                currentMenu = menus->top();

                //
                // Run a sub-menu
                //
                runMenu(al, menus);

                // Remove sub-menu from the stack
                menus->pop();
                currentMenu = menus->top();

                // Reset colors to current menu. 
                loadMenuColor(&menus->top());

                //
                // Fill in all the empty space with color
                //
                clearScreen(menu_window, LINES, COLS);
            
                //
                // Paint the menu header
                //
                dispMenuTitle(menus->top(), menu_window);

                // temp hack to force the items redisplay.
                menu_driver(curses_menu, REQ_UP_ITEM);
                menu_driver(curses_menu, REQ_DOWN_ITEM);
                pos_menu_cursor(curses_menu);
                //refresh();
                wrefresh(menu_window);

            } else {

                /* Turn off curses and run the command */
                al->log("curses call: endwin()");
                endwin();

                al->logCmd(menus->top().getItem(selName).getExec());
                retVal = system( menus->top().getItem(selName).getExec().c_str() );
                std::string logmsg  = "command return code: ";
                       logmsg += (char)(retVal);
                al->log(logmsg);

                if ( retVal != 0 ) {
                    std::cerr << "Command String: \"" 
                         << menus->top().getItem(selName).getExec()
                         << "\"" << std::endl
                         << "Return code: " << retVal << std::endl << std::endl
                         << "Command execution error." << std::endl
                         << "Press <ENTER> to continue..." << std::endl;

                    userInput = getch();
                } else {
                    if (debugIsOn) {
                        std::cerr << "Command String: \"" 
                             << currentMenu.getItem(selName).getExec()
                             << "\"" << std::endl
                             << "Return code: " << retVal << std::endl
                             << std::endl
                             << "Command completed successfully." << std::endl
                             << "Press <ENTER> to continue..." << std::endl;
                        userInput = getch();
                    }
                }

                /* Turn curses back on and refresh the screen */
                loadCurses(&currentMenu);

                refresh();
                wrefresh(menu_window);

                break;
            }
        }
    }

    /* Free Resources */
    unpost_menu(curses_menu);
    for(int i = 0; i < currentMenu.getNumOfItems(); ++i)
        free_item(menu_items[i]);
    free_menu(curses_menu);
    endwin();
}

/**
 * Beginning of Program Execution
 */
int main( int argc, char** argv ) {

    ActionLogger* log = new ActionLogger();
    log->log("curmenu begin");

    std::stack<CursedMenu> menus;
    CursedMenu currentMenu;
 
    std::string menuFile = "default.cmd";
    bool performMenuCheck = false;
    bool debugIsOn = false;
    std::string buffer;

    // Read arguments from command line //
    if ( parse_args(argc,argv,&menuFile,&performMenuCheck) != 0 ) {
        std::cerr << "Error: Invalid usage" << std::endl;
        displayUsage();
        return(ERROR_INVALID_USAGE);
    }

    //
    // There are two paths of execution based on command line arguments
    //   1) Perform a menu configuration check
    //   2) Run the menu
    //
    if (performMenuCheck)
    {
        std::vector<CursedMenu> menus = CursedMenuLoader::loadConfig(menuFile, debugIsOn);

        for (int itr=0; itr < menus.size(); itr++)
        {
            std::cout << menus.at(itr).toString() << std::endl;
        }

        std::cout << "Number of menus correctly parsed: " << menus.size() << std::endl;

    } else {

        // Read in the items from the menu file
        menus.push(CursedMenu(debugIsOn, menuFile));

        // If we don't have any items in the menu, we've got a problem
        if ( menus.top().getNumOfItems() == 0 ) {
            std::cerr << "No menu found..." << std::endl;
            exit(0);
        }

        //
        // Start the menu loops
        //
        runMenu(log, &menus);
    }

    return(0);
}
