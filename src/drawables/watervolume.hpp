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
#include <drawable.hpp>
#include <waterbase.hpp>

namespace Orbis {

namespace Drawable {
	   
/*!
 * \brief This class represents a water volume composed of
 * several finite elements.
 */
class WaterVolume : public WaterBase , public Drawable {
public:
	/*!
	 * \brief Default constructor.
	 */
	WaterVolume();

	/*!
	 * \brief Copy constructor.
	 * \param field The original water volume field.
	 * \param copyOp Tells how the copy must be done.
	 */
	WaterVolume(const WaterVolume& src,
				const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

	/*!
	 * \brief Most-used constructor.
	 * \param origin Bottom-left-front corner of the volume.
	 * \param size Number of elements of volume.
	 * \param step The size of one element.
	 */
	WaterVolume(const Orbis::Util::Point& origin, unsigned size, double step);

	/*!
	 * \brief Clones this class type.
	 * \return A pointer to an object of this class.
	 */
	virtual osg::Object* cloneType() const;

	/*!
	 * \brief Clones this class instance.
	 * \param copyOp The way the copy must be done.
	 * \return A pointer to a clone of this object.
	 */
	virtual osg::Object* clone(const osg::CopyOp& copyOp) const;

	/*!
	 * \brief Tests if two objects have the same type.
	 * \param obj The other object to compare to.
	 * \return true if same kind, false otherwise.
	 */
	virtual bool isSameKindAs(const osg::Object* obj) const;

	/*!
	 * \brief The name of the library this class belongs to.
	 * \return The name of the library.
	 */
	virtual const char* libraryName() const;

	/*!
	 * \brief The name of this class.
	 * \return The name of the class.
	 */
	virtual const char* className() const;

	/*!
	 * \brief Finds a point in space given its grid coordinates.
	 * \param i The grid coordinate of the point in the x direction.
	 * \param j The grid coordinate of the point in the y direction.
	 * \param k The grid coordinate of the point in the k direction.
	 * \return The point in space.
	 */
	Point point(unsigned i, unsigned j, unsigned k) const;

	/*!
	 * \brief Sets the water bottom.
	 * \param bottom The HeightField that is the bottom of the simulation.
	 */
	void setBottom(const HeightField* const bottom);

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
	unsigned i3d(unsigned i, unsigned j, unsigned k) const;
	// translates points to grid positions
	bool locate(const Point& p, unsigned* i, unsigned* j, unsigned* k) const;
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
	std::vector<unsigned> _bound_list;
	// density in each element
	DoubleVector _dens;
	// previous density
	DoubleVector _dens_prev;
	// velocity components, in each direction
	DoubleVector _u, _v, _w;
	// previous velocity components
	DoubleVector _u_prev, _v_prev, _w_prev;
};

inline osg::Object* WaterVolume::cloneType() const
{
	return new WaterVolume;
}

inline osg::Object* WaterVolume::clone(const osg::CopyOp& copyOp) const
{
	return new WaterVolume(*this, copyOp);
}

inline bool WaterVolume::isSameKindAs(const osg::Object* obj) const
{
	return dynamic_cast<const WaterVolume*>(obj) != 0;
}

inline const char* WaterVolume::libraryName() const
{
	return "Orbis";
}

inline const char* WaterVolume::className() const
{
	return "WaterVolume";
}

inline Point WaterVolume::point(unsigned i, unsigned j, unsigned k) const
{
	return Point(_origin.x() + i * _step,
					_origin.y() + j * _step, _origin.z() + k * _step);
}

inline unsigned WaterVolume::i3d(unsigned i, unsigned j, unsigned k) const
{
	return k * Orbis::Math::sqr(_size) + j * _size + i;
}

} } // namespace declarations

#endif  // __ORBIS_WATERVOLUME_HPP__
