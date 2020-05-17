# todo remove the version from main
VERSION = 1.1.3

PREFIX = /usr/local

INCS =

CPPFLAGS = $(INCS) -DVERSION=\"$(VERSION)\"

CXXFLAGS = -pedantic -Wall -Wextra -Werror -O3 -std=c++14

LDFLAGS = -lX11 -lXcursor -lXrandr -lboost_program_options

CXX = g++

