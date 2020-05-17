include config.mk

HDR = $(wildcard src/*.h)
SRC = main.cpp $(wildcard src/*.cpp)

OBJ = $(SRC:.cpp=.o)

all: xlayoutdisplay

$(OBJ): config.mk $(HDR)

xlayoutdisplay: $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f xlayoutdisplay $(OBJ)

install:
	mkdir -p $(PREFIX)/bin
	cp -f xlayoutdisplay $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/xlayoutdisplay

uninstall:
	rm -f $(PREFIX)/bin/xlayoutdisplay

# https://github.com/alex-courtis/arch/blob/b530f331dacaaba27484593a87ca20a9f53ab73f/home/bin/ctags-something
ctags:
	ctags-c++ $(CPPFLAGS) $(HDR) $(SRC)

.PHONY: all clean install uninstall ctags

