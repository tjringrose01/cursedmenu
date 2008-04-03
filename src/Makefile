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

