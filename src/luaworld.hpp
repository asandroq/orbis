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
 * $Id: luaworld.hpp,v 1.8 2004/05/24 15:19:33 asandro Exp $
 */

#ifndef __ORBIS_LUAWORLD_HPP__
#define __ORBIS_LUAWORLD_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

namespace Orbis {

namespace Script {

/*!
 * \brief Exports the World class to the Lua interpreter.
 */
class LuaWorld {
public:
	/*!
	 * \brief Registers the World's binding to Lua.
	 * \param L The Lua state.
	 */
	static void registerIntoLua(lua_State *L);

private:
	/*!
	 * \brief Restarts the World.
	 * \param L The Lua state.
	 */
	static int reset(lua_State *L);

	/*!
	 * \brief Starts the simulation's timer.
	 * \param L The Lua state.
	 */
	static int start(lua_State *L);

	/*!
	 * \brief Stops the simulation's timer.
	 * \param L The Lua state.
	 */
	static int stop(lua_State *L);

	/*!
	 * \brief Pushs the World's root node onto Lua stack.
	 * \param L The Lua state.
	 */
	static int root(lua_State *L);

	/*!
	 * \brief Pushs one of the World's views, given its index,
	 * onto the Lua stack.
	 * \param L The Lua state.
	 */
	static int view(lua_State *L);

	/*!
	 * \brief Retrieves a named object.
	 * \param L The Lua state.
	 */
	static int objectByName(lua_State *L);

	/*!
	 * \brief Adds a new drawable to the World.
	 * \param L The Lua state.
	 */
	static int addDrawable(lua_State *L);

	/*!
	 * \brief Adds a whole group to the World.
	 * \param L The Lua state.
	 */
	static int addGroup(lua_State *L);

	// metaclass data
	static const luaL_reg methods[];
};

} } // namespace declarations

#endif // __ORBIS_LUAWORLD_HPP__

