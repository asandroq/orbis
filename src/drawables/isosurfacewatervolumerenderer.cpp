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

#include <osg/BlendFunc>

#include <isosurfacewatervolumerenderer.hpp>

// provisory
#include <fosterwatervolume.hpp>

namespace Orbis {

namespace Drawable {

IsoSurfaceWaterVolumeRenderer::IsoSurfaceWaterVolumeRenderer(const WaterVolume* const wv, double threshold)
	: MarchingCubesWaterVolumeRenderer(wv, threshold)
{
	// activating blending in this drawable
	osg::StateSet *stateSet = getOrCreateStateSet();
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	osg::BlendFunc *bf = new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	stateSet->setAttribute(bf);
}

void IsoSurfaceWaterVolumeRenderer::drawImplementation(osg::State& state) const
{
	if(waterVolume() == 0) {
		return;
	}

	Point p;
	
	glBegin(GL_LINES);
		// x axis
		glColor4f(1.0, 0.0, 0.0, 1.0);
		p = waterVolume()->point(0, 0, 0);
		glVertex3d(p.x(), p.y(), p.z());
		p = waterVolume()->point(waterVolume()->sizeX()-1, 0, 0);
		glVertex3d(p.x(), p.y(), p.z());
		// y axis
		glColor4f(0.0, 1.0, 0.0, 1.0);
		p = waterVolume()->point(0, 0, 0);
		glVertex3d(p.x(), p.y(), p.z());
		p = waterVolume()->point(0, waterVolume()->sizeY()-1, 0);
		glVertex3d(p.x(), p.y(), p.z());
		// z axis
		glColor4f(0.0, 0.0, 1.0, 1.0);
		p = waterVolume()->point(0, 0, 0);
		glVertex3d(p.x(), p.y(), p.z());
		p = waterVolume()->point(0, 0, waterVolume()->sizeZ()-1);
		glVertex3d(p.x(), p.y(), p.z());
	glEnd();

	Locker lock(waterVolume());

	// showing velocities
	glBegin(GL_LINES);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	for(unsigned i = 0; i < waterVolume()->sizeX(); i++) {
		for(unsigned j = 0; j < waterVolume()->sizeY(); j++) {
			for(unsigned k = 0; k < waterVolume()->sizeZ(); k++) {
				Vector v = waterVolume()->velocity(i, j, k);
				Point p1 = waterVolume()->point(i, j, k);
				Point p2 = p1 + v;
				glVertex3d(p1.x(), p1.y(), p1.z());
				glVertex3d(p2.x(), p2.y(), p2.z());
			}
		}
	}
	glEnd();

	// provisory
	const FosterWaterVolume *fwv = dynamic_cast<const FosterWaterVolume*>(waterVolume());

	// showing densities
	glBegin(GL_TRIANGLES);
	for(unsigned i = 0; i < waterVolume()->sizeX(); i++) {
		for(unsigned j = 0; j < waterVolume()->sizeY(); j++) {
			for(unsigned k = 0; k < waterVolume()->sizeZ(); k++) {
				//double d = waterVolume()->density(i, j, k);
				//glColor4d(0.0, 0.3, 0.7, d/threshold());
				switch(fwv->status(i, j, k)) {
					case FosterWaterVolume::EMPTY:
						glColor4d(0.0, 0.0, 0.0, 0.0);
						break;
					case FosterWaterVolume::SOLID:
						glColor4d(0.5, 0.0, 0.0, 0.0);
						break;
					case FosterWaterVolume::FULL:
						glColor4d(0.0, 0.0, 0.5, 0.3);
						break;
					case FosterWaterVolume::SURFACE:
						glColor4d(0.0, 0.5, 0.0, 0.3);
						break;
				}
				Point p1 = waterVolume()->point(i, j, k);
				Point p2 = waterVolume()->point(i+1, j+1, k+1);
				Point p3 = waterVolume()->point(i+1, j, k);
				Vector n = ((p2 - p1) ^ (p3 - p1)).normalise();
				glNormal3d(n.x(), n.y(), n.z());
				glVertex3d(p1.x(), p1.y(), p1.z());
				glVertex3d(p2.x(), p2.y(), p2.z());
				glVertex3d(p3.x(), p3.y(), p3.z());
			}
		}
	}
	glEnd();
}

} } // namespace declarations
