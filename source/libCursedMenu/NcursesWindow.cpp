#include "NcursesWindow.hpp"

#include <curses.h>

namespace cursedmenu {

NcursesWindow::NcursesWindow(
    const int lines,
    const int cols,
    const int y,
    const int x)
    : window(newwin(lines, cols, y, x)) {
}

NcursesWindow::~NcursesWindow() noexcept {
    if (window != nullptr) {
        delwin(window);
    }
}

WINDOW* NcursesWindow::get() const noexcept {
    return window;
}

} // namespace cursedmenu
