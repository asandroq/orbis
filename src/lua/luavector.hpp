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
 * $Id: luavector.hpp,v 1.2 2004/02/26 19:29:45 asandro Exp $
 */

#ifndef __ORBIS_LUAVECTOR_HPP__
#define __ORBIS_LUAVECTOR_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <vector.hpp>

using Orbis::Util::Vector;

namespace Orbis {

namespace Script {

/*!
 * \brief Exports the Vector class to the Lua interpreter.
 */
class LuaVector {
public:
	/*!
	 * \brief Register this class to the Lua interpreter.
	 * \param L The Lua state.
	 */
	static void registerIntoLua(lua_State* L);

	/*!
	 * \brief checks if element at given index is a Vector.
	 * \param L The Lua state.
	 * \param index Stack index of element to be checked.
	 * \return A pointer to the object or 0 if element is not of this type
	 */
	static Vector* checkInstance(lua_State* L, int index);

private:
	/*!
	 * \brief Creates a new instance of a Vector.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int create(lua_State* L);

	/*!
	 * \brief Collects the memory of a Vector.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int collect(lua_State* L);

	/*!
	 * \brief The vector's x component.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int x(lua_State* L);

	/*!
	 * \brief The vector's y component.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int y(lua_State* L);

	/*!
	 * \brief The vector's z component.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int z(lua_State* L);

	/*!
	 * \brief The vector's length, or norm.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int length(lua_State* L);

	/*!
	 * \brief Normalises this vector.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int normalise(lua_State* L);

	/*!
	 * \brief Adds two vectors, or a vector and a point.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int add(lua_State* L);

	/*!
	 * \brief Subtracts two vectors, or a vector and a point.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int sub(lua_State* L);

	/*!
	 * \brief Multiplies a vector and a scalar, or a vector dot product.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int mul(lua_State* L);

	/*!
	 * \brief Vector cross product.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int cross(lua_State* L);

	// metadata
	static const char className[];
	static const luaL_reg methods[];
};

} } // namespace declarations

#endif // __ORBIS_LUAVECTOR_HPP__

