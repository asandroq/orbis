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

#ifndef __ORBIS_POINT_HPP__
#define __ORBIS_POINT_HPP__

#include <math.hpp>

/*!
 * \file point.hpp
 * \brief Contains the class Point and related functions.
 */

namespace Orbis {

	namespace Util {

/*!
 * \class Point
 * \brief This class represents a simple geometric point.
 *
 * The Point class represents a simple three-dimensional location
 * in space, useful for storing coordinates such as spheres' centres.
 */
class Point {
public:
	//! Point constructor.
	/*!
	 * Creates a new point, located are the origin by default.
	 * \param x Point's x coordinate.
	 * \param y Point's y coordinate.
	 * \param z Point's z coordinate.
	 */
	Point(double x = 0.0, double y = 0.0, double z = 0.0)
		: _x(x), _y(y), _z(z) {}

	//! Reads x coordinate.
	/*!
	 * \return Point's x coordinate.
	 */ 
	double x() const { return _x; }

	//! Reads y coordinate.
	/*!
	 * \return Point's y coordinate.
	 */ 
	double y() const { return _y; }

	//! Reads z coordinate.
	/*!
	 * \return Point's z coordinate.
	 */ 
	double z() const { return _z; }

private:
	// point coordinates
	double _x, _y, _z;
};

/*!
 * \brief Test for equality of two points.
 *
 * The coordinates are tested one by one with a small tolerance.
 */
inline bool operator==(const Point& p1, const Point& p2)
{
	return Orbis::Math::areEqual(p1.x(), p2.x()) &&
	       Orbis::Math::areEqual(p1.y(), p2.y()) &&
	       Orbis::Math::areEqual(p1.z(), p2.z());
}

} } // namespace declarations

#endif // __ORBIS_POINT_HPP__
