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
 * $Id: luagridterrain.cpp,v 1.10 2004/03/26 20:39:26 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <world.hpp>
#include <luapoint.hpp>
#include <luapatch.hpp>
#include <luagridterrain.hpp>

namespace Orbis {

namespace Script {
	   
const char LuaGridTerrain::className[] = "GridTerrain";
	
#define method(class, name) {#name, class::name}

const luaL_reg LuaGridTerrain::methods[] = {
	method(LuaGridTerrain, addPatch),
	method(LuaGridTerrain, faultLineGeneration),
	method(LuaGridTerrain, smooth),
	method(LuaGridTerrain, point),
	method(LuaGridTerrain, setPoint),
	method(LuaGridTerrain, setTexture),
	method(LuaGridTerrain, addToWorld),
	{0, 0}
};

/* register this class to the Lua interpreter */
void LuaGridTerrain::registerIntoLua(lua_State* L)
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

/* checks if element at given index is a GridTerrain */
GridTerrain* LuaGridTerrain::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, className);
	if(!ud) {
		luaL_typerror(L, index, className);
	}

	return *(GridTerrain**)ud;  // unbox pointer
}

/* creates a new instance of a GridTerrain */
int LuaGridTerrain::create(lua_State* L)
{
	GridTerrain *t = 0;

	void *ud = luaL_checkudata(L, 1, "Point");
	if(ud) {
		Point *p = *(Point**)ud;
		double xstep = luaL_checknumber(L, 2);
		double ystep = luaL_checknumber(L, 3);
		double xsize = luaL_checknumber(L, 4);
		double ysize = luaL_checknumber(L, 5);

		t = new GridTerrain(*p, xstep, ystep,
						static_cast<unsigned>(xsize),
						static_cast<unsigned>(ysize));
	} else {
		const char* fname = luaL_checklstring(L, 1, 0);
		double xstep = luaL_checknumber(L, 2);
		double ystep = luaL_checknumber(L, 3);

		t = new GridTerrain(fname, xstep, ystep);
	}

	lua_boxpointer(L, t);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
}

/* collects the memory of a GridTerrain */
int LuaGridTerrain::collect(lua_State* L)
{
//	GridTerrain *t = (GridTerrain*) lua_unboxpointer(L, 1);

	/*
	 * TODO:
	 * not sure what to do here, OSG must destroy it, but what if it
	 * wasn't added to the scene graph yet?
	 */
//	t->unref();

	return 0;
}

/* adds a new patch to the terrain */
int LuaGridTerrain::addPatch(lua_State* L)
{
	GridTerrain *t = checkInstance(L, 1);
	Patch *p = LuaPatch::checkInstance(L, 2);

	t->addPatch(*p);

	return 0;
}

/* generates a random terrain */
int LuaGridTerrain::faultLineGeneration(lua_State* L)
{
	GridTerrain *t = checkInstance(L, 1);
	double iters = luaL_checknumber(L, 2);

	t->faultLineGeneration(static_cast<unsigned>(iters));

	return 0;
}

/* smooths the terrain */
int LuaGridTerrain::smooth(lua_State* L)
{
	GridTerrain *t = checkInstance(L, 1);
	double k = luaL_checknumber(L, 2);

	t->smooth(k);

	return 0;
}

/* retrieves the height of a point on the terrain */
int LuaGridTerrain::point(lua_State* L)
{
	GridTerrain *t = checkInstance(L, 1);
	double i = luaL_checknumber(L, 2);
	double j = luaL_checknumber(L, 3);

	Point p1 = t->point(static_cast<unsigned>(i), static_cast<unsigned>(j));
	Point *p = new Point(p1);

	lua_boxpointer(L, p);
	luaL_getmetatable(L, "Point");
	lua_setmetatable(L, -2);

	return 1;
}

/* sets a point on the terrain */
int LuaGridTerrain::setPoint(lua_State* L)
{
	GridTerrain *t = checkInstance(L, 1);
	double i = luaL_checknumber(L, 2);
	double j = luaL_checknumber(L, 3);
	double z = luaL_checknumber(L, 4);

	t->setPoint(static_cast<unsigned>(i), static_cast<unsigned>(j), z);

	return 0;
}

/* applies a texture to the terrain patch */
int LuaGridTerrain::setTexture(lua_State* L)
{
	GridTerrain *terrain = checkInstance(L, 1);
	const char* fname = luaL_checklstring(L, 2, 0);

	terrain->setTexture(fname);

	return 0;
}

/* adds this drawable to the World */
int LuaGridTerrain::addToWorld(lua_State* L)
{
	GridTerrain *t = checkInstance(L, 1);

	World::instance()->addDrawable(t);

	return 0;
}

} } // namespace declarations

