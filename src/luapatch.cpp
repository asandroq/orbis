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
 * $Id: luapatch.cpp,v 1.1 2004/03/02 21:45:38 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <luapoint.hpp>
#include <luapatch.hpp>

namespace Orbis {

namespace Script {

const char LuaPatch::className[] = "Patch";

#define method(class, name) {#name, class::name}

const luaL_reg LuaPatch::methods[] = {
	method(LuaPatch, addPoints),
	method(LuaPatch, attribute),
	method(LuaPatch, setAttribute),
	{0, 0}
};

/* register this class to the Lua interpreter */
void LuaPatch::registerIntoLua(lua_State* L)
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

/* checks if element at given index is a Patch */
Patch* LuaPatch::checkInstance(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, index, className);
	if(!ud) {
		luaL_typerror(L, index, className);
	}

	return *(Patch**)ud;  // unbox pointer
}

/* creates a new instance of a Patch */
int LuaPatch::create(lua_State* L)
{
	Patch *p = new Patch;
	lua_boxpointer(L, p);
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);

	return 1;
}

/* collects the memory of a Patch */
int LuaPatch::collect(lua_State* L)
{
	Patch *p = (Patch*) lua_unboxpointer(L, 1);

	delete p;

	return 0;
}

int LuaPatch::addPoints(lua_State* L)
{
	// ok, we gonna receive as many points as passed to the method
	int nr_args = lua_gettop(L);
	Patch *p = checkInstance(L, 1);

	for(int i = 2; i <= nr_args; i++) {
		Point *pt = LuaPoint::checkInstance(L, i);
		p->addPoint(*pt);
	}

	return 0;
}

int LuaPatch::attribute(lua_State* L)
{
	Patch *p = checkInstance(L, 1);
	const char *attr = luaL_checklstring(L, 2, 0);

	lua_pushstring(L, p->attribute(attr).c_str());

	return 1;
}

int LuaPatch::setAttribute(lua_State* L)
{
	Patch *p = checkInstance(L, 1);
	const char *attr = luaL_checklstring(L, 2, 0);
	const char *value = luaL_checklstring(L, 3, 0);

	p->setAttribute(attr, value);

	return 0;
}

} } // namespace declarations

