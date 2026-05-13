#ifndef CURSEDMENU_MENU_RENDERER_HPP
#define CURSEDMENU_MENU_RENDERER_HPP

#include <ctime>
#include <string>

struct _win_st;
typedef _win_st WINDOW;
struct tagMENU;
typedef tagMENU MENU;

class CursedMenu;

namespace cursedmenu {

class MenuRenderer {
public:
    struct DateTimeParts {
        std::string date;
        std::string time;
    };

    explicit MenuRenderer(WINDOW* window) noexcept;

    void initializeFrame() const noexcept;
    void clearScreen(int lines, int cols) const noexcept;
    void drawTitle(const CursedMenu& menu) const;
    void drawDateTime(int cols, bool showDate, bool showTime) const;
    void drawDescription(const char* description, int cols, int lines) const;
    void refresh(MENU* menu) const noexcept;

    static DateTimeParts formatLocalDateTime(std::time_t now);

private:
    WINDOW* window;
};

} // namespace cursedmenu

#endif // CURSEDMENU_MENU_RENDERER_HPP
