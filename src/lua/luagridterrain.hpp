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
 * $Id: luagridterrain.hpp,v 1.6 2004/03/02 21:45:38 asandro Exp $
 */

#ifndef __ORBIS_LUAGRIDTERRAIN_HPP__
#define __ORBIS_LUAGRIDTERRAIN_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <gridterrain.hpp>

namespace Orbis {

namespace Script {

/*!
 * \brief Exports the GridTerrain class to the Lua interpreter.
 */
class LuaGridTerrain {
public:
	/*!
	 * \brief Register this class to the Lua interpreter.
	 * \param L The Lua state.
	 */
	static void registerIntoLua(lua_State* L);

	/*!
	 * \brief checks if element at given index is a GridTerrain.
	 * \param L The Lua state.
	 * \param index Stack index of element to be checked.
	 * \return A pointer to the object or 0 if element is not of this type
	 */
	static Orbis::Drawable::GridTerrain* checkInstance(lua_State* L, int index);

private:
	/*!
	 * \brief Creates a new instance of a GridTerrain.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int create(lua_State* L);

	/*!
	 * \brief Collects the memory of a GridTerrain.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int collect(lua_State* L);

	/*!
	 * \brief Adds a new Patch to this terrain.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int addPatch(lua_State* L);

	/*!
	 * \brief Generates a random terrain.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int faultLineGeneration(lua_State* L);

	/*!
	 * \brief Smooths the terrain.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int smooth(lua_State* L);

	/*!
	 * \brief Retrieves the height of a point on the terrain.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int point(lua_State* L);

	/*!
	 * \brief Sets a point on the terrain.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setPoint(lua_State* L);

	/*!
	 * \brief Applies a texture to the terrain patch.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setTexture(lua_State* L);

	/*!
	 * \brief Adds this drawable to the World.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int addToWorld(lua_State* L);

	// metadata
	static const char className[];
	static const luaL_reg methods[];
};

} } // namespace declarations

#endif // __ORBIS_LUAGRIDTERRAIN_HPP__

