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
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <math.hpp>
#include <waterheightfield.hpp>

using std::abs;
using Orbis::Math::max;
using Orbis::Math::sqr;
using Orbis::Math::isZero;

/*
 * Solves for a vector u the tridiagonal linear set. a, b, c, r are the
 * input vectors and are not modified.
 * Taken from Numerical Recipes
 */
static int triag(double* a, double* b, double* c, double *r, double *u,
								unsigned long n)
{
	double bet = b[0];
	if(isZero(bet)) {
		return -1;
	}
	u[0] = r[0] / bet;
	double *gam = new double[n];
	// decomposition and forward substitution
	for(unsigned j = 1; j < n; j++) {
		gam[j] = c[j-1] / bet;
		bet = b[j] - a[j] * gam[j];
		// algorithm fails
		if(isZero(bet)) {
			delete [] gam;
			return -1;
		}
		u[j] = (r[j] - a[j]*u[j-1]) / bet;
	}
	// backsubstitution
	for(unsigned j = n - 2; j > 0; j--)
		u[j] -= gam[j+1] * u[j+1];
	u[0] -= gam[1] * u[1];

	delete [] gam;
	return 0;
}

namespace Orbis {

	namespace Drawable {

void WaterHeightField::locate(const Point& p, unsigned *i, unsigned *j) const
{
	double x = floor((p.x() - origin().x()) / stepX());
	double y = floor((p.y() - origin().y()) / stepY());

	*i = static_cast<unsigned>(x);
	*j = static_cast<unsigned>(y);
}

WaterHeightField::WaterHeightField(const Point& origin,
					double stepX, double stepY,
						unsigned samplesX, unsigned samplesY)
	: GridHeightField(origin, stepX, stepY, samplesX, samplesY),
				_old_z(0), _e(0), _f(0), _r(0), _u(0)
						
{
	// auxiliary vectors
	_e = new double[max(samplesX, samplesY)];
	_f = new double[max(samplesX, samplesY)];
	_r = new double[max(samplesX, samplesY)];
	_u = new double[max(samplesX, samplesY)];

	setUseDisplayList(false);
}

WaterHeightField::~WaterHeightField()
{
	delete [] _e;
	delete [] _f;
	delete [] _r;
	delete [] _u;
}

void WaterHeightField::evolve(unsigned long time)
{
	// epsilon
	const double e = 0.05;
	// gravity
	const double g = 10.0;
	// time step in seconds
	double tstep = time / 1000.0;

	lock();
	// no bottom, no simulation
	if(!bottom()) {
		return;
	}
	if(!_old_z) {
		// first run, the "old" height field must be created...
		_old_z = new FloatArray(numSamplesX() * numSamplesY());
		// ... and initialised
		for(unsigned i = 0; i < numSamplesX(); i++) {
			for(unsigned j = 0; j < numSamplesY(); j++) {
				Point p = point(i, j);
				p = bottom()->point(p.x(), p.y());
				setPoint(i, j, p.z() - e);
				(*_old_z)[j * numSamplesX() + i] = p.z() - e;
			}
		}
	}
	// update surface using sources/sinks of water
	// I'd use a bell-like function, but I'd have to run
	// through the whole height-field, instead only the cell
	// containing the source will be updated
	SourceList::const_iterator it;
	for(it = sourcesBegin(); it != sourcesEnd(); it++) {
		unsigned i, j;
		Point p = it->first;
		double val = abs(it->second);

		// finding squared distances from p to grid points a, b, c, d
		locate(p, &i, &j);
		double a = (p - point(  i,   j)).sqrLength();
		double b = (p - point(  i, j+1)).sqrLength();
		double c = (p - point(i+1, j+1)).sqrLength();
		double d = (p - point(i+1,   j)).sqrLength();
		// calculating each grid point's share from the whole at p
		double fac = val / (a + b + c + d);
		a *= fac; b *= fac; c *= fac; d *= fac;
		if(it->second > 0.0) {
			// updating grid points
			setPoint(  i,   j, point(  i,   j).z() + a);
			setPoint(  i, j+1, point(  i, j+1).z() + b);
			setPoint(i+1, j+1, point(i+1, j+1).z() + c);
			setPoint(i+1,   j, point(  i,   j).z() + d);
		} else {
			// updating grid points
			setPoint(  i,   j, point(  i,   j).z() - a);
			setPoint(  i, j+1, point(  i, j+1).z() - b);
			setPoint(i+1, j+1, point(i+1, j+1).z() - c);
			setPoint(i+1,   j, point(  i,   j).z() - d);
		}
	}
	/// now the most important step, the differential solver ///
	// I update first the rows
	double fac = (g * sqr(tstep)) / (2.0 * sqr(stepX()));
	for(unsigned j = 0; j < numSamplesY(); j++) {
		Point h, b;
		// depths
		h = point(0, j);
		b = bottom()->point(h.x(), h.y());
		double d0 = max(0.0, h.z() - b.z());
		h = point(1, j);
		b = bottom()->point(h.x(), h.y());
		double d1 = max(0.0, h.z() - b.z());
		// some before the loop
		_f[0] = -fac * (d0 + d1);
		_e[0] = 1.0 + (d0 + d1) * fac;
		_r[0] = 2.0 * point(0, j).z() - (*_old_z)[j*numSamplesX()];
		for(unsigned i = 1; i < numSamplesX(); i++) {
			double d2;
			if(i == numSamplesX() - 1) {
				d2 = -d1;
			} else {
				h = point(i+1, j);
				b = bottom()->point(h.x(), h.y());
				d2 = max(0.0, h.z() - b.z());
			}
			_f[i] = -fac * (d1 + d2);
			_e[i] = 1.0 + (d0 + 2.0 * d1 + d2) * fac;
			_r[i] = 2.0 * point(i, j).z() -
					(*_old_z)[j*numSamplesX()+i];
			d0 = d1;
			d1 = d2;
		}
		// solving for this row
		triag(_f-1, _e, _f, _r, _u, numSamplesX());
		// updating height field
		for(unsigned i = 0; i < numSamplesX(); i++) {
			h = point(i, j);
			b = bottom()->point(h.x(), h.y());
			if(_u[i] < b.z()) {
				(*_old_z)[j * numSamplesX() + i] = b.z() - e;
				setPoint(i, j, b.z() - e);
			} else {
				(*_old_z)[j * numSamplesX() + i] = point(i, j).z();
				setPoint(i, j, _u[i]);
			}
		}
	}
	// Now I'm gonna update the columns
	fac = (g * sqr(tstep)) / (2.0 * sqr(stepY()));
	for(unsigned i = 0; i < numSamplesX(); i++) {
		Point h, b;
		// depths
		h = point(i, 0);
		b = bottom()->point(h.x(), h.y());
		double d0 = max(0.0, h.z() - b.z());
		h = point(i, 1);
		b = bottom()->point(h.x(), h.y());
		double d1 = max(0.0, h.z() - b.z());
		// some before the loop
		_f[0] = -fac * (d0 + d1);
		_e[0] = 1.0 + (d0 + d1) * fac;
		_r[0] = 2.0 * point(i, 0).z() - (*_old_z)[i];
		for(unsigned j = 1; j < numSamplesY(); j++) {
			double d2;
			if(j == numSamplesY() - 1) {
				d2 = -d1;
			} else {
				h = point(i, j+1);
				b = bottom()->point(h.x(), h.y());
				d2 = max(0.0, h.z() - b.z());
			}
			_f[j] = -fac * (d1 + d2);
			_e[j] = 1.0 + (d0 + 2.0 * d1 + d2) * fac;
			_r[j] = 2.0 * point(i, j).z() -
					(*_old_z)[j*numSamplesX()+i];
			d0 = d1;
			d1 = d2;
		}
		// solving for this column
		triag(_f-1, _e, _f, _r, _u, numSamplesY());
		// updating height field
		for(unsigned j = 0; j < numSamplesY(); j++) {
			h = point(i, j);
			b = bottom()->point(h.x(), h.y());
			if(_u[j] < b.z()) {
				(*_old_z)[j * numSamplesX() + i] = b.z() - e;
				setPoint(i, j, b.z() - e);
			} else {
				(*_old_z)[j * numSamplesX() + i] = point(i, j).z();
				setPoint(i, j, _u[j]);
			}
		}
	}
	unlock();
}

void WaterHeightField::drawImplementation(osg::State& state) const
{
	lock();

	glColor4f(0.0, 0.3, 0.7, 0.5);

	// the grid is composed of a set of triangle strips
	for(unsigned j = 0; j < numSamplesY() - 1; j++) {
		glBegin(GL_TRIANGLE_STRIP);
		for(unsigned i = 0; i < numSamplesX(); i++) {
			Point p = point(i, j);
			Vector n = normal(i, j);
			// first point
			glTexCoord2f(
				static_cast<float>(i) / (numSamplesX() - 1),
				static_cast<float>(j) / (numSamplesY() - 1));
			glNormal3f(n.x(), n.y(), n.z());
			glVertex3f(p.x(), p.y(), p.z());
			// second point
			p = point(i, j + 1);
			n = normal(i, j + 1);
			glTexCoord2f(
				static_cast<float>(i) / (numSamplesX() - 1),
				static_cast<float>(j+1) / (numSamplesY() - 1));
			glNormal3f(n.x(), n.y(), n.z());
			glVertex3f(p.x(), p.y(), p.z());
		}
		glEnd();
	}

	unlock();
}

} } // namespace declarations

