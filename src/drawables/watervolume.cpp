/*
 * The Orbis world simulator
 * Copyright (C) 2001-2004 Alex Sandro Queiroz e Silva
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
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <geometry.hpp>
#include <watervolume.hpp>

namespace Orbis {

namespace Drawable {

bool WaterVolume::locate(const Point& p, unsigned* i, unsigned* j, unsigned* k) const
{
	if(!i || !j || !k) {
		return false;
	}

	Point end = Point(_origin.x() + (_size_x-1) * _step_x,
						_origin.y() + (_size_y-1) * _step_y,
						_origin.z() + (_size_z-1) * _step_z);
	if(!pointInVolume(p, _origin, end)) {
		return false;
	}

	double x = floor((p.x() - _origin.x()) / _step_x);
	double y = floor((p.y() - _origin.y()) / _step_y);
	double z = floor((p.z() - _origin.z()) / _step_z);

	*i = static_cast<unsigned>(x);
	*j = static_cast<unsigned>(y);
	*k = static_cast<unsigned>(z);

	return true;
}

} } // namespace declarations
