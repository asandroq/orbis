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

#ifdef __GNUG__
#pragma implementation
#endif

#include <cassert>

#include <osg/TexEnv>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include <math.hpp>
#include <gridterrain.hpp>
 
using Orbis::Math::sqr;
using Orbis::Math::clamp;
using Orbis::Math::interpolate;

namespace Orbis {

	namespace Drawable {

GridTerrain::UpdateCallback::UpdateCallback()
	: osg::Drawable::UpdateCallback(), _init(false)
{
}

void GridTerrain::UpdateCallback::dirtyLists()
{
	_init = false;
}

// I use this callback to do the vertex list creation just before drawing
void GridTerrain::UpdateCallback::update(osg::NodeVisitor* nv,
			 							osg::Drawable* drawable)
{
	using osg::TexEnv;
	using osg::StateSet;
	using osg::Texture2D;

	if(_init) {
		return;
	}
	_init = true;

	Orbis::Drawable::GridTerrain *hf =
		   dynamic_cast<Orbis::Drawable::GridTerrain*>(drawable);
	assert(hf);

	// creating geometry's vertex arrays
	osg::Vec2Array *tex = new osg::Vec2Array;
	osg::Vec3Array *verts = new osg::Vec3Array;
	osg::Vec3Array *norms = new osg::Vec3Array;
	osg::Vec3Array *colors = new osg::Vec3Array;
	colors->push_back(osg::Vec3(0.7, 0.7, 0.5));
	for(unsigned j = 0; j < hf->numSamplesY(); j++) {
		for(unsigned i = 0; i < hf->numSamplesX(); i++) {
			Point p = hf->point(i, j);
			Vector n = hf->normal(i, j);
			double s = static_cast<double>(i) / (hf->numSamplesX() - 1);
			double t = static_cast<double>(j) / (hf->numSamplesY() - 1);
			tex->push_back(osg::Vec2(s, t));
			verts->push_back(osg::Vec3(p.x(), p.y(), p.z()));
			norms->push_back(osg::Vec3(n.x(), n.y(), n.z()));
		}
	}
	hf->setVertexArray(verts);
	hf->setNormalArray(norms);
	hf->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	hf->setColorArray(colors);
	hf->setColorBinding(osg::Geometry::BIND_OVERALL);
	hf->setTexCoordArray(0, tex);

	// now specifying indices
	for(unsigned j = 0; j < hf->numSamplesY() - 1; j++) {
		osg::VectorUInt *indices = new osg::VectorUInt;
		for(unsigned i = 0; i < hf->numSamplesX(); i++) {
			indices->push_back((j+1) * hf->numSamplesX() + i);
			indices->push_back(j     * hf->numSamplesX() + i);
		}
		hf->addPrimitiveSet(new osg::DrawElementsUInt(
						    osg::PrimitiveSet::TRIANGLE_STRIP,
					    		indices->begin(), indices->end()));
	}

	// if the terrain doesn't have any patches, we're done
	if(hf->patches() == hf->patchesEnd()) {
		return;
	}

	// the magic of multitexturing
	unsigned u = 1;
	for(PatchListIterator it = hf->patches();
				 			it != hf->patchesEnd(); u++, it++) {
		// no texture, no deal
		osg::Image *img = osgDB::readImageFile(it->attribute("texture"));
		if(!img) {
			continue;
		}

		// gathering coordinates
		osg::Vec2Array *mtex = new osg::Vec2Array;
		for(unsigned j = 0; j < hf->numSamplesY(); j++) {
			for(unsigned i = 0; i < hf->numSamplesX(); i++) {
				Point p = hf->point(i, j);
				if(it->contains(p)) {
					double x = interpolate(p.x(), it->minX(), it->maxX());
					double y = interpolate(p.y(), it->minY(), it->maxY());
					mtex->push_back(osg::Vec2(x, y));
				} else {
					mtex->push_back(osg::Vec2(-1.0, -1.0));
				}
			}
		}

		// setting up new texture
		hf->setTexCoordArray(u, mtex);
		Texture2D *texture = new Texture2D;
		texture->setImage(img);
//		texture->setBorderWidth(1);
//		texture->setBorderColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
		texture->setInternalFormatMode(Texture2D::USE_ARB_COMPRESSION);
		texture->setWrap(Texture2D::WRAP_S, Texture2D::CLAMP_TO_BORDER);
		texture->setWrap(Texture2D::WRAP_T, Texture2D::CLAMP_TO_BORDER);
		osg::StateSet *ss = hf->getOrCreateStateSet();
		ss->setTextureAttributeAndModes(u, texture, osg::StateAttribute::ON);
		osg::TexEnv *tex_env = new osg::TexEnv;
		tex_env->setMode(osg::TexEnv::BLEND);
		tex_env->setColor(osg::Vec4(0.3, 0.3, 0.3, 0.3));
		ss->setTextureAttribute(u, tex_env);
	}
}

GridTerrain::GridTerrain()
	: Terrain(), GridHeightField()
{
	setUpdateCallback(new GridTerrain::UpdateCallback);
}

GridTerrain::GridTerrain(const GridTerrain& src,
					const osg::CopyOp& copyOp)
	: Terrain(src, copyOp), GridHeightField(src, copyOp)
{
	setUpdateCallback(new GridTerrain::UpdateCallback);
}

GridTerrain::GridTerrain(const Point& origin, double xstep, double ystep,
					unsigned xsize, unsigned ysize)
	: Terrain(), GridHeightField(origin, xstep, ystep, xsize, ysize)
{
	setUpdateCallback(new GridTerrain::UpdateCallback);
}

GridTerrain::GridTerrain(const std::string& filename,
								double xstep, double ystep)
:	Terrain(), GridHeightField(filename, xstep, ystep)
{
	setUpdateCallback(new GridTerrain::UpdateCallback);
}

void GridTerrain::faultLineGeneration(unsigned iters)
{
	srand(time(0));
	double d = sqrt(sqr(sizeX()) + sqr(sizeY()));
	for(unsigned i = 0; i < iters; i++) {
		double v = rand();
		double a = sin(v);
		double b = cos(v);
		double c = (rand() / (double)RAND_MAX) * d - d/2;
		for(unsigned j = 0; j < numSamplesX(); j++) {
			for(unsigned k = 0; k < numSamplesY(); k++) {
				Point p = point(j, k);
				if(a*p.x() + b*p.y() - c > 0.0) {
					setPoint(j, k, p.z()+0.7);
				} else {
					setPoint(j, k, p.z()-0.7);
				}
			}
		}
	}
}

void GridTerrain::smooth(double k)
{
	Point p, p1;
	unsigned i, j;

	k = clamp(k, 0.0, 1.0);
	for(i = 0; i < numSamplesY(); i++) {
		for(j = 1; j < numSamplesX(); j++) {
			p =  point(j, i);
			p1 = point(j-1, i);
			setPoint(j, i, p.z()*(1-k)+p1.z()*k);
		}
	}
	for(i = 1; i < numSamplesY(); i++) {
		for(j = 0; j < numSamplesX(); j++) {
			p =  point(j, i);
			p1 = point(j, i-1);
			setPoint(j, i, p.z()*(1-k)+p1.z()*k);
		}
	}
	for(i = 0; i < numSamplesY(); i++) {
		for(j = numSamplesX()-2; j > 0; j--) {
			p =  point(j, i);
			p1 = point(j+1, i);
			setPoint(j, i, p.z()*(1-k)+p1.z()*k);
		}
		p =  point(0, i);
		p1 = point(1, i);
		setPoint(0, i, p.z()*(1-k)+p1.z()*k);
	}
	for(i = numSamplesY()-2; i > 0; i--) {
		for(j = 0; j < numSamplesX(); j++) {
			p =  point(j, i);
			p1 = point(j, i+1);
			setPoint(j, i, p.z()*(1-k)+p1.z()*k);
		}
	}
	for(j = 0; j < numSamplesX(); j++) {
		p =  point(j, 0);
		p1 = point(j, 1);
		setPoint(j, 0, p.z()*(1-k)+p1.z()*k);
	}
}

} } // namespace declarations

