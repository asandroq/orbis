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
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <osg/TexEnv>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include <world.hpp>
#include <luapoint.hpp>
#include <luagridterrain.hpp>
#include <luawaterheightfield.hpp>

namespace Orbis {

namespace Script {
	   
const char LuaWaterHeightField::className[] = "WaterHeightField";

#define method(class, name) {#name, class::name}

const luaL_reg LuaWaterHeightField::methods[] = {
	method(LuaWaterHeightField, addSource),
	method(LuaWaterHeightField, addSink),
	method(LuaWaterHeightField, setBottom),
	method(LuaWaterHeightField, setTexture),
	method(LuaWaterHeightField, addToWorld),
	{0, 0}
};
/* Registers this class to the Lua interpreter. */
void LuaWaterHeightField::registerIntoLua(lua_State* L)
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

/* checks if element at given index is a WaterHeightField. */
WaterHeightField* LuaWaterHeightField::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, className);
	if(!ud) {
		luaL_typerror(L, index, className);
	}

	return *(WaterHeightField**)ud;  // unbox pointer
}

/* Creates a new instance of a WaterHeightField. */
int LuaWaterHeightField::create(lua_State* L)
{
	Point *p = LuaPoint::checkInstance(L, 1);
	double xstep = luaL_checknumber(L, 2);
	double ystep = luaL_checknumber(L, 3);
	double xsize = luaL_checknumber(L, 4);
	double ysize = luaL_checknumber(L, 5);

	WaterHeightField *w = new WaterHeightField(*p,
									xstep, ystep,
									static_cast<unsigned>(xsize),
									static_cast<unsigned>(ysize));

	w->ref();
	lua_boxpointer(L, w);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
}

/* Collects the memory of a WaterHeightField. */
int LuaWaterHeightField::collect(lua_State* L)
{
	WaterHeightField *w = (WaterHeightField*) lua_unboxpointer(L, 1);

	/*
	 * TODO:
	 * not sure what to do here, OSG must destroy it, but what if it
	 * wasn't added to the scene graph yet?
	 */
	w->unref();

	return 0;
}

/* Adds a new source of water. */
int LuaWaterHeightField::addSource(lua_State* L)
{
	WaterHeightField *water = checkInstance(L, 1);
	Point *p = LuaPoint::checkInstance(L, 2);
	double s = luaL_checknumber(L, 3);

	water->addSource(Orbis::Drawable::Source(*p, Vector(), s));

	return 0;
}

/* Adds a new sink of water. */
int LuaWaterHeightField::addSink(lua_State* L)
{
	WaterHeightField *water = checkInstance(L, 1);
	Point *p = LuaPoint::checkInstance(L, 2);
	double s = luaL_checknumber(L, 3);

	water->addSink(Orbis::Drawable::Source(*p, Vector(), s));

	return 0;
}

/* Sets the bottom of the water. */
int LuaWaterHeightField::setBottom(lua_State* L)
{
	WaterHeightField *water = checkInstance(L, 1);
	Orbis::Drawable::GridTerrain *t = LuaGridTerrain::checkInstance(L, 2);

	water->setBottom(t);

	return 0;
}

/* Sets the texture of the water. */
int LuaWaterHeightField::setTexture(lua_State* L)
{
	WaterHeightField *water = checkInstance(L, 1);
	const char* fname = luaL_checklstring(L, 2, 0);

	water->setTexture(fname);

	return 0;
}

/* Adds this drawable to the World. */
int LuaWaterHeightField::addToWorld(lua_State* L)
{
	WaterHeightField *water = checkInstance(L, 1);

	World::instance()->addDynamic(water);
	World::instance()->addDrawable(water);

	return 0;
}

} } // namespace declarations
