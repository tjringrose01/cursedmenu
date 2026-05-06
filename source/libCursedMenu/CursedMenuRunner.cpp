#include "CursedMenuRunner.hpp"

#include <iostream>

#include <curses.h>
#include <menu.h>

#include "debug.hpp"

namespace cursedmenu {

namespace {

#define PROGRAM "cursedmenu"

void dispDesc(WINDOW* win, std::string desc, int pos_x, int pos_y) {
    for (int y = 1; y < pos_x - 1; y++) {
        mvwprintw(win, pos_y - 2, y, " ");
    }

    mvwprintw(win, pos_y - 2, 1, "%s", desc.c_str());
}

int xCtr(std::string str, int width) {
    return static_cast<int>((width - str.length()) / 2);
}

void dispMenuTitle(CursedMenu mc, WINDOW* menu_window) {
    std::string title = mc.getMenuTitle();

    if (title.length() == 0) {
        title = "Cursed Menu";
    }

    unsigned int centerX = xCtr(title, COLS);
    unsigned int titleLen = title.length();

    mvwaddch(menu_window, 2, centerX - 2, ACS_ULCORNER);

    wmove(menu_window, 2, centerX - 1);
    whline(menu_window, ACS_HLINE, titleLen + 2);

    mvwaddch(menu_window, 2, centerX + titleLen + 1, ACS_URCORNER);
    mvwaddch(menu_window, 3, centerX + titleLen + 1, ACS_VLINE);
    mvwaddch(menu_window, 4, centerX + titleLen + 1, ACS_LRCORNER);
    mvwaddch(menu_window, 3, centerX - 2, ACS_VLINE);
    mvwaddch(menu_window, 4, centerX - 2, ACS_LLCORNER);

    wmove(menu_window, 4, centerX - 1);
    whline(menu_window, ACS_HLINE, titleLen + 2);

    mvwprintw(menu_window, 3, centerX, "%s", title.c_str());
}

void clearScreen(WINDOW* win, int lines, int cols) {
    for (int x = 1; x < lines - 1; x++) {
        for (int y = 1; y < cols - 1; y++) {
            mvwprintw(win, x, y, " ");
        }
    }
}

void loadMenuColor(CursedMenu* menu) {
    init_pair(1, menu->getForeColor(), menu->getBackColor());
}

void loadCurses(CursedMenu* menu) {
    initscr();

    start_color();

    loadMenuColor(menu);

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void refreshWin(WINDOW* win, MENU* menu) {
    pos_menu_cursor(menu);
    refresh();
    wrefresh(win);
}

void runMenu(
    ActionLogger& actionLogger,
    std::stack<CursedMenu>* menus) {
    actionLogger.logMenu(COMING, menus->top().getMenuTitle());

    bool debugIsOn = actionLogger.getDebugMode();

    std::string selName;

    int menuCenter;
    int userInput;
    int retVal;

    MENU* curses_menu;
    WINDOW* menu_window;
    ITEM** menu_items;

    CursedMenu currentMenu = menus->top();

    loadCurses(&currentMenu);

    menu_window = newwin(LINES, COLS, 0, 0);

    wattron(menu_window, COLOR_PAIR(1));
    box(menu_window, 0, 0);
    keypad(menu_window, TRUE);

    clearScreen(menu_window, LINES, COLS);

    dispMenuTitle(currentMenu, menu_window);

    menu_items = new ITEM*[currentMenu.getNumOfItems() + 1];

    for (int i = 0; i < currentMenu.getNumOfItems(); i++) {
        menu_items[i] = new_item(
            currentMenu.getItem(i).getName().c_str(),
            currentMenu.getItem(i).getDesc().c_str());
    }

    menu_items[currentMenu.getNumOfItems()] = NULL;

    curses_menu = new_menu((ITEM**)menu_items);

    set_menu_format(curses_menu, LINES - 2, 0);

    menu_opts_on(curses_menu, O_ROWMAJOR);
    menu_opts_off(curses_menu, O_SHOWDESC);
    menu_opts_off(curses_menu, O_NONCYCLIC);

    set_menu_win(curses_menu, menu_window);

    menuCenter =
        (COLS / 2) - (currentMenu.getMenuCenterX() / 2);

    set_menu_sub(
        curses_menu,
        derwin(menu_window, 0, 0, 6, menuCenter));

    set_menu_fore(curses_menu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_back(curses_menu, COLOR_PAIR(1));

    post_menu(curses_menu);

    dispDesc(
        menu_window,
        item_description(current_item(curses_menu)),
        COLS,
        LINES);

    refreshWin(menu_window, curses_menu);

    while ((userInput = getch())) {
        switch (userInput) {
            case KEY_DOWN:
                menu_driver(curses_menu, REQ_DOWN_ITEM);
                break;

            case KEY_UP:
                menu_driver(curses_menu, REQ_UP_ITEM);
                break;

            case 10:
                selName = item_name(current_item(curses_menu));

                if (currentMenu.getItem(selName).getExec() == "MenuExit") {
                    unpost_menu(curses_menu);
                    free_menu(curses_menu);
                    endwin();

                    actionLogger.logMenu(
                        GOING,
                        menus->top().getMenuTitle());

                    return;
                }

                if (menus->top()
                        .getItem(selName)
                        .getExec()
                        .find("MenuSub ")
                    != std::string::npos) {
                    std::string temp =
                        currentMenu.getItem(selName).getExec();

                    std::string subMenuFile = temp.substr(8);

                    menus->push(CursedMenu(debugIsOn, subMenuFile));

                    currentMenu = menus->top();

                    runMenu(actionLogger, menus);

                    menus->pop();
                    currentMenu = menus->top();

                    loadMenuColor(&menus->top());

                    clearScreen(menu_window, LINES, COLS);

                    dispMenuTitle(menus->top(), menu_window);

                    menu_driver(curses_menu, REQ_UP_ITEM);
                    menu_driver(curses_menu, REQ_DOWN_ITEM);

                    pos_menu_cursor(curses_menu);

                    wrefresh(menu_window);
                } else {
                    endwin();

                    actionLogger.logCmd(
                        menus->top().getItem(selName).getExec());

                    retVal = system(
                        menus->top()
                            .getItem(selName)
                            .getExec()
                            .c_str());

                    if (debugIsOn || retVal != 0) {
                        std::cerr
                            << "Press <ENTER> to continue..."
                            << std::endl;

                        getch();
                    }

                    loadCurses(&currentMenu);

                    refresh();
                    wrefresh(menu_window);
                }

                break;
        }

        dispDesc(
            menu_window,
            item_description(current_item(curses_menu)),
            COLS,
            LINES);

        refreshWin(menu_window, curses_menu);
    }

    unpost_menu(curses_menu);

    for (int i = 0; i < currentMenu.getNumOfItems(); ++i) {
        free_item(menu_items[i]);
    }

    free_menu(curses_menu);
    endwin();
}

} // namespace

CursedMenuRunner::CursedMenuRunner(ActionLogger& actionLogger)
    : actionLogger(actionLogger) {
}

void CursedMenuRunner::run(std::stack<CursedMenu>& menus) {
    runMenu(actionLogger, &menus);
}

} // namespace cursedmenu
