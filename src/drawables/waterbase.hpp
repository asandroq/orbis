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

/*!
 * \brief A type of a vector of doubles.
 */
typedef std::vector<double> DoubleVector;
	
/*!
 * \brief A source is an inflow of water into the system.
 */
class Source {
public:
	/*!
	 * \brief Constructor.
	 * \param pos The position of source in space.
	 * \param vel The initial velocity of the inflow.
	 * \param str The strength of the inflow.
	 */
	Source(const Point& pos, const Vector& vel, double str);

	/*!
	 * \brief Destructor.
	 */
	~Source();

	/*!
	 * \brief Queries the position of this source.
	 * \return The position.
	 */
	Point position() const;

	/*!
	 * \brief Queries the velocity of the inflow.
	 * \return The velocity.
	 */
	Vector velocity() const;

	/*!
	 * \brief Queries the strength of this source.
	 * \return The strength.
	 */
	double strength() const;

private:
	// position
	Point _pos;
	// velocity
	Vector _vel;
	// strength
	double _str;
};
	
inline Source::Source(const Point& pos, const Vector& vel, double str)
	: _pos(pos), _vel(vel), _str(str)
{
}

inline Source::~Source()
{
}

inline Point Source::position() const
{
	return _pos;
}


inline Vector Source::velocity() const
{
	return _vel;
}

inline double Source::strength() const
{
	return _str;
}

typedef std::vector<Source> SourceList;

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
	 * \param s The source of water.
	 */
	void addSource(const Source& s);

	/*!
	 * \brief Adds a new sink of water.
	 * \param s The source of water.
	 */
	void addSink(const Source& s);

	/*!
	 * \brief Gives an iterator pointing to the beginning of the sources.
	 * \return The iterator.
	 */
	SourceIterator sources() const;

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
	virtual void setBottom(const HeightField* const bottom);

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

inline void WaterBase::addSource(const Source& s)
{
	_source_list.push_back(s);
}

inline void WaterBase::addSink(const Source& s)
{
	_source_list.push_back(s);
}

inline SourceIterator WaterBase::sources() const
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
