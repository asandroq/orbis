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
 *
 * $Id: gridheightfield.hpp,v 1.5 2004/03/30 20:15:42 asandro Exp $
 */

#ifndef __ORBIS_GRIDHEIGHTFIELD_HPP__
#define __ORBIS_GRIDHEIGHTFIELD_HPP__

#ifdef __GNUG__
#pragma interface
#endif

/*!
 * \file gridheightfield.hpp
 * \brief This file declares the GridHeightField class.
 */

#include <heightfield.hpp>

using osg::FloatArray;
using Orbis::Util::Point;

namespace Orbis {

	namespace Drawable {

/*!
 * \brief This class implements a HeightField as a regular grid.
 *
 * It is commonly used for the foundations of a simple terrain or
 * as a water layer.
 */
class GridHeightField : public HeightField {
public:
	// OpenSceneGraph stuff
	META_Object(Orbis, GridHeightField);

	//! Default constructor
	GridHeightField();

	//! Copy constructor
	/*!
	 * \param src The source HeightField
	 * \param copyOp Copy operation spec
	 */
	GridHeightField(const GridHeightField& src,
				const osg::CopyOp& copyOp
					= osg::CopyOp::SHALLOW_COPY);

	//! This is the most-used constructor
	/*!
	 * \param origin The origin of the height field
	 * \param xstep The spacing between samples in the x direction
	 * \param ystep The spacing between samples in the y direction
	 * \param xsize The number of samples in the x direction
	 * \param ysize The number of samples in the y direction
	 */
	GridHeightField(const Point& origin,
				double xstep, double ystep,
					unsigned xsize, unsigned ysize);

	/*!
	 * \brief Loads the heightfield from a data file.
	 * \param filename The name of the data file.
	 * \param origin The origin of the height field.
	 * \param xstep The spacing between samples in the x direction
	 * \param ystep The spacing between samples in the y direction
	 */
	GridHeightField(const std::string& filename, double xstep, double ystep);

	/*!
	 * \brief Loads the heightfield from a data file.
	 * \param filename The name of the data file.
	 * \return True if succeeded, false otherwise.
	 */
	bool load(const std::string& filename);

	//! Size of the height field on the x direction
	double sizeX() const;

	//! Size of the height field on the y direction
	double sizeY() const;

	//! Spacing between samples in the x direction
	double stepX() const;

	//! Spacing between samples in the y direction
	double stepY() const;

	//! Number of samples in the x direction
	unsigned numSamplesX() const;

	//! Number of samples in the y direction
	unsigned numSamplesY() const;

	//! A point on the height field
	/*!
	 * \param x The x coordinate of the point on the height field
	 * \param y The y coordinate of the point on the height field
	 */
	Point point(double x, double y) const;

	//! Calculates the normal vector on the height field
	/*!
	 * \param x The x coordinate of the point on the height field
	 * \param y The y coordinate of the point on the height field
	 */
	Vector normal(double x, double y) const;

	//! A point on the grid
	/*!
	 * \param i The point's index in the x direction
	 * \param j The point's index in the y direction
	 */
	Point point(unsigned i, unsigned j) const;

	//! Calculates the normal vector at a grid point
	/*!
	 * \param i The point's index in the x direction
	 * \param j The point's index in the y direction
	 */
	Vector normal(unsigned i, unsigned j) const;

	//! Sets a point on the grid
	/*!
	 * \param i The point's index in the x direction
	 * \param j The point's index in the y direction
	 * \param val The new vertex elevation
	 */
	void setPoint(unsigned i, unsigned j, double val);

protected:
	//! Destructor
	virtual ~GridHeightField();

private:
	// beware, don't check bounds
	FloatArray::value_type point(unsigned i) const;

	// array of elevations
	osg::ref_ptr<FloatArray> _elevs;
	// number of samples
	unsigned _xsamples, _ysamples;
	// spacing between samples
	double _xstep, _ystep;
};

inline GridHeightField::~GridHeightField()
{
}

inline double GridHeightField::sizeX() const
{
	return (_xsamples - 1) * _xstep;
}

inline double GridHeightField::sizeY() const
{
	return (_ysamples - 1) * _ystep;
}

inline double GridHeightField::stepX() const
{
	return _xstep;
}

inline double GridHeightField::stepY() const
{
	return _ystep;
}

inline unsigned GridHeightField::numSamplesX() const
{
	return _xsamples;
}

inline unsigned GridHeightField::numSamplesY() const
{
	return _ysamples;
}

inline FloatArray::value_type GridHeightField::point(unsigned i) const
{
	return (*_elevs)[i];
}

} } // namespace declarations

#endif // __ORBIS_GRIDHEIGHTFIELD_HPP__

