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

#include <stdexcept>

#include <waterbase.hpp>

namespace Orbis {

	namespace Drawable {
	   
/*!
 * \brief This class encapsulates the simulation of the behaviour of
 * a mass of water.
 */
class WaterVolume : public WaterBase {
public:
	/*!
	 * \brief Default constructor.
	 */
	WaterVolume();

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
	WaterVolume(const Orbis::Util::Point& origin,
					unsigned size_x, unsigned size_y, unsigned size_z,
							double step_x, double step_y, double step_z);

	/*!
	 * \brief Destructor.
	 */
	virtual ~WaterVolume();

	/*!
	 * \brief The origin, or lower-left-front vertex of the volume.
	 * \return The origin point.
	 */
	Point origin() const;

	/*!
	 * \brief The number of elements in the x direction.
	 * \return The number of elements.
	 */
	unsigned sizeX() const;

	/*!
	 * \brief The number of elements in the y direction.
	 * \return The number of elements.
	 */
	unsigned sizeY() const;

	/*!
	 * \brief The number of elements in the z direction.
	 * \return The number of elements.
	 */
	unsigned sizeZ() const;

	/*!
	 * \brief The size of an element in the x direction.
	 * \return The size of the element.
	 */
	double stepX() const;

	/*!
	 * \brief The size of an element in the y direction.
	 * \return The size of the element.
	 */
	double stepY() const;

	/*!
	 * \brief The size of an element in the z direction.
	 * \return The size of the element.
	 */
	double stepZ() const;

	/*!
	 * \brief Finds a point in space given its grid coordinates.
	 * \param i The grid coordinate of the point in the x direction.
	 * \param j The grid coordinate of the point in the y direction.
	 * \param k The grid coordinate of the point in the z direction.
	 * \return The point in space.
	 */
	Point point(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief The current calculated density in a cell.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current density.
	 */
	virtual double density(unsigned i, unsigned j, unsigned k) const = 0;

	/*!
	 * \brief The current calculated pressure in a cell.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current pressure.
	 */
	virtual double pressure(unsigned i, unsigned j, unsigned k) const = 0;

	/*!
	 * \brief The current calculated velocity in a cell.
	 * \param i The grid coordinate of the vertex in the x direction.
	 * \param j The grid coordinate of the vertex in the y direction.
	 * \param k The grid coordinate of the vertex in the z direction.
	 * \return The current velocity.
	 */
	virtual Vector velocity(unsigned i, unsigned j, unsigned k) const = 0;

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

protected:
	// method to map 3d indices into linear array
	unsigned i3d(unsigned i, unsigned j, unsigned k) const;

	// translates points to grid positions
	bool locate(const Point& p, unsigned* i, unsigned* j, unsigned* k) const;

private:
	// viscosity of the fluid
	double _visc;
	// origin of grid
	Orbis::Util::Point _origin;
	// grid spacing
	double _step_x, _step_y, _step_z;
	// number of elements
	unsigned _size_x, _size_y, _size_z;
};

inline WaterVolume::WaterVolume()
	: WaterBase(), _visc(0.001),
		_step_x(0.0), _step_y(0.0), _step_z(0.0),
					_size_x(0), _size_y(0), _size_z(0)
{
}

inline WaterVolume::WaterVolume(const Orbis::Util::Point& origin,
							unsigned size_x, unsigned size_y, unsigned size_z,
									double step_x, double step_y, double step_z)
	: WaterBase(), _visc(0.001),
		_step_x(step_x), _step_y(step_y), _step_z(step_z),
					_size_x(size_x), _size_y(size_y), _size_z(size_z)
{
}

inline WaterVolume::~WaterVolume()
{
}

inline Point WaterVolume::origin() const
{
	return _origin;
}

inline unsigned WaterVolume::sizeX() const
{
	return _size_x;
}

inline unsigned WaterVolume::sizeY() const
{
	return _size_y;
}

inline unsigned WaterVolume::sizeZ() const
{
	return _size_z;
}

inline double WaterVolume::stepX() const
{
	return _step_x;
}

inline double WaterVolume::stepY() const
{
	return _step_y;
}

inline double WaterVolume::stepZ() const
{
	return _step_z;
}

inline Point WaterVolume::point(unsigned i, unsigned j, unsigned k) const
{
	return Point(_origin.x() + i * _step_x,
					_origin.y() + j * _step_y,
					_origin.z() + k * _step_z);
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
	if(i >= _size_x || j >= _size_y || k >= _size_z ) {
		throw std::out_of_range("invalid grid coordinates");
	}

	return k * _size_x * _size_y + j * _size_x + i;
}

} } // namespace declarations

#endif  // __ORBIS_WATERVOLUME_HPP__
