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
 * $Id: spline.hpp,v 1.1 2003/07/18 03:11:49 asandro Exp $
 */

#ifndef __ORBIS_SPLINE_HPP__
#define __ORBIS_SPLINE_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <vector>

#include <point.hpp>
#include <vector.hpp>

namespace Orbis {

	namespace Util {

/*!
 * \brief This class represents a n-point continuous spline.
 */
class Spline {
public:
	//! Default constructor
	Spline();

	//! Constructor that takes the number of subdivisions
	Spline(int subdivs);

	//! Destructor
	virtual ~Spline();

	//! The number of control points
	size_t size() const;

	//! The number of subdivisions
	int subDivs() const;

	//! Adds a new control point
	void addCtrlPoint(const Point& p);

	//! Returns a point on spline given its parameter
	Point pointOnSpline(double t);

	//! Returns a normal to spline given its parameter
	Vector normalToSpline(double t);

	//! Returns a control point at index
	Point operator[](size_t i) const;

	//! Returns the derivative of the spline given its parameter
	Vector derivativeOnSpline(double);

private:
	//! Returns a point on the quadratic defined by p1, p2 and p3
	Point quadratic(Point p1, Point p2, Point p3, double t);

	//! I've got to find out
	Point B(Point p1, Point p2, Point p3, Point p4, double t);

	//! Same here
	Vector deriv_B(Point p1, Point p2, Point p3, Point p4, double t);

	//! Returns a point on the Bezier curve defined by the given points
	Point bezier(Point p1, Point p2, Point p3, Point p4, double t);

	//! Number of subdivisions
	int _sub_divs;
	//! Auxiliary vector
	std::vector<Point> _b;
	//! Control points
	std::vector<Point> _ctrl_points;
};

inline Spline::Spline()
	: _sub_divs(0)
{
}

inline Spline::Spline(int subdivs)
	: _sub_divs(subdivs)
{
}

inline Spline::~Spline()
{
}

inline size_t Spline::size() const
{
	return _ctrl_points.size();
}

inline int Spline::subDivs() const
{
	return _sub_divs;
}

inline Point Spline::operator[](size_t i) const
{
	if(i < _ctrl_points.size()) {
		return _ctrl_points[i];
	} else {
		return Point();
	}
}

} } // namespace declarations

#endif // __ORBIS_SPLINE_HPP__
