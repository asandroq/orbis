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
 * $Id: dynamic.hpp,v 1.4 2004/02/26 19:29:45 asandro Exp $
 */

#ifndef __ORBIS_DYNAMIC_HPP__
#define __ORBIS_DYNAMIC_HPP__

#include <OpenThreads/Mutex>

namespace Orbis {

/*!
 * \brief This class is the parent of all the dynamic objects.
 * 
 * Every object of the system which changes over time or needs to be notified
 * by the global Timer is a descendant of this class. As the Timer runs in
 * another thread, it has its own mutex.
 */
class Dynamic {
public:
	//! Constructor
	Dynamic();

	//! Destructor
	virtual ~Dynamic();

	//! Called to allow the object to evolve
	/*!
	 * \param time Elapsed time since last update.
	 */
	virtual void evolve(unsigned long time) = 0;

protected:
	//! Locks the object so it's not modified by another thread.
	/*!
	 * \returns 0 in case of success, or the error code otherwise.
	 */
	int lock() const;

	//! Unlocks the object.
	/*!
	 * \returns 0 in case of success, or the error code otherwise.
	 */
	int unlock() const;

private:
	//! To prevent the evolving from different threads
	mutable OpenThreads::Mutex _mutex;
};

inline Dynamic::Dynamic()
{
}

inline Dynamic::~Dynamic()
{
}

inline int Dynamic::lock() const
{
	return _mutex.lock();
}

inline int Dynamic::unlock() const
{
	return _mutex.unlock();
}

} // namespace declarations

#endif  // __ORBIS_DYNAMIC_HPP__

