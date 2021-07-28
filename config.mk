VERSION ?= 1.3.0

PREFIX ?= /usr/local

INCS +=

CPPFLAGS += $(INCS) -DVERSION=\"$(VERSION)\"

CXXFLAGS += -pedantic -Wall -Wextra -Werror -O3 -std=c++14

# hack to force inclusion of the static BPO instead of linking the dynamic (g++ preference)
LDFLAGS += /usr/lib/libboost_program_options.a -lX11 -lXcursor -lXrandr
LDFLAGS_TEST += -lgmock -lgtest -pthread

CXX ?= g++

