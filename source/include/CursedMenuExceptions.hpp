#ifndef CURSEDMENU_EXCEPTIONS_HPP
#define CURSEDMENU_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace cursedmenu {

class CursedMenuException : public std::runtime_error {
public:
    explicit CursedMenuException(const std::string& message)
        : std::runtime_error(message) {
    }
};

class ParserException : public CursedMenuException {
public:
    explicit ParserException(const std::string& message)
        : CursedMenuException(message) {
    }
};

class ValidationException : public CursedMenuException {
public:
    explicit ValidationException(const std::string& message)
        : CursedMenuException(message) {
    }
};

class MenuLoadException : public CursedMenuException {
public:
    explicit MenuLoadException(const std::string& message)
        : CursedMenuException(message) {
    }
};

class RuntimeException : public CursedMenuException {
public:
    explicit RuntimeException(const std::string& message)
        : CursedMenuException(message) {
    }
};

} // namespace cursedmenu

#endif // CURSEDMENU_EXCEPTIONS_HPP
