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
	: WaterVolume(origin, size_x+1, size_y+1, size_z+1, step_x, step_y, step_z)
{
	unsigned size = sizeX() * sizeY() * sizeZ();

	_u.reserve(size);
	_v.reserve(size);
	_w.reserve(size);
	_p.reserve(size);
	_status.reserve(size);

	// all cells at the boundaries of the volume are treated as solid
	for(unsigned i = 0; i < sizeX(); i++) {
		for(unsigned j = 0; j < sizeY(); j++) {
			for(unsigned k = 0; k < sizeZ(); k++) {
				unsigned l = i3d(i, j, k);
				if(i == 0 || i > sizeX() - 3 ||
							j == 0 || j > sizeY() - 3 ||
								k == 0 || k > sizeZ() - 3) {
					_status[l] = SOLID;
				} else {
					_status[l] = EMPTY;
					_u[l] = _v[l] = _w[l] = _p[l] = 0.0;
				}
			}
		}
	}
}

Vector FosterWaterVolume::velocity(unsigned i, unsigned j, unsigned k) const
{
	Locker lock(this);
}

void FosterWaterVolume::evolve(unsigned long time)
{
	const Vector g(0.0, -10.0, 0.0);
	double const dt = time / 1000.0;

//	track_surface();
	set_bounds();
	update_velocity(g, dt);
	update_pressure(dt);
	set_bounds();
//	update_surface();
}

