#include "CursedMenuRunner.hpp"

#include <exception>
#include <iostream>
#include <utility>

#include <curses.h>
#include <menu.h>

#include "MenuRenderer.hpp"
#include "CursedMenuExceptions.hpp"
#include "NcursesMenu.hpp"
#include "NcursesSession.hpp"
#include "NcursesWindow.hpp"
#include "RuntimeAction.hpp"

namespace cursedmenu {

namespace {
bool containsUnsafeControlCharacter(const std::string& value) {
    for (unsigned char character : value) {
        if (character == '\n' || character == '\r' || character == '\0') {
            return true;
        }
    }
    return false;
}

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
    std::stack<CursedMenu>& menus,
    const std::function<std::optional<CursedMenu>(const std::string&)>& submenuResolver) {
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
    renderer.drawDateTime(COLS);

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
                    if (action.value.empty()) {
                        throw RuntimeException(
                            "Encountered empty submenu target in menu item: "
                            + selName);
                    }
                    if (containsUnsafeControlCharacter(action.value)) {
                        throw RuntimeException(
                            "Encountered unsafe submenu target in menu item: "
                            + selName);
                    }

                    if (submenuResolver) {
                        const auto resolvedSubmenu = submenuResolver(action.value);
                        if (!resolvedSubmenu.has_value()) {
                            throw RuntimeException(
                                "Unable to resolve submenu: " + action.value);
                        }
                        menus.push(resolvedSubmenu.value());
                    } else {
                        menus.push(CursedMenu(debugIsOn, action.value));
                    }

                    currentMenu = menus.top();

                    runMenu(actionLogger, menus, submenuResolver);

                    menus.pop();
                    currentMenu = menus.top();

                    ncursesSession.applyMenuColor(menus.top());

                    renderer.clearScreen(LINES, COLS);

                    renderer.drawTitle(menus.top());

                    cursesMenu.nudgeSelection();
                    renderer.drawDateTime(COLS);

                    renderer.refresh(cursesMenu.get());
                } else {
                    if (action.value.empty()) {
                        throw RuntimeException(
                            "Encountered empty command in menu item: "
                            + selName);
                    }
                    if (containsUnsafeControlCharacter(action.value)) {
                        throw RuntimeException(
                            "Encountered unsafe command in menu item: "
                            + selName);
                    }

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
        renderer.drawDateTime(COLS);

        renderer.refresh(cursesMenu.get());
    }
}

} // namespace

CursedMenuRunner::CursedMenuRunner(ActionLogger& actionLogger)
    : actionLogger(actionLogger) {
}

CursedMenuRunner::CursedMenuRunner(
    ActionLogger& actionLogger,
    std::function<std::optional<CursedMenu>(const std::string&)> submenuResolver)
    : actionLogger(actionLogger),
      submenuResolver(std::move(submenuResolver)) {
}

void CursedMenuRunner::run(std::stack<CursedMenu>& menus) {
    try {
        runMenu(actionLogger, menus, submenuResolver);
    } catch (const RuntimeException&) {
        throw;
    } catch (const std::exception& exception) {
        throw RuntimeException(
            std::string("CursedMenuRunner failure: ")
            + exception.what());
    } catch (...) {
        throw RuntimeException(
            "CursedMenuRunner failure: unknown exception");
    }
}

} // namespace cursedmenu
