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
 * $Id: luacamera.cpp,v 1.2 2004/02/18 18:55:07 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <luapoint.hpp>
#include <luavector.hpp>
#include <luacamera.hpp>

namespace Orbis {

namespace Script {

const char LuaCamera::className[] = "Camera";

#define method(class, name) {#name, class::name}

const luaL_reg LuaCamera::methods[] = {
	method(LuaCamera, location),
	method(LuaCamera, setLocation),
	method(LuaCamera, view),
	method(LuaCamera, setView),
	method(LuaCamera, up),
	method(LuaCamera, setUp),
	{0, 0}
};

/* register this class to the Lua interpreter */
void LuaCamera::registerIntoLua(lua_State* L)
{
	lua_newtable(L);
	int methodtable = lua_gettop(L);
	luaL_newmetatable(L, className);
	int metatable = lua_gettop(L);

	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, methodtable);
	// hide metatable from Lua getmetatable()
	lua_settable(L, metatable);

	lua_pushliteral(L, "__index");
	lua_pushvalue(L, methodtable);
	lua_settable(L, metatable);

	lua_pushliteral(L, "__gc");
	lua_pushcfunction(L, collect);
	lua_settable(L, metatable);

	// drop metatable
	lua_pop(L, 1);

	// fill methodtable
	luaL_openlib(L, 0, methods, 0);
	// drop methodtable
	lua_pop(L, 1);

	lua_register(L, className, create);
}

/* checks if element at given index is a Camera */
Camera* LuaCamera::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, className);
	if(!ud) {
		luaL_typerror(L, index, className);
	}

	return *(Camera**)ud;  // unbox pointer
}

/* creates a new instance of a Camera */
int LuaCamera::create(lua_State* L)
{
	Camera *c;

	// checks for args
	if(lua_type(L, 1) == LUA_TUSERDATA) {
		Point *loc = LuaPoint::checkInstance(L, 1);
		if(lua_type(L, 2) == LUA_TUSERDATA) {
			Vector *view = LuaVector::checkInstance(L, 2);
			if(lua_type(L, 3) == LUA_TUSERDATA) {
				Vector *up = LuaVector::checkInstance(L, 3);
				c = new Camera(*loc, *view, *up);
			} else {
				c = new Camera(*loc, *view);
			}
		} else {
			c = new Camera(*loc);
		}
	} else {
		c = new Camera;
	}

	lua_boxpointer(L, c);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
}

/* collects the memory of a Camera */
int LuaCamera::collect(lua_State* L)
{
	Camera *c = (Camera*) lua_unboxpointer(L, 1);

	delete c;

	return 0;
}

/* gets the location of the Camera */
int LuaCamera::location(lua_State* L)
{
	Camera *c = checkInstance(L, 1);

	Point *p = new Point;
	*p = c->location();

	lua_boxpointer(L, p);
	luaL_getmetatable(L, "Point");
	lua_setmetatable(L, -2);

	return 1;
}

/* sets the location of the Camera */
int LuaCamera::setLocation(lua_State* L)
{
	Camera *c = checkInstance(L, 1);
	Point *p = LuaPoint::checkInstance(L, 2);

	c->setLocation(*p);

	return 0;
}

/* gets the view direction of the Camera */
int LuaCamera::view(lua_State* L)
{
	Camera *c = checkInstance(L, 1);

	Vector *v = new Vector;
	*v = c->view();

	lua_boxpointer(L, v);
	luaL_getmetatable(L, "Vector");
	lua_setmetatable(L, -2);

	return 1;
}

/* sets the view direction of the Camera */
int LuaCamera::setView(lua_State* L)
{
	Camera *c = checkInstance(L, 1);
	Vector *v = LuaVector::checkInstance(L, 2);

	c->setView(*v);

	return 0;
}

/* gets the up direction of the Camera */
int LuaCamera::up(lua_State* L)
{
	Camera *c = checkInstance(L, 1);

	Vector *v = new Vector;
	*v = c->up();

	lua_boxpointer(L, v);
	luaL_getmetatable(L, "Vector");
	lua_setmetatable(L, -2);

	return 1;
}

/* sets the up direction of the Camera */
int LuaCamera::setUp(lua_State* L)
{
	Camera *c = checkInstance(L, 1);
	Vector *v = LuaVector::checkInstance(L, 2);

	c->setUp(*v);

	return 0;
}

} } // namespace declarations

