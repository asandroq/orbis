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

#ifndef __ORBIS_WATERHEIGHTFIELD_HPP__
#define __ORBIS_WATERHEIGHTFIELD_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <waterbase.hpp>
#include <gridheightfield.hpp>

using osg::ref_ptr;
using osg::FloatArray;

using Orbis::Util::Point;

namespace Orbis {

	namespace Drawable {

/*!
 * \brief This class represents a water layer over a terrain.
 */
class WaterHeightField : public WaterBase, public GridHeightField {
public:
	// OpenSceneGraph stuff
	META_Object(Orbis, WaterHeightField)

	//! Constructor.
	WaterHeightField();

	/*!
	 * \brief Copy constructor.
	 * \param field The original water height field.
	 * \param copyOp Tells how the copy must be done.
	 */
	WaterHeightField(const WaterHeightField& field,
			const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

	/*!
	 * \brief This is the most-used constructor.
	 * \param origin The lower-left origin of the height field.
	 * \param xstep Spacing between samples in the x direction.
	 * \param ystep Spacing between samples in the y direction.
	 * \param xsize Number of samples in the x direction.
	 * \param ysize Number of samples in the y direction.
	 */
	WaterHeightField(const Point& origin, double xstep, double ystep,
						unsigned xsize, unsigned ysize);

	/*!
	 * \brief Calculates the next state.
	 * \param time The elapsed time.
	 */
	void evolve(unsigned long time);

	/*!
	 * \brief Draws this drawable.
	 * \param state The rendering engine state.
	 */
	virtual void drawImplementation(osg::State& state) const;

protected:
	//! destructor
	virtual ~WaterHeightField();

private:
	// set of old heights
	ref_ptr<FloatArray> _old_z;
	// auxiliary vectors
	double *_e, *_f, *_r, *_u, *_g;

	// locates a point in the grid
	void locate(const Point& p, unsigned *i, unsigned *j) const;
};

inline WaterHeightField::WaterHeightField()
	: GridHeightField()
{
}

inline WaterHeightField::WaterHeightField(const WaterHeightField& field,
						const osg::CopyOp& copyOp)
	: GridHeightField(field, copyOp)
{
}

} } // namespace declarations

#endif  // __ORBIS_WATERHEIGHTFIELD_HPP__
