#include "NcursesSession.hpp"

#include <curses.h>

#include "CursedMenu.hpp"

namespace cursedmenu {

namespace {
constexpr int kInputPollMs = 1000;
}

NcursesSession::NcursesSession(const CursedMenu& menu)
    : active(false) {
    resume(menu);
}

NcursesSession::~NcursesSession() noexcept {
    suspend();
}

void NcursesSession::applyMenuColor(const CursedMenu& menu) const noexcept {
    if (!active) {
        return;
    }

    init_pair(1, menu.getForeColor(), menu.getBackColor());
}

void NcursesSession::suspend() noexcept {
    if (!active) {
        return;
    }

    endwin();
    active = false;
}

void NcursesSession::resume(const CursedMenu& menu) noexcept {
    if (!active) {
        initscr();
        start_color();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        timeout(kInputPollMs);
        active = true;
    }

    applyMenuColor(menu);
}

int NcursesSession::readInput() const noexcept {
    return getch();
}

void NcursesSession::waitForAcknowledge() const noexcept {
    timeout(-1);
    (void)getch();
    timeout(kInputPollMs);
}

} // namespace cursedmenu
