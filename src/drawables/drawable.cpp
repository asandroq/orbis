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
 * $Id: drawable.cpp,v 1.3 2004/04/29 14:27:21 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <osg/TexEnv>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include <drawable.hpp>

namespace Orbis {

namespace Drawable {

void Drawable::setTexture(const std::string& texture)
{
	_texture = texture;

	osg::Image *img = osgDB::readImageFile(_texture);
	// creating and setting texture
	if(img) {
		osg::StateSet *dstate = getOrCreateStateSet();
		osg::Texture2D *tex = new osg::Texture2D;
		tex->setImage(img);
		tex->setInternalFormatMode(osg::Texture2D::USE_ARB_COMPRESSION);
		dstate->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		dstate->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
		dstate->setTextureAttribute(0, new osg::TexEnv);
	}
}

} } // namespace declarations

