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
 * $Id: luavector.cpp,v 1.2 2004/02/18 18:55:07 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <luapoint.hpp>
#include <luavector.hpp>

namespace Orbis {

namespace Script {

const char LuaVector::className[] = "Vector";

#define method(class, name) {#name, class::name}

const luaL_reg LuaVector::methods[] = {
	method(LuaVector, x),
	method(LuaVector, y),
	method(LuaVector, z),
	method(LuaVector, length),
	method(LuaVector, normalise),
	{0, 0}
};

/* register this class to the Lua interpreter */
void LuaVector::registerIntoLua(lua_State* L)
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

	lua_pushliteral(L, "__add");
	lua_pushcfunction(L, add);
	lua_settable(L, metatable);

	lua_pushliteral(L, "__sub");
	lua_pushcfunction(L, sub);
	lua_settable(L, metatable);

	lua_pushliteral(L, "__mul");
	lua_pushcfunction(L, mul);
	lua_settable(L, metatable);

	lua_pushliteral(L, "__pow");
	lua_pushcfunction(L, cross);
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

/* checks if element at given index is a Vector */
Vector* LuaVector::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, className);
	if(!ud) {
		luaL_typerror(L, index, className);
	}

	return *(Vector**)ud;  // unbox vectorer
}

/* creates a new instance of a Vector */
int LuaVector::create(lua_State* L)
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

	Vector *v = new Vector(x, y, z);
	lua_boxpointer(L, v);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
}

/* collects the memory of a Vector */
int LuaVector::collect(lua_State* L)
{
	Vector *v = (Vector*) lua_unboxpointer(L, 1);

	delete v;

	return 0;
}

/* the vector's x component */
int LuaVector::x(lua_State* L)
{
	Vector *v = checkInstance(L, 1);

	lua_pushnumber(L, v->x());

	return 1;
}

/* the vector's y component */
int LuaVector::y(lua_State* L)
{
	Vector *v = checkInstance(L, 1);

	lua_pushnumber(L, v->y());

	return 1;
}

/* the vector's z component */
int LuaVector::z(lua_State* L)
{
	Vector *v = checkInstance(L, 1);

	lua_pushnumber(L, v->z());

	return 1;
}

 /* the vector's length, or norm */
int LuaVector::length(lua_State* L)
{
	Vector *v = checkInstance(L, 1);

	lua_pushnumber(L, v->length());

	return 1;
}

/* normalises this vector */
int LuaVector::normalise(lua_State* L)
{
	Vector *v = checkInstance(L, 1);

	Vector *norm = new Vector();
	*norm = v->normalise();
	lua_boxpointer(L, norm);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
}

/* adds two vectors, or a vector and a point */
int LuaVector::add(lua_State* L)
{
	// both args must be userdata
	luaL_checktype(L, 1, LUA_TUSERDATA);
	luaL_checktype(L, 2, LUA_TUSERDATA);

	// checking if first arg is a vector
	void *ud = luaL_checkudata(L, 1, className);
	if(ud) {
		Vector *v1 = *static_cast<Vector**>(ud);
		// the second arg must be a vector or a point
		ud = luaL_checkudata(L, 2, className);
		if(ud) {
			// adding two vectors
			Vector *res = new Vector();
			Vector *v2 = *static_cast<Vector**>(ud);
			*res = (*v1) + (*v2);

			lua_boxpointer(L, res);
			luaL_getmetatable(L, className);
			lua_setmetatable(L, -2);
		} else {
			// second arg must be a point
			Point *p = LuaPoint::checkInstance(L, 2);
			Point *res = new Point();
			*res = (*p) + (*v1);

			lua_boxpointer(L, res);
			luaL_getmetatable(L, "Point");
			lua_setmetatable(L, -2);
		}
	} else {
		// first arg must be a point
		Point *p = LuaPoint::checkInstance(L, 1);
		// and second one must be a vector
		Vector *v = checkInstance(L, 2);
		Point *res = new Point();
		*res = (*p) + (*v);

		lua_boxpointer(L, res);
		luaL_getmetatable(L, "Point");
		lua_setmetatable(L, -2);
	}

	return 1;
}

/*
 * subtracts two vectors
 * the subtraction of a point and a vector is handled by LuaPoint
 */
int LuaVector::sub(lua_State* L)
{
	Vector *v1 = checkInstance(L, 1);
	Vector *v2 = checkInstance(L, 2);

	Vector *res = new Vector;
	*res = (*v1) - (*v2);

	lua_boxpointer(L, res);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
/*
	// both args must be userdata
	luaL_checktype(L, 1, LUA_TUSERDATA);
	luaL_checktype(L, 2, LUA_TUSERDATA);

	// checking if first arg is a vector
	void *ud = luaL_checkudata(L, 1, className);
	if(ud) {
		Vector *v1 = *static_cast<Vector**>(ud);
		// the second arg must be a vector
		Vector *v2 = checkInstance(L, 2);
		// subtracting two vectors
		Vector *res = new Vector();
		*res = (*v1) - (*v2);

		lua_boxpointer(L, res);
		luaL_getmetatable(L, className);
		lua_setmetatable(L, -2);
	} else {
		// first arg must be a point
		Point *p = LuaPoint::checkInstance(L, 1);
		// and second one must be a vector
		Vector *v = checkInstance(L, 2);
		Point *res = new Point();
		*res = (*p) - (*v);

		lua_boxpointer(L, res);
		luaL_getmetatable(L, "Point");
		lua_setmetatable(L, -2);
	}

	return 1;
*/
}

/* multiplies a vector and a scalar, or a vector dot product */
int LuaVector::mul(lua_State* L)
{
	// first arg may be a number or a vector
	if(lua_isnumber(L, 1)) {
		// second arg must be a vector
		Vector *v = checkInstance(L, 2);
		Vector *res = new Vector();
		*res = lua_tonumber(L, 1) * (*v);

		lua_boxpointer(L, res);
		luaL_getmetatable(L, className);
		lua_setmetatable(L, -2);
	} else {
		// first arg must be a vector
		Vector *v1 = checkInstance(L, 1);
		// second arg may be a number or a vector
		if(lua_isnumber(L, 2)) {
			Vector *res = new Vector();
			*res = lua_tonumber(L, 2) * (*v1);

			lua_boxpointer(L, res);
			luaL_getmetatable(L, className);
			lua_setmetatable(L, -2);
		} else {
			// second arg must be a vector
			Vector *v2 = checkInstance(L, 2);
			lua_Number res = (*v1) * (*v2);

			lua_pushnumber(L, res);
		}
	}

	return 1;
}

/* vector cross product */
int LuaVector::cross(lua_State* L)
{
	Vector *v1 = checkInstance(L, 1);
	Vector *v2 = checkInstance(L, 2);

	Vector *res = new Vector();
	*res = (*v1) ^ (*v2);
	lua_boxpointer(L, res);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
}

} } // namespace declarations

