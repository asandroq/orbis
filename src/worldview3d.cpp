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
 * $Id: worldview3d.cpp,v 1.17 2004/05/12 14:25:03 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <osg/Geode>
#include <osgGA/TrackballManipulator>

#include <world.hpp>
#include <worldview3d.hpp>

namespace Orbis {

	namespace Drawable {

void DrawSelected::drawImplementation(osg::State& state) const
{
	// anything to draw?
	if(!_selected) {
		return;
	}

	// the box I'm gonna draw
	osg::BoundingBox bbox = _selected->getBound();
	// giving some slack
	float spcx = (bbox.xMax() - bbox.xMin()) * 0.05;
	float spcy = (bbox.yMax() - bbox.yMin()) * 0.05;
	float spcz = (bbox.zMax() - bbox.zMin()) * 0.05;
	float x1 = bbox.xMin() - spcx;
	float y1 = bbox.yMin() - spcy;
	float z1 = bbox.zMin() - spcz;
	float x2 = bbox.xMax() + spcx;
	float y2 = bbox.yMax() + spcy;
	float z2 = bbox.zMax() + spcz;

	// drawing
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(x1, y1, z1);
		glVertex3f(x1, y2, z1);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y1, z2);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y1, z2);
	}
	glEnd();
	glBegin(GL_LINES);
	{
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
	}
	glEnd();
}

}

WorldView3D::WorldView3D(World* world, osgGA::GUIActionAdapter *aa)
	: WorldView(world), _action_adapter(aa)
{
	// creating scene decorator
	_selected = new Drawable::DrawSelected;
	osg::Group *group = new Group;
	group->addChild(world->root().get());
	osg::Geode *geode = new osg::Geode;
	geode->addDrawable(_selected.get());
	group->addChild(geode);

	// creates a new scene handler with a scene view
	_scene_view = new osgUtil::SceneView;

	// setup defaults, scene data
	_scene_view->setDefaults();
	_scene_view->setSceneData(world->root().get());

	// uses a trackball manipulator
	_matrix_manipulator = new osgGA::TrackballManipulator;
	_matrix_manipulator->setNode(world->root().get());
}

WorldView3D::~WorldView3D()
{
}

void WorldView3D::render()
{
	// updating viewing parameters
	_scene_view->setViewMatrix(_matrix_manipulator->getInverseMatrix());

	_scene_view->setFusionDistance(
				_matrix_manipulator->getFusionDistanceMode(),
				_matrix_manipulator->getFusionDistanceValue());

	_scene_view->update();
	_scene_view->cull();
	_scene_view->draw();
}

void WorldView3D::worldHasChanged()
{
//	reset();

	_action_adapter->requestRedraw();
}

void WorldView3D::reset(osgGA::GUIEventAdapter& ea,
						osgGA::GUIActionAdapter& aa)
{
	if(ea.getEventType() == osgGA::GUIEventAdapter::RESIZE) {
		float width, height;
		osg::Matrix projection;

		width  = ea.getXmax() - ea.getXmin();
		height = ea.getYmax() - ea.getYmin();
		projection.makePerspective(50.0f, width/height, 1.0f, 1000.0f);

		_scene_view->setProjectionMatrix(projection);
		_scene_view->setViewport(0, 0,
						static_cast<int>(width),
						static_cast<int>(height));
	}

	_matrix_manipulator->init(ea, aa);
	_matrix_manipulator->home(ea, aa);
}

bool WorldView3D::handle(osgGA::GUIEventAdapter& ea,
						osgGA::GUIActionAdapter& aa)
{
	if(ea.getEventType() == osgGA::GUIEventAdapter::RESIZE) {
		float width, height;
		osg::Matrix projection;

		width  = ea.getXmax() - ea.getXmin();
		height = ea.getYmax() - ea.getYmin();
		projection.makePerspective(50.0f, width/height, 1.0f, 1000.0f);

		_scene_view->setProjectionMatrix(projection);
		_scene_view->setViewport(0, 0,
					static_cast<int>(width),
					static_cast<int>(height));
	}

	return _matrix_manipulator->handle(ea, aa);
}

void WorldView3D::setCamera(const Orbis::Util::Camera& cam)
{
	using osg::Vec3;
	using Orbis::Util::Point;
	using Orbis::Util::Vector;

	WorldView::setCamera(cam);

	Point loc = cam.location();
	Point ct = cam.location() + cam.view();
	Vector up = cam.up();
	Vec3 eye(loc.x(), loc.y(), loc.z());
	Vec3 center(ct.x(), ct.y(), ct.z());
	Vec3 upv(up.x(), up.y(), up.z());
	_matrix_manipulator->setByInverseMatrix(
					osg::Matrixd::lookAt(eye, center, upv));
}

} // namespace declarations

