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
 * $Id: terrain.hpp,v 1.3 2004/03/12 02:46:57 asandro Exp $
 */

#ifndef __ORBIS_TERRAIN_HPP__
#define __ORBIS_TERRAIN_HPP__

/*!
 * \file terrain.hpp
 * \brief This file declares the Terrain class.
 */

#include <drawable.hpp>

#include <patch.hpp>

using Orbis::Util::Patch;
using Orbis::Util::PatchList;
using Orbis::Util::PatchListIterator;

namespace Orbis {

	namespace Drawable {

/*!
 * \brief This class represents a limited terrain.
 */
class Terrain : public virtual Drawable {
public:

	/*!
	 * \brief Default constructor.
	 */
	Terrain();

	/*!
	 * \brief Copy constructor.
	 */
	Terrain(const Terrain& terrain,
			const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

	/*!
	 * \brief Adds a new patch to the list.
	 * \param patch The new patch to be added.
	 */
	void addPatch(const Patch& patch);

	/*!
	 * \brief Iterator of the list of patches.
	 */
	PatchListIterator patches() const;

	/*!
	 * \brief Marks the end of the list of patches.
	 */
	PatchListIterator patchesEnd() const;

protected:
	//! destructor
	virtual ~Terrain();

private:
	//! list of regions on this terrain
	PatchList _patch_list;
};

inline Terrain::Terrain()
	: Drawable()
{
}

inline Terrain::Terrain(const Terrain& src, const osg::CopyOp& copyOp)
	: Drawable(src, copyOp)
{
}

inline Terrain::~Terrain()
{
}

inline void Terrain::addPatch(const Patch& patch)
{
	_patch_list.push_back(patch);
}

inline PatchListIterator Terrain::patches() const
{
	return _patch_list.begin();
}

inline PatchListIterator Terrain::patchesEnd() const
{
	return _patch_list.end();
}

} } // namespace declarations

#endif // __ORBIS_TERRAIN_HPP__

