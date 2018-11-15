AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -I./include

lib_LTLIBRARIES = libcfile.la
libcfile_la_SOURCES = src/cfile_base.cpp src/shared_file.cpp src/unique_file.cpp
libcfile_la_LDFLAGS = -version-info 3:0:3

include_HEADERS = include/cfile.hpp

EXTRA_DIST = LICENSE README.md



