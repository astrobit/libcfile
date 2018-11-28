AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -I${srcdir}/include

lib_LTLIBRARIES = libcfile.la
libcfile_la_SOURCES = src/cfile_base.cpp src/shared_file.cpp src/unique_file.cpp src/string_management.cpp
libcfile_la_LDFLAGS = -version-info 1:1:0

include_HEADERS = include/cfile.hpp
noinst_HEADERS = include/cfile_inst.hpp

EXTRA_DIST = LICENSE README.md ${PACKAGE_NAME}-${API_VERSION}.pc
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc
DISTCLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc




