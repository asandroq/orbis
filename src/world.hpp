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

#ifndef __ORBIS_WORLD_HPP__
#define __ORBIS_WORLD_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <map>
#include <vector>

#include <osg/Group>

#include <timer.hpp>
#include <drawable.hpp>
#include <luascript.hpp>

using osg::Node;
using osg::Group;
using osg::ref_ptr;
using osg::Transform;

namespace Orbis {

class WorldView;
	   
/**
 * \brief This class is a high-level representation of the entire world.
 *
 * Design patterns: entity, singleton
 */

class World {
public:
	//! Access the World's unique instance.
	/*!
	 * \return The World instance.
	 */
	static World* instance();

	//! Resets the World.
	void reset();

	//! Starts the simulation's timer.
	void start();

	//! Stops the simulation's timer.
	void stop();

	//! Accesses the scene graph.
	/*!
	 * \return The scene graph root node.
	 */
	ref_ptr<Group> root() const;

	/*!
	 * \brief Accesses one of the World's views.
	 * \param i The index of the desired view.
	 */
	WorldView* view(unsigned i) const;

	/*!
	 * \brief Gets the current time step of the simulation.
	 * \return The time step, in miliseconds.
	 */
	unsigned long timeStep() const;

	/*!
	 * \brief Sets the time step of the simulation.
	 * \param tstep The new time step, in miliseconds.
	 */
	void setTimeStep(unsigned long tstep);

	/*!
	 * \brief Sets the Lua action adapter.
	 * \param adapter The new LuaActionAdapter.
	 */
	void setLuaActionAdapter(Orbis::Script::LuaActionAdapter *adapter);

	/*!
	 * \brief Retrieves a pointer to a named object.
	 * \param name The name of the object.
	 * \return A pointer to the object.
	 */
	osg::Drawable* objectByName(const std::string& name) const;

	//! Adds a drawable to the world.
	/*!
	 * \param drawable The new drawable added.
	 * \param transform An optional transformation to be applied.
	 */
	void addDrawable(Drawable::Drawable *drawable,
				 	Transform *transform = 0, std::string name = "");

	//! Adds an entire group to the world.
	/*!
	 * \param group The new group added.
 	 * \param transform An optional transformation to be applied.
	 */
	void addGroup(osg::Group *group, Transform *transform = 0);

	//! Adds a dynamic object to be updated constantly.
	/*!
	 * \param obj Dynamic object that will receive timely notices.
	 */
	void addDynamic(Orbis::Dynamic* obj);

	/*!
	 * \brief Adds a WorldView to the list of objects which will be
	 * notified when the World changes.
	 * \param viewer The viewer which will receive notices.
	 */
	void addView(Orbis::WorldView *view);

	/*!
	 * \brief Removes a WorldView of this World.
	 * \param view The WorldView to be removed.
	 */
	bool removeView(Orbis::WorldView *view);

	//! Notifies all views when the World changes.
	void notifyViews();

	//! Runs a script file.
	/*!
	 * \param filename The script's file name.
	 */
	void runFile(const std::string& filename);

private:
	//! Default constructor
	World();

	//! Destructor
	~World();

	//! Root of the scene graph
	ref_ptr<Group> _root;

	//! Global timer
	Orbis::Util::Timer _timer;

	//! Scripting engine
	Orbis::Script::LuaScript _lua_script;

	//! List of objects watching this world
	std::vector<Orbis::WorldView*> _views_list;

	//! Map from names to objects
	std::map<std::string, osg::Drawable*> _names;
};

inline void World::start()
{
	   _timer.setActive(true);
}

inline void World::stop()
{
	   _timer.setActive(false);
}

inline ref_ptr<Group> World::root() const
{
	return _root;
}

inline WorldView* World::view(unsigned i) const
{
	if(i < _views_list.size()) {
		return _views_list[i];
	} else {
		return 0;
	}
}

inline unsigned long World::timeStep() const
{
	return _timer.timeout();
}

inline void World::setTimeStep(unsigned long tstep)
{
	_timer.setTimeout(tstep);
}

inline void World::setLuaActionAdapter(Orbis::Script::LuaActionAdapter *adapter)
{
	_lua_script.setActionAdapter(adapter);
}

inline void World::addDynamic(Orbis::Dynamic* obj)
{
	if(obj) {
		_timer.addDynamic(obj);
	}
}

inline void World::addView(Orbis::WorldView *view)
{
	if(view) {
		   _views_list.push_back(view);
	}
}

} // namespace declarations

#endif // __ORBIS_WORLD_HPP__

