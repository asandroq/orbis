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

#include <ctime>
#include <cstdlib>

#include <math.hpp>

namespace Orbis {

	namespace Math {

unsigned Random::_state = time(0);

double Random::rand()
{
	return rand_r(&_state) / static_cast<double>(RAND_MAX);
}

double Random::rand2()
{
	return (2.0 * rand_r(&_state) / static_cast<double>(RAND_MAX)) - 1.0;
}

} } // namespace declarations
