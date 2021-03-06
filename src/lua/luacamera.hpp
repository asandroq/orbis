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

#ifndef __ORBIS_LUACAMERA_HPP__
#define __ORBIS_LUACAMERA_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <camera.hpp>

namespace Orbis {

namespace Script {

/*!
 * \brief This class exports the Camera to the Lua interpreter.
 */
class LuaCamera {
public:
	/*!
	 * \brief Register this class to the Lua interpreter.
	 * \param L The Lua state.
	 */
	static void registerIntoLua(lua_State* L);

	/*!
	 * \brief checks if element at given index is a Camera.
	 * \param L The Lua state.
	 * \param index Stack index of element to be checked.
	 * \return A pointer to the object or 0 if element is not of this type
	 */
	static Orbis::Util::Camera* checkInstance(lua_State* L, int index);

private:
	/*!
	 * \brief Creates a new instance of a Camera.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int create(lua_State* L);

	/*!
	 * \brief Collects the memory of a Camera.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int collect(lua_State* L);

	/*!
	 * \brief Gets the location of the Camera. 
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int location(lua_State* L);

	/*!
	 * \brief Sets the location of the Camera. 
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setLocation(lua_State* L);

	/*!
	 * \brief Gets the view direction of the Camera. 
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int view(lua_State* L);

	/*!
	 * \brief Sets the view direction of the Camera. 
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setView(lua_State* L);

	/*!
	 * \brief Gets the up direction of the Camera. 
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int up(lua_State* L);

	/*!
	 * \brief Sets the up direction of the Camera. 
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setUp(lua_State* L);

	// metadata
	static const char className[];
	static const luaL_reg methods[];
};

} } // namespace declarations

#endif // __ORBIS_LUACAMERA_HPP__

