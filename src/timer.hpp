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
 * $Id: timer.hpp,v 1.8 2004/05/12 14:25:03 asandro Exp $
 */

#ifndef __ORBIS_TIMER_HPP__
#define __ORBIS_TIMER_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <vector>

#include <OpenThreads/Mutex>
#include <OpenThreads/Thread>

#include <dynamic.hpp>

namespace Orbis {

	namespace Util {

/*!
 * \brief Timer used to synchronise all the World's dynamic objects.
 */
class Timer : public OpenThreads::Thread {
public:
	//! Constructor
	/*!
	 * \param timeout The initial timeout.
	 */
	Timer(unsigned long timeout = 50);

	//! Destructor
	virtual ~Timer();

	/*!
	 * \brief Starts anew.
	 */
	void reset();

	//! The timeout.
	/*!
	 * \returns The current timeout, in miliseconds.
	 * \sa setTimeout
	 */
	unsigned long timeout() const;

	//! Sets the new timeout.
	/*!
	 * \param timeout The new timeout, in miliseconds.
	 * \sa timeout
	 */
	void setTimeout(unsigned long timeout);

	//! Sets the timer active or inactive.
	/*!
	 * \param active The new status of the timer.
	 */
	void setActive(bool active);

	//! Adds another dynamic object to be updated.
	/*!
	 * \param obj The newly added object.
	 */
	void addDynamic(Orbis::Dynamic* obj);
	
	//! Runs the timer
	virtual void run();

private:
	// status
	bool _active;
	// the timeout
	unsigned long _time_out;
	// mutex for syncronized access
	OpenThreads::Mutex _mutex;
	// list of pointer to objects that must be notified
	std::vector<Orbis::Dynamic*> _dynamic_objects;
};

inline Timer::Timer(unsigned long timeout)
	: OpenThreads::Thread(), _active(false), _time_out(timeout)
{
}

inline unsigned long Timer::timeout() const
{
	return _time_out;
}

} } // namespace declarations

#endif  // __ORBIS_TIMER_HPP__

