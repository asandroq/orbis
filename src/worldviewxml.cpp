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
 * $Id: worldviewxml.cpp,v 1.5 2004/02/26 19:29:45 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <fstream>

//#include <libxml/tree.h>

#include <osg/Group>
#include <osg/Geode>
#include <osg/NodeVisitor>

#include <world.hpp>
#include <gridterrain.hpp>
#include <worldviewxml.hpp>

using Orbis::Drawable::GridTerrain;

/*!
 * \brief A class to visit every node and write its description
 * to the XML stream.
 */
class WriteNodeVisitor : public osg::NodeVisitor {
public:
	//! Constructor
	WriteNodeVisitor(std::ostream& out);

	//! Apply this visitor to a group
	void apply(osg::Group& group);

	//! Apply this visitor to a geode
	void apply(osg::Geode& geode);

protected:
	//! Destructor
	~WriteNodeVisitor();

private:
	// write a GridTerrain to output stream
	void write(const GridTerrain*) const;

	// reference to output stream
	std::ostream& _out;
};

WriteNodeVisitor::WriteNodeVisitor(std::ostream& out)
	: _out(out)
{
}

WriteNodeVisitor::~WriteNodeVisitor()
{
}

void WriteNodeVisitor::apply(osg::Group& group)
{
	_out << "<group>\n";
	
	// visiting children
	group.traverse(*this);

	_out << "</group>\n";
}

void WriteNodeVisitor::apply(osg::Geode& geode)
{
	// iterating through list of drawables
	for(unsigned i = 0; i < geode.getNumDrawables(); i++) {
		const osg::Drawable *drawable = geode.getDrawable(i);
		// which kind of drawable is this
		if(const GridTerrain* grid_terrain = dynamic_cast<const GridTerrain*>(drawable)) {
			write(grid_terrain);
		} else if(0) {
			// TODO: add more type tests
		}
	}
}

void WriteNodeVisitor::write(const GridTerrain* terrain) const
{
	_out << "<gridterrain>\n";
	_out << "</gridterrain>\n";
}

namespace Orbis {

WorldViewXML::WorldViewXML(World* world, const std::string& filename)
	: WorldView(world), _file_name(filename)
{
}

void WorldViewXML::render()
{
	// trying to open out stream
	std::ofstream out(_file_name.c_str());
	if(!out.is_open()) {
		return;
	}

	// writing header
	out << "<!DOCTYPE world SYSTEM \"http://www.lcg.dc.ufc.br/orbis/world.dtd\">\n";
	out << "<world xmlns=\"http://www.lcg.dc.ufc.br/orbis\">\n";

	// traversing the graph rendering the nodes
	osg::ref_ptr<WriteNodeVisitor> visitor = new WriteNodeVisitor(out);
	world()->root()->accept(*visitor);

	// finishing file
	out << "</world>\n";
}

void WorldViewXML::worldHasChanged()
{
}

World* WorldViewXML::load(const std::string& filename)
{
	World *world = 0;

	return world;
}

} // namespace declarations
