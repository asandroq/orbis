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
 * $Id: worldview.hpp,v 1.4 2004/03/15 02:55:21 asandro Exp $
 */

#ifndef __ORBIS_WORLDVIEW_HPP__
#define __ORBIS_WORLDVIEW_HPP__

#include <world.hpp>
#include <camera.hpp>

namespace Orbis {

/*!
 * \brief This is the base class of all "views" into the World.
 */
class WorldView {
public:
	//! Default constructor
	/*!
	 * \param world a World instance to render
	 */
	WorldView(World* world);

	//! Destructor
	virtual ~WorldView();

	//! The world this class will render
	/*!
	 * \returns a pointer to the World instance
	 */
	World* world() const;

	/*!
	 * \brief Gets the camera of this view.
	 */
	virtual Orbis::Util::Camera camera() const;

	/*!
	 * \brief Sets the camera of this view.
	 * \param cam The new Camera.
	 */
	virtual void setCamera(const Orbis::Util::Camera& cam);

	//! Renders the World
	virtual void render() = 0;

	//! Notifies this class that the World has changed somehow.
	virtual void worldHasChanged() = 0;

private:
	// the world to render
	World *_world;
	// a camera to define the observer's position
	Orbis::Util::Camera _camera;
};

inline WorldView::WorldView(World* world)
	: _world(world)
{
	_world->addView(this);
}

inline WorldView::~WorldView()
{
	_world->removeView(this);
}

inline World* WorldView::world() const
{
	return _world;
}

inline Orbis::Util::Camera WorldView::camera() const
{
	return _camera;
}

inline void WorldView::setCamera(const Orbis::Util::Camera& cam)
{
	_camera = cam;
}

} // namespace delarations

#endif  // __ORBIS_WORLDVIEW_HPP__

