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
 * $Id: vector.hpp,v 1.1 2003/07/18 03:11:49 asandro Exp $
 */

#ifndef __ORBIS_VECTOR_HPP__
#define __ORBIS_VECTOR_HPP__

#include <math.hpp>
#include <point.hpp>

namespace Orbis {

	namespace Util {

/*!
 * \file vector.hpp
 * \brief Contains the Vector class
 */

/*!
 * \class Vector
 * \brief This class represents a tridimensional vector.
 *
 * The Vector class represents a vector in space. Vectors have length,
 * may be normalised, summed, subtracted and their dot and cross products
 * may be calculated.
 */
class Vector {
public:
	//! Vector's constructor
	/*!
	 * Creates a new vector, with null components by default
	 * \param x Vector's x component
	 * \param y Vector's y component
	 * \param z Vector's z component
	 */
	Vector(double x = 0.0, double y = 0.0, double z = 0.0);

	//! Destructor
	~Vector();

	//! Reads x component
	/*!
	 * \return Vector's x component
	 */
	double x() const;

	//! Reads y component
	/*!
	 * \return Vector's y component
	 */
	double y() const;

	//! Reads z component
	/*!
	 * \return Vector's z component
	 */
	double z() const;

	//! Calculates the vector's length
	/*!
	 * \return the vector's length
	 */
	double length() const;

	//! Calculates the vector's squared length
	/*!
	 * \return the vector's squared length
	 */
	double sqrLength() const;

	Vector normalise() const;

	// scaling
	Vector& operator*=(double);

	// adding and subtracting
	Vector& operator+=(const Vector&);
	Vector& operator-=(const Vector&);

	// cross product
	Vector& operator^=(const Vector&);

private:
	// vector components
	double _x, _y, _z;
};

inline Vector::Vector(double x, double y, double z)
	: _x(x), _y(y), _z(z)
{
}

inline Vector::~Vector()
{
}

inline double Vector::x() const
{
	return _x;
}

inline double Vector::y() const
{
	return _y;
}

inline double Vector::z() const
{
	return _z;
}

inline double Vector::length() const
{
	return sqrt(_x*_x + _y*_y + _z*_z);
}

inline double Vector::sqrLength() const
{
	return _x*_x + _y*_y + _z*_z;
}

inline Vector Vector::normalise() const
{
	double mag = length();
	if(mag > 0.0001) {
		return Vector(_x/mag, _y/mag, _z/mag);
	} else {
		return Vector();
	}
}

inline Vector& Vector::operator*=(double d)
{
	_x *= d;
	_y *= d;
	_z *= d;

	return *this;
}

inline Vector& Vector::operator+=(const Vector& v)
{
	_x += v._x;
	_y += v._y;
	_z += v._z;

	return *this;
}

inline Vector& Vector::operator-=(const Vector& v)
{
	_x -= v._x;
	_y -= v._y;
	_z -= v._z;

	return *this;
}

inline Vector& Vector::operator^=(const Vector& v)
{
	double x, y, z;

	x = this->_y * v._z - this->_z * v._y;
	y = this->_z * v._x - this->_x * v._z;
	z = this->_x * v._y - this->_y * v._x;

	this->_x = x; this->_y = y; this->_z = z;
	return *this;
}

inline bool operator==(const Vector& v1, const Vector& v2)
{
	return Orbis::Math::areEqual(v1.x(), v2.x()) &&
	       Orbis::Math::areEqual(v1.y(), v2.y()) &&
	       Orbis::Math::areEqual(v1.z(), v2.z());
}

inline Vector operator-(const Vector& v)
{
	return Vector(-v.x(), -v.y(), -v.z());
}

inline Vector operator*(double d, const Vector& v)
{
	Vector r  = v;
	return r *= d;
}

inline Vector operator+(const Vector& v1, const Vector& v2)
{
	Vector r  = v1;
	return r += v2;
}

inline Vector operator-(const Vector& v1, const Vector& v2)
{
	Vector r  = v1;
	return r -= v2;
}

// dot product
inline double operator*(const Vector& v1, const Vector& v2)
{
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline double operator*(const Point& p, const Vector& v)
{
	return p.x() * v.x() + p.y() * v.y() + p.z() * v.z();
}

inline Vector operator^(const Vector& v1, const Vector& v2)
{
	Vector r  = v1;
	return r ^= v2;
}

inline Vector operator-(const Point& p1, const Point&p2)
{
	return Vector(p1.x()-p2.x(), p1.y()-p2.y(), p1.z()-p2.z());
}

inline Point operator+(const Point& p, const Vector& v)
{
	return Point(p.x()+v.x(), p.y()+v.y(), p.z()+v.z());
}

inline Point operator-(const Point& p, const Vector& v)
{
	return Point(p.x()-v.x(), p.y()-v.y(), p.z()-v.z());
}

} } // namespace declarations

#endif // __ORBIS_VECTOR_HPP__
