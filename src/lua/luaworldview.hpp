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
 * $Id: luaworldview.hpp,v 1.1 2004/03/26 20:39:26 asandro Exp $
 */

#ifndef __ORBIS_LUAWORLDVIEW_HPP__
#define __ORBIS_LUAWORLDVIEW_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <worldview.hpp>

using Orbis::WorldView;

namespace Orbis {

namespace Script {

/*!
 * \brief Exports the WorldView class to the Lua interpreter.
 */
class LuaWorldView {
public:
	/*!
	 * \brief Register this class to the Lua interpreter.
	 * \param L The Lua state.
	 */
	static void registerIntoLua(lua_State* L);

	/*!
	 * \brief checks if element at given index is a WorldView.
	 * \param L The Lua state.
	 * \param index Stack index of element to be checked.
	 * \return A pointer to the object or 0 if element is not of this type.
	 */
	static WorldView* checkInstance(lua_State* L, int index);

private:
	/*!
	 * \brief Creates a new instance of a WorldView.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int create(lua_State* L);

	/*!
	 * \brief Collects the memory of a WorldView.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int collect(lua_State* L);

	/*!
	 * \brief Gets the Camera of this viewer.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int camera(lua_State* L);


	/*!
	 * \brief Sets the Camera of this viewer.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setCamera(lua_State* L);

	/*!
	 * \brief Renders the World into its file representation.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	 static int render(lua_State* L);

	// metadata
	static const char className[];
	static const luaL_reg methods[];
};

} } // namespace declarations

#endif // __ORBIS_LUAWORLDVIEW_HPP__

