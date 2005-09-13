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
# The author may be contacted by eletronic e-mail at <asandro@lcg.dc.ufc.br>
#

SUBDIRS = drawables #lua

EXTRA_DIST = config.h.in

bin_PROGRAMS = orbis

orbis_SOURCES = \
		camera.hpp \
		config.h \
		dynamic.hpp \
		geometry.hpp geometry.cpp \
		main.cpp \
		mainwindow.hpp mainwindow.cpp \
		math.hpp math.cpp \
		matrix.hpp matrix.cpp \
		patch.hpp patch.cpp \
		point.hpp \
		spline.hpp spline.cpp \
		timer.hpp timer.cpp \
		vector.hpp \
		viewarea.hpp viewarea.cpp \
		world.hpp world.cpp \
		worldview.hpp \
		worldview3d.hpp worldview3d.cpp \
		worldviewpov.hpp worldviewpov.cpp \
		worldviewxml.hpp worldviewxml.cpp

#		consolewindow.hpp consolewindow.cpp \
#		foxactionadapter.hpp foxactionadapter.cpp \
#		textwindow.hpp textwindow.cpp \
#		viewwindow.hpp viewwindow.cpp

orbis_LDADD   = drawables/liborbis-drawables.a @GTKMM_LIBS@ @X_LIBS@

INCLUDES = -Idrawables -Ilua @GTKMM_CFLAGS@ @X_CFLAGS@
AM_CFLAGS   = -Wall -ansi
AM_CXXFLAGS = -Wall -ansi

DISTCLEANFILES = config.h

AUTOMAKE_OPTIONS = foreign

# Done.