void FosterWaterVolume::set_bounds()
{
	// boundary conditions for velocity and pressure
	for(unsigned i = 0; i < sizeX(); i++) {
		for(unsigned j = 0; j < sizeY(); j++) {
			for(unsigned k = 0; k < sizeZ(); k++) {
				unsigned l = i3d(i, j, k);
				if(_status[l] == SOLID) {
					if(i == 0) {
						_u[l] = 0.0;
						_u[i3d(i+1, j, k)] = 0.0;
						_p[l] = _p[i3d(i+1, j, k)];
					} else if(i > sizeX() - 3) {
						_u[l] = 0.0;
						_u[i3d(i-1, j, k)] = 0.0;
						_p[l] = _p[i3d(i+1, j, k)];
					} else {
						// solid cell in the middle of the environment
						if(_status[i3d(i-1, j, k)] != SOLID) {
							// interface with liquid, velocity == 0
							_u[l] = 0.0;
							_p[l] = _p[i3d(i-1, j, k)];
						} else {
							// this face is inside a solid, set tangencial
							
						}
						if(_status[i3d(i+1, j, k)] != SOLID) {
							// interface with liquid, velocity == 0
							_u[i3d(i+1, j, k)] = 0.0;
							_p[l] = _p[i3d(i+1, j, k)];
						} else {
							// it's a matter of taste
							_p[l] = _p[i3d(i+1, j, k)];
						}
					}
					if(j == 0) {
						_v[l] = 0.0;
						_v[i3d(i, j+1, k)] = 0.0;
						_p[l] = _p[i3d(i, j+1, k)];
					} else if(j > sizeY() - 3) {
						_v[l] = 0.0;
						_v[i3d(i, j-1, k)] = 0.0;
						_p[l] = _p[i3d(i, j-1, k)];
					} else {
						// solid cell in the middle of the environment
						if(_status[i3d(i, j-1, k)] != SOLID) {
							// interface with liquid, velocity == 0
							_v[l] = 0.0;
							_p[l] = _p[i3d(i, j-1, k)];
						} else {
							// this face is inside a solid, set tangencial
							
						}
						if(_status[i3d(i, j+1, k)] != SOLID) {
							// interface with liquid, velocity == 0
							_v[i3d(i, j+1, k)] = 0.0;
							_p[l] = _p[i3d(i, j+1, k)];
						} else {
							// why not?
							_p[l] = _p[i3d(i, j+1, k)];
						}
					}
					if(k == 0) {
						_w[l] = 0.0;
						_w[i3d(i, j, k+1)] = 0.0;
						_p[l] = _p[i3d(i, j, k+1)];
					} else if(k > sizeZ() - 3) {
						_w[l] = 0.0;
						_w[i3d(i, j, k-1)] = 0.0;
						_p[l] = _p[i3d(i, j, k-1)];
					} else {
						// solid cell in the middle of the environment
						if(_status[i3d(i, j, k-1)] != SOLID) {
							// interface with liquid, velocity == 0
							_w[l] = 0.0;
							_p[l] = _p[i3d(i, j, k-1)];
						} else {
							// this face is inside a solid, set tangencial
							
						}
						if(_status[i3d(i, j, k+1)] != SOLID) {
							// interface with liquid, velocity == 0
							_w[i3d(i, j, k+1)] = 0.0;
							_p[l] = _p[i3d(i, j, k+1)];
						} else {
							// could be the other way around as well
							_p[l] = _p[i3d(i, j, k+1)];
						}
					}
				} else if(_status[i3d(i, j, k)] == SURFACE) {
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
	using Orbis::Math::sqr;

	double v = viscosity();
	double inv_x = 1.0 / stepX();
	double inv_x2 = sqr(inv_x);
	double inv_y = 1.0 / stepY();
	double inv_y2 = sqr(inv_y);
	double inv_z = 1.0 / stepZ();
	double inv_z2 = sqr(inv_z);
	for(unsigned i = 1; i < sizeX() - 2; i++) {
		for(unsigned j = 1; j < sizeY() - 2; j++) {
			for(unsigned k = 1; k < sizeZ() - 2; k++) {
				if(_status[i3d(i, j, k)] != FULL) {
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

				// calculating acceleration in the x direction
				double acc_x = inv_x * (sqr(uijk) - sqr(ui1jk) + pijk - pi1jk) +
							inv_y * (ui1_2j_1_2k*vi1_2j_1_2k - ui1_2j1_2k*vi1_2j1_2k) +
							inv_z * (ui1_2jk_1_2*wi1_2jk_1_2 - ui1_2jk1_2*wi1_2jk1_2) +
							v * (inv_x2 * (ui3_2jk - ui1_2jk__2 + ui_1_2jk) +
								inv_y2 * (ui1_2j1k - ui1_2jk__2 + ui1_2j_1k) +
								inv_z2 + (ui1_2jk1 - ui1_2jk__2 + ui1_2jk_1)) + g.x();

				// updating velocity
				_u[i3d(i+1, j, k)] += dt * acc_x;

				// y component
				// mapping velocities from arrays to cells' faces
				double vijk  = 0.5 * (_v[i3d(  i, j, k)] + _v[i3d(i, j+1, k)]);
				double vij1k = 0.5 * (_v[i3d(i+1, j, k)] + _v[i3d(i, j+2, k)]);
				double pij1k = _p[i3d(i, j+1, k)];
				double vi_1_2j1_2k = 0.5 * (_v[i3d(i, j+1, k)] + _v[i3d(i+1, j+1, k)]);
				double ui_1_2j1_2k = 0.5 * (_u[i3d(i,   j, k)] + _u[i3d(  i, j+1, k)]);
				double vij1_2k_1_2 = 0.5 * (_v[i3d(i, j, k-1)] + _v[i3d(  i,   j, k)]);
				double wij1_2k_1_2 = 0.5 * (_w[i3d(i, j, k-1)] + _w[i3d(i, j+1, k-1)]);
				double vij1_2k1_2  = 0.5 * (_v[i3d(i, j+1, k)] + _v[i3d(i, j+1, k+1)]);
				double wij1_2k1_2  = 0.5 * (_w[i3d(i, j, k+1)] + _w[i3d(i, j+1, k+1)]);
				double vij3_2k = _v[i3d(i,j+2, k)];
				double vij1_2k__2 = 2.0 * _v[i3d(i,j+1, k)];
				double vij_1_2k = _v[i3d(i, j, k)];
				double vi1j1_2k = _v[i3d(i+1, j+1, k)];
				double vi_1j1_2k = _v[i3d(i-1, j+1, k)];
				double vij1_2k1 = _v[i3d(i, j+1, k+1)];
				double vij1_2k_1 = _v[i3d(i, j+1, k-1)];

				// calculating acceleration in the y direction
				double acc_y = inv_y * (sqr(vijk) - sqr(vij1k) + pijk - pij1k) +
							inv_x * (vi_1_2j1_2k*ui_1_2j1_2k - vi1_2j1_2k*ui1_2j1_2k) +
							inv_z * (vij1_2k_1_2*wij1_2k_1_2 - vij1_2k1_2*wij1_2k1_2) +
							v * (inv_y2 * (vij3_2k - vij1_2k__2 + vij_1_2k) +
								inv_x2 * (vi1j1_2k - vij1_2k__2 + vi_1j1_2k) +
								inv_z2 * (vij1_2k1 - vij1_2k__2 + vij1_2k_1)) + g.y();

				// updating velocity
				_v[i3d(i, j+1, k)] += dt * acc_y;

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

				// calculating acceleration in the z direction
				double acc_z = inv_z * (sqr(wijk) - sqr(wijk1) + pijk - pijk1) +
							inv_x * (wi_1_2jk1_2*ui_1_2jk1_2 - wi1_2jk1_2*ui1_2jk1_2) +
							inv_y * (wij_1_2k1_2*vij_1_2k1_2 - wij1_2k1_2*vij1_2k1_2) +
							v * (inv_z2 * (wijk3_2 - wijk1_2__2 + wijk_1_2) +
								inv_x2 * (wi1jk1_2 - wijk1_2__2 + wi_1jk1_2) +
								inv_y2 * (wij1k1_2 - wijk1_2__2 + wij_1k1_2)) + g.z();

				// updating velocity
				_w[i3d(i, j, k+1)] += dt * acc_z;
			}
		}
	}
}

void FosterWaterVolume::update_pressure(double dt)
{
	using Orbis::Math::max;
	using Orbis::Math::sqr;

	const double beta0 = 1.7;
	const double epsilon = 0.0001;
	const unsigned max_iters = 10;

	double inv_x = 1.0 / stepX();
	double inv_y = 1.0 / stepY();
	double inv_z = 1.0 / stepZ();

	// relaxation factor
	double beta = beta0 / (2.0 * dt * (sqr(inv_x) + sqr(inv_y) + sqr(inv_z)));

	for(unsigned l = 0; l < max_iters; l++) {
		double max_div = -1.0;
		for(unsigned i = 1; i < sizeX() - 1; i++) {
			for(unsigned j = 1; j < sizeY() - 1; j++) {
				for(unsigned k = 1; k < sizeZ() - 1; k++) {
					// divergence of fluid within cell
					double D = -(inv_x * (_u[i3d(i+1, j, k)] - _u[i3d(i, j, k)]) +
								inv_y * (_v[i3d(i, j+1, k)] - _v[i3d(i, j, k)]) +
								inv_z * (_w[i3d(i, j, k+1)] - _w[i3d(i, j, k)]));
					max_div = max(std::abs(D), max_div);

					// pressure variation
					double dp = beta * D;

					// updating velocities
					_u[i3d(i, j, k)]   -= dt * dp * inv_x;
					_u[i3d(i+1, j, k)] += dt * dp * inv_x;
					_v[i3d(i, j, k)]   -= dt * dp * inv_y;
					_v[i3d(i, j+1, k)] += dt * dp * inv_y;
					_w[i3d(i, j, k)]   -= dt * dp * inv_z;
					_w[i3d(i, j, k+1)] += dt * dp * inv_z;

					// updating pressure in the last pass
					if(l == max_iters - 1) {
						_p[i3d(i, j, k)] += dp;
					}
				}
			}
		}
		if(max_div < epsilon) {
			// divergence converged
			// one last pass to update pressures
			l = max_iters - 2;
		}
	}
}

} } // namespace declaration
