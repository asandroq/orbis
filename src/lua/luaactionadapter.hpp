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

#ifndef __ORBIS_LUAACTIONADAPTER_HPP__
#define __ORBIS_LUAACTIONADAPTER_HPP__

#include <string>

namespace Orbis {

	namespace Script {

/*!
 * \brief Base class of all adaptors of Lua requests to the underlying
 * toolkit.
 */
class LuaActionAdapter {
public:
	/*!
	 * \brief Constructor default.
	 */
	LuaActionAdapter();

	/*!
	 * \brief Destructor.
	 */
	virtual ~LuaActionAdapter();

	/*!
	 * \brief Processes pending events.
	 */
	virtual void processEvents() = 0;

	/*!
	 * \brief Shows an ordinary message, for instance invoked
	 * from Lua's print function.
	 * \param msg The message to be shown.
	 */
	virtual void showMessage(const std::string& msg) const = 0;

	/*!
	 * \brief Shows an error message.
	 * \param msg The error message.
	 */
	virtual void showErrorMessage(const std::string& msg) const = 0;
};

inline LuaActionAdapter::LuaActionAdapter()
{
}

inline LuaActionAdapter::~LuaActionAdapter()
{
}

} } // namespace declarations

#endif  // __ORBIS_LUAACTIONADAPTER_HPP__

