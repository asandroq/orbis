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
 * $Id: luapatch.hpp,v 1.1 2004/03/02 21:45:38 asandro Exp $
 */

#ifndef __ORBIS_LUAPATCH_HPP__
#define __ORBIS_LUAPATCH_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <patch.hpp>

using Orbis::Util::Patch;

namespace Orbis {

namespace Script {

/*!
 * \brief Exports the Patch class to the Lua interpreter.
 */
class LuaPatch {
public:
	/*!
	 * \brief Register this class to the Lua interpreter.
	 * \param L The Lua state.
	 */
	static void registerIntoLua(lua_State* L);

	/*!
	 * \brief checks if element at given index is a Patch.
	 * \param L The Lua state.
	 * \param index Stack index of element to be checked.
	 * \return A pointer to the object or 0 if element is not of this type
	 */
	static Patch* checkInstance(lua_State* L, int index);

private:
	/*!
	 * \brief Creates a new instance of a Patch.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int create(lua_State* L);

	/*!
	 * \brief Collects the memory of a Patch.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int collect(lua_State* L);

	/*!
	 * \brief Adds Point's to the Patch boundary.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int addPoints(lua_State* L);

	/*!
	 * \brief Gets an attribute of the Patch.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int attribute(lua_State* L);

	/*!
	 * \brief Sets an attribute of the Patch.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setAttribute(lua_State* L);

	// metadata
	static const char className[];
	static const luaL_reg methods[];
};

} } // namespace declarations

#endif // __ORBIS_LUAPATCH_HPP__

