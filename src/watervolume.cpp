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
 * $Id: watervolume.cpp,v 1.2 2004/05/20 02:57:46 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <watervolume.hpp>

namespace Orbis {

namespace Drawable {
	   
WaterVolume::WaterVolume(unsigned size)
	: _size(size)
{
	_u.reserve(size);
	_v.reserve(size);
	_w.reserve(size);
	_u_prev.reserve(size);
	_v_prev.reserve(size);
	_w_prev.reserve(size);
	_dens.reserve(size);
	_dens_prev.reserve(size);
}

WaterVolume::~WaterVolume()
{
}

void WaterVolume::evolve(unsigned long time)
{
	lock();

	unlock();
}

void WaterVolume::add_sources(DoubleVector& x,
							const DoubleVector& srcs, double dt)
{
	for(unsigned i = 0; i < _size; i++) {
		x[i] += dt * srcs[i];
	}
}

void WaterVolume::diffuse(int b, DoubleVector& x,
							DoubleVector& x0, double diff, double dt)
{
	double a = dt * diff * Orbis::Math::sqr(_size-2);

	for(unsigned l = 0; l < 20; l++) {
		for(unsigned i = 1; i < _size - 1; i++) {
			for(unsigned j = 1; j < _size - 1; j++) {
				for(unsigned k = 1; k < _size - 1; k++) {
					x[i3d(i, j, k)] =
						(x0[i3d(i, j, k)] +
							a *(x[i3d(i-1, j, k)] + x[i3d(i+1, j, k)] +
								x[i3d(i, j-1, k)] + x[i3d(i, j+1, k)] +
								x[i3d(i, j, k-1)] + x[i3d(i, j, k+1)])) / (1+6*a);
							
				}
			}
		}
	}

	set_bounds(b, x);
}

void WaterVolume::advect(int b, DoubleVector& d,
					DoubleVector& d0, DoubleVector& u,
						DoubleVector& v, DoubleVector& w, double dt)
{
	double dt0 = dt * (_size-2);
	for(unsigned i = 1; i < _size - 1; i++) {
		for(unsigned j = 1; j < _size - 1; j++) {
			for(unsigned k = 1; k < _size - 1; k++) {
				double x = i - dt0 * u[i3d(i, j, k)];
				double y = j - dt0 * v[i3d(i, j, k)];
				double z = k - dt0 * w[i3d(i, j, k)];
				Orbis::Math::clamp(x, 0.5, _size - 1.5);
				int i0 = static_cast<int>(x);
				int i1 = i0 + 1;
				double s1 = x - i0;
				double s0 = 1 - s1;
				Orbis::Math::clamp(y, 0.5, _size - 1.5);
				int j0 = static_cast<int>(y);
				int j1 = j0 + 1;
				double t1 = y - j0;
				double t0 = 1 - t1;
				Orbis::Math::clamp(z, 0.5, _size - 1.5);
				int k0 = static_cast<int>(k);
				int k1 = k0 + 1;
				double r1 = z - k0;
				double r0 = 1 - r1;
			}
		}
	}
}

void WaterVolume::set_bounds(int b, DoubleVector& x)
{
}

} } // namespace declarations

