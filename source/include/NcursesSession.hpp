#ifndef CURSEDMENU_NCURSES_SESSION_HPP
#define CURSEDMENU_NCURSES_SESSION_HPP

class CursedMenu;

namespace cursedmenu {

/**
 * RAII owner for ncurses global session state.
 */
class NcursesSession {
public:
    explicit NcursesSession(const CursedMenu& menu);
    ~NcursesSession() noexcept;

    NcursesSession(const NcursesSession&) = delete;
    NcursesSession& operator=(const NcursesSession&) = delete;

    void applyMenuColor(const CursedMenu& menu) const noexcept;
    void suspend() noexcept;
    void resume(const CursedMenu& menu) noexcept;
    int readInput() const noexcept;
    void waitForAcknowledge() const noexcept;

private:
    bool active;
};

} // namespace cursedmenu

#endif // CURSEDMENU_NCURSES_SESSION_HPP
