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
#include <luacamera.hpp>
#include <luaworldview.hpp>
#include <worldviewpov.hpp>

namespace Orbis {

namespace Script {

const char LuaWorldView::className[] = "WorldView";

#define method(class, name) {#name, class::name}

const luaL_reg LuaWorldView::methods[] = {
	method(LuaWorldView, camera),
	method(LuaWorldView, setCamera),
	method(LuaWorldView, render),
	{0, 0}
};

/* register this class to the Lua interpreter */
void LuaWorldView::registerIntoLua(lua_State* L)
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

/* checks if element at given index is a WorldView */
WorldView* LuaWorldView::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, className);
	if(!ud) {
		luaL_typerror(L, index, className);
	}

	return *(WorldView**)ud;  // unbox pointer
}

/* creates a new instance of a WorldView */
int LuaWorldView::create(lua_State* L)
{
	WorldView *w = 0;

	const char *type = luaL_checklstring(L, 1, 0);

	if(std::string(type) == "POV") {
		const char *fname = luaL_checklstring(L, 2, 0);
		w = new WorldViewPOV(World::instance(), fname);
	}

	if(w) {
		lua_boxpointer(L, w);
		luaL_getmetatable(L, className);
		lua_setmetatable(L, -2);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

/* collects the memory of a WorldViewPOV */
int LuaWorldView::collect(lua_State* L)
{
	WorldView *w = (WorldView*) lua_unboxpointer(L, 1);

	delete w;

	return 0;
}

/* gets the Camera of this viewer */
int LuaWorldView::camera(lua_State* L)
{
	WorldView *w = checkInstance(L, 1);

	Orbis::Util::Camera *c = new Orbis::Util::Camera;
	*c = w->camera();

	lua_boxpointer(L, c);
	luaL_getmetatable(L, "Camera");
	lua_setmetatable(L, -2);

	return 1;
}

/* sets the Camera of this viewer */
int LuaWorldView::setCamera(lua_State* L)
{
	WorldView *w = checkInstance(L, 1);
	Orbis::Util::Camera *c = LuaCamera::checkInstance(L, 2);

	w->setCamera(*c);

	return 0;
}

/* renders the world into its file representation */
int LuaWorldView::render(lua_State* L)
{
	WorldView *w = checkInstance(L, 1);

	w->render();

	return 0;
}

} } // namespace declarations

