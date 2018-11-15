# Release version
m4_define([package_version_major],[1])
m4_define([package_version_minor],[0])
m4_define([package_version_micro],[0])
m4_define([package_repository],[https://github.com/astrobit/cfile])
m4_define([package_auth_email],[bwmulligan@astronaos.com])

# API version
m4_define([api_version],package_version_major)

AC_INIT(mkcpp,package_version_major.package_version_minor.package_version_micro,package_auth_email,mkcpp,package_repository)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

#AC_PROG_RANLIB

AC_LANG(C++)

LT_INIT

AC_SUBST([PACKAGE_VERSION_MAJOR],package_version_major)
AC_SUBST([PACKAGE_VERSION_MINOR],package_version_minor)
AC_SUBST([PACKAGE_VERSION_MICRO],package_version_micro)
AC_SUBST([PACKAGE_REPOSITORY],package_repository)
AC_SUBST([PACKAGE_AUTH_EMAIL],package_auth_email)
AC_SUBST([API_VERSION],api_version)

AC_CONFIG_FILES([Makefile],[],[PACKAGE_NAME='$PACKAGE_NAME' API_VERSION='$API_VERSION'])

AC_OUTPUT


