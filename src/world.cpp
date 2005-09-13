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
 *
 * $Id: world.cpp,v 1.20 2004/05/24 15:19:33 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <osg/Geode>
#include <osg/Transform>

#include <world.hpp>
#include <worldview.hpp>

using osg::Geode;
using osg::Transform;

namespace Orbis {

World* World::_world = 0;

World::World()
{
	OpenThreads::Thread::Init();

	_root = new osg::Group;

	reset();

	_timer.start();
}

World::~World()
{
}

void World::reset()
{
	_timer.reset();
//	_lua_script.reset();
//	_timer.addDynamic(&_lua_script);

	// removing all children
	_root->removeChild(0, _root->getNumChildren());

	/* adding sky
	osg::Node* sky = osgDB::readNodeFile("skydome.osg");
	if(sky) {
		_root->addChild(sky);
	}
	*/
}

void World::notifyViews()
{
	std::vector<Orbis::WorldView*>::iterator it;

	for(it = _views_list.begin(); it != _views_list.end(); it++) {
		   (*it)->worldHasChanged();
	}
}

osg::Drawable* World::objectByName(const std::string& name) const
{ 
	std::map<std::string, osg::Drawable*>::const_iterator it;

	it = _names.find(name);
	if(it != _names.end()) {
		return it->second;
	} else {
		return 0;
	}
}

void World::addDrawable(Drawable::Drawable *drawable,
			 			Transform *transform, std::string name)
{
	// adding all to top-level group for now
	Geode *geo = new Geode;
	geo->addDrawable(drawable);
	if(transform) {
		transform->addChild(geo);
		_root->addChild(transform);
	} else {
		_root->addChild(geo);
	}

	if(!name.empty()) {
		_names[name] = drawable;
	}

	//notifyViews();
}

void World::addGroup(osg::Group *group, Transform *transform)
{
	// adding all to top-level group for now
	if(transform) {
		transform->addChild(group);
		_root->addChild(transform);
	} else {
		_root->addChild(group);
	}

	//notifyViews();
}

void World::runFile(const std::string& filename)
{
//	_lua_script.runFile(filename);

	//notifyViews();
}

bool World::removeView(Orbis::WorldView *view)
{
	std::vector<Orbis::WorldView*>::iterator it;

	if(!view) {
		return false;
	}
	it = find(_views_list.begin(), _views_list.end(), view);
	if(it == _views_list.end()) {
		return false;
	} else {
		_views_list.erase(it);
	}

	return true;
}

} // namespace declarations
