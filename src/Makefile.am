#
# The Orbis world simulator
# Copyright (C) 2001-2003 Alex Sandro Queiroz e Silva
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
# The author may be contacted by eletronic e-mail at <asandro@lcg.dc.ufc.br>
#
# $Id: Makefile.am,v 1.23 2004/05/19 14:38:26 asandro Exp $
#

EXTRA_DIST = config.h.in

bin_PROGRAMS = orbis

orbis_SOURCES = \
		camera.hpp \
		config.h \
		drawable.hpp drawable.cpp \
		dynamic.hpp \
		foxactionadapter.hpp foxactionadapter.cpp \
		heightfield.hpp \
		geometry.hpp geometry.cpp \
		gridheightfield.hpp gridheightfield.cpp \
		luaactionadapter.hpp \
		luacamera.hpp luacamera.cpp \
		luagridterrain.hpp luagridterrain.cpp \
		luapatch.hpp luapatch.cpp \
		luapoint.hpp luapoint.cpp \
		luascript.hpp luascript.cpp \
		luavector.hpp luavector.cpp \
		luawaterheightfield.hpp luawaterheightfield.cpp \
		luaworld.hpp luaworld.cpp \
		luaworldview.hpp luaworldview.cpp \
		main.cpp \
		mainwindow.hpp mainwindow.cpp \
		math.hpp \
		matrix.hpp matrix.cpp \
		patch.hpp patch.cpp \
		point.hpp \
		spline.hpp spline.cpp \
		terrain.hpp \
		gridterrain.hpp gridterrain.cpp \
		textwindow.hpp textwindow.cpp \
		timer.hpp timer.cpp \
		vector.hpp \
		viewarea.hpp viewarea.cpp \
		viewwindow.hpp viewwindow.cpp \
		waterheightfield.hpp waterheightfield.cpp \
		watervolume.hpp watervolume.cpp \
		world.hpp world.cpp \
		worldview.hpp \
		worldview3d.hpp worldview3d.cpp \
		worldviewpov.hpp worldviewpov.cpp \
		worldviewxml.hpp worldviewxml.cpp

orbis_LDADD   = @X_LIBS@

INCLUDES = @X_CFLAGS@
AM_CFLAGS   = -Wall -ansi -ffast-math
AM_CXXFLAGS = -Wall -ansi -ffast-math

DISTCLEANFILES = config.h

AUTOMAKE_OPTIONS = foreign

# Done.
