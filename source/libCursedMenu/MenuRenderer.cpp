#include "MenuRenderer.hpp"

#include <curses.h>
#include <menu.h>

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
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

MenuRenderer::DateTimeParts MenuRenderer::formatLocalDateTime(
    const std::time_t now) {
    std::tm localTime {};
#if defined(_WIN32)
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    std::ostringstream dateStream;
    std::ostringstream timeStream;
    dateStream << std::put_time(&localTime, "%Y-%m-%d");
    timeStream << std::put_time(&localTime, "%I:%M:%S %p");
    return {dateStream.str(), timeStream.str()};
}

void MenuRenderer::drawDateTime(
    const int cols,
    const bool showDate,
    const bool showTime) const {
    if (cols <= 2) {
        return;
    }
    if (!showDate && !showTime) {
        return;
    }

    const DateTimeParts parts = formatLocalDateTime(std::time(nullptr));
    std::string fullText;
    if (showDate && showTime) {
        fullText = parts.date + " " + parts.time;
    } else if (showDate) {
        fullText = parts.date;
    } else {
        fullText = parts.time;
    }
    const int contentWidth = cols - 2;

    std::string displayText;
    if (static_cast<int>(fullText.size()) <= contentWidth) {
        displayText = fullText;
    } else if (showTime && static_cast<int>(parts.time.size()) <= contentWidth) {
        displayText = parts.time;
    } else if (showDate && static_cast<int>(parts.date.size()) <= contentWidth) {
        displayText = parts.date;
    } else {
        displayText.clear();
    }

    if (!displayText.empty()) {
        const int startCol = std::max(1, cols - 1 - static_cast<int>(displayText.size()));
        mvwaddnstr(
            window,
            LINES - 2,
            startCol,
            displayText.c_str(),
            cols - 1 - startCol);
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
