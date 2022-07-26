VERSION ?= 1.4.0

PREFIX ?= /usr/local

INCS +=

CPPFLAGS += $(INCS) -DVERSION=\"$(VERSION)\"

OFLAGS = -O3
WFLAGS = -pedantic -Wall -Wextra -Werror
COMPFLAGS = $(WFLAGS) $(OFLAGS)

CXXFLAGS += $(COMPFLAGS) -std=c++14

LDFLAGS +=

PKGS = x11 xcursor xrandr libprocps
CXXFLAGS += $(foreach p,$(PKGS),$(shell pkg-config --cflags $(p)))
LDLIBS += $(foreach p,$(PKGS),$(shell pkg-config --libs $(p)))

PKGS_TEST = gmock
CXXFLAGS_TEST += $(CXXFLAGS) $(foreach p,$(PKGS_TEST),$(shell pkg-config --cflags $(p)))
LDLIBS_TEST += $(LDLIBS) $(foreach p,$(PKGS_TEST),$(shell pkg-config --libs $(p)))

CXX = g++

