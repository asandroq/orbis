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
 * $Id: luapoint.cpp,v 1.2 2004/02/18 18:55:07 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <luapoint.hpp>
#include <luavector.hpp>

namespace Orbis {

namespace Script {

const char LuaPoint::className[] = "Point";

#define method(class, name) {#name, class::name}

const luaL_reg LuaPoint::methods[] = {
	method(LuaPoint, x),
	method(LuaPoint, y),
	method(LuaPoint, z),
	{0, 0}
};

/* register this class to the Lua interpreter */
void LuaPoint::registerIntoLua(lua_State* L)
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

	lua_pushliteral(L, "__sub");
	lua_pushcfunction(L, sub);
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

/* checks if element at given index is a Point */
Point* LuaPoint::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, className);
	if(!ud) {
		luaL_typerror(L, index, className);
	}

	return *(Point**)ud;  // unbox pointer
}

/* creates a new instance of a Point */
int LuaPoint::create(lua_State* L)
{
	double x, y, z;

	if(!lua_isnumber(L, 1)) {
		x = y = z = 0.0;
	} else {
		x = lua_tonumber(L, 1);
		if(!lua_isnumber(L, 2)) {
			y = z = 0.0;
		} else {
			y = lua_tonumber(L, 2);
			if(!lua_isnumber(L, 3)) {
				   z = 0.0;
			} else {
				   z = lua_tonumber(L, 3);
			}
		}
	}

	Point *p = new Point(x, y, z);
	lua_boxpointer(L, p);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
}

/* collects the memory of a Point */
int LuaPoint::collect(lua_State* L)
{
	Point *p = (Point*) lua_unboxpointer(L, 1);

	delete p;

	return 0;
}

/* the point's x coordinate */
int LuaPoint::x(lua_State* L)
{
	Point *p = checkInstance(L, 1);

	lua_pushnumber(L, p->x());

	return 1;
}

/* the point's y coordinate */
int LuaPoint::y(lua_State* L)
{
	Point *p = checkInstance(L, 1);

	lua_pushnumber(L, p->y());

	return 1;
}

/* the point's z coordinate */
int LuaPoint::z(lua_State* L)
{
	Point *p = checkInstance(L, 1);

	lua_pushnumber(L, p->z());

	return 1;
}

int LuaPoint::sub(lua_State* L)
{
	// both args must be userdata
	luaL_checktype(L, 1, LUA_TUSERDATA);
	luaL_checktype(L, 2, LUA_TUSERDATA);

	// first one must be a point
	Point *p1 = checkInstance(L, 1);
	// second one may be a point or a vector
	void *ud = luaL_checkudata(L, 2, className);
	if(ud) {
		// subtracting two points
		Point *p2 = *static_cast<Point**>(ud);
		Vector *res = new Vector;
		*res = (*p1) - (*p2);

		lua_boxpointer(L, res);
		luaL_getmetatable(L, "Vector");
		lua_setmetatable(L, -2);
	} else {
		// second one must be a vector
		Vector *v2 = LuaVector::checkInstance(L, 2);
		Point *res = new Point;
		*res = (*p1) - (*v2);

		lua_boxpointer(L, res);
		luaL_getmetatable(L, className);
		lua_setmetatable(L, -2);
	}

	return 1;
}

} } // namespace declarations

