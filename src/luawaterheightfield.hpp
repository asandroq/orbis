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
 * $Id: luawaterheightfield.hpp,v 1.5 2004/02/26 19:29:45 asandro Exp $
 */

#ifndef __ORBIS__LUAWATERHEIGHTFIELD_HPP__
#define __ORBIS__LUAWATERHEIGHTFIELD_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <waterheightfield.hpp>

using Orbis::Drawable::WaterHeightField;

namespace Orbis {

namespace Script {

/*!
 * \brief Exports the WaterHeightField class to the Lua interpreter.
 */
class LuaWaterHeightField {
public:
	/*!
	 * \brief Register this class to the Lua interpreter.
	 * \param L The Lua state.
	 */
	static void registerIntoLua(lua_State* L);

	/*!
	 * \brief checks if element at given index is a WaterHeightField.
	 * \param L The Lua state.
	 * \param index Stack index of element to be checked.
	 * \return A pointer to the object or 0 if element is not of this type
	 */
	static WaterHeightField* checkInstance(lua_State* L, int index);

private:
	/*!
	 * \brief Creates a new instance of a WaterHeightField.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int create(lua_State* L);

	/*!
	 * \brief Collects the memory of a WaterHeightField.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int collect(lua_State* L);

	/*!
	 * \brief Adds a new source of water.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int addSource(lua_State* L);

	/*!
	 * \brief Adds a new sink of water.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int addSink(lua_State* L);

	/*!
	 * \brief Sets the bottom of the water.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setBottom(lua_State* L);

	/*!
	 * \brief Sets the texture of the water.
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

#endif // __ORBIS__LUAWATERHEIGHTFIELD_HPP__

