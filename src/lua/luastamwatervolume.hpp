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

#ifndef __ORBIS_LUASTAMWATERVOLUME_HPP__
#define __ORBIS_LUASTAMWATERVOLUME_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <stamwatervolume.hpp>

namespace Orbis {

namespace Script {

/*!
 * \brief This class exports the WaterVolume to the Lua interpreter.
 */
class LuaStamWaterVolume {
public:
	/*!
	 * \brief Register this class to the Lua interpreter.
	 * \param L The Lua state.
	 */
	static void registerIntoLua(lua_State* L);

	/*!
	 * \brief checks if element at given index is a WaterVolume.
	 * \param L The Lua state.
	 * \param index Stack index of element to be checked.
	 * \return A pointer to the object or 0 if element is not of this type
	 */
	static Orbis::Drawable::StamWaterVolume* checkInstance(lua_State* L, int index);

private:
	/*!
	 * \brief Creates a new instance of a WaterVolume.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int create(lua_State* L);

	/*!
	 * \brief Collects the memory of a WaterVolume.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int collect(lua_State* L);

	/*!
	 * \brief The number of samples in each side of the volume.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int size(lua_State* L);

	/*!
	 * \brief Finds a point in space given its grid coordinates.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int point(lua_State* L);

	/*!
	 * \brief The current calculated density at a grid vertex.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int density(lua_State* L);

	/*!
	 * \brief The current calculated velocity at a grid vertex.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int velocity(lua_State* L);

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
	 * \brief Queries the diffusion rate of the fluid.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int diffuse(lua_State* L);

	/*!
	 * \brief Sets the diffusion rate of the fluid.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setDiffuse(lua_State* L);

	/*!
	 * \brief Queries the viscosity of the fluid.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int viscosity(lua_State* L);

	/*!
	 * \brief Sets the viscosity of the fluid.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setViscosity(lua_State* L);

	/*!
	 * \brief Sets the water bottom.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int setBottom(lua_State* L);

	/*!
	 * \brief Adds this object to the world.
	 * \param L The Lua state.
	 * \return The number of results pushed onto the stack.
	 */
	static int addToWorld(lua_State* L);

	// metadata
	static const char className[];
	static const luaL_reg methods[];
};

} } // namespace declarations

#endif //__ORBIS_LUASTAMWATERVOLUME_HPP__
