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

#include <stdexcept>

#include <gdal_priv.h>

#include <gridheightfield.hpp>

using Orbis::Math::min;

inline static double det(double a, double b, double c,
				double d, double e, double f)
{
	return a*e + b*f + c*d - c*e - b*d - a*f;
}

namespace Orbis {

	namespace Drawable {

GridHeightField::GridHeightField()
	: HeightField(), _xsamples(0), _ysamples(0), _xstep(0.0), _ystep(0.0)
{
	_elevs = new FloatArray;
}

GridHeightField::GridHeightField(const GridHeightField& src,
					const osg::CopyOp& copyOp)
	: HeightField(src, copyOp),
		_elevs(new FloatArray(*src._elevs, copyOp)),
		_xsamples(src._xsamples), _ysamples(src._ysamples),
		_xstep(src._xstep), _ystep(src._ystep)
{
}	

GridHeightField::GridHeightField(const Point& origin,
					double xstep, double ystep,
						unsigned xsize, unsigned ysize)
	: HeightField(origin),
		_elevs(new FloatArray(xsize * ysize)),
		_xsamples(xsize), _ysamples(ysize),
		_xstep(xstep), _ystep(ystep)
{
}

GridHeightField::GridHeightField(const std::string& filename,
									double xstep, double ystep)
	: HeightField(), _xstep(xstep), _ystep(ystep)
{
	_elevs = new FloatArray;
	load(filename);
}

/* loads the heightfield from a data file */
bool GridHeightField::load(const std::string& filename)
{
	static bool gdal_init = false;

	/* one-time initialisation of GDAL */
	if(!gdal_init) {
		gdal_init = true;
		GDALAllRegister();
	}

	std::auto_ptr<GDALDataset> dataset(
				(GDALDataset*)GDALOpen(filename.c_str(), GA_ReadOnly));
	if(!dataset.get()) {
		return false;
	}

	int width = dataset->GetRasterXSize();
	width = min(width, 512);
	int depth = dataset->GetRasterYSize();
	depth = min(depth, 512);

	double geo_trans[6];
	dataset->GetGeoTransform(geo_trans);

	GDALRasterBand *bandGray = 0;
	GDALRasterBand *bandRed = 0;
	GDALRasterBand *bandGreen = 0;
	GDALRasterBand *bandBlue = 0;
	GDALRasterBand *bandAlpha = 0;
	for(int i = 1; i <= dataset->GetRasterCount(); i++) {
		GDALRasterBand *band = dataset->GetRasterBand(i);

		switch(band->GetColorInterpretation()) {
			case GCI_GrayIndex:
				bandGray = band;
				break;
			case GCI_RedBand:
				bandRed = band;
				break;
			case GCI_GreenBand:
				bandGreen = band;
				break;
			case GCI_BlueBand:
				bandBlue = band;
				break;
			case GCI_AlphaBand:
				bandAlpha = band;
				break;
			default:
				bandGray = band;
		}
	}

	GDALRasterBand *bandSelected = 0;
	if(!bandSelected && bandGray) {
		bandSelected = bandGray;
	} else if(!bandSelected && bandAlpha) {
		bandSelected = bandAlpha;
	} else if(!bandSelected && bandRed) {
		bandSelected = bandRed;
	} else if(!bandSelected && bandGreen) {
		bandSelected = bandGreen;
	} else if(!bandSelected && bandBlue) {
		bandSelected = bandBlue;
	}

	if(bandSelected) {
		_xsamples = width;
		_ysamples = depth;
		setOrigin(Point(-1.0*(_xsamples-1)*_xstep/2.0,
					 -1.0*(_ysamples-1)*_ystep/2.0));
	
		// loading from GDAL into array of elevations
		_elevs->resize(width*depth);
		bandSelected->RasterIO(GF_Read, 0, 0, width, depth,
					 static_cast<void*>(&_elevs->front()),
							 width, depth, GDT_Float32, 0, 0);

		// changing GDAL NODATA values to something more apropriated
		FloatArray::iterator it;
		for(it = _elevs->begin(); it != _elevs->end(); it++) {
			if(*it < -9000.0) {
				*it = 0.0;
			}
		}

		// flipping
		unsigned int copy_r = _ysamples - 1;
		for(unsigned int r = 0; r < copy_r; r++, copy_r--) {
			for(unsigned int c = 0; c < _xsamples; c++) {
				float temp = point(c, r).z();
				setPoint(c, r, point(c, copy_r).z());
				setPoint(c, copy_r, temp);
			}
		}

		return true;
	} else {
		return false;
	}
}

Point GridHeightField::point(double x, double y) const
{
	// is the point over the terrain?
	if(x < origin().x() || x > origin().x() + sizeX() ||
					y < origin().y() || y > origin().y() + sizeY()) {
		throw std::out_of_range("point is outside the heightfield");
	}
	// mapping from coordinates to grid vertices
	unsigned i = static_cast<unsigned>(floor((x - origin().x()) / stepX()));
	unsigned j = static_cast<unsigned>(floor((y - origin().y()) / stepY()));
	// base point
	Point bp = point(i, j);
	// using the tangent to find out in which triangle
	// of the two lies the point
	double z;
	double fx = bp.x();
	double fy = bp.y();
	double dx = stepX();
	double dy = stepY();
	double distx = x - fx;
	double disty = y - fy;
	if(disty/distx > dy/dx) {
		// computing coordinates
		z         = det(fx, fx+dx, fx, fy, fy+dy, fy+dy);
		double l1 = det( x, fx+dx, fx,  y, fy+dy, fy+dy) / z;
		double l2 = det(fx,     x, fx, fy,     y, fy+dy) / z;
		double l3 = det(fx, fx+dx,  x, fy, fy+dy,     y) / z;
		z = l1 * point(j * _xsamples + i) +
			l2 * point((j+1) * _xsamples + i+1) +
				l3 * point((j+1)*_xsamples + i);
	} else {
		// computing coordinates
		z         = det(fx, fx+dx, fx+dx, fy, fy, fy+dy);
		double l1 = det( x, fx+dx, fx+dx,  y, fy, fy+dy) / z;
		double l2 = det(fx,     x, fx+dx, fy,  y, fy+dy) / z;
		double l3 = det(fx, fx+dx,     x, fy, fy,     y) / z;
		z = l1 * point(j * _xsamples + i) +
			l2 * point(j * _xsamples + i+1) +
				l3 * point((j+1) * _xsamples + i+1);
	}

	return Point(x, y, z);
}

Vector GridHeightField::normal(double x, double y) const
{
	// is the point over the terrain?
	if(x < origin().x() || x > origin().x() + sizeX() ||
					y < origin().y() || y > origin().y() + sizeY()) {
		throw std::out_of_range("normal is outside the height field");
	}
	// mapping from coordinates to grid vertices
	unsigned i = static_cast<unsigned>(floor((x - origin().x()) / stepX()));
	unsigned j = static_cast<unsigned>(floor((y - origin().y()) / stepY()));
	// points
	Point p1 = point(i, j), p2, p3;
	// using the tangent to find out in which triangle
	// of the two lies the point
	double distx = x - p1.x();
	double disty = y - p1.y();
	if(disty/distx > stepY()/stepX()) {
		p2 = point(i+1, j+1);
		p3 = point(  i, j+1);
	} else {
		p2 = point(i+1, j  );
		p3 = point(i+1, j+1);
	}
	return ((p2 - p1) ^ (p3 - p1)).normalise();
}

Point GridHeightField::point(unsigned i, unsigned j) const
{
	if(i >= _xsamples || j >= _ysamples) {
		throw std::out_of_range("invalid grid coordinates");
	}

	return Point(i * stepX() + origin().x(),
				j * stepY() + origin().y(),
					(*_elevs)[j * _xsamples + i]);
}

void GridHeightField::setPoint(unsigned i, unsigned j, double val)
{
	if(i >= _xsamples || j >= _ysamples) {
		throw std::out_of_range("invalid grid coordinates");
	}

	(*_elevs)[j * _xsamples + i] = val;

	if(val < _min_elev) {
		_min_elev = val;
		dirtyBound();
	}
	if(val > _max_elev) {
		_max_elev = val;
		dirtyBound();
	}
}

Vector GridHeightField::normal(unsigned i, unsigned j) const
{
	double za, zb;
	unsigned o, a, b, n1, n2;
	double d1, d2, nx, ny, nz;

	if(i >= _xsamples || j >= _ysamples) {
		throw std::out_of_range("invalid grid coordinates");
	}

	d1 = _xstep;
	d2 = _ystep;
	n1 = _xsamples;
	n2 = _ysamples;
	// distances
	nz = d1 * d2;
	// points base index
	o = j * n1 + i;
	// averaging normals
	if(i == 0) {
		if(j == 0) {
			// in bottom-left corner
			// first normal
			a = o + 1;
			b = a + n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx = - d2 * za;
			ny = d1 * (za - zb);
			// second normal
			b = o + n1;
			a = b + 1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * (zb - za);
			ny += - d1 * zb;
			// z fix
			nz *= 2.0;
		} else if(j == n2 - 1) {
			// in top-left corner
			// just one normal
			b = o + 1;
			a = o - n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx = - d2 * zb;
			ny =   d1 * za;
		} else {
			// on the left edge
			// first normal
			b = o + 1;
			a = o - n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx = - d2 * zb;
			ny =   d1 * za;
			// second normal
			a = o + 1;
			b = a + n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += - d2 * za;
			ny += d1 * (za - zb);
			// third normal
			b = o + n1;
			a = b + 1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * (zb - za);
			ny += - d1 * zb;
			// z fix
			nz *= 3.0;
		}
	} else if(i == n1 - 1) {
		if(j == 0) {
			// in bottom-right corner
			// just one normal
			b = o - 1;
			a = o + n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx =   d2 * zb;
			ny = - d1 * za;
		} else if(j == n2 - 1) {
			// in top-right corner
			// first normal
			a = o - 1;
			b = a - n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx = d2 * za;
			ny = d1 * (zb - za);
			// second normal
			b = o - n1;
			a = b - 1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * (za - zb);
			ny += d1 * zb;
			// z fix
			nz *= 2.0;
		} else {
			// on right edge
			// first normal
			b = o - 1;
			a = o + n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx =   d2 * zb;
			ny = - d1 * za;
			// second normal
			a = o - 1;
			b = a - n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * za;
			ny += d1 * (zb - za);
			// third normal
			b = o - n1;
			a = b - 1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * (za - zb);
			ny += d1 * zb;
			// z fix
			nz *= 3.0;
		}
	} else {
		if(j == 0) {
			// on botton edge
			// first normal
			b = o - 1;
			a = o + n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx =   d2 * zb;
			ny = - d1 * za;
			// second normal
			b = o + n1;
			a = b + 1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * (zb - za);
			ny += - d1 * zb;
			// third normal
			a = o + 1;
			b = a + n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += - d2 * za;
			ny += d1 * (za - zb);
			// z fix
			nz *= 3.0;
		} else if(j == n2 - 1) {
			// on top edge
			// first normal
			a = o - 1;
			b = a - n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx = d2 * za;
			ny = d1 * (zb - za);
			// second normal
			b = o - n1;
			a = b - 1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * (za - zb);
			ny += d1 * zb;
			// third normal
			b = o + 1;
			a = o - n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += - d2 * zb;
			ny +=   d1 * za;
			// z fix
			nz *= 3.0;
		} else {
			// in the middle
			// first normal
			b = o + 1;
			a = o - n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx = - d2 * zb;
			ny =   d1 * za;
			// second normal
			a = o + 1;
			b = a + n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += - d2 * za;
			ny += d1 * (za - zb);
			// third normal
			b = o + n1;
			a = b + 1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * (zb - za);
			ny += - d1 * zb;
			// fourth normal
			b = o - 1;
			a = o + n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx +=   d2 * zb;
			ny += - d1 * za;
			// fifth normal
			a = o - 1;
			b = a - n1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * za;
			ny += d1 * (zb - za);
			// sixth normal
			b = o - n1;
			a = b - 1;
			za = point(a) - point(o);
			zb = point(b) - point(o);
			nx += d2 * (za - zb);
			ny += d1 * zb;
			// z fix
			nz *= 6.0;
		}
	}

	// the result is the averaged vector
	return Vector(nx, ny, nz).normalise();
}

} } // namespace declarations
