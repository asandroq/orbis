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
 * $Id: worldviewpov.cpp,v 1.8 2004/03/15 02:55:21 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <fstream>

#include <osg/Group>
#include <osg/Geode>
#include <osg/NodeVisitor>

#include <world.hpp>
#include <gridterrain.hpp>
#include <worldviewpov.hpp>
#include <waterheightfield.hpp>

using Orbis::Drawable::GridTerrain;
using Orbis::Drawable::GridHeightField;
using Orbis::Drawable::WaterHeightField;

/*!
 * \brief A class to visit every node and write its description.
 */
class POVWriteNodeVisitor : public osg::NodeVisitor {
public:
	//! Constructor
	POVWriteNodeVisitor(std::ostream& out);

	//! Apply this visitor to a group
	void apply(osg::Group& group);

	//! Apply this visitor to a geode
	void apply(osg::Geode& geode);

protected:
	//! Destructor
	~POVWriteNodeVisitor();

private:
	// write a GridTerrain to output stream
	void write(const GridTerrain*) const;

	// write a WaterHeightField to output stream
	void write(const WaterHeightField*) const;

	// write a GridHeightField to output stream
	void write(const GridHeightField*) const;

	// reference to output stream
	std::ostream& _out;
};

POVWriteNodeVisitor::POVWriteNodeVisitor(std::ostream& out)
	: _out(out)
{
}

POVWriteNodeVisitor::~POVWriteNodeVisitor()
{
}

void POVWriteNodeVisitor::apply(osg::Group& group)
{
	_out << "/* A group starts... */\n\n";
	
	// visiting children
	group.traverse(*this);

	_out << "/* A group ends... */\n\n";
}

void POVWriteNodeVisitor::apply(osg::Geode& geode)
{
	// iterating through list of drawables
	for(unsigned i = 0; i < geode.getNumDrawables(); i++) {
		const osg::Drawable *drawable = geode.getDrawable(i);
		// which kind of drawable is this
		if(const GridTerrain* grid_terrain =
					 dynamic_cast<const GridTerrain*>(drawable)) {
			write(grid_terrain);
		} else if(const WaterHeightField* water_field =
					 dynamic_cast<const WaterHeightField*>(drawable)) {
			write(water_field);
		} else if(0) {
			// TODO: add more type tests
		}
	}
}

void POVWriteNodeVisitor::write(const GridTerrain* terrain) const
{
	_out << "/* This is a GridTerrain */\n";
	write(static_cast<const GridHeightField*>(terrain));
	_out << "texture {\n";
	_out << "pigment {\n";
	_out << "colour rgb <0.7, 0.7, 0.5>\n";
	_out << "}\n";
	_out << "}\n";
	_out << "}\n\n";			// mesh2
}

void POVWriteNodeVisitor::write(const WaterHeightField* water) const
{
	_out << "/* This is a WaterHeightField */\n";
	write(static_cast<const GridHeightField*>(water));
	_out << "texture {\n";
	_out << "pigment {\n";
	_out << "colour rgbf <1.0, 1.0, 1.0, 1.0>\n";
	_out << "}\n";
	_out << "finish {\n";
	_out << "phong 1\n";
	_out << "phong_size 90\n";
	_out << "diffuse 0\n";
	_out << "reflection 0.2\n";
	_out << "}\n";
	_out << "normal {\n";
	_out << "ripples\n";
	_out << "scale 3000\n";
	_out << "frequency 5000\n";
	_out << "}\n";
	_out << "}\n";
	_out << "interior {\n";
	_out << "ior 1.33\n";
	_out << "media {\n";
	_out << "absorption <0.1, 0.05, 0.05>\n";
	_out << "samples 2, 2\n";
	_out << "}\n";
	_out << "}\n";
	_out << "hollow\n";
	_out << "}\n\n";			// mesh2
}

