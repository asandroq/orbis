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
 * $Id: gridterrain.hpp,v 1.7 2004/03/30 20:15:42 asandro Exp $
 */

#ifndef __ORBIS_GRIDTERRAIN_HPP__
#define __ORBIS_GRIDTERRAIN_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <terrain.hpp>
#include <gridheightfield.hpp>

namespace Orbis {

	namespace Drawable {

/*!
 * \brief This class represents a terrain which is a grid.
 *
 * This class represents a terrain which is the graphic of a two-dimensional
 * function (and hence has no caves) and that is implemented as a regular grid.
 */
class GridTerrain : public Terrain, public GridHeightField {
public:
	// OpenSceneGraph stuff
	META_Object(Orbis, GridTerrain);

	//! Default constructor.
	GridTerrain();

	//! Copy constructor.
	GridTerrain(const GridTerrain& terrain,
			const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

	//! Most-used constructor.
	/*!
	 * \param origin The origin of the height field
	 * \param xstep The spacing between samples in the x direction
	 * \param ystep The spacing between samples in the y direction
	 * \param xsize The number of samples in the x direction
	 * \param ysize The number of samples in the y direction
	 */
	GridTerrain(const Point& origin, double xstep, double ystep,
					unsigned xsize, unsigned ysize);

	/*!
	 * \brief Loads the heightfield from a data file.
	 * \param filename The name of the data file.
	 */
	GridTerrain(const std::string& filename);

	//! Generates a random terrain using the fault line algorithm.
	/*!
	 * \param iters Number of iterations.
	 */
	void faultLineGeneration(unsigned iters);

	//! Smooths the terrain.
	/*!
	 * Smooths the terrain using a filter one-neighbour wide.
	 * \param k The weight.
	 */
	void smooth(double k);

	/*!
	 * \brief I use this callback to do the vertex list creation
	 * just before drawing
	 */
	struct UpdateCallback : public osg::Drawable::UpdateCallback {
		/*!
		 * \brief Default constructor.
		 */
		UpdateCallback();

		/*!
		 * \brief Should the lists be created again?
		 */
		void dirtyLists();

		/*!
		 * \brief Creates the vertex lists.
		 */
		virtual void update(osg::NodeVisitor*, osg::Drawable*);

		private: bool _init;
	};

protected:
	//! Destructor.
	virtual ~GridTerrain();
};

inline GridTerrain::~GridTerrain()
{
}

} } // namespace declarations

#endif // __ORBIS_GRIDTERRAIN_HPP__

