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
 * $Id: spline.cpp,v 1.3 2003/07/18 03:11:49 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <math.hpp>
#include <spline.hpp>

namespace Orbis {

	namespace Util {

void Spline::addCtrlPoint(const Point& p)
{
	_ctrl_points.push_back(p);
}


Point Spline::pointOnSpline(double x)
{
	int t;
	Point p;
	unsigned i, size;
	double aux1, t_aux;

	size = _ctrl_points.size();
	switch(size) {
	case 0:
	case 1:
		return Point();
	case 2:
		_b.clear();
		_b.push_back(_ctrl_points[0]);
		_b.push_back(_ctrl_points[1]);
		t_aux = x / static_cast<double>(_sub_divs);
		p = Point(_b[1].x()*(1-t_aux) + _b[0].x()*t_aux, 
				_b[1].y()*(1-t_aux) + _b[0].y()*t_aux);
		break;

	case 3:
		_b.clear();
		_b.push_back(_ctrl_points[0]);
		_b.push_back(_ctrl_points[1]);
		_b.push_back(_ctrl_points[2]);
		t_aux = x / static_cast<double>(_sub_divs);
		p = quadratic(_b[0], _b[1], _b[2], t_aux);
		break;

	case 4:
		_b.clear();
		_b.push_back(_ctrl_points[0]);
		_b.push_back(_ctrl_points[1]);
		_b.push_back(_ctrl_points[2]);
		_b.push_back(_ctrl_points[3]);
		t_aux = x / static_cast<double>(_sub_divs);
		p = bezier(_b[0], _b[1], _b[2], _b[3], t_aux);
		break;

	default:
		_b.clear();
		_b.reserve(3 * size - 8);
		double *delta = new double[size-1];

		for(i = 0; i < size-1; i++) {
			delta[i] = 1.0;
		}

		_b[0] = _ctrl_points[0];
		_b[1] = _ctrl_points[1];

		_b[2] = Point((delta[1]/(delta[0]+delta[1])) *
				_ctrl_points[1].x() +
				(delta[0]/(delta[0]+delta[1])) *
				_ctrl_points[2].x(),
				(delta[1]/(delta[0]+delta[1])) *
				_ctrl_points[1].y() +
				(delta[0]/(delta[0]+delta[1])) *
				_ctrl_points[2].y());

		_b[3*size-11] =
			Point((delta[size-4]/(delta[size-5] +
				delta[size-4])) *_ctrl_points[size-3].x() +
				(delta[size-5]/(delta[size-5] +
				 delta[size-4]))*_ctrl_points[size-2].x(),
				(delta[size-4]/(delta[size-5] +
				 delta[size-4]))*_ctrl_points[size-3].y() +
				(delta[size-5]/(delta[size-5] +
				 delta[size-4]))*_ctrl_points[size-2].y());

		_b[3*size-10] = Point(_ctrl_points[size-2].x(),
					_ctrl_points[size-2].y());

		_b[3*size-9] = Point(_ctrl_points[size-1].x(),
					_ctrl_points[size-1].y());

		for(i = 1; i <= size - 5; i++) {
			aux1 = delta[i-1] + delta[i] + delta[i+1];
			_b[3*i+1] =
				Point(((delta[i]+delta[i+1])/aux1) *
					_ctrl_points[i+1].x() +
					((delta[i-1])/aux1) *
					_ctrl_points[i+2].x(),
					((delta[i] + delta[i+1])/aux1) *
					_ctrl_points[i+1].y()+
					((delta[i-1])/aux1) *
					_ctrl_points[i+2].y());

			_b[3*i+2] =
				Point(((delta[i+1])/aux1) *
					_ctrl_points[i+1].x() +
					((delta[i-1]+delta[i])/aux1) *
					_ctrl_points[i+2].x(),
					((delta[i+1])/aux1) *
					_ctrl_points[i+1].y()+
					((delta[i-1]+delta[i])/aux1) *
					_ctrl_points[i+2].y());

			_b[3*i] =
				Point((delta[i]/(delta[i-1]+delta[i])) *
					_b[3*i-1].x()+
					(delta[i-1]/(delta[i-1]+delta[i])) *
					_b[3*i+1].x(),
					(delta[i]/(delta[i-1]+delta[i])) *
					_b[3*i-1].y()+
					(delta[i-1]/(delta[i-1]+delta[i])) *
					_b[3*i+1].y());
		}

		_b[3*i] =
			Point((delta[i]/(delta[i-1]+delta[i])) *
				_b[3*i-1].x()+
				(delta[i-1]/(delta[i-1]+delta[i])) *
				_b[3*i+1].x(),
				(delta[i]/(delta[i-1]+delta[i])) *
				_b[3*i-1].y()+
				(delta[i-1]/(delta[i-1] + delta[i])) *
				_b[3*i+1].y());

		t_aux = x * static_cast<double>(size-3) / _sub_divs;
		t = static_cast<int>(t_aux);
		t_aux -= static_cast<int>(t_aux);

		if(x == static_cast<double>(_sub_divs)) {
			t--;
			t_aux = 1.0;
		}

		p = bezier(_b[3*t], _b[3*t+1], _b[3*t+2], _b[3*t+3], t_aux);
		delete [] delta;
		_b.clear();
	}
	
	return p;
}

Vector Spline::derivativeOnSpline(double x)
{
	int t;
	unsigned i;
	double t_aux;
	Vector aux;
	Point p1, p2, p3, p4;

	unsigned size = _ctrl_points.size();
	switch(size) {
	case 0:
	case 1:
		return Vector();
	case 2:
		aux = _ctrl_points[0] - _ctrl_points[1];
		break;

	case 3:
		p1 = _ctrl_points[0];
		p2 = _ctrl_points[1];
		p3 = _ctrl_points[2];
		t_aux = x / static_cast<double>(_sub_divs);
		aux =
			Vector(-2 * p1.x() * (1-t_aux) +
				2 * p2.x() * (1-2*t_aux) +
				2 * p3.x() * t_aux,
				-2 * p1.y() * (1-t_aux) +
				2 * p2.y() * (1-2*t_aux) +
				2 * p3.y() * t_aux);
		break;

	case 4:
		p1 = _ctrl_points[0];
		p2 = _ctrl_points[1];
		p3 = _ctrl_points[2];
		p4 = _ctrl_points[3];
		t_aux = x / static_cast<double>(_sub_divs);
		aux = deriv_B(p1, p2, p3, p4, t_aux);
		break;

	default:
		_b.clear();
		_b.reserve(3*size-8);
		double *delta = new double[size-1];

		for(i = 0; i < size-1; i++) {
			delta[i] = 1.0;
		}

		_b[0] = _ctrl_points[0];
		_b[1] = _ctrl_points[1];	

		_b[2] =
			Point((delta[1]/(delta[0]+delta[1])) *
				_ctrl_points[1].x()+
				(delta[0]/(delta[0]+delta[1])) *
				_ctrl_points[2].x(),
				(delta[1]/(delta[0]+delta[1])) *
				_ctrl_points[1].y()+
				(delta[0]/(delta[0]+delta[1])) *
				_ctrl_points[2].y());

		_b[3*_ctrl_points.size()-11] =
			Point((delta[size-4] / (delta[size-5] +
				 delta[size-4])) * _ctrl_points[size-3].x() +
				(delta[size-5] /(delta[size-5] +
				 delta[size-4])) * _ctrl_points[size-2].x(),
				(delta[size-4] / (delta[size-5] +
				 delta[size-4])) * _ctrl_points[size-3].y() +
				(delta[size-5] / (delta[size-5] +
				 delta[size-4])) * _ctrl_points[size-2].y());

		_b[3*size-10] = _ctrl_points[size-2];
		_b[3*size- 9] = _ctrl_points[size-1];

		for(i = 1; i <= _ctrl_points.size() - 5; i++)
		{
			double aux1 = delta[i - 1] + delta[i] + delta[i + 1];
			_b[3*i+1] =
				Point(((delta[i]+delta[i+1])/aux1) *
					_ctrl_points[i+1].x() +
					((delta[i-1])/aux1) *
					_ctrl_points[i+2].x(),
					((delta[i]+delta[i+1])/aux1) *
					_ctrl_points[i+1].y()+
					((delta[i-1])/aux1) *
					_ctrl_points[i+2].y());

			_b[3*i+2] =
				Point(((delta[i+1])/aux1) *
					_ctrl_points[i+1].x() +
					((delta[i-1]+delta[i])/aux1) *
					_ctrl_points[i+2].x(),
					((delta[i+1])/aux1) *
					_ctrl_points[i+1].y() +
					((delta[i-1]+delta[i])/aux1) *
					_ctrl_points[i+2].y());
	
			_b[3*i] =
				Point((delta[i]/(delta[i-1]+delta[i])) *
					_b[3*i-1].x() +
					(delta[i-1]/(delta[i-1]+delta[i])) *
					_b[3*i+1].x(),
					(delta[  i]/(delta[i-1]+delta[i])) *
					_b[3*i-1].y() +
					(delta[i-1]/(delta[i-1]+delta[i])) *
					_b[3*i+1].y());
		}

		_b[3*i] =
			Point((delta[i]/(delta[i-1]+delta[i])) *
				_b[3*i-1].x() +
				(delta[i-1]/(delta[i-1]+delta[i])) *
				_b[3*i+1].x(),
				(delta[  i]/(delta[i-1]+delta[i])) *
				_b[3*i-1].y() +
				(delta[i-1]/(delta[i-1]+delta[i])) *
				_b[3*i+1].y());

		t_aux = x * double(size-3)/(double)(_sub_divs);
		t = static_cast<int>(t_aux);
		t_aux -= static_cast<int>(t_aux);

		if(x == static_cast<double>(_sub_divs)) {
			t--;
			t_aux = 1.0;
		}

		aux = deriv_B(_b[3*t], _b[3*t+1], _b[3*t+2], _b[3*t+3], t_aux);
		_b.clear();
		delete [] delta;
	}

	return aux;
}

Vector Spline::normalToSpline(double x)
{
	Vector aux;

	aux = derivativeOnSpline(x);

	return Vector(-aux.y(), aux.x());
}


Point Spline::bezier(Point p1, Point p2, Point p3, Point p4, double t)
{
	return B(p1, p2, p3, p4, t);
}

#define S_SQR(x) ((x)*(x))
#define S_CUB(x) ((x)*(x)*(x))

Point Spline::B(Point p1, Point p2, Point p3, Point p4, double t)
{
	return Point(p1.x()*S_CUB(1-t) + p2.x()*3*t*S_SQR(1-t) +
			p3.x()*3*S_SQR(t)*(1-t) + p4.x()*S_CUB(t),
			p1.y()*S_CUB(1-t) + p2.y()*3*t*S_SQR(1-t) +
			p3.y()*3*S_SQR(t)*(1-t) + p4.y()*S_CUB(t));
}

Vector Spline::deriv_B(Point p1, Point p2, Point p3, Point p4, double t)
{
	return Vector(-3*p1.x()*S_SQR(1-t) + 3*p2.x()*(t*(-2*(1-t))+S_SQR(1-t)) +
			3*p3.x()*(-S_SQR(t) + 2*t*(1-t)) + 3*p4.x()*S_SQR(t),
			-3*p1.y()*S_SQR(1-t) + 3*p2.y()*(t*(-2*(1-t))+S_SQR(1-t)) +
			3*p3.y()*(-S_SQR(t) + 2*t*(1-t)) + 3*p4.y()*S_SQR(t));
}


Point Spline::quadratic(Point p1, Point p2, Point p3, double t)
{
	return Point(p1.x()*(1-t)*(1-t) + 2*p2.x()*t*(1-t) + p3.x()*t*t,
			p1.y()*(1-t)*(1-t) + 2*p2.y()*t*(1-t) + p3.y()*t*t);
}

#undef S_SQR
#undef S_CUB

} }

// Done.
