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
 * $Id: patch.hpp,v 1.5 2004/04/01 20:51:55 asandro Exp $
 */

#ifndef __ORBIS_PATCH_HPP__
#define __ORBIS_PATCH_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <map>
#include <string>

#include <geometry.hpp>

using Orbis::Math::Omega;

namespace Orbis {

	namespace Util {

// list of general purpose attributes
typedef std::map<std::string, std::string> AttributeList;

/*!
 * \brief This class represents a patch on the heightfield and its
 * general-purpose properties.
 */
class Patch
{
public:
	//! Constructor
	Patch();

	//! Constructor that takes a boundary
	Patch(const PolyLine& boundary);

	//! Destructor
	~Patch();

	//! Adds a new point to the region's limit
	void addPoint(const Point& p);

	//! Queries the region's boundary
	PolyLineIterator boundary() const;

	//! The boundary end marker
	PolyLineIterator boundaryEnd() const;

	double minX() const;

	double maxX() const;

	double minY() const;

	double maxY() const;

	/*!
	 * \brief Tests if a point is in the patch.
	 * \param p The point to be tested.
	 */
	bool contains(const Point& p) const;

	//! Gets an attribute from the list
	const std::string attribute(const std::string& key) const;

	//! Sets an attribute in the list
	void setAttribute(const std::string& key, const std::string& value);

private:
	// the region's limit
	PolyLine _poly_line;
	// list of general purpose attributes
	AttributeList _attr_list;
	// patch's extremes
	double _minx, _maxx, _miny, _maxy;
};

// a patch list
typedef std::vector<Patch> PatchList;
// and its iterator
typedef PatchList::const_iterator PatchListIterator;

inline Patch::Patch()
	: _minx(Omega), _maxx(-Omega), _miny(Omega), _maxy(-Omega)
{
}

inline Patch::Patch(const PolyLine& boundary)
	: _poly_line(boundary), _minx(Omega), _maxx(-Omega),
								_miny(Omega), _maxy(-Omega)
{
}

inline Patch::~Patch()
{
}

inline PolyLineIterator Patch::boundary() const
{
	return _poly_line.begin();
}

inline PolyLineIterator Patch::boundaryEnd() const
{
	return _poly_line.end();
}

inline double Patch::minX() const
{
	return _minx;
}

inline double Patch::maxX() const
{
	return _maxx;
}

inline double Patch::minY() const
{
	return _miny;
}

inline double Patch::maxY() const
{
	return _maxy;
}

} } // namespace declarations

#endif // __ORBIS_PATCH_HPP__

