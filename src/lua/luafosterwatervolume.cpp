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
#include <luapoint.hpp>
#include <luagridterrain.hpp>
#include <luafosterwatervolume.hpp>

using Orbis::Drawable::FosterWaterVolume;

namespace Orbis {

namespace Script {

const char LuaFosterWaterVolume::_class_name[] = "FosterWaterVolume";

#define method(class, name) {#name, class::name}

const luaL_reg LuaFosterWaterVolume::_methods[] = {
	method(LuaFosterWaterVolume, size),
	method(LuaFosterWaterVolume, point),
	method(LuaFosterWaterVolume, density),
	method(LuaFosterWaterVolume, velocity),
	method(LuaFosterWaterVolume, addSource),
	method(LuaFosterWaterVolume, addSink),
	method(LuaFosterWaterVolume, viscosity),
	method(LuaFosterWaterVolume, setViscosity),
	method(LuaFosterWaterVolume, setBottom),
	method(LuaFosterWaterVolume, addToWorld),
	{0, 0}
};

void LuaFosterWaterVolume::registerIntoLua(lua_State* L)
{
	lua_newtable(L);
	int methodtable = lua_gettop(L);
	luaL_newmetatable(L, _class_name);
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
	luaL_openlib(L, 0, _methods, 0);
	// drop methodtable
	lua_pop(L, 1);

	lua_register(L, _class_name, create);
}

FosterWaterVolume* LuaFosterWaterVolume::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, _class_name);
	if(!ud) {
		luaL_typerror(L, index, _class_name);
	}

	// unbox pointer
	return *static_cast<FosterWaterVolume**>(ud);
}

int LuaFosterWaterVolume::create(lua_State* L)
{
	Point *p = LuaPoint::checkInstance(L, 1);
	double size_x = luaL_checknumber(L, 2);
	double size_y = luaL_checknumber(L, 3);
	double size_z = luaL_checknumber(L, 4);
	double step_x = luaL_checknumber(L, 5);
	double step_y = luaL_checknumber(L, 6);
	double step_z = luaL_checknumber(L, 7);

	FosterWaterVolume *wv =
			new FosterWaterVolume(*p, static_cast<unsigned>(size_x),
									static_cast<unsigned>(size_y),
									static_cast<unsigned>(size_z),
									step_x, step_y, step_z);

	lua_boxpointer(L, wv);
	luaL_getmetatable(L, _class_name);
	lua_setmetatable(L, -2);

	return 1;
}

int LuaFosterWaterVolume::collect(lua_State* L)
{
	FosterWaterVolume *wv = static_cast<FosterWaterVolume*>(lua_unboxpointer(L, 1));

	delete wv;

	return 0;
}

int LuaFosterWaterVolume::size(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);

	lua_pushnumber(L, wv->sizeX());

	return 1;
}

int LuaFosterWaterVolume::point(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);
	double i = luaL_checknumber(L, 2);
	double j = luaL_checknumber(L, 3);
	double k = luaL_checknumber(L, 4);

	Point *p = new Point;
	*p = wv->point(static_cast<unsigned>(i),
					static_cast<unsigned>(j),
					static_cast<unsigned>(k));

	lua_boxpointer(L, p);
	luaL_getmetatable(L, "Point");
	lua_setmetatable(L, -2);

	return 1;
}

int LuaFosterWaterVolume::density(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);
	double i = luaL_checknumber(L, 2);
	double j = luaL_checknumber(L, 3);
	double k = luaL_checknumber(L, 4);

	lua_pushnumber(L, wv->density(static_cast<unsigned>(i),
									static_cast<unsigned>(j),
									static_cast<unsigned>(k)));

	return 1;
}

int LuaFosterWaterVolume::velocity(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);
	double i = luaL_checknumber(L, 2);
	double j = luaL_checknumber(L, 3);
	double k = luaL_checknumber(L, 4);

	Vector *v = new Vector;
	*v = wv->velocity(static_cast<unsigned>(i),
						static_cast<unsigned>(j),
						static_cast<unsigned>(k));

	lua_boxpointer(L, v);
	luaL_getmetatable(L, "Vector");
	lua_setmetatable(L, -2);

	return 1;
}

int LuaFosterWaterVolume::addSource(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);
	Point *p = LuaPoint::checkInstance(L, 2);
	double s = luaL_checknumber(L, 3);

	wv->addSource(*p, s);

	return 0;
}

int LuaFosterWaterVolume::addSink(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);
	Point *p = LuaPoint::checkInstance(L, 2);
	double s = luaL_checknumber(L, 3);

	wv->addSink(*p, s);

	return 0;
}

int LuaFosterWaterVolume::viscosity(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);

	lua_pushnumber(L, wv->viscosity());

	return 1;
}

int LuaFosterWaterVolume::setViscosity(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);
	double visc = luaL_checknumber(L, 2);

	wv->setViscosity(visc);

	return 0;
}

int LuaFosterWaterVolume::setBottom(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);
	Orbis::Drawable::GridTerrain *t = LuaGridTerrain::checkInstance(L, 2);

	wv->setBottom(t);

	return 0;
}

int LuaFosterWaterVolume::addToWorld(lua_State* L)
{
	FosterWaterVolume *wv = checkInstance(L, 1);

	Orbis::World::instance()->addDynamic(wv);

	return 0;
}

} } // namespace declarations
