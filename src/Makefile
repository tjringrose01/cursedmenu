 ##
 #  Makefile - Makefile for the cursedmenu project.
 #
 #  Copyright 2007, 2008 Timothy Ringrose
 #
 #  This file is part of cursedmenu.
 #
 #  cursedmenu is free software: you can redistribute it and/or modify
 #  it under the terms of the GNU General Public License as published by
 #  the Free Software Foundation, either version 3 of the License, or
 #  (at your option) any later version.
 #
 #  cursedmenu is distributed in the hope that it will be useful,
 #  but WITHOUT ANY WARRANTY; without even the implied warranty of
 #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #  GNU General Public License for more details.
 #
 #  You should have received a copy of the GNU General Public License
 #  along with cursedmenu.  If not, see <http://www.gnu.org/licenses/>.
 #
 ##

SOURCE = ActionLogger.cc ActionLogger.hh cursedmenu.cc debug.cc debug.hh Makefile MenuConfig.cc MenuConfig.hh MenuItem.cc MenuItem.hh
OBJECT = cursedmenu.o ActionLogger.o debug.o MenuConfig.o MenuItem.o

BINARY = cursedmenu

LDFLAGS = -lmenu -lncurses
CXX = g++
CC = g++

$(BINARY):	$(OBJECT)

###$(OBJECT):	$(SOURCE)

clean:
	-rm -f $(BINARY) $(OBJECT)
