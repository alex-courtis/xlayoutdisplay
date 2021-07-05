include config.mk

HDR = $(wildcard src/*.h)
SRC = $(wildcard src/*.cpp)
SRC_TEST = $(wildcard test/*.cpp)

OBJ = $(SRC:.cpp=.o)
OBJ_TEST = $(SRC_TEST:.cpp=.o)

all: xlayoutdisplay

$(OBJ): config.mk $(HDR)
$(OBJ_TEST): config.mk $(HDR)
main.o: config.mk $(HDR)

xlayoutdisplay: $(OBJ) main.o
	$(CXX) -o $@ main.o $(OBJ) $(LDFLAGS)

gtest: $(OBJ) $(OBJ_TEST)
	$(CXX) -o $@ $(OBJ) $(OBJ_TEST) $(LDFLAGS) $(LDFLAGS_TEST)
	./gtest

clean:
	rm -f xlayoutdisplay main.o $(OBJ) $(OBJ_TEST)

install: xlayoutdisplay
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f xlayoutdisplay $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/xlayoutdisplay

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/xlayoutdisplay

# https://github.com/alex-courtis/arch/blob/b530f331dacaaba27484593a87ca20a9f53ab73f/home/bin/ctags-something
ctags:
	ctags-c++ $(CPPFLAGS) $(HDR) $(SRC) $(SRC_TEST) main.cpp

.PHONY: all clean test install uninstall ctags

