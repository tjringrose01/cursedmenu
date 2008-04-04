# give any path to the source for sanity check
AC_INIT(COPYING)

# For automake
AM_INIT_AUTOMAKE(cursedmenu, 1.0.3)
AM_CONFIG_HEADER(config.h)

# check for programs (just use that is necessary)
#AC_PROG_CC # check for gcc
AC_PROG_CXX # check for g++
#AC_PROG_RANLIB
AC_PROG_INSTALL # check for install

### Check for stdc header files ###
AC_HEADER_STDC

### Find Curses Libraries ###
AC_SEARCH_LIBS(initscr, [curses], [], [AC_MSG_ERROR([no curses library found])], [])

AC_SEARCH_LIBS(new_menu, [menu], [], [AC_MSG_ERROR([no curses menu library found])], [])

AC_CHECK_HEADER([curses.h],,AC_MSG_ERROR([Error: Unable to find curses.h]))
AC_CHECK_HEADER([menu.h],,AC_MSG_ERROR([Error: Unable to find menu.h]))

# Output of configure. Usually location of the Makefile to be placed
AC_OUTPUT( Makefile src/Makefile )

AC_ARG_ENABLE([debug],
     [  --enable-debug    Turn on debugging],
     [debug=true],
     [debug=true])

if test "$debug" = "true"; then
    CPPFLAGS="$CPPFLAGS -g"
    CXXFLAGS="$CXXFLAGS -g"
fi
