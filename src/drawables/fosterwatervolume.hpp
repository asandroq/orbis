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
 
#ifndef __ORBIS_FOSTERWATERVOLUME_HPP__
#define __ORBIS_FOSTERWATERVOLUME_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <list>

#include <watervolume.hpp>

namespace Orbis {

	namespace Drawable {
	   
/*!
 * \brief This class encapsulates the simulation of the behaviour of
 * a mass of water.
 * 
 * The algorythm used here is the one developed by Foster and Metaxas in 1996.
 * The volume is divided in cubic cells.
 */
class FosterWaterVolume : public WaterVolume {
public:
	/*!
	 * \brief The status of each cell.
	 */
	enum Status {
		EMPTY,			//!< The cell is empty.
		FULL,			//!< The cell is full of fluid
		SOLID,			//!< The cell is a solid obstacle.
		SOURCE,			//!< The cell contains a source of fluid.
		SURFACE			//!< The cell is at the fluid boundary.
	};

	/*!
	 * \brief Massless particle used to track surface.
	 */
	class Particle {
	public:
		/*!
		 * \brief Default constructor.
		 */
		Particle() {}

		/*!
		 * \brief Constructor.
		 */
		Particle(const Point& p) : _pos(p) {}

		/*!
		 * \brief Destructor.
		 */
		~Particle() {}

		/*!
		 * \brief Queries the position of the particle.
		 * \return The position.
		 */
		Point pos() const { return _pos; }

		/*!
		 * \brief Sets new particle position.
		 */
		void setPos(const Point& p) { _pos = p; }

	private:
		// position of this particle
		Point _pos;
	};

	/*!
	 * \brief The type of a list of particles.
	 */
	typedef std::list<Particle> ParticleList;

	/*!
	 * \brief Default constructor.
	 */
	FosterWaterVolume();

	/*!
	 * \brief Most-used constructor.
	 * \param origin Lower-left-front corner of the volume.
	 * \param size_x Number of elements of volume, in the x direction.
	 * \param size_y Number of elements of volume, in the y direction.
	 * \param size_z Number of elements of volume, in the z direction.
	 * \param step_x The size of one element, in the x direction.
	 * \param step_y The size of one element, in the y direction.
	 * \param step_z The size of one element, in the z direction.
	 */
	FosterWaterVolume(const Orbis::Util::Point& origin,
					unsigned size_x, unsigned size_y, unsigned size_z,
							double step_x, double step_y, double step_z);

	/*!
	 * \brief Destructor.
	 */
	~FosterWaterVolume();

	/*!
	 * \brief Queries the status of a cell.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current status.
	 */
	Status status(unsigned i, unsigned j, unsigned k) const;

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
	 * \brief Tells the simulation this cell is solid.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 */
	void setSolid(unsigned i, unsigned j, unsigned k);

	/*!
	 * \brief Updates the water volume state.
	 * \param time The time slice.
	 */
	void evolve(unsigned long time);

private:
	/*!
	 * \brief The current velocity at any point.
	 * \param p The point where the velocity is needed.
	 * \return The current velocity.
	 */
	Vector velocity(const Point& p) const;

	/*!
	 * \brief Tests if a cell has an empty neighbour.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return True if there is an empty neighbour, false otherwise.
	 */
	bool any_empty_neighbour(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief Classify all cells in grid.
	 */
	void classifyAll();

	/*!
	 * \brief Sets the solid boundary conditions.
	 * \param slip Tells if the boundary cells are slip or non-slip.
	 */
	void set_bounds(bool slip);

	/*!
	 * \brief Updates the surface of the water.
	 * \param dt The time step.
	 */
	void update_surface(double dt);

	/*!
	 * \brief Update the velocities of the FULL cells.
	 * \param g The gravity vector.
	 * \param dt The time step.
	 */
	void update_velocity(const Vector& g, double dt);

	/*!
	 * \brief Update the pressure of the cells.
	 * \param dt The time step.
	 */
	void update_pressure(double dt);

	// atmosferic pressure
	double _atm_p;
	// pressure within the fluid
	DoubleVector _p, _p_prev;
	// status of each cell
	std::vector<Status> _status, _status_prev;
	// acceleration in each face
	DoubleVector _acc_x, _acc_y, _acc_z;
	// velocity components
	DoubleVector _u, _u_prev, _v, _v_prev, _w, _w_prev;
	// list of particles per cell
	std::vector<ParticleList> _part_lists;
};

inline FosterWaterVolume::FosterWaterVolume()
	: WaterVolume(), _atm_p(1.0)
{
}

inline FosterWaterVolume::~FosterWaterVolume()
{
}

inline double FosterWaterVolume::density(unsigned i, unsigned j, unsigned k) const
{
	return 0.0;
}

inline FosterWaterVolume::Status FosterWaterVolume::status(unsigned i, unsigned j, unsigned k) const
{
	return _status_prev[i3d(i, j, k)];
}

inline double FosterWaterVolume::pressure(unsigned i, unsigned j, unsigned k) const
{
	return _p_prev[i3d(i, j, k)];
}

inline void FosterWaterVolume::setSolid(unsigned i, unsigned j, unsigned k)
{
	_status_prev[i3d(i, j, k)] = SOLID;
}

} } // namespace declarations

#endif // __ORBIS_FOSTERWATERVOLUME_HPP__
