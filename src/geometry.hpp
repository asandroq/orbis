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
 * $Id: geometry.hpp,v 1.3 2004/04/01 20:51:55 asandro Exp $
 */

#ifndef __ORBIS_GEOMETRY_HPP__
#define __ORBIS_GEOMETRY_HPP__

#include <vector>

#include <point.hpp>

namespace Orbis {
	
	namespace Util {

// type of patch boundary
typedef std::vector<Point> PolyLine;
// iterator type
typedef PolyLine::const_iterator PolyLineIterator;

/*!
 * \brief Finds the intersection of two line segments.
 * \param p1 First point of the first segment.
 * \param p2 Second point of the first segment.
 * \param p3 First point of the second segment.
 * \param p4 Second point of the second segment.
 */
bool intersectSegments(const Point& p1, const Point& p2,
					const Point& p3, const Point& p4, Point& res);

/*!
 * \brief Finds if point is inside the polygon.
 * \param p The point which will be tested.
 * \param poly The enclosing polyline.
 */
bool pointInPolygon(const Point& p, const PolyLine& poly);

} } // namespace declarations

#endif // __ORBIS_GEOMETRY_HPP__

