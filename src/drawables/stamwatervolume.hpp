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
 
#ifndef __ORBIS_STAMWATERVOLUME_HPP__
#define __ORBIS_STAMWATERVOLUME_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <watervolume.hpp>

namespace Orbis {

	namespace Drawable {
	   
/*!
 * \brief This class encapsulates the simulation of the behaviour of
 * a mass of water.
 * 
 * The algorythm used here, one developed by Jos Stam, is indeed general for
 * all fluids. The volume is divided in cubic cells.
 */
class StamWaterVolume : public WaterVolume {
public:
	/*!
	 * \brief Default constructor.
	 */
	StamWaterVolume();

	/*!
	 * \brief Most-used constructor.
	 * \param origin Lower-left-front corner of the volume.
	 * \param size Number of elements of volume, in each direction.
	 * \param step The size of one element, in each direction.
	 */
	StamWaterVolume(const Orbis::Util::Point& origin, unsigned size, double step);

	/*!
	 * \brief Destructor.
	 */
	virtual ~StamWaterVolume();

	/*!
	 * \brief The current calculated density in a cell.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current density.
	 */
	double density(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief The current calculated pressure in a cell.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current pressure.
	 */
	double pressure(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief The current calculated velocity in a cell.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current velocity.
	 */
	Vector velocity(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief Queries the diffusion rate of the material.
	 * \return The diffusion rate.
	 */
	double diffuse() const;

	/*!
	 * \brief Sets the diffusion rate of the material.
	 * \param diff The new diffusion rate.
	 */
	void setDiffuse(double diff);

	/*!
	 * \brief Updates the water volume state.
	 * \param time The time slice.
	 */
	void evolve(unsigned long time);

private:
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

	// diffusion rate
	double _diff;
	// density in each element
	DoubleVector _dens;
	// previous density
	DoubleVector _dens_prev;
	// velocity components, in each direction
	DoubleVector _u, _v, _w;
	// previous velocity components
	DoubleVector _u_prev, _v_prev, _w_prev;
};

inline double StamWaterVolume::density(unsigned i, unsigned j, unsigned k) const
{
	Locker lock(this);

	return _dens[i3d(i, j, k)];
}

inline double StamWaterVolume::pressure(unsigned i, unsigned j, unsigned k) const
{
	return 0.0;
}

inline Vector StamWaterVolume::velocity(unsigned i, unsigned j, unsigned k) const
{
	Locker lock(this);

	unsigned l = i3d(i, j, k);

	return Vector(_u[l], _v[l], _w[l]);
}

inline double StamWaterVolume::diffuse() const
{
	return _diff;
}

inline void StamWaterVolume::setDiffuse(double diff)
{
	_diff = diff;
}

} } // namespace declarations

#endif // __ORBIS_STAMWATERVOLUME_HPP__
