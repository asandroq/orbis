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
 * $Id: geometry.cpp,v 1.5 2004/04/28 14:13:03 asandro Exp $
 */

#include <geometry.hpp>

using Orbis::Math::order;
using Orbis::Math::areEqual;

namespace Orbis {
	
	namespace Util {

/* finds the intersection of two 2D line segments. */
bool intersectSegments(const Point& p1, const Point& p2,
					const Point& p3, const Point& p4, Point& res)
{
	// is first line vertical?
	if(areEqual(p2.x(), p1.x())) {
		// is second line vertical?
		if(areEqual(p4.x(), p3.x())) {
			return false;
		}
		// finding line equation of second line
		double y1 = p3.y(), y2 = p4.y();
		double m = (y2 - y1) / (p4.x() - p3.x());
		double c = p3.y() - m * p3.x();
		double y = m * p1.x() + c;
		order(y1, y2);
		if(y1 <= y && y <= y2) {
			res = Point(p1.x(), y);
		} else {
			return false;
		}
	} else {
		// is second line vertical?
		if(areEqual(p4.x(), p3.x())) {
			// finding line equation of first line
			double y1 = p1.y(), y2 = p2.y();
			double m = (y2 - y1) / (p2.x() - p1.x());
			double c = p1.y() - m * p1.x();
			double y = m * p3.x() + c;
			order(y1, y2);
			if(y1 <= y && y <= y2) {
				res = Point(p3.x(), y);
			} else {
				return false;
			}
		} else {
			// neither line is vertical
		}
	}

	return true;
}

/* finds if point is inside the polygon */
bool pointInPolygon(const Point& p, const PolyLine& poly)
{
	int inter = 0;

	if(poly.empty()) {
		return false;
	}

	Point p1 = poly.back();
	for(PolyLineIterator p2 = poly.begin(); p2 != poly.end(); p2++) {
		// a horizontal side doesn't change the result
		if(areEqual(p1.y(), p2->y())) {
			p1 = *p2;
			continue;
		}
		// is side vertical?
		if(areEqual(p1.x(), p2->x())) {
			double y1 = p1.y(), y2 = p2->y();
			order(y1, y2);
			if(!areEqual(p.y(), y1) &&
							 p.x() < p1.x() &&
							 p.y() <= y2 &&
							 p.y() >= y1) {
				inter++;
			}
		} else {
			double y1 = p1.y(), y2 = p2->y();
			double m = (y1 - y2) / (p1.x() - p2->x());
			double c = y1 - m * p1.x();
			double x = (p.y() - c) / m;
			order(y1, y2);
			if(!areEqual(p.y(), y1) &&
							 p.x() < x &&
							 p.y() <= y2 &&
							 p.y() >= y1) {
				inter++;
			}
		}
		p1 = *p2;
	}

	return inter % 2;
}

} } // namespace declarations

