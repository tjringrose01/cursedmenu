# give any path to the source for sanity check
AC_INIT([cursedmenu], [1.0.4], [tjringrose01@users.sourceforge.net])

# Verify that the src/ folder is available
AC_CONFIG_SRCDIR(src/cursedmenu.cc)

# For automake
AM_CONFIG_HEADER(config.h)

# check for programs (just use that is necessary)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AM_INIT_AUTOMAKE

# Use C++ compilers and linkers
AC_LANG_CPLUSPLUS

### Find Required Libraries ###
AC_SEARCH_LIBS(initscr, [curses], [], [AC_MSG_ERROR([no curses library found])], [])
AC_SEARCH_LIBS(new_menu, [menu], [], [AC_MSG_ERROR([no curses menu library found])], [])

### Check for stdc header files ###
AC_HEADER_STDC
AC_CHECK_HEADER([curses.h],,AC_MSG_ERROR([Error: Unable to find curses.h]))
AC_CHECK_HEADER([menu.h],,AC_MSG_ERROR([Error: Unable to find menu.h]))

CXXFLAGS=""

# Output of configure. Usually location of the Makefile to be placed
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])

AC_OUTPUT()
