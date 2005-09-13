/*
 * The Orbis world simulator
 * Copyright (C) 2001-2003 Alex Sandro Queiroz e Silva
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * The author may be contacted by eletronic e-mail at <asandro@lcg.dc.ufc.br>
 *
 * $Id: patch.cpp,v 1.6 2004/04/01 20:51:55 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <patch.hpp>

using Orbis::Math::min;
using Orbis::Math::max;

namespace Orbis {

	namespace Util {

void Patch::addPoint(const Point& p)
{
	_poly_line.push_back(p);

	_minx = min(p.x(), _minx);
	_maxx = max(p.x(), _maxx);
	_miny = min(p.y(), _miny);
	_maxy = max(p.y(), _maxy);
}

/* tests if a point is in the patch */
bool Patch::contains(const Point& p) const
{
	return pointInPolygon(p, _poly_line);
}

const std::string Patch::attribute(const std::string& key) const
{
	std::string res;

	AttributeList::const_iterator it = _attr_list.find(key);
	if(it != _attr_list.end()) {
		res = it->second;
	}

	return res;
}

void Patch::setAttribute(const std::string& key, const std::string& value)
{
	_attr_list[key] = value;
}

Patch& Patch::operator=(const Patch& src)
{
	_poly_line = src._poly_line;
	_attr_list = src._attr_list;

	_minx = src._minx;
	_maxx = src._maxx;
	_miny = src._miny;
	_maxy = src._maxy;

	return *this;
}

} } // namespace declarations

// Done.

