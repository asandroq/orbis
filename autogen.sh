#!/bin/sh
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
#
# Requires: automake, autoconf, libtool
#

set -e

# Refresh GNU autotools toolchain.
for i in config.guess config.sub; do
	test -r /usr/share/automake-1.7/${i} && {
		rm -f config/${i}
		cp /usr/share/automake-1.7/${i} config
	}
	chmod 755 config/${i}
done

test -r /usr/share/libtool/ltmain.sh && {
	rm -f config/ltmain.sh
	cp /usr/share/libtool/ltmain.sh config
}
chmod 755 config/ltmain.sh

aclocal
autoheader
automake --foreign --add-missing
autoconf

test -d autom4te.cache && {
	rm -rf autom4te.cache
}

exit 0