void POVWriteNodeVisitor::write(const GridHeightField* hf) const
{
	unsigned num_x = hf->numSamplesX();
	unsigned num_y = hf->numSamplesY();

	_out << "mesh2 {\n";
	_out << "vertex_vectors {\n";
	_out << num_x * num_y << ",\n";
	for(unsigned j = 0; j < num_y; j++) {
		for(unsigned i = 0; i < num_x; i++) {
			Point p = hf->point(i, j);
			_out << "<" << p.x() << "," << p.y() << "," << p.z() << ">";
			if((i != num_x - 1) && (j != num_y - 1)) {
				_out << ", ";
			}
			if((i+1) % 2 == 0) {
				_out << "\n";
			}
		}
	}
	_out << "}\n";					// vertex_vectors
	_out << "normal_vectors {\n";
	_out << num_x * num_y << ",\n";
	for(unsigned j = 0; j < num_y; j++) {
		for(unsigned i = 0; i < num_x; i++) {
			Vector n = hf->normal(i, j);
			_out << "<" << n.x() << "," << n.y() << "," << n.z() << ">";
			if((i != num_x - 1) && (j != num_y - 1)) {
				_out << ", ";
			}
			if((i+1) % 2 == 0) {
				_out << "\n";
			}
		}
	}
	_out << "}\n";					// normal vertices
	_out << "face_indices {\n";
	_out << (num_x - 1) * (num_y - 1) * 2 << ",\n";
	for(unsigned j = 0; j < num_y - 1; j++) {
		for(unsigned i = 0; i < num_x - 1; i++) {
			// upper face
			_out << "<" << j * num_x + i
				<< "," << (j+1) * num_x + i + 1
				<< "," << (j+1) * num_x + i
				<< ">, ";
			// lower face
			_out << "<" << j* num_x + i
				<< "," << j * num_x + i + 1
				<< "," << (j+1) * num_x + i + 1
				<< ">";
			if((i != num_x - 2) && (j != num_y - 2)) {
				_out << ", ";
			}
			if((i+1) % 2 == 0) {
				_out << "\n";
			}
		}
	}
	_out << "}\n";					// face indices
}

namespace Orbis {

WorldViewPOV::WorldViewPOV(World* world, const std::string& filename)
	: WorldView(world), _file_name(filename)
{
}

void WorldViewPOV::render()
{
	Orbis::Util::Camera cam = camera();

	// trying to open out stream
	std::ofstream out(_file_name.c_str());
	if(!out.is_open()) {
		return;
	}

	// writing comment
	out << "/*\n";
	out << " * This POVRay file was created by the Orbis engine.\n";
	out << " */\n\n";

	out << "#include \"colors.inc\"\n";
	out << "#include \"skies.inc\"\n\n";

	out << "background {\n";
	out << "\tcolour rgb <0.0, 0.0, 0.4>\n";
	out << "}\n\n";

	out << "sky_sphere {\n";
	out << "\tS_Cloud5\n";
	out << "\trotate 90 * x\n";
	out <<"}\n\n";

	out << "light_source {\n";
	out << "\t<0.0, 0.0, 100.0>\n";
	out << "\tcolour rgb <1.0, 1.0, 1.0>\n";
	out << "}\n\n";
 
	Point from = cam.location();
	Point to = from + 1.0 * cam.view();
	out << "camera {\n";
	out << "\tlocation <" << from.x() << ","
					<< from.y() << ","
					<< from.z() << ">\n";
	out << "\tsky <" << cam.up().x() << ","
					<< cam.up().y() << ","
					<< cam.up().z() << ">\n";
	out << "\tup <0, 0, 1>\n";
	out << "\tright <-1.33, 0.0, 0.0>\n";
	out << "\tlook_at <" << to.x() << ","
					<< to.y() << ","
					<< to.z() << ">\n";
	out << "}\n\n";

	// traversing the graph rendering the nodes
	osg::ref_ptr<POVWriteNodeVisitor> visitor =
						new POVWriteNodeVisitor(out);
	world()->root()->accept(*visitor);
}

void WorldViewPOV::worldHasChanged()
{
}

} // namespace declarations

