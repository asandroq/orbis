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

#include <geometry.hpp>
#include <watervolume.hpp>

namespace Orbis {

namespace Drawable {

bool WaterVolume::locate(const Point& p, unsigned* i, unsigned* j, unsigned* k) const
{
	if(!i || !j || !k) {
		return false;
	}

	Point end = Point(_origin.x() + (_size-1) * _step,
						_origin.y() + (_size-1) * _step,
						_origin.z() + (_size-1) * _step);
	if(!pointInVolume(p, _origin, end)) {
		return false;
	}

	double x = floor((p.x() - _origin.x()) / _step);
	double y = floor((p.y() - _origin.y()) / _step);
	double z = floor((p.z() - _origin.z()) / _step);

	*i = static_cast<unsigned>(x);
	*j = static_cast<unsigned>(y);
	*k = static_cast<unsigned>(z);

	return true;
}

WaterVolume::Status WaterVolume::classifyCell(unsigned i, unsigned j, unsigned k) const
{
	Point p;
	// a cube has eight vertices, and a byte has eight bits... :-)
	unsigned char vertices = 0x00;

	if(bottom() == 0) {
		throw std::logic_error("classifying cells without bottom");
	}

	p = point(i, j, k);
	if(p.z() > bottom()->point(p.x(), p.y()).z()) vertices |= 0x01;
	p = point(i, j, k+1);
	if(p.z() > bottom()->point(p.x(), p.y()).z()) vertices |= 0x02;
	p = point(i, j+1, k);
	if(p.z() > bottom()->point(p.x(), p.y()).z()) vertices |= 0x04;
	p = point(i+1, j, k);
	if(p.z() > bottom()->point(p.x(), p.y()).z()) vertices |= 0x08;
	p = point(i+1, j+1, k);
	if(p.z() > bottom()->point(p.x(), p.y()).z()) vertices |= 0x10;
	p = point(i+1, j, k+1);
	if(p.z() > bottom()->point(p.x(), p.y()).z()) vertices |= 0x20;
	p = point(i, j+1, k+1);
	if(p.z() > bottom()->point(p.x(), p.y()).z()) vertices |= 0x40;
	p = point(i+1, j+1, k+1);
	if(p.z() > bottom()->point(p.x(), p.y()).z()) vertices |= 0x80;

	if(vertices == 0x00) {
		return BELOW;
	} else if(vertices == 0xff) {
		return ABOVE;
	} else {
		return BOUNDARY;
	}
}

WaterVolume::WaterVolume(const Orbis::Util::Point& point,
									unsigned size, double step)
	: WaterBase(), _origin(point), _step(step),
						_diff(0.1), _visc(5.0), _size(size)
{
	unsigned size3 = Orbis::Math::cub(size);

	_status.reserve(size3);

	_u.reserve(size3);
	_v.reserve(size3);
	_w.reserve(size3);
	_u_prev.reserve(size3);
	_v_prev.reserve(size3);
	_w_prev.reserve(size3);

	_dens.reserve(size3);
	_dens_prev.reserve(size3);

	for(unsigned i = 0; i < size3; i++) {
		_status[i] = ABOVE;
		_dens[i] = _dens_prev[i] = 0.0;
		_u[i] = _v[i] = _w[i] = _u_prev[i] = _v_prev[i] = _w_prev[i] = 0.0;
	}
}

void WaterVolume::setBottom(const HeightField* const hf)
{
	WaterBase::setBottom(hf);

	if(!bottom()) {
		return;
	}

	for(unsigned i = 0; i < _size; i++) {
		for(unsigned j = 0; j < _size; j++) {
			for(unsigned k = 0; k < _size; k++) {
				_status[i3d(i, j, k)] = classifyCell(i, j, k);
			}
		}
	}
}

void WaterVolume::evolve(unsigned long time)
{
	// gravity
	const double g = -10.0;

	double dt = time / 1000.0;
	unsigned size = Orbis::Math::cub(_size);

	// initial state
	for(unsigned i = 0; i < size; i++) {
		_w_prev[i] = g;
		_dens_prev[i] = _u_prev[i] = _v_prev[i] = 0.0;
	}

	// adding sources to vectors
	for(SourceIterator it = sources(); it != sourcesEnd(); it++) {
		unsigned i, j, k;
		Point p = it->first;
		if(locate(p, &i, &j, &k)) {
			_dens_prev[i3d(i, j, k)] = it->second;
		}
	}

	Locker lock(this);

	vel_step(_u, _v, _w, _u_prev, _v_prev, _w_prev, _visc, dt);
	dens_step(_dens, _dens_prev, _u, _v, _w, _diff, dt);
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
		set_bounds(b, x);
	}
}

void WaterVolume::advect(int b, DoubleVector& d,
						DoubleVector& d0, DoubleVector& u,
						DoubleVector& v, DoubleVector& w, double dt) const
{
	using Orbis::Math::clamp;

	double dt0 = dt * (_size-2);
	for(unsigned i = 1; i < _size - 1; i++) {
		for(unsigned j = 1; j < _size - 1; j++) {
			for(unsigned k = 1; k < _size - 1; k++) {
				double x = i - dt0 * u[i3d(i, j, k)];
				double y = j - dt0 * v[i3d(i, j, k)];
				double z = k - dt0 * w[i3d(i, j, k)];
				x = clamp(x, 0.5, _size - 1.5);
				int i0 = static_cast<int>(x);
				int i1 = i0 + 1;
				double s1 = x - i0;
				double s0 = 1.0 - s1;
				y = clamp(y, 0.5, _size - 1.5);
				int j0 = static_cast<int>(y);
				int j1 = j0 + 1;
				double t1 = y - j0;
				double t0 = 1.0 - t1;
				z = clamp(z, 0.5, _size - 1.5);
				int k0 = static_cast<int>(z);
				int k1 = k0 + 1;
				double r1 = z - k0;
				double r0 = 1.0 - r1;
				d[i3d(i, j, k)] =
					s0 * (t0 * (r0 * d0[i3d(i0, j0, k0)] + r1 * d0[i3d(i0, j0, k1)])  +
                          t1 * (r0 * d0[i3d(i0, j1, k0)] + r1 * d0[i3d(i0, j1, k1)])) +
					s1 * (t0 * (r0 * d0[i3d(i1, j0, k0)] + r1 * d0[i3d(i1, j0, k1)])  +
                          t1 * (r0 * d0[i3d(i1, j1, k0)] + r1 * d0[i3d(i1, j1, k1)]));
			}
		}
	}

	set_bounds(b, d);
}

void WaterVolume::project(DoubleVector& u,
				 			DoubleVector& v, DoubleVector& w,
								DoubleVector& p, DoubleVector& div) const
{
	double h = _step / (_size - 2);
	for(unsigned i = 1; i < _size - 1; i++) {
		for(unsigned j = 1; j < _size - 1; j++) {
			for(unsigned k = 1; k < _size - 1; k++) {
				div[i3d(i, j, k)] = -0.5 * h * (u[i3d(i+1, j, k)] - u[i3d(i-1, j, k)] +
							                    v[i3d(i, j+1, k)] - v[i3d(i, j-1, k)] +
										        w[i3d(i, j, k+1)] - w[i3d(i, j, k-1)]);
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
	// bottom boundary
	if(bottom() != 0) {
		for(unsigned i = 1; i < _size - 1; i++) {
			for(unsigned j = 1; j < _size - 1; j++) {
				for(unsigned k = 1; k < _size - 1; k++) {
					if(_status[i3d(i, j, k)] == BOUNDARY) {
						switch(b) {
							case 1:
								if(_status[i3d(i+1, j, k)] == ABOVE) {
									x[i3d(i, j, k)] = -x[i3d(i+1, j, k)];
								} else {
									x[i3d(i, j, k)] = -x[i3d(i-1, j, k)];
								}
								break;
							case 2:
								if(_status[i3d(i, j+1, k)] == ABOVE) {
									x[i3d(i, j, k)] = -x[i3d(i, j-1, k)];
								} else {
									x[i3d(i, j, k)] = -x[i3d(i, j+1, k)];
								}
								break;
							case 3:
								x[i3d(i, j, k)] = -x[i3d(i, j, k+1)];
								break;
							default:
								x[i3d(i, j, k)] =  x[i3d(i, j, k+1)];
						}
					}
				}
			}
		}
	}

	// faces
	for(unsigned i = 1; i < _size - 1; i++) {
		for(unsigned j = 1; j < _size - 1; j++) {
			switch(b) {
				case 1:
					x[i3d(      0, i, j)] = -x[i3d(      1, i, j)];
					x[i3d(_size-1, i, j)] = -x[i3d(_size-2, i, j)];
					x[i3d(i,       0, j)] =  x[i3d(i,       1, j)];
					x[i3d(i, _size-1, j)] =  x[i3d(i, _size-2, j)];
					x[i3d(i, j,       0)] =  x[i3d(i, j,       1)];
					x[i3d(i, j, _size-1)] =  x[i3d(i, j, _size-2)];
					break;
				case 2:
					x[i3d(      0, i, j)] =  x[i3d(      1, i, j)];
					x[i3d(_size-1, i, j)] =  x[i3d(_size-2, i, j)];
					x[i3d(i,       0, j)] = -x[i3d(i,       1, j)];
					x[i3d(i, _size-1, j)] = -x[i3d(i, _size-2, j)];
					x[i3d(i, j,       0)] =  x[i3d(i, j,       1)];
					x[i3d(i, j, _size-1)] =  x[i3d(i, j, _size-2)];
					break;
				case 3:
					x[i3d(      0, i, j)] =  x[i3d(      1, i, j)];
					x[i3d(_size-1, i, j)] =  x[i3d(_size-2, i, j)];
					x[i3d(i,       0, j)] =  x[i3d(i,       1, j)];
					x[i3d(i, _size-1, j)] =  x[i3d(i, _size-2, j)];
					x[i3d(i, j,       0)] = -x[i3d(i, j,       1)];
					x[i3d(i, j, _size-1)] = -x[i3d(i, j, _size-2)];
					break;
				default:
					x[i3d(      0, i, j)] = x[i3d(      1, i, j)];
					x[i3d(_size-1, i, j)] = x[i3d(_size-2, i, j)];
					x[i3d(i,       0, j)] = x[i3d(i,       1, j)];
					x[i3d(i, _size-1, j)] = x[i3d(i, _size-2, j)];
					x[i3d(i, j,       0)] = x[i3d(i, j,       1)];
					x[i3d(i, j, _size-1)] = x[i3d(i, j, _size-2)];
			}
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
	x[i3d(0, 0, 0)] =
		(x[i3d(1, 0, 0)] +
			x[i3d(0, 1, 0)] + x[i3d(0, 0, 1)]) / 3.0;
	x[i3d(_size-1, 0, 0)] =
		(x[i3d(_size-2, 0, 0)] +
			x[i3d(_size-1, 1, 0)] + x[i3d(_size-1, 0, 1)]) / 3.0;
	x[i3d(0, _size-1, 0)] =
		(x[i3d(1, _size-1, 0)] +
			x[i3d(0, _size-2, 0)] + x[i3d(0, _size-1, 1)]) / 3.0;
	x[i3d(_size-1, _size-1, 0)] =
		(x[i3d(_size-2, _size-1, 0)] +
			x[i3d(_size-1, _size-2, 0)] + x[i3d(_size-1, _size-1, 1)]) / 3.0;
	x[i3d(0, 0, _size-1)] =
		(x[i3d(1, 0, _size-1)] +
			x[i3d(0, 1, _size-1)] + x[i3d(0, 0, _size-2)]) / 3.0;
	x[i3d(_size-1, 0, _size-1)] =
		(x[i3d(_size-2, 0, _size-1)] +
			x[i3d(_size-1, 1, _size-1)] + x[i3d(_size-1, 0, _size-2)]) / 3.0;
	x[i3d(0, _size-1, _size-1)] =
		(x[i3d(1, _size-1, _size-1)] +
			x[i3d(0, _size-2, _size-1)] + x[i3d(0, _size-1, _size-2)]) / 3.0;
	x[i3d(_size-1, _size-1, _size-1)] =
		(x[i3d(_size-2, _size-1, _size-1)] +
			x[i3d(_size-1, _size-2, _size-1)] + x[i3d(_size-1, _size-1, _size-2)]) / 3.0;
}

void WaterVolume::dens_step(DoubleVector& d, DoubleVector& d0,
						DoubleVector& u, DoubleVector& v,
						DoubleVector& w, double diff, double dt) const
{
	add_sources(d, d0, dt);
	swap(d, d0);
	diffuse(0, d, d0, diff, dt);
	swap(d, d0);
	advect(0, d, d0, u, v, w, dt);
}

void WaterVolume::vel_step(DoubleVector& u, DoubleVector& v,
						DoubleVector& w, DoubleVector& u0,
							DoubleVector& v0, DoubleVector& w0,
										double visc, double dt) const
{
	add_sources(u, u0, dt);
	add_sources(v, v0, dt);
	add_sources(w, w0, dt);
	swap(u, u0);
	swap(v, v0);
	swap(w, w0);
	diffuse(1, u, u0, visc, dt);
	diffuse(2, v, v0, visc, dt);
	diffuse(3, w, w0, visc, dt);
	project(u, v, w, u0, v0);
	swap(u, u0);
	swap(v, v0);
	swap(w, w0);
	advect(1, u, u0, u0, v0, w0, dt);
	advect(2, v, v0, u0, v0, w0, dt);
	advect(3, w, w0, u0, v0, w0, dt);
	project(u, v, w, u0, v0);
}

} } // namespace declarations

