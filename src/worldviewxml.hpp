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
 * $Id: worldviewxml.hpp,v 1.3 2004/02/26 19:29:45 asandro Exp $
 */

#ifndef __ORBIS_WORLDVIEWXML_HPP__
#define __ORBIS_WORLDVIEWXML_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <string>

#include <worldview.hpp>

namespace Orbis {

/*!
 * \brief This class represents a XML "view" of the world.
 *
 * It renders the world into the file specified in the constructor.
 */
class WorldViewXML : public WorldView {
public:
	//! Constructor
	WorldViewXML(World *wold, const std::string& filename);

	//! Destructor
	virtual ~WorldViewXML();

	// renders the world into its file representation
	virtual void render();

	//! Notifies this class that the World has changed somehow.
	virtual void worldHasChanged();

	// loads a World from its file definition
	static World* load(const std::string& filename);

private:
	// name of file this view renders to
	std::string _file_name;
};

inline WorldViewXML::~WorldViewXML()
{
}

} // namespace declarations

#endif  // __ORBIS_WORLDVIEWXML_HPP__
