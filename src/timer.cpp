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
 * $Id: timer.cpp,v 1.9 2004/02/19 20:59:10 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <timer.hpp>

namespace Orbis {

	namespace Util {

Timer::~Timer()
{
	/*
	 * I'm not gonna destroy the objects in the dynamic list
	 * because the timer doesn't own them, the world does.
	 */
}

void Timer::reset()
{
	_mutex.lock();
	_active = false;
	_dynamic_objects.clear();
	_mutex.unlock();
}

void Timer::setTimeout(unsigned long timeout)
{
	_mutex.lock();
	_time_out = timeout;
	_mutex.unlock();
}

void Timer::setActive(bool active)
{
	_mutex.lock();
	_active = active;
	_mutex.unlock();
}

void Timer::addDynamic(Orbis::Dynamic* obj)
{
	_mutex.lock();
	_dynamic_objects.push_back(obj);
	_mutex.unlock();
}

void Timer::run()
{
	struct timeval tv;

	// loop till the thread ends
	while(true) {
		// timeout may change anytime
		tv.tv_sec  =  _time_out / 1000L;
		tv.tv_usec = (_time_out % 1000L) * 1000L;

		// wait specified timeout
		select(0, 0, 0, 0, &tv);

		// should I do anything?
		if(!_active) {
			continue;
		}

		// notify all registered objects
		// I should just notify, not wait for them to evolve
		_mutex.lock();
		std::vector<Orbis::Dynamic*>::iterator it;
		for(it = _dynamic_objects.begin();
					it != _dynamic_objects.end(); it++) {
			(*it)->evolve(_time_out);
		}
		_mutex.unlock();
	}
}

}} // namespace declarations

