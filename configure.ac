#
# The Orbis world simulator
# Copyright (C) 2001-2005 Alex Sandro Queiroz e Silva
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# The author may be contacted by eletronic e-mail at <ventonegro@ventonegro.org>
#

#
# Initialisation stuff
#

AC_INIT(Orbis, 0.1.0, ventonegro@ventonegro.org, orbis)
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE

AC_LANG(C++)
AC_DISABLE_SHARED

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_PATH_PROG(DOXYGEN, doxygen, no)
AC_PATH_PROG(RESWRAP, reswrap, no)

# Checks for libraries.
AC_PATH_XTRA
AC_CHECK_LIB(lua, lua_close)
AC_CHECK_LIB(gdal, GDALCreate)
AC_CHECK_LIB(osg, osgGetVersion)
AC_CHECK_LIB(osgDB, osgDBGetVersion)
AC_CHECK_LIB(osgGA, osgGAGetVersion)
AC_CHECK_LIB(osgUtil, osgUtilGetVersion)

# Packages
PKG_CHECK_MODULES(GTKMM, [gtkglextmm-x11-1.2 >= 1.1])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([floor pow sqrt])

AC_CONFIG_FILES([Makefile
                 src/Makefile
		 src/drawables/Makefile
		 src/lua/Makefile])
AC_OUTPUT

