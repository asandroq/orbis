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
 
#ifndef __ORBIS_WATERBASE_HPP__
#define __ORBIS_WATERBASE_HPP__

#include <map>

#include <vector.hpp>
#include <dynamic.hpp>
#include <heightfield.hpp>

using Orbis::Util::Point;

/*!
 * \brief A functor, returns true if a is less, or closer to the origin,
 * than b.
 */
struct PointCmp {
	bool operator()(const Point& a, const Point& b) const
	{
		Point p(0.0, 0.0, 0.0);

		return (a-p).sqrLength() < (b-p).sqrLength();
	}
};

namespace Orbis {

namespace Drawable {

typedef std::vector<double> DoubleVector;

typedef std::multimap<Point, double, PointCmp> SourceList;

typedef SourceList::const_iterator SourceIterator;

/*!
 * \brief Base class for Water-simulating classes.
 */
class WaterBase : public Dynamic {
public:
	/*!
	 * \brief Constructor.
	 */
	WaterBase();

	/*!
	 * \brief Adds a new source of water.
	 * \param p The point where the water enters the flow.
	 * \param val The amount of water that enters at each step.
	 */
	void addSource(const Point& p, double val);

	/*!
	 * \brief Adds a new sink of water.
	 * \param p The point where the water exits the flow.
	 * \param val The amount of water that exits at each step.
	 */
	void addSink(const Point& p, double val);

	/*!
	 * \brief Gives an iterator pointing to the beginning of the sources.
	 * \return The iterator.
	 */
	SourceIterator sourcesBegin() const;

	/*!
	 * \brief Gives an iterator pointing to the beginning of the sources.
	 * \return The iterator.
	 */
	SourceIterator sourcesEnd() const;

	/*!
	 * \brief The heightfield at the bottom.
	 */
	const HeightField* bottom() const;

	/*!
	 * \brief Sets the water bottom.
	 * \param bottom The HeightField that is the bottom of the simulation.
	 */
	void setBottom(const HeightField* const bottom);

private:
	// bottom
	const HeightField *_bottom;
	// list of water sources/sinks
	SourceList _source_list;
};

inline WaterBase::WaterBase()
	: _bottom(0)
{
}

inline void WaterBase::addSource(const Point& p, double val)
{
	if(val > 0.0) {
		_source_list.insert(SourceList::value_type(p, val));
	} else {
		_source_list.insert(SourceList::value_type(p, -val));
	}
}

inline void WaterBase::addSink(const Point& p, double val)
{
	if(val < 0.0) {
		_source_list.insert(SourceList::value_type(p, val));
	} else {
		_source_list.insert(SourceList::value_type(p, -val));
	}
}

inline SourceIterator WaterBase::sourcesBegin() const
{
	return _source_list.begin();
}

inline SourceIterator WaterBase::sourcesEnd() const
{
	return _source_list.end();
}

inline const HeightField* WaterBase::bottom() const
{
	return _bottom;
}

inline void WaterBase::setBottom(const HeightField* const bottom)
{
	_bottom = bottom;
}

} } //namespace declarations

#endif // __ORBIS_WATERBASE_HPP__
