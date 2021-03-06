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
 
#ifndef __ORBIS_NOISEVOLUMERENDERER_HPP__
#define __ORBIS_NOISEVOLUMERENDERER_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <watervolumerenderer.hpp>

namespace Orbis {

namespace Drawable {

class NoiseVolumeRenderer : public WaterVolumeRenderer {
public:
	/*!
	 * \brief Constructor.
	 * \param wv A pointer to an instance of \a WaterVolume that will be rendered.
	 * \param threshold The threshold of the surface function.
	 */
	NoiseVolumeRenderer(const WaterVolume* const wv, double threshold = 1.0);

	/*!
	 * \brief Draws this drawable.
	 * \param state The current rendering state.
	 */
	void drawImplementation(osg::State& state) const;

protected:
	/*!
	 * \brief Destructor.
	 */
	virtual ~NoiseVolumeRenderer();
};

inline NoiseVolumeRenderer::~NoiseVolumeRenderer()
{
}

} } // namespace declarations

#endif // __ORBIS_NOISEVOLUMERENDERER_HPP__
