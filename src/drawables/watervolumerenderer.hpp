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
 
#ifndef __ORBIS_WATERVOLUMERENDERER_HPP__
#define __ORBIS_WATERVOLUMERENDERER_HPP__

#include <drawable.hpp>
#include <watervolume.hpp>

namespace Orbis {

namespace Drawable {

/*!
 * \brief This is the base class of the classes able to render a WaterVolume.
 */
class WaterVolumeRenderer : public Drawable {
public:
	/*!
	 * \brief Constructor.
	 * \param wv WaterVolume to be rendered.
	 * \param threshold Surface function threshold.
	 */
	WaterVolumeRenderer(const WaterVolume* const wv = 0, double threshold = 1.0);

	/*!
	 * \brief The current surface threshold.
	 * \return The value of the threshold.
	 */
	double threshold() const;

	/*!
	 * \brief Sets the surface threshold.
	 * \param threshold The new threshold value.
	 */
	void setThreshold(double threshold);

	/*!
	 * \brief The WaterVolume this class must render.
	 * \return A pointer to the WaterVolume this class must render.
	 */
	const WaterVolume* waterVolume() const;

	/*!
	 * \brief Sets the WaterVolume this class must render.
	 * \param wb The new WaterVolume.
	 */
	void setWaterVolume(const WaterVolume* const wv);

protected:
	/*!
	 * \brief Destructor.
	 */
	virtual ~WaterVolumeRenderer();

private:
	// pointer to water volume this class should render
	const WaterVolume* _wv;
	// threshold for "isosurface"
	double _threshold;
};

inline WaterVolumeRenderer::WaterVolumeRenderer(const WaterVolume* const wv, double threshold)
	: _wv(wv), _threshold(threshold)
{
}

inline WaterVolumeRenderer::~WaterVolumeRenderer()
{
}

inline double WaterVolumeRenderer::threshold() const
{
	return _threshold;
}

inline void WaterVolumeRenderer::setThreshold(double threshold)
{
	_threshold = threshold;
}

inline const WaterVolume* WaterVolumeRenderer::waterVolume() const
{
	return _wv;
}

inline void WaterVolumeRenderer::setWaterVolume(const WaterVolume* const wv)
{
	_wv = wv;
}

} } // namespace declarations

#endif // __ORBIS_WATERVOLUMERENDERER_HPP__
