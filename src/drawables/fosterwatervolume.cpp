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

#include <iostream>

#include <fosterwatervolume.hpp>

namespace Orbis {

	namespace Drawable {

/*
 * One row more is allocated in each direction because the velocities are
 * at the faces of the cells, not at their center.
 */
FosterWaterVolume::FosterWaterVolume(const Orbis::Util::Point& origin,
								unsigned size_x, unsigned size_y, unsigned size_z,
										double step_x, double step_y, double step_z)
	: WaterVolume(origin, size_x+1, size_y+1, size_z+1, step_x, step_y, step_z),
		_atm_p(1.0), _actual_dt(-1.0)
{
	using Orbis::Math::sqr;

	_inv_x = 1.0 / stepX();
	_inv_x2 = sqr(_inv_x);
	_inv_y = 1.0 / stepY();
	_inv_y2 = sqr(_inv_y);
	_inv_z = 1.0 / stepZ();
	_inv_z2 = sqr(_inv_z);

	unsigned size = sizeX() * sizeY() * sizeZ();

	_u.resize(size);
	_u_prev.resize(size);
	_v.resize(size);
	_v_prev.resize(size);
	_w.resize(size);
	_w_prev.resize(size);
	_p.resize(size);
	_p_prev.resize(size);
	_acc_x.resize(size);
	_acc_y.resize(size);
	_acc_z.resize(size);
	_status.resize(size);
	_status_prev.resize(size);
	_part_lists.resize(size);

	// all cells at the boundaries of the volume are treated as solid
	for(unsigned i = 0; i < sizeX(); i++) {
		for(unsigned j = 0; j < sizeY(); j++) {
			for(unsigned k = 0; k < sizeZ(); k++) {
				unsigned l = i3d(i, j, k);
				_p[l] = _p_prev[l] = _atm_p;
				_u[l] = _v[l] = _w[l] = 0.0;
				_u_prev[l] = _v_prev[l] = _w_prev[l] = 0.0;
				if(i == 0 || i > sizeX() - 3 ||
							j == 0 || j > sizeY() - 3 ||
								k == 0 || k > sizeZ() - 3) {
					_status[l] = _status_prev[l] = SOLID;
				} else {
					_status[l] = _status_prev[l] = EMPTY;
				}
			}
		}
	}
}

Vector FosterWaterVolume::velocity(const Point& p) const
{
	using Orbis::Math::interpolate;

	double t;
	unsigned i, j, k;

	if(!locate(p, &i, &j, &k)) {
		throw std::logic_error("point is outside volume");
	}

	// simple linear interpolation
	t = interpolate(p.x(), point(i, j, k).x(), point(i+1, j, k).x());
	double u = _u[i3d(i, j, k)] * (1.0 - t) + _u[i3d(i+1, j, k)] * t;
	t = interpolate(p.y(), point(i, j, k).y(), point(i, j+1, k).y());
	double v = _v[i3d(i, j, k)] * (1.0 - t) + _v[i3d(i, j+1, k)] * t;
	t = interpolate(p.z(), point(i, j, k).z(), point(i, j, k+1).z());
	double w = _w[i3d(i, j, k)] * (1.0 - t) + _w[i3d(i, j, k+1)] * t;

	return Vector(u, v, w);
}

/*
 * This class stores the faces' velocities, so it's necessary to do some math to
 * find the velocity vector at a specified grid point.
 */
Vector FosterWaterVolume::velocity(unsigned i, unsigned j, unsigned k) const
{
	if(i == 0 || i == sizeX() - 1 || j == 0 || j == sizeY() - 1 || k == 0 || k == sizeZ() - 1) {
		return Vector(0.0, 0.0, 0.0);
	} else {
		double u = 0.25 * (_u_prev[i3d(i, j, k)] + _u_prev[i3d(i, j-1, k)] +
						_u_prev[i3d(i, j, k-1)] + _u_prev[i3d(i, j-1, k-1)]);
		double v = 0.25 * (_v_prev[i3d(i, j, k)] + _v_prev[i3d(i-1, j, k)] +
						_v_prev[i3d(i, j, k-1)] + _v_prev[i3d(i-1, j, k-1)]);
		double w = 0.25 * (_w_prev[i3d(i, j, k)] + _w_prev[i3d(i-1, j, k)] +
						_w_prev[i3d(i, j-1, k)] + _w_prev[i3d(i-1, j-1, k)]);
		return Vector(u, v, w);
	}
}

void FosterWaterVolume::evolve(unsigned long time)
{
	using Orbis::Math::min;
	using Orbis::Math::Random;

	// gravity. must at some point go to Orbis::World
	const Vector g(0.0, 0.0, -9.81);

	if(_actual_dt < 0.0) {
		_actual_dt = min(time / 1000.0, 5e-4);
	}

	std::cerr << "Current time step: " << _actual_dt << std::endl;

	// updating particles in the system based on sources
	SourceIterator it;
	for(it = sources(); it != sourcesEnd(); it++) {
		unsigned i, j, k;
		Point pos = it->position();
		if(locate(pos, &i, &j, &k)) {
			unsigned l = i3d(i, j, k);
			_status[l] = SOURCE;
			// setting fixed inflow velocities
			_u[l] = _u[i3d(i+1, j, k)] = it->velocity().x();
			_v[l] = _v[i3d(i, j+1, k)] = it->velocity().y();
			_w[l] = _w[i3d(i, j, k+1)] = it->velocity().z();
			unsigned nr_part = static_cast<unsigned>(it->strength() * 100.0);
			for(unsigned m = 0; m < nr_part; m++) {
				// perturbing source
				Point p = Point(pos.x() + Random::rand2() * stepX() * 0.3,
								pos.y() + Random::rand2() * stepY() * 0.3,
								pos.z() + Random::rand2() * stepZ() * 0.3);
				_part_lists[l].push_back(Particle(p));
			}
		}
	}

	// main simulation step
	classifyAll();
	set_bounds(true);
	update_velocity(g, _actual_dt);
	update_pressure(_actual_dt);
	set_bounds(true);
	update_surface(_actual_dt);

	std::cerr << "----\n";

	/*
	 * Beware that with this type of buffering this class work doubled
	 * for the same result, but at least rendering rates are smoother
	 */
	Locker lock(this);

	swap(_u, _u_prev);
	swap(_v, _v_prev);
	swap(_w, _w_prev);
	swap(_p, _p_prev);
	swap(_status, _status_prev);
}

bool FosterWaterVolume::any_empty_neighbour(unsigned i, unsigned j, unsigned k) const
{
	if((_part_lists[i3d(i-1, j, k)].empty() && _status[i3d(i-1, j, k)] != SOLID) ||
		(_part_lists[i3d(i+1, j, k)].empty() && _status[i3d(i+1, j, k)] != SOLID) ||
		(_part_lists[i3d(i, j-1, k)].empty() && _status[i3d(i, j-1, k)] != SOLID) ||
		(_part_lists[i3d(i, j+1, k)].empty() && _status[i3d(i, j+1, k)] != SOLID) ||
		(_part_lists[i3d(i, j, k-1)].empty() && _status[i3d(i, j, k-1)] != SOLID) ||
		(_part_lists[i3d(i, j, k+1)].empty() && _status[i3d(i, j, k+1)] != SOLID)) {
		return true;
	} else {
		return false;
	}
}

void FosterWaterVolume::classifyAll()
{
	using Orbis::Math::max;

	double max_vel = -1.0;

	// updating status of cells
	for(unsigned i = 1; i < sizeX() - 1; i++) {
		for(unsigned j = 1; j < sizeY() - 1; j++) {
			for(unsigned k = 1; k < sizeZ() - 1; k++) {
				// getting maximum velociy
				double vel = max(std::abs(_u[i3d(i+1, j, k)]),
								std::abs(_v[i3d(i, j+1, k)]),
								std::abs(_w[i3d(i, j, k+1)]));
				max_vel = max(max_vel, vel);

				// classifying cells
				unsigned l = i3d(i, j, k);
				if(_status[l] == SOLID || _status[l] == SOURCE) {
					continue;
				} else if(_part_lists[l].empty()) {
					_status[l] = EMPTY;
				} else if(any_empty_neighbour(i, j, k)) {
					_status[l] = SURFACE;
				} else {
					_status[l] = FULL;
				}
			}
		}
	}

	if(max_vel > 0.0 && _actual_dt > stepX() / max_vel) {
		_actual_dt = stepX() / max_vel - Orbis::Math::Epsilon;
	}
}

void FosterWaterVolume::update_surface(double dt)
{
	// updating position of particles in grid
	for(unsigned i = 1; i < sizeX() - 1; i++) {
		for(unsigned j = 1; j < sizeY() - 1; j++) {
			for(unsigned k = 1; k < sizeZ() - 1; k++) {
				unsigned l = i3d(i, j, k);

				// this cell has no fluid
				if(_status[l] == SOLID) {
					continue;
				}

				ParticleList::iterator it;
				for(it = _part_lists[l].begin(); it != _part_lists[l].end(); it++) {
					unsigned a, b, c;
					it->setPos(it->pos() + dt * velocity(it->pos()));
					if(locate(it->pos(), &a, &b, &c)) {
						unsigned m = i3d(a, b, c);
						if(m != l) {
							// particle changed cell
							if(_status[m] != SOLID) {
								_part_lists[m].push_back(*it);
							}
							it = _part_lists[l].erase(it);
						}
					} else {
						// this particle is out of the system
						// shouldn't happen because of the bondary conditions
						it = _part_lists[l].erase(it);
					}
				}
			}
		}
	}
}

void FosterWaterVolume::set_bounds(bool slip)
{
	double s = 0.0;
//	double s = slip ? 1.0 : -1.0;

	// boundary conditions for velocity and pressure
	for(unsigned i = 0; i < sizeX(); i++) {
		for(unsigned j = 0; j < sizeY(); j++) {
			for(unsigned k = 0; k < sizeZ(); k++) {
				unsigned l = i3d(i, j, k);
				if(_status[l] == EMPTY) {
					_p[l] = _atm_p;
					if(_status[i3d(i-1, j, k)] == EMPTY) {
						_u[l] = 0.0;
					}
					if(_status[i3d(i+1, j, k)] == EMPTY) {
						_u[i3d(i+1, j, k)] = 0.0;
					}
					if(_status[i3d(i, j-1, k)] == EMPTY) {
						_v[l] = 0.0;
					}
					if(_status[i3d(i, j+1, k)] == EMPTY) {
						_v[i3d(i, j+1, k)] = 0.0;
					}
					if(_status[i3d(i, j, k-1)] == EMPTY) {
						_w[l] = 0.0;
					}
					if(_status[i3d(i, j, k+1)] == EMPTY) {
						_w[i3d(i, j, k+1)] = 0.0;
					}
				} else if(_status[l] == SOLID) {
					double t = 0.0;
					unsigned m = 0;
					// setting up pressure
					if(i > 0 && _status[i3d(i-1, j, k)] != SOLID) {
						t += _p[i3d(i-1, j, k)];
						m++;
					}
					if(i < sizeX() - 1 && _status[i3d(i+1, j, k)] != SOLID) {
						t += _p[i3d(i+1, j, k)];
						m++;
					}
					if(j > 0 && _status[i3d(i, j-1, k)] != SOLID) {
						t += _p[i3d(i, j-1, k)];
						m++;
					}
					if(j < sizeY() - 1 && _status[i3d(i, j+1, k)] != SOLID) {
						t += _p[i3d(i, j+1, k)];
						m++;
					}
					if(k > 0 && _status[i3d(i, j, k-1)] != SOLID) {
						t += _p[i3d(i, j, k-1)];
						m++;
					}
					if(k < sizeZ() - 1 && _status[i3d(i, j, k+1)] != SOLID) {
						t += _p[i3d(i, j, k+1)];
						m++;
					}
					if(m > 0) {
						_p[l] = t / m;
					} else {
						_p[l] = _atm_p;
					}

					// setting up velocities
					if(i == 0) {
						_u[l] = 0.0;
						_u[i3d(i+1, j, k)] = 0.0;
					} else if(i == sizeX() - 1) {
						_u[l] = 0.0;
						_u[i3d(i-1, j, k)] = 0.0;
					} else {
						// solid cell in the middle of the environment
						if(_status[i3d(i-1, j, k)] != SOLID) {
							// interface with liquid, normal velocity == 0
							_u[l] = 0.0;
						} else {
							t = 0.0;
							m = 0;
							// this face is inside a solid,
							// set tangencial velocity
							if(k < sizeZ() - 1 && _status[i3d(i-1, j, k+1)] != SOLID) {
								t += _u[i3d(i, j, k+1)];
								m++;
							}
							if(j < sizeY() - 1 && _status[i3d(i-1, j+1, k)] != SOLID) {
								t += _u[i3d(i, j+1, k)];
								m++;
							}
							if(j > 0 && _status[i3d(i-1, j-1, k)] != SOLID) {
								t += _u[i3d(i, j-1, k)];
								m++;
							}
							if(k > 0 && _status[i3d(i-1, j, k-1)] != SOLID) {
								t += _u[i3d(i, j, k-1)];
								m++;
							}
							if(m > 0) {
								_u[l] = s * t / m;
							} else {
								_u[l] = 0.0;
							}
						}
						if(_status[i3d(i+1, j, k)] != SOLID) {
							// interface with liquid, normal velocity == 0
							_u[i3d(i+1, j, k)] = 0.0;
						} else {
							t = 0.0;
							m = 0;
							// this face is inside a solid,
							// set tangencial velocity
							if(k < sizeZ() - 1 && _status[i3d(i+1, j, k+1)] != SOLID) {
								t += _u[i3d(i+1, j, k+1)];
								m++;
							}
							if(j < sizeY() - 1 && _status[i3d(i+1, j+1, k)] != SOLID) {
								t += _u[i3d(i+1, j+1, k)];
								m++;
							}
							if(j > 0 && _status[i3d(i+1, j-1, k)] != SOLID) {
								t += _u[i3d(i+1, j-1, k)];
								m++;
							}if(k > 0 && _status[i3d(i+1, j, k-1)] != SOLID) {
								t += _u[i3d(i+1, j, k-1)];
								m++;
							}
							if(m > 0) {
								_u[i3d(i+1, j, k)] = s * t / m;
							} else {
								_u[i3d(i+1, j, k)] = 0.0;
							}
						}
					}
					if(j == 0) {
						_v[l] = 0.0;
						_v[i3d(i, j+1, k)] = 0.0;
					} else if(j == sizeY() - 1) {
						_v[l] = 0.0;
						_v[i3d(i, j-1, k)] = 0.0;
					} else {
						// solid cell in the middle of the environment
						if(_status[i3d(i, j-1, k)] != SOLID) {
							// interface with liquid, velocity == 0
							_v[l] = 0.0;
						} else {
							t = 0.0;
							m = 0;
							// this face is inside a solid,
							// set tangencial velocity
							if(k < sizeZ() - 1 && _status[i3d(i, j-1, k+1)] != SOLID) {
								t += _v[i3d(i, j, k+1)];
								m++;
							}
							if(i < sizeX() - 1 && _status[i3d(i+1, j-1, k)] != SOLID) {
								t += _v[i3d(i+1, j, k)];
								m++;
							}
							if(i > 0 && _status[i3d(i-1, j-1, k)] != SOLID) {
								t += _v[i3d(i-1, j, k)];
								m++;
							}
							if(k > 0 && _status[i3d(i, j-1, k-1)] != SOLID) {
								t += _v[i3d(i, j, k-1)];
								m++;
							}
							if(m > 0) {
								_v[l] = s * t / m;
							} else {
								_v[l] = 0.0;
							}
						}
						if(_status[i3d(i, j+1, k)] != SOLID) {
							// interface with liquid, velocity == 0
							_v[i3d(i, j+1, k)] = 0.0;
						} else {
							t = 0.0;
							m = 0;
							// this face is inside a solid,
							// set tangencial velocity
							if(k < sizeZ() - 1 && _status[i3d(i, j+1, k+1)] != SOLID) {
								t += _v[i3d(i, j+1, k+1)];
								m++;
							}
							if(i < sizeX() - 1 && _status[i3d(i+1, j+1, k)] != SOLID) {
								t += _v[i3d(i+1, j+1, k)];
								m++;
							}
							if(i > 0 && _status[i3d(i-1, j+1, k)] != SOLID) {
								t += _v[i3d(i-1, j+1, k)];
								m++;
							}
							if(k > 0 && _status[i3d(i, j+1, k-1)] != SOLID) {
								t += _v[i3d(i, j+1, k-1)];
								m++;
							}
							if(m > 0) {
								_v[i3d(i, j+1, k)] = s * t / m;
							} else {
								_v[i3d(i, j+1, k)] = 0.0;
							}
						}
					}
					if(k == 0) {
						_w[l] = 0.0;
						_w[i3d(i, j, k+1)] = 0.0;
					} else if(k == sizeZ() - 1) {
						_w[l] = 0.0;
						_w[i3d(i, j, k-1)] = 0.0;
					} else {
						// solid cell in the middle of the environment
						if(_status[i3d(i, j, k-1)] != SOLID) {
							// interface with liquid, velocity == 0
							_w[l] = 0.0;
						} else {
							t = 0.0;
							m = 0;
							// this face is inside a solid,
							// set tangencial velocity
							if(i < sizeX() - 1 && _status[i3d(i+1, j, k-1)] != SOLID) {
								t += _w[i3d(i+1, j, k)];
								m++;
							}
							if(i > 0 && _status[i3d(i-1, j, k-1)] != SOLID) {
								t += _w[i3d(i-1, j, k)];
								m++;
							}
							if(j < sizeY() - 1 && _status[i3d(i, j+1, k-1)] != SOLID) {
								t += _w[i3d(i, j+1, k)];
								m++;
							}
							if(j > 0 && _status[i3d(i, j-1, k-1)] != SOLID) {
								t += _w[i3d(i, j-1, k)];
								m++;
							}
							if(m > 0) {
								_w[l] = s * t / m;
							} else {
								_w[l] = 0.0;
							}
						}
						if(_status[i3d(i, j, k+1)] != SOLID) {
							// interface with liquid, velocity == 0
							_w[i3d(i, j, k+1)] = 0.0;
						} else {
							t = 0.0;
							m = 0;
							// this face is inside a solid,
							// set tangencial velocity
							if(i < sizeX() - 1 && _status[i3d(i+1, j, k+1)] != SOLID) {
								t += _w[i3d(i+1, j, k+1)];
								m++;
							}
							if(i > 0 && _status[i3d(i-1, j, k+1)] != SOLID) {
								t += _w[i3d(i-1, j, k+1)];
								m++;
							}
							if(j < sizeY() - 1 && _status[i3d(i, j+1, k+1)] != SOLID) {
								t += _w[i3d(i, j+1, k+1)];
								m++;
							}
							if(j > 0 && _status[i3d(i, j-1, k+1)] != SOLID) {
								t += _w[i3d(i, j-1, k+1)];
								m++;
							}
							if(m > 0) {
								_w[i3d(i, j, k+1)] = s * t / m;
							} else {
								_w[i3d(i, j, k+1)] = 0.0;
							}
						}
					}
				} else if(_status[l] == SOURCE) {
					//
					// velocities are hopefully already set
					//
				} else if(_status[l] == SURFACE) {
					_p[l] = _atm_p;
					// here there are 64 possible Empty-Fluid configurations
					unsigned config = 0x00;
					if(_status[i3d(i-1, j, k)] == EMPTY) {
						config |= 0x01;
					}
					if(_status[i3d(i, j-1, k)] == EMPTY) {
						config |= 0x02;
					}
					if(_status[i3d(i+1, j, k)] == EMPTY) {
						config |= 0x04;
					}
					if(_status[i3d(i, j+1, k)] == EMPTY) {
						config |= 0x08;
					}
					if(_status[i3d(i, j, k-1)] == EMPTY) {
						config |= 0x10;
					}
					if(_status[i3d(i, j, k+1)] == EMPTY) {
						config |= 0x20;
					}
					switch(config) {
						case 0x00:
							// how could this be a surface cell?
							throw std::runtime_error("surface cell in middle of fluid");
							break;
						case 0x01:
							// only the minus x face sees an empty cell
							_u[l] = _u[i3d(i+1, j, k)] + stepX() *
									((_v[i3d(i, j+1, k)] - _v[l]) / stepY() +
									( _w[i3d(i, j, k+1)] - _w[l]) / stepZ());
							break;
						case 0x02:
							// only the minus y face sees an empty cell
							_v[l] = _v[i3d(i, j+1, k)] + stepY() *
									((_u[i3d(i+1, j, k)] - _u[l]) / stepX() +
									( _w[i3d(i, j, k+1)] - _w[l]) / stepZ());
							break;
						case 0x04:
							// only the plus x face sees an empty cell
							_u[i3d(i+1, j, k)] = _u[l] - stepX() *
									((_v[i3d(i, j+1, k)] - _v[l]) / stepY() +
									( _w[i3d(i, j, k+1)] - _w[l]) / stepZ());
							break;
						case 0x08:
							// only the plus y face sees an empty cell
							_v[i3d(i, j+1, k)] = _v[l] - stepY() *
									((_u[i3d(i+1, j, k)] - _u[l]) / stepX() +
									( _w[i3d(i, j, k+1)] - _w[l]) / stepZ());
							break;
						case 0x10:
							// only the minus z face sees an empty cell
							_w[l] = _w[i3d(i, j, k+1)] + stepZ() *
									((_u[i3d(i+1, j, k)] - _u[l]) / stepX() +
									( _v[i3d(i, j+1, k)] - _v[l]) / stepY());
							break;
						case 0x20:
							// only the plus z face sees an empty cell
							_w[i3d(i, j, k+1)] = _w[l] - stepZ() *
									((_u[i3d(i+1, j, k)] - _u[l]) / stepX() +
									( _v[i3d(i, j+1, k)] - _v[l]) / stepY());
							break;
						case 0x03:
							// minus x and minus y
							_u[l] = _u[i3d(i+1, j, k)];
							_v[l] = _v[i3d(i, j+1, k)];
							break;
						case 0x05:
							// minus x and plus x
							break;
						case 0x09:
							// minus x and plus y
							_u[l] = _u[i3d(i+1, j, k)];
							_v[i3d(i, j+1, k)] = _v[l];
							break;
						case 0x11:
							// minus x and minus z
							_u[l] = _u[i3d(i+1, j, k)];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x21:
							// minus x and plus z
							_u[l] = _u[i3d(i+1, j, k)];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x06:
							// plus x and minus y
							_u[i3d(i+1, j, k)] = _u[l];
							_v[l] = _v[i3d(i, j+1, k)];
							break;
						case 0x0a:
							// minus y and plus y
							break;
						case 0x12:
							// minus y and minus z
							_v[l] = _v[i3d(i, j+1, k)];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x22:
							// minus y and plus z
							_v[l] = _v[i3d(i, j+1, k)];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x0c:
							// plus x and plus y
							_u[i3d(i+1, j, k)] = _u[l];
							_v[i3d(i, j+1, k)] = _v[l];
							break;
						case 0x14:
							// plus x and minus z
							_u[i3d(i+1, j, k)] = _u[l];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x24:
							// plus x and plus z
							_u[i3d(i+1, j, k)] = _u[l];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x18:
							// plus y and minus z
							_v[i3d(i, j+1, k)] = _v[l];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x28:
							// plus y and plus z
							_v[i3d(i, j+1, k)] = _v[l];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x30:
							// minus z and plus z
							break;
						case 0x07:
							// minus x and minus y and plus x
							_v[l] = _v[i3d(i, j+1, k)];
							break;
						case 0x0b:
							// minus x and minus y and plus y
							_u[l] = _u[i3d(i+1, j, k)];
							break;
						case 0x13:
							// minus x and minus y and minus z
							_u[l] = _u[i3d(i+1, j, k)];
							_v[l] = _v[i3d(i, j+1, k)];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x23:
							// minus x and minus y and plus z
							_u[l] = _u[i3d(i+1, j, k)];
							_v[l] = _v[i3d(i, j+1, k)];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x0d:
							// minus x and plus x and plus y
							_v[i3d(i, j+1, k)] = _v[l];
							break;
						case 0x15:
							// minus x and plus x and minus z
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x25:
							// minus x and plus x and plus z
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x19:
							// minus x and plus y and minus z
							_u[l] = _u[i3d(i+1, j, k)];
							_v[i3d(i, j+1, k)] = _v[l];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x29:
							// minus x and plus y and plus z
							_u[l] = _u[i3d(i+1, j, k)];
							_v[i3d(i, j+1, k)] = _v[l];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x31:
							// minus x and minus z and plus z
							_u[l] = _u[i3d(i+1, j, k)];
							break;
						case 0x0e:
							// plus x and minus y and plus y
							_u[i3d(i+1, j, k)] = _u[l];
							break;
						case 0x16:
							// plus x and minus y and minus z
							_u[i3d(i+1, j, k)] = _u[l];
							_v[l] = _v[i3d(i, j+1, k)];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x26:
							// plus x and minus y and plus z
							_u[i3d(i+1, j, k)] = _u[l];
							_v[l] = _v[i3d(i, j+1, k)];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x1a:
							// minus y and plus y and minus z
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x2a:
							// minus y and plus y and plus z
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x32:
							// minus y and minus z and plus z
							_v[l] = _v[i3d(i, j+1, k)];
							break;
						case 0x1c:
							// plus x and plus y and minus z
							_u[i3d(i+1, j, k)] = _u[l];
							_v[i3d(i, j+1, k)] = _v[l];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x2c:
							// plus x and plus y and plus z
							_u[i3d(i+1, j, k)] = _u[l];
							_v[i3d(i, j+1, k)] = _v[l];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x34:
							// plus x and minus z and plus z
							_u[i3d(i+1, j, k)] = _u[l];
							break;
						case 0x38:
							// plus y and minus z and plus z
							_v[i3d(i, j+1, k)] = _v[l];
							break;
						case 0x0f:
							// minus x and minus y and plus x and plus y
							break;
						case 0x17:
							// minus x and minus y and plus x and minus z
							_v[l] = _v[i3d(i, j+1, k)];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x27:
							// minus x and minus y and plus x and plus z
							_v[l] = _v[i3d(i, j+1, k)];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x1b:
							// minus x and minus y and plus y and minus z
							_u[l] = _u[i3d(i+1, j, k)];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x2b:
							// minus x and minus y and plus y and plus z
							_u[l] = _u[i3d(i+1, j, k)];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x33:
							// minus x and minus y and minus z and plus z
							_u[l] = _u[i3d(i+1, j, k)];
							_v[l] = _v[i3d(i, j+1, k)];
							break;
						case 0x1d:
							// minus x and plus x and plus y and minus z
							_v[i3d(i, j+1, k)] = _v[l];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x2d:
							// minus x and plus x and plus y and plus z
							_v[i3d(i, j+1, k)] = _v[l];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x35:
							// minus x and plus x and minus z and plus z
							break;
						case 0x39:
							// minus x and plus y and minus z and plus z
							_u[l] = _u[i3d(i+1, j, k)];
							_v[i3d(i, j+1, k)] = _v[l];
							break;
						case 0x1e:
							// plus x and minus y and plus y and minus z
							_u[i3d(i+1, j, k)] = _u[l];
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x2e:
							// plus x and minus y and plus y and plus z
							_u[i3d(i+1, j, k)] = _u[l];
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x36:
							// plus x and minus y and minus z and plus z
							_u[i3d(i+1, j, k)] = _u[l];
							_v[l] = _v[i3d(i, j+1, k)];
							break;
						case 0x3a:
							// minus y and plus y and minus z and plus z
							break;
						case 0x3c:
							// plus x and plus y and minus z and plus z
							_u[i3d(i+1, j, k)] = _u[l];
							_v[i3d(i, j+1, k)] = _v[l];
							break;
						case 0x1f:
							// minus x and minus y and plus x and plus y and minus z
							_w[l] = _w[i3d(i, j, k+1)];
							break;
						case 0x2f:
							// minus x and minus y and plus x and plus y and plus z
							_w[i3d(i, j, k+1)] = _w[l];
							break;
						case 0x37:
							// minus x and minus y and plus x minus z and plus z
							_v[l] = _v[i3d(i, j+1, k)];
							break;
						case 0x3b:
							// minus x and minus y and plus y and minus z and plus z
							_u[l] = _u[i3d(i+1, j, k)];
							break;
						case 0x3d:
							// minus x and plus x and plus y and minus z and plus z
							_v[i3d(i, j+1, k)] = _v[l];
							break;
						case 0x3e:
							// minus y and plus x and plus y and minus z and plus z
							_u[i3d(i+1, j, k)] = _u[l];
							break;
						case 0x3f:
							// all of them... a waterdrop?
							break;
					}

					// divergence of fluid within cell
					double D = _inv_x * (_u[i3d(i+1, j, k)] - _u[i3d(i, j, k)]) +
							 _inv_y * (_v[i3d(i, j+1, k)] - _v[i3d(i, j, k)]) +
							 _inv_z * (_w[i3d(i, j, k+1)] - _w[i3d(i, j, k)]);

					if(Orbis::Math::isZero(D)) {
						// divergence already zero
						continue;
					}

					// pressure variation
					double aa = 0.0;
					if(_status[i3d(i-1, j, k)] == EMPTY) {
						aa += _inv_x2;
					}
					if(_status[i3d(i+1, j, k)] == EMPTY) {
						aa += _inv_x2;
					}
					if(_status[i3d(i, j-1, k)] == EMPTY) {
						aa += _inv_y2;
					}
					if(_status[i3d(i, j+1, k)] == EMPTY) {
						aa += _inv_y2;
					}
					if(_status[i3d(i, j, k-1)] == EMPTY) {
						aa += _inv_z2;
					}
					if(_status[i3d(i, j, k+1)] == EMPTY) {
						aa += _inv_z2;
					}

					double dp = D / aa;

					// updating velocities
					if(_status[i3d(i-1, j, k)] == EMPTY) {
						_u[i3d(  i, j, k)] += dp * _inv_x;
					}
					if(_status[i3d(i+1, j, k)] == EMPTY) {
						_u[i3d(i+1, j, k)] -= dp * _inv_x;
					}
					if(_status[i3d(i, j-1, k)] == EMPTY) {
						_v[i3d(i,   j, k)] += dp * _inv_y;
					}
					if(_status[i3d(i, j+1, k)] == EMPTY) {
						_v[i3d(i, j+1, k)] -= dp * _inv_y;
					}
					if(_status[i3d(i, j, k-1)] == EMPTY) {
						_w[i3d(i, j,   k)] += dp * _inv_z;
					}
					if(_status[i3d(i, j, k+1)] == EMPTY) {
						_w[i3d(i, j, k+1)] -= dp * _inv_z;
					}

				}
			}
		}
	}
}

/*
 * WARNING:
 * A velocity found with _a[i3d(i, j, k)] is in fact a velocity at one FACE
 * of the cell, because of the staggered approach. At each step, we set the three
 * faces that this cell shares with the next cells. The faces that this cell shares
 * with the previous ones were set by them or, at the boundaries, by the set_bounds
 * method. It's not necessary to iterate until the last cells at the other boundaries.
 */
void FosterWaterVolume::update_velocity(const Vector& g, double dt)
{
//	using Orbis::Math::max;
	using Orbis::Math::sqr;

	// calculating accelerations
	for(unsigned i = 1; i < sizeX() - 2; i++) {
		for(unsigned j = 1; j < sizeY() - 2; j++) {
			for(unsigned k = 1; k < sizeZ() - 2; k++) {

				double v = viscosity();
				if(_status[i3d(i, j, k)] != FULL && _status[i3d(i, j, k)] != SURFACE) {
					continue;
				}

				// x component
				// mapping velocities from arrays to cells' faces
				double uijk  = 0.5 * (_u[i3d(  i, j, k)] + _u[i3d(i+1, j, k)]);
				double ui1jk = 0.5 * (_u[i3d(i+1, j, k)] + _u[i3d(i+2, j, k)]);
				double pijk  = _p[i3d(  i, j, k)];
				double pi1jk = _p[i3d(i+1, j, k)];
				double ui1_2j_1_2k = 0.5 * (_u[i3d(i+1, j-1, k)] + _u[i3d(i+1, j, k)]);
				double vi1_2j_1_2k = 0.5 * (_v[i3d(i+1,   j, k)] + _v[i3d(  i, j, k)]);
				double ui1_2j1_2k  = 0.5 * (_u[i3d(i+1, j, k)] + _u[i3d(i+1, j+1, k)]);
				double vi1_2j1_2k  = 0.5 * (_v[i3d(i, j+1, k)] + _v[i3d(i+1, j+1, k)]);
				double ui1_2jk_1_2 = 0.5 * (_u[i3d(i+1, j, k-1)] + _u[i3d(i+1, j, k)]);
				double wi1_2jk_1_2 = 0.5 * (_w[i3d(  i, j,   k)] + _w[i3d(i+1, j, k)]);
				double ui1_2jk1_2  = 0.5 * (_u[i3d(i+1, j, k)] + _u[i3d(i+1, j, k+1)]);
				double wi1_2jk1_2  = 0.5 * (_w[i3d(i, j, k+1)] + _w[i3d(i+1, j, k+1)]);
				double ui3_2jk = _u[i3d(i+2, j, k)];
				double ui1_2jk__2 = 2.0 * _u[i3d(i+1, j, k)];
				double ui_1_2jk = _u[i3d(i, j, k)];
				double ui1_2j1k = _u[i3d(i+1, j+1, k)];
				double ui1_2j_1k = _u[i3d(i+1, j-1, k)];
				double ui1_2jk1 = _u[i3d(i+1, j, k+1)];
				double ui1_2jk_1 = _u[i3d(i+1, j, k-1)];

				// y component
				// mapping velocities from arrays to cells' faces
				double vijk  = 0.5 * (_v[i3d(  i, j, k)] + _v[i3d(i, j+1, k)]);
				double vij1k = 0.5 * (_v[i3d(i, j+1, k)] + _v[i3d(i, j+2, k)]);
				double pij1k = _p[i3d(i, j+1, k)];
				double vi_1_2j1_2k = 0.5 * (_v[i3d(i-1, j+1, k)] + _v[i3d(i, j+1, k)]);
				double ui_1_2j1_2k = 0.5 * (_u[i3d(i,   j, k)] + _u[i3d(i, j+1, k)]);
				double vij1_2k_1_2 = 0.5 * (_v[i3d(i, j+1, k-1)] + _v[i3d(i, j+1, k)]);
				double wij1_2k_1_2 = 0.5 * (_w[i3d(i, j, k)] + _w[i3d(i, j+1, k)]);
				double vij1_2k1_2  = 0.5 * (_v[i3d(i, j+1, k)] + _v[i3d(i, j+1, k+1)]);
				double wij1_2k1_2  = 0.5 * (_w[i3d(i, j, k+1)] + _w[i3d(i, j+1, k+1)]);
				double vij3_2k = _v[i3d(i,j+2, k)];
				double vij1_2k__2 = 2.0 * _v[i3d(i,j+1, k)];
				double vij_1_2k = _v[i3d(i, j, k)];
				double vi1j1_2k = _v[i3d(i+1, j+1, k)];
				double vi_1j1_2k = _v[i3d(i-1, j+1, k)];
				double vij1_2k1 = _v[i3d(i, j+1, k+1)];
				double vij1_2k_1 = _v[i3d(i, j+1, k-1)];

				// z component
				// mapping velocities from arrays to cells' faces
				double wijk  = 0.5 * (_w[i3d(i, j, k  )] + _w[i3d(i, j, k+1)]);
				double wijk1 = 0.5 * (_w[i3d(i, j, k+1)] + _w[i3d(i, j, k+2)]);
				double pijk1 = _p[i3d(i, j, k+1)];
				double wi_1_2jk1_2 = 0.5 * (_w[i3d(i-1, j, k+1)] + _w[i3d(i, j, k+1)]);
				double ui_1_2jk1_2 = 0.5 * (_u[i3d(  i, j,   k)] + _u[i3d(i, j, k+1)]);
				double wij_1_2k1_2 = 0.5 * (_w[i3d(i, j-1, k+1)] + _w[i3d(i, j, k+1)]);
				double vij_1_2k1_2 = 0.5 * (_v[i3d(i,   j, k  )] + _v[i3d(i, j, k+1)]);
				double wijk3_2 = _w[i3d(i,j, k+2)];
				double wijk1_2__2 = 2.0 * _v[i3d(i,j, k+1)];
				double wijk_1_2 = _w[i3d(i, j, k)];
				double wi1jk1_2 = _w[i3d(i+1, j, k+1)];
				double wi_1jk1_2 = _w[i3d(i-1, j, k+1)];
				double wij1k1_2 = _w[i3d(i, j+1, k+1)];
				double wij_1k1_2 = _w[i3d(i, j-1, k+1)];

				// finding acceleration of fluid
				// viscosity is changed locally to keep simulation stable
//				double md = v;
//				do {
//					v = md;

					// calculating acceleration in the x direction
					_acc_x[i3d(i+1, j, k)] =
							_inv_x * (sqr(uijk) - sqr(ui1jk) + pijk - pi1jk) +
							_inv_y * (ui1_2j_1_2k*vi1_2j_1_2k - ui1_2j1_2k*vi1_2j1_2k) +
							_inv_z * (ui1_2jk_1_2*wi1_2jk_1_2 - ui1_2jk1_2*wi1_2jk1_2) +
							v * (_inv_x2 * (ui3_2jk - ui1_2jk__2 + ui_1_2jk) +
								_inv_y2 * (ui1_2j1k - ui1_2jk__2 + ui1_2j_1k) +
								_inv_z2 + (ui1_2jk1 - ui1_2jk__2 + ui1_2jk_1)) + g.x();

					// calculating acceleration in the y direction
					_acc_y[i3d(i, j+1, k)] =
							_inv_y * (sqr(vijk) - sqr(vij1k) + pijk - pij1k) +
							_inv_x * (vi_1_2j1_2k*ui_1_2j1_2k - vi1_2j1_2k*ui1_2j1_2k) +
							_inv_z * (vij1_2k_1_2*wij1_2k_1_2 - vij1_2k1_2*wij1_2k1_2) +
							v * (_inv_y2 * (vij3_2k - vij1_2k__2 + vij_1_2k) +
								_inv_x2 * (vi1j1_2k - vij1_2k__2 + vi_1j1_2k) +
								_inv_z2 * (vij1_2k1 - vij1_2k__2 + vij1_2k_1)) + g.y();

					// calculating acceleration in the z direction
					_acc_z[i3d(i, j, k+1)] =
							_inv_z * (sqr(wijk) - sqr(wijk1) + pijk - pijk1) +
							_inv_x * (wi_1_2jk1_2*ui_1_2jk1_2 - wi1_2jk1_2*ui1_2jk1_2) +
							_inv_y * (wij_1_2k1_2*vij_1_2k1_2 - wij1_2k1_2*vij1_2k1_2) +
							v * (_inv_z2 * (wijk3_2 - wijk1_2__2 + wijk_1_2) +
								_inv_x2 * (wi1jk1_2 - wijk1_2__2 + wi_1jk1_2) +
								_inv_y2 * (wij1k1_2 - wijk1_2__2 + wij_1k1_2)) + g.z();

//					double du = _u[i3d(i+1, j, k)] - _u[i3d(i, j, k)] + dt * acc_x;
//					double dv = _v[i3d(i, j+1, k)] - _v[i3d(i, j, k)] + dt * acc_y;
//					double dw = _w[i3d(i, j, k+1)] - _w[i3d(i, j, k)] + dt * acc_z;
//					md = max(stepX() * du, stepY() * dv, stepZ() * dw);
//					md *= 0.5 * dt;
//				} while(v < md);
			}
		}
	}

	// updating velocities
	for(unsigned i = 1; i < sizeX() - 2; i++) {
		for(unsigned j = 1; j < sizeY() - 2; j++) {
			for(unsigned k = 1; k < sizeZ() - 2; k++) {
				Status st;

				st = _status[i3d(i, j, k)];
				if(st != FULL && st != SURFACE) {
					continue;
				}

				st = _status[i3d(i+1, j, k)];
				if(st != SOLID && st != SOURCE) {
//				if(st == FULL) {
					_u[i3d(i+1, j, k)] += dt * _acc_x[i3d(i+1, j, k)];
				}

				st = _status[i3d(i, j+1, k)];
				if(st != SOLID && st != SOURCE) {
//				if(st == FULL) {
					_v[i3d(i, j+1, k)] += dt * _acc_y[i3d(i, j+1, k)];
				}

				st = _status[i3d(i, j, k+1)];
				if(st != SOLID && st != SOURCE) {
//				if(st == FULL) {
					_w[i3d(i, j, k+1)] += dt * _acc_z[i3d(i, j, k+1)];
				}
			}
		}
	}
}

void FosterWaterVolume::update_pressure(double dt)
{
	using Orbis::Math::max;

	const double beta0 = 1.7;
	const double epsilon = 0.0001;
	const unsigned max_iters = 50;

	for(unsigned l = 0; l < max_iters; l++) {
		double max_div = -1.0;
		for(unsigned i = 1; i < sizeX() - 1; i++) {
			for(unsigned j = 1; j < sizeY() - 1; j++) {
				for(unsigned k = 1; k < sizeZ() - 1; k++) {

					if(_status[i3d(i, j, k)] != FULL) {
						continue;
					}

					// divergence of fluid within cell
					double D = _inv_x * (_u[i3d(i+1, j, k)] - _u[i3d(i, j, k)]) +
							 _inv_y * (_v[i3d(i, j+1, k)] - _v[i3d(i, j, k)]) +
							 _inv_z * (_w[i3d(i, j, k+1)] - _w[i3d(i, j, k)]);
					max_div = max(std::abs(D), max_div);

					// pressure variation
					Status st;
					double aa = 0.0;
					st = _status[i3d(i-1, j, k)];
					if(st != SOLID && st != SOURCE) {
						aa += _inv_x2;
					}
					st = _status[i3d(i+1, j, k)];
					if(st != SOLID && st != SOURCE) {
						aa += _inv_x2;
					}
					st = _status[i3d(i, j-1, k)];
					if(st != SOLID && st != SOURCE) {
						aa += _inv_y2;
					}
					st = _status[i3d(i, j+1, k)];
					if(st != SOLID && st != SOURCE) {
						aa += _inv_y2;
					}
					st = _status[i3d(i, j, k-1)];
					if(st != SOLID && st != SOURCE) {
						aa += _inv_z2;
					}
					st = _status[i3d(i, j, k+1)];
					if(st != SOLID && st != SOURCE) {
						aa += _inv_z2;
					}
					double dp = beta0 * D / aa;

					// updating velocities
					st = _status[i3d(i-1, j, k)];
					if(st != SOLID && st != SOURCE) {
						_u[i3d(  i, j, k)] += dp * _inv_x;
					}
					st = _status[i3d(i+1, j, k)];
					if(st != SOLID && st != SOURCE) {
						_u[i3d(i+1, j, k)] -= dp * _inv_x;
					}
					st = _status[i3d(i, j-1, k)];
					if(st != SOLID && st != SOURCE) {
						_v[i3d(i,   j, k)] += dp * _inv_y;
					}
					st = _status[i3d(i, j+1, k)];
					if(st != SOLID && st != SOURCE) {
						_v[i3d(i, j+1, k)] -= dp * _inv_y;
					}
					st = _status[i3d(i, j, k-1)];
					if(st != SOLID && st != SOURCE) {
						_w[i3d(i, j,   k)] += dp * _inv_z;
					}
					st = _status[i3d(i, j, k+1)];
					if(st != SOLID && st != SOURCE) {
						_w[i3d(i, j, k+1)] -= dp * _inv_z;
					}

					// updating pressure
					_p[i3d(i, j, k)] -= dp / dt;
				}
			}
		}
		if(max_div < epsilon) {
			std::cerr << "Converged after " << l << " iterations with D = "
					<< max_div << std::endl;
			// divergence converged
			break;
		} else if(l == max_iters - 1) {
			std::cerr << "Not converged, exiting with D = " << max_div << std::endl;
		}
	}
}

} } // namespace declaration
