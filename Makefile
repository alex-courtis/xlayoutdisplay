include config.mk

HDR = $(wildcard src/*.h)
SRC = $(wildcard src/*.cpp)
SRC_TEST = $(wildcard test/*.cpp)

OBJ = $(SRC:.cpp=.o)
OBJ_TEST = $(SRC_TEST:.cpp=.o)

all: xlayoutdisplay

$(OBJ): $(HDR) config.mk Makefile
main.o: $(HDR) config.mk Makefile

xlayoutdisplay: $(OBJ) main.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(@) $(^) $(LDLIBS)

$(OBJ_TEST): $(HDR) config.mk Makefile
	$(CXX) $(CXXFLAGS_TEST) $(CPPFLAGS) -c -o $(@) $(@:.o=.cpp)

gtest: $(OBJ) $(OBJ_TEST)
	$(CXX) $(CXXFLAGS_TEST) $(CPPFLAGS) -o $(@) $(^) $(LDLIBS) $(LDLIBS_TEST)
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
	ctags-c++ $(CPPFLAGS) --project-src $(HDR) $(SRC) $(SRC_TEST) main.cpp

.PHONY: all clean test install uninstall ctags

