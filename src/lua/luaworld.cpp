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

#ifdef __GNUG__
#pragma implementation
#endif

#include <world.hpp>
#include <luaworld.hpp>
#include <worldview3d.hpp>

namespace Orbis {

namespace Script {

#define method(class, name) {#name, class::name}

const luaL_reg LuaWorld::methods[] = {
  method(LuaWorld, reset),
  method(LuaWorld, start),
  method(LuaWorld, stop),
  method(LuaWorld, root),
  method(LuaWorld, view),
  method(LuaWorld, timeStep),
  method(LuaWorld, setTimeStep),
  method(LuaWorld, objectByName),
  method(LuaWorld, addDrawable),
  method(LuaWorld, addGroup),
  {0,0}
};

int LuaWorld::reset(lua_State *L)
{
	World *world = World::instance();
	world->reset();

	return 0;
}

int LuaWorld::start(lua_State *L)
{
	World *world = World::instance();
	world->start();

	return 0;
}

int LuaWorld::stop(lua_State *L)
{
	World *world = World::instance();
	world->stop();

	return 0;
}

/* pushs the World's root node onto Lua stack. */
int LuaWorld::root(lua_State *L)
{
	World *world = World::instance();

	return 1;
}

/* pushs one of the World's views, given its index, onto the Lua stack */
int LuaWorld::view(lua_State *L)
{
	World *world = World::instance();
	double index = luaL_checknumber(L, 1);

	if(index < 0) {
		luaL_argerror(L, 1, "view index must be non-negative");
	}
	WorldView *wv = world->view(static_cast<unsigned>(index));

	/*
	 * If this view is a 3D view I can't delete it when it gets out of
	 * the Lua scope, for it belongs to the toolkit. I then add a
	 * reference to it in the Lua registry. Must think of a better
	 * way of sync this two worlds later.
	 */
	if(wv) {
		lua_pushlightuserdata(L, wv);
		lua_gettable(L, LUA_REGISTRYINDEX);
		if(!lua_isuserdata(L, -1) || lua_islightuserdata(L, -1)) {
			lua_pop(L, 1);

			lua_boxpointer(L, wv);
			luaL_getmetatable(L, "WorldView");
			lua_setmetatable(L, -2);

			if(dynamic_cast<WorldView3D*>(wv)) {
				lua_pushlightuserdata(L, wv);
				lua_pushvalue(L, -2);
				lua_settable(L, LUA_REGISTRYINDEX);
			}
		}
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int LuaWorld::timeStep(lua_State *L)
{
	lua_pushnumber(L, World::instance()->timeStep());

	return 1;
}

int LuaWorld::setTimeStep(lua_State *L)
{
	double tstep = luaL_checknumber(L, 1);

	if(tstep < 0) {
		luaL_argerror(L, 1, "time step must be positive");
	} else {
		World::instance()->setTimeStep(static_cast<unsigned long>(tstep));
	}

	return 0;
}

int LuaWorld::objectByName(lua_State *L)
{
	osg::Drawable *drw;
	World *world = World::instance();
	const char *name = luaL_checklstring(L, 2, 0);

	drw = world->objectByName(name);
	if(drw) {
	} else {
		lua_pushnil(L);
	}

	return 1;
}

/* adds a new drawable to the World. */
int LuaWorld::addDrawable(lua_State *L)
{
	World *world = World::instance();

	return 0;
}

/* adds a whole group to the World. */
int LuaWorld::addGroup(lua_State *L)
{
	World *world = World::instance();

	return 0;
}

/* register the World's binding to Lua */
void LuaWorld::registerIntoLua(lua_State *L)
{
	luaL_openlib(L, "World", methods, 0);
}

} } // namespace declarations

