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
 * $Id: camera.hpp,v 1.3 2004/02/26 19:29:45 asandro Exp $
 */

#ifndef __ORBIS_CAMERA_HPP__
#define __ORBIS_CAMERA_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <point.hpp>
#include <vector.hpp>

/*!
 * \file camera.hpp
 * \brief Contains the interface of the Camera class.
 */

namespace Orbis {

namespace Util {

/*!
 * \class Camera
 * \brief This class represents a simple camera.
 *
 */
class Camera {
public:
	//! Camera constructor.
	/*!
	 * Creates a new camera, by default located at the origin looking
	 * down the positive y-axis.
	 * \param loc The camera location's in space.
	 * \param view The direction the Camera is aiming to.
	 * \param up The Camera's up vector.
	 */
	Camera(const Point& loc = Point(0.0, 0.0, 0.0),
			const Vector& view = Vector(0.0, 1.0, 0.0),
			const Vector& up = Vector(0.0, 0.0, 1.0));

	//! Camera destructor.
	~Camera();

	//! Gets the Camera's location.
	/*!
	 * \return The Camera's location.
	 */ 
	Point location() const;

	//! Sets the Camera's location.
	/*!
	 * \param loc The new Camera's location.
	 */
	void setLocation(const Point& loc);

	//! Gets the direction the Camera is aiming to.
	/*!
	 * \return The Camera's view direction.
	 */ 
	Vector view() const;

	//! Sets the direction the Camera is aiming to.
	/*!
	 * \param view The Camera's new view direction.
	 */
	void setView(const Vector& view);

	//! Gets the Camera's up vector.
	/*!
	 * \return The Camera's up vector.
	 */ 
	Vector up() const;

	//! Sets the Camera's up vector.
	/*!
	 * \param up The Camera's new up vector.
	 */
	void setUp(const Vector& up);

private:
	// location
	Point _loc;

	// view direction
	Vector _view;

	// up vector
	Vector _up;
};

inline Camera::Camera(const Point& loc, const Vector& view, const Vector& up)
	: _loc(loc), _view(view), _up(up)
{
}

inline Camera::~Camera()
{
}

inline Point Camera::location() const
{
	return _loc;
}

inline void Camera::setLocation(const Point& loc)
{
	_loc = loc;
}

inline Vector Camera::view() const
{
	return _view;
}

inline void Camera::setView(const Vector& view)
{
	_view = view;
}

inline Vector Camera::up() const
{
	   return _up;
}

inline void Camera::setUp(const Vector& up)
{
	_up = up;
}

} } // namespace declarations

#endif // __ORBIS_CAMERA_HPP__

