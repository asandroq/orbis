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

#ifndef __ORBIS_MATH_HPP__
#define __ORBIS_MATH_HPP__

#include <cmath>
#include <limits>

/*!
 * \file math.hpp
 * \brief This file declares several mathematical methods.
 */

namespace Orbis {

	namespace Math {

//! largest number
const double Omega = std::numeric_limits<double>::max();
//! minimum number such as 1 + epsilon != 1
const double Epsilon = 2.0 * std::numeric_limits<double>::epsilon();

#ifdef M_PI
	const double Pi = M_PI;
#else
	const double Pi = 3.1415926535898;
#endif

inline bool isZero(double t)
{
	return -Epsilon < t && t < Epsilon;
}

inline bool areEqual(double a, double b)
{
	return isZero(a-b);
}

template<typename T> inline T min(T a, T b)
{
	return a < b ? a : b;
}

template<typename T> inline T max(T a, T b)
{
	return a > b ? a : b;
}

template<typename T> inline T min(T a, T b, T c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

template<typename T> inline T max(T a, T b, T c)
{
	return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

template<typename T> inline T clamp(T a, T min, T max)
{
	return a < min ? min : a > max ? max : a;
}

template<typename T> inline T sqr(T x)
{
	return x * x;
}

template<typename T> inline T cub(T x)
{
	return x * x * x;
}

// convert degrees to radians
inline double radians(double degrees)
{
	return degrees * (Pi/180.0);
}

inline double degrees(double radians)
{
	return radians / (Pi/180.0);
}

// a function to find the cubic root
inline double cbrt(double x)
{
	return x > 0.0 ? pow(x, 1.0/3.0) : x < 0.0 ? -pow(-x, 1.0/3.0) : 0.0;
}

template <typename T> inline void order(T& a, T& b)
{
	if(a > b) {
		T tmp = a;
		a = b;
		b = tmp;
	}
}

/*!
 * \brief Finds the ratio of a value between two limits.
 * \param val Value to be interpolated.
 * \param min Lower limit.
 * \param max Upper limit.
 * \return A number in the range [0.0, 1.0].
 */
inline double interpolate(double val, double min, double max)
{
	if(areEqual(val, min)) {
		return 0.0;
	} else if(areEqual(val, max)) {
		return 1.0;
	} else if(areEqual(min, max)) {
		return 0.0;
	} else {
		return (val - min) / (max - min);
	}
}

/*!
 * \brief A class for the generation of random numbers.
 */
class Random {
public:
	/*!
	 * \brief Generates a random number in the interval [0.0, 1.0].
	 * \return The generated number.
	 */
	static double rand();

	/*!
	 * \brief Generates a random number in the interval [-1.0, 1.0].
	 * \return The generated number.
	 */
	static double rand2();

private:
	static unsigned _state;
};

} } // namespace declarations

#endif  // __ORBIS_MATH_HPP__
