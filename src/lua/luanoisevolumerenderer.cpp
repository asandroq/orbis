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
#include <stamwatervolume.hpp>
#include <fosterwatervolume.hpp>
#include <luanoisevolumerenderer.hpp>

using Orbis::Drawable::NoiseVolumeRenderer;

namespace Orbis {

namespace Script {
	   
const char LuaNoiseVolumeRenderer::_class_name[] = "NoiseVolumeRenderer";

#define method(class, name) {#name, class::name}

const luaL_reg LuaNoiseVolumeRenderer::_methods[] = {
	method(LuaNoiseVolumeRenderer, addToWorld),
	{0, 0}
};

void LuaNoiseVolumeRenderer::registerIntoLua(lua_State* L)
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

NoiseVolumeRenderer* LuaNoiseVolumeRenderer::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, _class_name);
	if(!ud) {
		luaL_typerror(L, index, _class_name);
	}

	return *(NoiseVolumeRenderer**)ud;  // unbox pointer
}

int LuaNoiseVolumeRenderer::create(lua_State* L)
{
	void *ud;
	Orbis::Drawable::WaterVolume *wv;
	NoiseVolumeRenderer *noise = 0;

	luaL_checktype(L, 1, LUA_TUSERDATA);
	ud = luaL_checkudata(L, 1, "FosterWaterVolume");
	if(ud) {
		wv = *(static_cast<Orbis::Drawable::FosterWaterVolume**>(ud));
	} else {
		ud = luaL_checkudata(L, 1, "StamWaterVolume");
		if(ud) {
			wv = *(static_cast<Orbis::Drawable::StamWaterVolume**>(ud));
		} else {
			wv = 0;
			luaL_typerror(L, 1, "WaterVolume");
		}
	}

	if(lua_gettop(L) == 1) {
		noise = new NoiseVolumeRenderer(wv);
	} else if(lua_gettop(L) == 2) {
		double t = luaL_checknumber(L, 2);
		noise = new NoiseVolumeRenderer(wv, t);
	} else {
		luaL_error(L, "wrong number of arguments to IsoSurfaceRenderer");
	}

	noise->ref();
	lua_boxpointer(L, noise);
	luaL_getmetatable(L, _class_name);
	lua_setmetatable(L, -2);

	return 1;
}

int LuaNoiseVolumeRenderer::collect(lua_State* L)
{
	NoiseVolumeRenderer *noise = (NoiseVolumeRenderer*) lua_unboxpointer(L, 1);

	noise->unref();

	return 0;
}

int LuaNoiseVolumeRenderer::addToWorld(lua_State* L)
{
	NoiseVolumeRenderer *noise = checkInstance(L, 1);

	World::instance()->addDrawable(noise);

	return 0;
}

} } // namespace declarations
