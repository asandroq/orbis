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

#ifndef __ORBIS_FOXACTIONADAPTER_HPP__
#define __ORBIS_FOXACTIONADAPTER_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <queue>
#include <string>

#include <fox/fx.h>
#include <OpenThreads/Mutex>

#include <luaactionadapter.hpp>

namespace Orbis {

	namespace Script {

/*!
 * \brief This class adapts Lua requests to the FOX toolkit.
 */
class FoxActionAdapter : public LuaActionAdapter {
public:
	/*!
	 * \brief Constructor default.
	 */
	FoxActionAdapter(FXWindow *owner);

	/*!
	 * \brief Destructor.
	 */
	~FoxActionAdapter();

	/*!
	 * \brief Locks this object so it can be accessed by only a thread
	 * at one time.
	 *
	 * It should not logically be a \a const function, but it is so to
	 * adhere to the LuaActionAdapter interace.
	 */
	int lock() const;

	/*!
	 * \brief Unlocks this object so it can be accessed by more than
	 * one thread.
	 *
	 * It should not logically be a \a const function, but it is so to
	 * adhere to the LuaActionAdapter interace.
	 */
	int unlock() const;

	/*!
	 * \brief Processes pending events.
	 */
	void processEvents();

	/*!
	 * \brief Shows an ordinary message.
	 * \param msg The message to be shown.
	 */
	void showMessage(const std::string& msg) const;

	/*!
	 * \brief Shows an error message.
	 * \param msg The error message.
	 */
	void showErrorMessage(const std::string& msg) const;

protected:
	// queues of messages
	mutable std::queue<std::string> _messages;
	mutable std::queue<std::string> _error_messages;

private:
	// owner window, to show messages over
	FXWindow *_owner;
	// mutex for protecting accesses
	mutable OpenThreads::Mutex _mutex;
};

inline FoxActionAdapter::FoxActionAdapter(FXWindow *owner)
	: LuaActionAdapter(), _owner(owner)
{
}

inline FoxActionAdapter::~FoxActionAdapter()
{
}

inline int FoxActionAdapter::lock() const
{
	return _mutex.lock();
}

inline int FoxActionAdapter::unlock() const
{
	return _mutex.unlock();
}

} } // namespace declarations

#endif  // __ORBIS_FOXACTIONADAPTER_HPP__

