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
 * $Id: heightfield.hpp,v 1.4 2004/03/26 20:39:26 asandro Exp $
 */

#ifndef __ORBIS_HEIGHTFIELD_HPP__
#define __ORBIS_HEIGHTFIELD_HPP__

/*!
 * \file heightfield.hpp
 * \brief This file declares the HeightField class.
 */

#include <point.hpp>
#include <vector.hpp>

#include <drawable.hpp>

using Orbis::Util::Point;
using Orbis::Util::Vector;

namespace Orbis {

	namespace Drawable {

/*!
 * \brief This class represents the bounded graphic of a bidimensional function.
 *
 * The HeightField class is an abstract class that represents the
 * graphic of a bidimensional function, usually used to represent
 * terrains that don't have caves or self-bending cliffs.
 *
 * It is a subclass osg::Drawable, a class that can draw itself and may
 * be included in a osg::Geode in the scene graph.
 */
class HeightField : public virtual Drawable {
public:
	//! Default constructor
	HeightField();

	//! Copy constructor
	/*!
	 * \param src The source HeightField
	 * \param copyOp Copy operation spec
	 */
	HeightField(const HeightField& src,
			const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

	//! This is the most-used constructor
	/*!
	 * \param origin The origin of the height field
	 */
	HeightField(const Point& origin);

	/*!
	 * \brief The origin of the heightfield.
	 * \sa setOrigin
	 */
	Point origin() const;

	/*!
	 * \brief Sets the origin of the height field.
	 * \param org The new origin.
	 * \sa origin
	 */
	void setOrigin(const Point& org);

	//! The size of the height field in the x direction
	virtual double sizeX() const = 0;

	//! The size of the height field in the y direction
	virtual double sizeY() const = 0;

	//! A point on the height field
	/*!
	 * \param x The x coordinate of the point on the height field
	 * \param y The y coordinate of the point on the height field
	 */
	virtual Point point(double x, double y) const = 0;

	//! Calculates the normal vector on the height field
	/*!
	 * \param x The x coordinate of the point on the height field
	 * \param y The y coordinate of the point on the height field
	 */
	virtual Vector normal(double x, double y) const = 0;

	//! The minimum height field elevation
	double minimumElevation() const;

	//! The maximum height field elevation
	double maximumElevation() const;

protected:
	//! Destructor
	virtual ~HeightField();

	//! Computes the bounding box of the height field
	virtual bool computeBound() const;

	//! Extreme elevations
	double _min_elev, _max_elev;

private:
	//! Height field origin
	Point _origin;
};

// pretty useless constructor, for osg's sake
inline HeightField::HeightField()
	: Drawable()
{
	_min_elev =  FLT_MAX;
	_max_elev = -FLT_MAX;
}

inline HeightField::HeightField(const Point& origin)
	: Drawable(), _origin(origin)
{
	_min_elev =  FLT_MAX;
	_max_elev = -FLT_MAX;
}

inline HeightField::HeightField(const HeightField& src,
					const osg::CopyOp& copyOp)
	: Drawable(src, copyOp),
		_min_elev(src._min_elev), _max_elev(src._max_elev),
		_origin(src._origin)
{
	_min_elev =  FLT_MAX;
	_max_elev = -FLT_MAX;
}

inline HeightField::~HeightField()
{
}

inline Point HeightField::origin() const
{
	return _origin;
}

inline void HeightField::setOrigin(const Point& org)
{
	_origin = org;
}

inline double HeightField::minimumElevation() const
{
	return _min_elev;
}

inline double HeightField::maximumElevation() const
{
	return _max_elev;
}

inline bool HeightField::computeBound() const
{
	_bbox._min.x() = origin().x();
	_bbox._min.y() = origin().y();
	_bbox._min.z() = minimumElevation();
	_bbox._max.x() = origin().x() + sizeX();
	_bbox._max.y() = origin().y() + sizeY();
	_bbox._max.z() = maximumElevation();

	return _bbox_computed = true;
}

} } // namespaces declaration

#endif  // __ORBIS_HEIGHTFIELD_HPP__

