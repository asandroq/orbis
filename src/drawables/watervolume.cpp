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

#ifdef __GNUG__
#pragma implementation
#endif

#include <watervolume.hpp>

namespace Orbis {

namespace Drawable {
	   
WaterVolume::WaterVolume(unsigned size)
	: _size(size)
{
	unsigned size3 = Orbis::Math::cub(size);

	_u.reserve(size3);
	_v.reserve(size3);
	_w.reserve(size3);
	_u_prev.reserve(size3);
	_v_prev.reserve(size3);
	_w_prev.reserve(size3);
	_dens.reserve(size3);
	_dens_prev.reserve(size3);
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
							const DoubleVector& srcs, double dt) const
{
	unsigned size3 = Orbis::Math::cub(_size);

	for(unsigned i = 0; i < size3; i++) {
		x[i] += dt * srcs[i];
	}
}

void WaterVolume::diffuse(int b, DoubleVector& x, DoubleVector& x0,
			 						double diff, double dt) const
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
						DoubleVector& v, DoubleVector& w, double dt) const
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
				d[i3d(i, j, k)] =
					s0 * (t0 * (r0 * d0[i3d(i0, j0, k0)] +
							  r1 * d0[i3d(i0, j0, k1)]) +
                               t1 * (r0 * d0[i3d(i0, j1, k0)] +
							  r1 * d0[i3d(i0, j1, k1)])) +
					s1 * (t0 * (r0 * d0[i3d(i1, j0, k0)] +
							  r1 * d0[i3d(i1, j0, k1)]) +
                               t1 * (r0 * d0[i3d(i1, j1, k0)] +
							  r1 * d0[i3d(i1, j1, k1)]));
			}
		}
	}

	set_bounds(b, d);
}

void WaterVolume::project(DoubleVector& u,
			 			DoubleVector& v, DoubleVector& w,
							DoubleVector& p, DoubleVector& div) const
{
	double h = 1.0 / (_size - 2);
	for(unsigned i = 1; i < _size - 1; i++) {
		for(unsigned j = 1; j < _size - 1; j++) {
			for(unsigned k = 1; k < _size - 1; k++) {
				div[i3d(i, j, k)] = -0.5 * h * (u[i3d(i+1, j, k)] -
							 			  u[i3d(i-1, j, k)] +
							                 v[i3d(i, j+1, k)] -
										  v[i3d(i, j-1, k)] +
										  w[i3d(i, j, k+1)] -
										  w[i3d(i, j, k-1)]);
				p[i3d(i, j, k)] = 0.0;
			}
		}
	}

	set_bounds(0, div);
	set_bounds(0, p);

	for(unsigned l = 0; l < 20; l++) {
		for(unsigned i = 1; i < _size - 1; i++) {
			for(unsigned j = 1; j < _size - 1; j++) {
				for(unsigned k = 1; k < _size - 1; k++) {
					p[i3d(i, j, k)] = (div[i3d(i, j, k)] +
									p[i3d(i-1, j, k)] +
									p[i3d(i+1, j, k)] +
									p[i3d(i, j-1, k)] +
									p[i3d(i, j+1, k)] +
									p[i3d(i, j, k-1)] +
									p[i3d(i, j, k+1)]) / 6.0;
				}
			}
		}
		set_bounds(0, p);
	}

	for(unsigned i = 1; i < _size - 1; i++) {
		for(unsigned j = 1; j < _size - 1; j++) {
			for(unsigned k = 1; k < _size - 1; k++) {
				u[i3d(i, j, k)] -=
					0.5 * (p[i3d(i+1, j, k)] - p[i3d(i-1, j, k)]) / h;
				v[i3d(i, j, k)] -=
					0.5 * (p[i3d(i, j+1, k)] - p[i3d(i, j-1, k)]) / h;
				w[i3d(i, j, k)] -=
					0.5 * (p[i3d(i, j, k+1)] - p[i3d(i, j, k-1)]) / h;
			}
		}
	}

	set_bounds(1, u);
	set_bounds(2, v);
	set_bounds(3, w);
}

void WaterVolume::set_bounds(int b, DoubleVector& x) const
{
	// faces
	for(unsigned i = 1; i < _size - 1; i++) {
		for(unsigned j = 1; j < _size - 1; j++) {
			x[i3d(0, i, j)] =
				   b == 1 ? -x[i3d(1, i, j)] : x[i3d(1, i, j)];
			x[i3d(_size-1, i, j)] =
				   b == 1 ? -x[i3d(_size-2, i, j)] : x[i3d(_size-2, i, j)];
			x[i3d(i, 0, j)] =
				   b == 2 ? -x[i3d(i, 1, j)] : x[i3d(i, 1, j)];
			x[i3d(i, _size-1, j)] =
				   b == 2 ? -x[i3d(i, _size-2, j)] : x[i3d(i, _size-2, j)];
			x[i3d(i, j, 0)] =
				   b == 3 ? -x[i3d(i, j, 1)] : x[i3d(i, j, 1)];
			x[i3d(i, j, _size-1)] =
				   b == 3 ? -x[i3d(i, j, _size-2)] : x[i3d(i, j, _size-2)];
		}
	}

	// edges
	for(unsigned i = 1; i < _size - 1; i++) {
		x[i3d(0, 0, i)] =
			0.5*(x[i3d(0, 1, i)] + x[i3d(1, 0, i)]);
		x[i3d(_size-1, 0, i)] =
			0.5*(x[i3d(_size-1, 1, i)] + x[i3d(_size-2, 0, i)]);
		x[i3d(0, _size-1, i)] =
			0.5*(x[i3d(1, _size-1, i)] + x[i3d(0, _size-2, i)]);
		x[i3d(_size-1, _size-1, i)] =
			0.5*(x[i3d(_size-2, _size-1, i)] + x[i3d(_size-1, _size-2, i)]);

		x[i3d(0, i, 0)] =
			0.5*(x[i3d(0, i, 1)] + x[i3d(1, i, 0)]);
		x[i3d(_size-1, i, 0)] =
			0.5*(x[i3d(_size-1, i, 1)] + x[i3d(_size-2, i, 0)]);
		x[i3d(0, i, _size-1)] =
			0.5*(x[i3d(1, i, _size-1)] + x[i3d(0, i, _size-2)]);
		x[i3d(_size-1, i, _size-1)] =
			0.5*(x[i3d(_size-2, i, _size-1)] + x[i3d(_size-1, i, _size-2)]);

		x[i3d(i, 0, 0)] =
			0.5*(x[i3d(i, 0, 1)] + x[i3d(i, 1, 0)]);
		x[i3d(i, _size-1, 0)] =
			0.5*(x[i3d(i, _size-1, 1)] + x[i3d(i, _size-2, 0)]);
		x[i3d(i, 0, _size-1)] =
			0.5*(x[i3d(i, 1, _size-1)] + x[i3d(i, 0, _size-2)]);
		x[i3d(i, _size-1, _size-1)] =
			0.5*(x[i3d(i, _size-2,  _size-1)] + x[i3d(i, _size-1, _size-2)]);
	}

	// vertices
	
}

} } // namespace declarations

