#ifndef CURSEDMENU_NCURSES_WINDOW_HPP
#define CURSEDMENU_NCURSES_WINDOW_HPP

struct _win_st;
typedef _win_st WINDOW;

namespace cursedmenu {

/**
 * RAII owner for a WINDOW* created by ncurses.
 */
class NcursesWindow {
public:
    NcursesWindow(int lines, int cols, int y, int x);
    ~NcursesWindow() noexcept;

    NcursesWindow(const NcursesWindow&) = delete;
    NcursesWindow& operator=(const NcursesWindow&) = delete;

    WINDOW* get() const noexcept;

private:
    WINDOW* window;
};

} // namespace cursedmenu

#endif // CURSEDMENU_NCURSES_WINDOW_HPP
