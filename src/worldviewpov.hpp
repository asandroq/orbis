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
 * $Id: worldviewpov.hpp,v 1.4 2004/03/15 02:55:21 asandro Exp $
 */

#ifndef __ORBIS_WORLDVIEWPOV_HPP__
#define __ORBIS_WORLDVIEWPOV_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <string>

#include <worldview.hpp>

namespace Orbis {

/*!
 * \brief This class represents a PovRay "view" of the world.
 *
 * It renders the world into the file specified in the constructor.
 */
class WorldViewPOV : public WorldView {
public:
	//! Constructor
	WorldViewPOV(World *wold, const std::string& filename);

	//! Destructor
	virtual ~WorldViewPOV();

	// renders the world into its file representation
	virtual void render();

	//! Notifies this class that the World has changed somehow.
	virtual void worldHasChanged();

private:
	// name of file this view renders to
	std::string _file_name;
};

inline WorldViewPOV::~WorldViewPOV()
{
}

} // namespace declarations

#endif  // __ORBIS_WORLDVIEWPOV_HPP__

