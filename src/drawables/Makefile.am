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

noinst_LIBRARIES = liborbis-drawables.a

liborbis_drawables_a_SOURCES = \
		drawable.hpp drawable.cpp \
		fosterwatervolume.hpp fosterwatervolume.cpp \
		gridheightfield.hpp gridheightfield.cpp \
		heightfield.hpp \
		heightfieldwatervolumerenderer.hpp heightfieldwatervolumerenderer.cpp \
		isosurfacewatervolumerenderer.hpp isosurfacewatervolumerenderer.cpp \
		noisevolumerenderer.hpp noisevolumerenderer.cpp \
		stamwatervolume.hpp stamwatervolume.cpp \
		terrain.hpp \
		gridterrain.hpp gridterrain.cpp \
		marchingcubeswatervolumerenderer.hpp marchingcubeswatervolumerenderer.cpp \
		waterheightfield.hpp waterheightfield.cpp \
		watervolume.hpp watervolume.cpp \
		watervolumerenderer.hpp

INCLUDES = -I..
AM_CFLAGS = -Wall -ansi
AM_CXXFLAGS = -Wall -ansi

AUTOMAKE_OPTIONS = foreign

# Done.

