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
#include <stdexcept>

#include <math.hpp>
#include <waterbase.hpp>

namespace Orbis {

	namespace Drawable {
	   
/*!
 * \brief This class encapsulates the simulation of the behaviour of
 * a mass of water.
 * 
 * The algorythm used here, one developed by Jos Stam, is indeed general for
 * all fluids. The volume is divided in cubic cells.
 */
class WaterVolume : public WaterBase {
public:
	/*!
	 * \brief Status of a cell relative to the bottom height field.
	 */ 
	typedef enum {
		ABOVE,
		BELOW,
		BOUNDARY
	} Status;

	/*!
	 * \brief Default constructor.
	 */
	WaterVolume();

	/*!
	 * \brief Most-used constructor.
	 * \param origin Bottom-left-front corner of the volume.
	 * \param size Number of elements of volume.
	 * \param step The size of one element.
	 */
	WaterVolume(const Orbis::Util::Point& origin, unsigned size, double step);

	/*!
	 * \brief Destructor.
	 */
	virtual ~WaterVolume();

	/*!
	 * \brief The number of samples at each side of the volume.
	 */
	unsigned size() const;

	/*!
	 * \brief Finds a point in space given its grid coordinates.
	 * \param i The grid coordinate of the point in the x direction.
	 * \param j The grid coordinate of the point in the y direction.
	 * \param k The grid coordinate of the point in the z direction.
	 * \return The point in space.
	 */
	Point point(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief The current calculated density at a grid vertex.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current density.
	 */
	double density(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief The current calculated velocity at a grid vertex.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current velocity.
	 */
	Vector velocity(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief The status of a cell, identified by its lower-left-front vertex.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current status.
	 */
	Status status(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief Sets the water bottom.
	 * \param bottom The HeightField that is the bottom of the simulation.
	 */
	void setBottom(const HeightField* const bottom);

	/*!
	 * \brief Queries the diffusion rate of the fluid.
	 * \return The diffusion rate.
	 */
	double diffuse() const;

	/*!
	 * \brief Sets the diffusion rate of the fluid.
	 * \param diff The new diffusion rate.
	 */
	void setDiffuse(double diff);

	/*!
	 * \brief Queries the viscosity of the fluid.
	 * \return The viscosity.
	 */
	double viscosity() const;

	/*!
	 * \brief Sets the viscosity of the fluid.
	 * \param visc The new viscosity.
	 */
	void setViscosity(double visc);

	/*!
	 * \brief Updates the water volume state.
	 * \param time The time slice.
	 */
	void evolve(unsigned long time);

private:
	// method to map 3d indices into linear array
	unsigned i3d(unsigned i, unsigned j, unsigned k) const;

	// translates points to grid positions
	bool locate(const Point& p, unsigned* i, unsigned* j, unsigned* k) const;

	/*!
	 * \brief Classify cells given their left-bottom-front vertex.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The status of the cell.
	 */
	Status classifyCell(unsigned i, unsigned j, unsigned k) const;

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
	// list of cells at boundary
	std::vector<Status> _status;
	// density in each element
	DoubleVector _dens;
	// previous density
	DoubleVector _dens_prev;
	// velocity components, in each direction
	DoubleVector _u, _v, _w;
	// previous velocity components
	DoubleVector _u_prev, _v_prev, _w_prev;
};

inline WaterVolume::WaterVolume()
	: WaterBase(), _step(0.0), _diff(0.5), _visc(1.0), _size(0)
{
}

inline WaterVolume::~WaterVolume()
{
}

inline unsigned WaterVolume::size() const
{
	return _size;
}

inline Point WaterVolume::point(unsigned i, unsigned j, unsigned k) const
{
	return Point(_origin.x() + i * _step,
					_origin.y() + j * _step, _origin.z() + k * _step);
}

inline double WaterVolume::density(unsigned i, unsigned j, unsigned k) const
{
	Locker(this);

	return _dens[i3d(i, j, k)];
}

inline Vector WaterVolume::velocity(unsigned i, unsigned j, unsigned k) const
{
	Locker(this);

	unsigned l = i3d(i, j, k);

	return Vector(_u[l], _v[l], _w[l]);
}

inline WaterVolume::Status WaterVolume::status(unsigned i, unsigned j, unsigned k) const
{
	return _status[i3d(i, j, k)];
}

inline double WaterVolume::diffuse() const
{
	return _diff;
}

inline void WaterVolume::setDiffuse(double diff)
{
	_diff = diff;
}

inline double WaterVolume::viscosity() const
{
	return _visc;
}

inline void WaterVolume::setViscosity(double visc)
{
	_visc = visc;
}

inline unsigned WaterVolume::i3d(unsigned i, unsigned j, unsigned k) const
{
	if(i >= _size || j >= _size || k >= _size ) {
		throw std::out_of_range("invalid grid coordinates");
	}

	return k * Orbis::Math::sqr(_size) + j * _size + i;
}

} } // namespace declarations

#endif  // __ORBIS_WATERVOLUME_HPP__
