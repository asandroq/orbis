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
 
#ifndef __ORBIS_WATERVOLUME_HPP__
#define __ORBIS_WATERVOLUME_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <vector>

#include <math.hpp>
#include <point.hpp>
#include <dynamic.hpp>
#include <drawable.hpp>
#include <heightfield.hpp>

namespace Orbis {

namespace Drawable {
	   
typedef std::vector<double> DoubleVector;

/*!
 * \brief This class represents a water volume composed of
 * several finite elements.
 */
class WaterVolume : public Dynamic , public Drawable {
public:
	// OpenSceneGraph stuff
	META_Object(Orbis, WaterVolume)

	/*!
	 * \brief Default constructor.
	 */
	WaterVolume();

	/*!
	 * \brief Copy constructor.
	 * \param field The original water volume field.
	 * \param copyOp Tells how the copy must be done.
	 */
	WaterVolume(const WaterVolume& field,
				const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

	/*!
	 * \brief Most-used constructor.
	 * \param origin Bottom-left-front corner of the volume.
	 * \param size Number of elements of volume.
	 * \param step The size of one element.
	 */
	WaterVolume(const Orbis::Util::Point& origin, unsigned size, double step);

	/*!
	 * \brief Sets the underlying height field of this water volume.
	 * \param bottom A const pointer to the height field.
	 */
	inline void setBottom(const Orbis::Drawable::HeightField* const bottom);

	/*!
	 * \brief Updates the water volume state.
	 * \param time The time slice.
	 */
	void evolve(unsigned long time);

	/*!
	 * \brief Draws this drawable.
	 * \param state The rendering engine state.
	 */
	virtual void drawImplementation(osg::State& state) const;

protected:
	/*!
	 * \brief Destructor.
	 */
	virtual ~WaterVolume();

private:
	// method to map 3d indices into linear array
	inline unsigned i3d(unsigned i, unsigned j, unsigned k) const;
	// adds from source
	void add_sources(DoubleVector& x,
				 		const DoubleVector& srcs, double dt) const;
	// diffuses through fluid
	void diffuse(int b, DoubleVector& x,
						DoubleVector& x0, double diff, double dt) const;
	// advects by fluid
	void advect(int b, DoubleVector& d,
				DoubleVector& d0, DoubleVector& u,
					DoubleVector& v, DoubleVector& w, double dt) const;
	// projects field onto mass-conserving one
	void project(DoubleVector& u, DoubleVector& v,
				 DoubleVector& w, DoubleVector &p, DoubleVector& div) const;
	// sets the boundary conditions
	void set_bounds(int b, DoubleVector& x) const;
	// the density step
	void dens_step(DoubleVector& d, DoubleVector& d0,
					DoubleVector& u, DoubleVector& v,
						DoubleVector& w, double diff, double dt) const;
	// the velocity step
	void vel_step(DoubleVector& u, DoubleVector& v, DoubleVector& w,
				DoubleVector& u0, DoubleVector& v0, DoubleVector& w0,
										double visc, double dt) const;
	// origin of grid
	Orbis::Util::Point _origin;
	// grid spacing
	double _step;
	// diffusion rate
	double _diff;
	// viscosity of the fluid
	double _visc;
	// number of elements
	unsigned _size;
	// density in each element
	DoubleVector _dens;
	// previous density
	DoubleVector _dens_prev;
	// velocity components, in each direction
	DoubleVector _u, _v, _w;
	// previous velocity components
	DoubleVector _u_prev, _v_prev, _w_prev;
	// underlying height field
	const Orbis::Drawable::HeightField* _height_field;
};

void WaterVolume::setBottom(const Orbis::Drawable::HeightField* const hf)
{
	_height_field = hf;
}

unsigned WaterVolume::i3d(unsigned i, unsigned j, unsigned k) const
{
	return k * Orbis::Math::sqr(_size) + j * _size + i;
}

} } // namespace declarations

#endif  // __ORBIS_WATERVOLUME_HPP__

