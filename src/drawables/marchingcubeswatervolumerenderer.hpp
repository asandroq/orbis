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
 
#ifndef __ORBIS_MARCHINGCUBESWATERVOLUMERENDERER_HPP__
#define __ORBIS_MARCHINGCUBESWATERVOLUMERENDERER_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <watervolumerenderer.hpp>

namespace Orbis {

namespace Drawable {

/*!
 * \brief This is a base class for all the classes that use some form
 * of marching-cubes based algorythm for rendering.
 * \warning The ordering of the vertices of cubes are different in this class. This
 * is so because third-party tables are in use. In the future this must be changed.
 */
class MarchingCubesWaterVolumeRenderer : public WaterVolumeRenderer {
public:
	/*!
	 * \brief Status of a cell relative to the isosurface at the given
	 * threshold.
	 */
	typedef enum {
		INSIDE,
		OUTSIDE,
		BOUNDARY
	} Status;

	/*!
	 * \brief Constructor.
	 * \param wv WaterVolume to be rendered.
	 * \param threshold Surface function threshold.
	 */
	MarchingCubesWaterVolumeRenderer(const WaterVolume* const wv = 0, double threshold = 1.0);

	/*!
	 * \brief Classify given cell with regard to the isosurface. The cell is
	 * specified by its lower-left-front corner.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The status of the cell.
	 */
	Status classifyCell(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief Make raw OpenGL calls for drawing triangles contained in the
	 * given cell. The cell is specified by its lower-left-front corner.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 */
	void drawCellOpenGL(unsigned i, unsigned j, unsigned k) const;

protected:
	/*!
	 * \brief Destructor.
	 */
	virtual ~MarchingCubesWaterVolumeRenderer();

private:
	// edge table
	static unsigned _edge_table[];
	// triangle table
	static int _triangle_table[][16];
};

inline MarchingCubesWaterVolumeRenderer::MarchingCubesWaterVolumeRenderer(const WaterVolume* const wv, double threshold)
	: WaterVolumeRenderer(wv, threshold)
{
}

inline MarchingCubesWaterVolumeRenderer::~MarchingCubesWaterVolumeRenderer()
{
}

} } // namespace declarations

#endif // __ORBIS_MARCHINGCUBESWATERVOLUMERENDERER_HPP__
