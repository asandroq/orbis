#
# The Orbis world simulator
# Copyright (C) 2001-2004 Alex Sandro Queiroz e Silva
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

noinst_LIBRARIES = liborbis-lua.a

liborbis_lua_a_SOURCES = \
		luaactionadapter.hpp \
		luacamera.hpp luacamera.cpp \
		luagridterrain.hpp luagridterrain.cpp \
		luaisosurfacerenderer.hpp luaisosurfacerenderer.cpp \
		luapatch.hpp luapatch.cpp \
		luapoint.hpp luapoint.cpp \
		luascript.hpp luascript.cpp \
		luavector.hpp luavector.cpp \
		luawaterheightfield.hpp luawaterheightfield.cpp \
		luastamwatervolume.hpp luastamwatervolume.cpp \
		luafosterwatervolume.hpp luafosterwatervolume.cpp \
		luaworld.hpp luaworld.cpp \
		luaworldview.hpp luaworldview.cpp

INCLUDES = -I.. -I../drawables
AM_CXXFLAGS = -Wall -ansi -ffast-math

AUTOMAKE_OPTIONS = foreign

# Done.

