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

#ifndef __ORBIS__LUASCRIPT_HPP__
#define __ORBIS__LUASCRIPT_HPP__

#ifdef __GNUG__
#pragma interface
#endif

extern "C" {
#include <lua.h>
}

#include <dynamic.hpp>
#include <luaactionadapter.hpp>

namespace Orbis {

namespace Script {
	   
/*!
 * \brief This class comprises the scripting engine.
 *
 * The LuaScript class exports the Orbis' primitives to the Lua interpreter
 * and runs external script files. It is notified at regular intervals by
 * the Timer so it can control animation as well.
 */
class LuaScript : public Orbis::Dynamic {
public:
	//! Default constructor
	LuaScript();

	//! Destructor
	~LuaScript();

	/*!
	 * \brief Starts anew.
	 */
	void reset();

	/*!
	 * \brief Sets the external action adapter.
	 * \param adapter The external action adapter.
	 */
	void setActionAdapter(LuaActionAdapter* adapter);

	/*!
	 * \brief Runs a script from file.
	 * \param filename The name of the file with the script.
	 */
	void runFile(const std::string& filename);

	/*!
	 * \brief Updates the interpreter state.
	 * \param time The elapsed time.
	 */
	void evolve(unsigned long time);

private:

	/*!
	 * \brief Lua function to show message to user.
	 * \param L The Lua state
	 * \return The number of results pushed onto the stack.
	 */
	static int showMessage(lua_State* L);

	bool _active;

	// tick counter
	unsigned long _ticks;

	// the interpreter state
	lua_State *_lua_state;

	// the action adapter
	LuaActionAdapter *_action_adapter;
};

inline void LuaScript::setActionAdapter(LuaActionAdapter *adapter)
{
	_action_adapter = adapter;
}

} } // namespace declarations

#endif  // __ORBIS_LUASCRIPT_HPP__

