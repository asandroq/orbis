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

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

#include <iostream>

#include <luaworld.hpp>
#include <luapoint.hpp>
#include <luapatch.hpp>
#include <luavector.hpp>
#include <luacamera.hpp>
#include <luaworldview.hpp>
#include <luagridterrain.hpp>
#include <luastamwatervolume.hpp>
#include <luafosterwatervolume.hpp>
#include <luawaterheightfield.hpp>
#include <luaisosurfacerenderer.hpp>

#include <luascript.hpp>

namespace Orbis {

namespace Script {

/*
 * Based on Lua's standard "print" function.
 */
int LuaScript::showMessage(lua_State* L)
{
	int i;
	std::string msg;
	int n = lua_gettop(L);  /* number of arguments */

	lua_getglobal(L, "tostring");
	for(i = 1; i <= n; i++) {
		const char *s;

		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if(s == NULL) {
			return luaL_error(L, "`tostring' must return a string");
		}
		if(i > 1) {
			msg += "\t";
		}
		msg += s;
		lua_pop(L, 1);  /* pop result */
	}
	msg += "\n";

	LuaActionAdapter **aa =
	static_cast<LuaActionAdapter**>(lua_touserdata(L, lua_upvalueindex(1)));
	if(*aa) {
		(*aa)->showMessage(msg);
	} else {
		std::cerr << msg;
	}

	return 0;
}

LuaScript::LuaScript()
	: _active(true), _ticks(0), _lua_state(0), _action_adapter(0)
{
	_lua_state = lua_open();

	// basic libraries
	luaopen_base(_lua_state);
	luaopen_math(_lua_state);

	// registering "print" replacement
	lua_pushstring(_lua_state, "print");
	lua_pushlightuserdata(_lua_state, &_action_adapter);
	lua_pushcclosure(_lua_state, LuaScript::showMessage, 1);
	lua_settable(_lua_state, LUA_GLOBALSINDEX);

	// registering modules
	LuaWorld::registerIntoLua(_lua_state);
	LuaPoint::registerIntoLua(_lua_state);
	LuaPatch::registerIntoLua(_lua_state);
	LuaVector::registerIntoLua(_lua_state);
	LuaCamera::registerIntoLua(_lua_state);
	LuaWorldView::registerIntoLua(_lua_state);
	LuaGridTerrain::registerIntoLua(_lua_state);
	LuaStamWaterVolume::registerIntoLua(_lua_state);
	LuaFosterWaterVolume::registerIntoLua(_lua_state);
	LuaWaterHeightField::registerIntoLua(_lua_state);
	LuaIsoSurfaceRenderer::registerIntoLua(_lua_state);

	reset();
}

LuaScript::~LuaScript()
{
	lua_close(_lua_state);
}

void LuaScript::reset()
{
	_ticks = 0;
	_active = true;
	unlock();
}

void LuaScript::runFile(const std::string& filename)
{
	try {
		if(luaL_loadfile(_lua_state, filename.c_str()) != 0) {
				// error loading Lua chunk
				if(_action_adapter) {
					_action_adapter->showErrorMessage(lua_tostring(_lua_state, -1));
				}
		} else {
			if(lua_pcall(_lua_state, 0, 0, 0) != 0) {
				// error running chunk
				if(_action_adapter) {
					_action_adapter->showErrorMessage(lua_tostring(_lua_state, -1));
				}
			}
		}
	} catch(const std::exception& e) {
		_active = false;
		if(_action_adapter) {
			_action_adapter->showErrorMessage(std::string("Error: ") + e.what());
		} else {
			std::cerr << std::string("Error: ") + e.what() << std::endl;
		}
	}
}

void LuaScript::evolve(unsigned long time)
{
	Locker lock(this);

	if(!_active) {
		return;
	}

	// updating frame count
	_ticks++;

	// calling scripting update function
	lua_pushstring(_lua_state, "FrameCallback");
	lua_gettable(_lua_state, LUA_GLOBALSINDEX);
	if(lua_type(_lua_state, -1) == LUA_TFUNCTION) {
		lua_pushnumber(_lua_state, _ticks);
		try {
			if(lua_pcall(_lua_state, 1, 0, 0) != 0) {
				// error in Lua function
				_active = false;
				if(_action_adapter) {
					_action_adapter->showErrorMessage(lua_tostring(_lua_state, -1));
				}
			}
		} catch(const std::exception& e) {
			_active = false;
			if(_action_adapter) {
				_action_adapter->showErrorMessage(std::string("Error: ") + e.what());
			} else {
				std::cerr << std::string("Error: ") + e.what() << std::endl;
			}
		}
	} else {
		lua_settop(_lua_state, -2);
	}
}

} } // namespace declarations
