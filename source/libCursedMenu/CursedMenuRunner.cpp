#include "CursedMenuRunner.hpp"

#include <exception>
#include <iostream>

#include <curses.h>
#include <menu.h>

#include "MenuRenderer.hpp"
#include "NcursesMenu.hpp"
#include "NcursesSession.hpp"
#include "NcursesWindow.hpp"
#include "RuntimeAction.hpp"

namespace cursedmenu {

namespace {

class NcursesCommandScope {
public:
    NcursesCommandScope(
        NcursesSession& session,
        const CursedMenu& menu)
        : session(session),
          menu(menu) {
        session.suspend();
    }

    ~NcursesCommandScope() noexcept {
        session.resume(menu);
    }

    NcursesCommandScope(const NcursesCommandScope&) = delete;
    NcursesCommandScope& operator=(const NcursesCommandScope&) = delete;

private:
    NcursesSession& session;
    const CursedMenu& menu;
};

void runMenu(
    ActionLogger& actionLogger,
    std::stack<CursedMenu>& menus) {
    actionLogger.logMenu(COMING, menus.top().getMenuTitle());

    bool debugIsOn = actionLogger.getDebugMode();

    std::string selName;

    int userInput;
    int retVal;

    CursedMenu currentMenu = menus.top();
    NcursesSession ncursesSession(currentMenu);

    NcursesWindow menuWindow(LINES, COLS, 0, 0);
    WINDOW* menu_window = menuWindow.get();
    MenuRenderer renderer(menu_window);

    renderer.initializeFrame();

    renderer.clearScreen(LINES, COLS);

    renderer.drawTitle(currentMenu);

    NcursesMenu cursesMenu(menu_window, currentMenu);
    cursesMenu.post();

    renderer.drawDescription(
        cursesMenu.currentItemDescription(),
        COLS,
        LINES);

    renderer.refresh(cursesMenu.get());

    while ((userInput = ncursesSession.readInput())) {
        switch (userInput) {
            case KEY_DOWN:
                cursesMenu.driver(REQ_DOWN_ITEM);
                break;

            case KEY_UP:
                cursesMenu.driver(REQ_UP_ITEM);
                break;

            case 10:
                selName = cursesMenu.currentItemName();

                const RuntimeAction action =
                    classifyRuntimeAction(
                        currentMenu.getItem(selName).getExec());

                if (action.type == RuntimeActionType::ExitMenu) {
                    actionLogger.logMenu(
                        GOING,
                        menus.top().getMenuTitle());

                    return;
                }

                if (action.type == RuntimeActionType::OpenSubmenu) {
                    menus.push(CursedMenu(debugIsOn, action.value));

                    currentMenu = menus.top();

                    runMenu(actionLogger, menus);

                    menus.pop();
                    currentMenu = menus.top();

                    ncursesSession.applyMenuColor(menus.top());

                    renderer.clearScreen(LINES, COLS);

                    renderer.drawTitle(menus.top());

                    cursesMenu.nudgeSelection();

                    renderer.refresh(cursesMenu.get());
                } else {
                    NcursesCommandScope commandScope(
                        ncursesSession,
                        currentMenu);

                    actionLogger.logCmd(
                        action.value);

                    retVal = system(
                        action.value.c_str());

                    if (debugIsOn || retVal != 0) {
                        std::cerr
                            << "Press <ENTER> to continue..."
                            << std::endl;

                        ncursesSession.waitForAcknowledge();
                    }

                    renderer.refresh(cursesMenu.get());
                }

                break;
        }

        renderer.drawDescription(
            cursesMenu.currentItemDescription(),
            COLS,
            LINES);

        renderer.refresh(cursesMenu.get());
    }
}

} // namespace

CursedMenuRunner::CursedMenuRunner(ActionLogger& actionLogger)
    : actionLogger(actionLogger) {
}

void CursedMenuRunner::run(std::stack<CursedMenu>& menus) {
    try {
        runMenu(actionLogger, menus);
    } catch (const std::exception& exception) {
        std::cerr
            << "Runtime error: " << exception.what()
            << std::endl;
        throw;
    } catch (...) {
        std::cerr
            << "Unknown runtime error in CursedMenuRunner"
            << std::endl;
        throw;
    }
}

} // namespace cursedmenu
