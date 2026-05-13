#include "MenuRenderer.hpp"

#include <curses.h>
#include <menu.h>

#include <algorithm>
#include <string>

#include "CursedMenu.hpp"

namespace cursedmenu {

namespace {

int xCtr(const std::string& str, const int width) {
    return static_cast<int>((width - str.length()) / 2);
}

} // namespace

MenuRenderer::MenuRenderer(WINDOW* window) noexcept
    : window(window) {
}

void MenuRenderer::initializeFrame() const noexcept {
    wattron(window, COLOR_PAIR(1));
    box(window, 0, 0);
    keypad(window, TRUE);
}

void MenuRenderer::clearScreen(const int lines, const int cols) const noexcept {
    for (int x = 1; x < lines - 1; ++x) {
        for (int y = 1; y < cols - 1; ++y) {
            mvwprintw(window, x, y, " ");
        }
    }
}

void MenuRenderer::drawTitle(const CursedMenu& menu) const {
    std::string title = menu.getMenuTitle();

    if (title.empty()) {
        title = "Cursed Menu";
    }

    const int titleLen = static_cast<int>(title.length());
    const int centerX = std::max(2, xCtr(title, COLS));
    const int titleWidth = std::max(0, std::min(titleLen + 2, COLS - centerX - 2));

    mvwaddch(window, 2, centerX - 2, ACS_ULCORNER);
    wmove(window, 2, centerX - 1);
    whline(window, ACS_HLINE, titleWidth);

    mvwaddch(window, 2, centerX + titleWidth - 1, ACS_URCORNER);
    mvwaddch(window, 3, centerX + titleWidth - 1, ACS_VLINE);
    mvwaddch(window, 4, centerX + titleWidth - 1, ACS_LRCORNER);
    mvwaddch(window, 3, centerX - 2, ACS_VLINE);
    mvwaddch(window, 4, centerX - 2, ACS_LLCORNER);

    wmove(window, 4, centerX - 1);
    whline(window, ACS_HLINE, titleWidth);

    if (titleWidth > 2) {
        mvwaddnstr(window, 3, centerX, title.c_str(), titleWidth - 2);
    }
}

void MenuRenderer::drawDescription(
    const char* description,
    const int cols,
    const int lines) const {
    const char* safeDescription =
        (description != nullptr) ? description : "";

    for (int y = 1; y < cols - 1; ++y) {
        mvwprintw(window, lines - 2, y, " ");
    }

    const int maxDescriptionWidth = std::max(0, cols - 2);
    mvwaddnstr(window, lines - 2, 1, safeDescription, maxDescriptionWidth);
}

void MenuRenderer::refresh(MENU* menu) const noexcept {
    pos_menu_cursor(menu);
    ::refresh();
    wrefresh(window);
}

} // namespace cursedmenu
