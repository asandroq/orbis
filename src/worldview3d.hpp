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
 * $Id: worldview3d.hpp,v 1.11 2004/03/26 20:39:26 asandro Exp $
 */

#ifndef __ORBIS_WORLDVIEW3D_HPP__
#define __ORBIS_WORLDVIEW3D_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <osgUtil/SceneView>
#include <osgGA/GUIActionAdapter>
#include <osgGA/MatrixManipulator>

#include <worldview.hpp>

using osg::ref_ptr;
using osgUtil::SceneView;

namespace Orbis {

	namespace Drawable {

/*!
 * \brief This class draws the bounding box of the selected drawable.
 */
class DrawSelected : public Drawable {
public:
	//! Default constructor
	DrawSelected();

	//! Copy constructor
	DrawSelected(const DrawSelected& src,
			const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

	//! Returns an object of the same type
	virtual osg::Object* cloneType() const;

	//! Returns a clone of this object
	virtual osg::Object* clone(const osg::CopyOp& copyop) const;

	//! The currently selected drawable
	const Drawable* selected() const;

	//! Sets the currently selected drawable
	void setSelected(Drawable* sel);

	//! Draws the bounding box of selected drawable
	virtual void drawImplementation(osg::State& state) const;

protected:
	//! Destructor
	virtual ~DrawSelected();

private:
	// currently selected drawable
	Drawable *_selected;
};

inline DrawSelected::DrawSelected()
	: Drawable(), _selected(0)
{
	// the selected object may change anytime
	setSupportsDisplayList(false);
}

inline DrawSelected::DrawSelected(const DrawSelected& src,
						const osg::CopyOp& copyOp)
	: Drawable(src, copyOp), _selected(src._selected)
{
	// the selected object may change anytime
	setSupportsDisplayList(false);
}

inline DrawSelected::~DrawSelected()
{
}

inline osg::Object* DrawSelected::cloneType() const
{
	return new DrawSelected();
}

inline osg::Object* DrawSelected::clone(const osg::CopyOp& copyop) const
{
	return new DrawSelected(*this, copyop);
}

inline const Drawable* DrawSelected::selected() const
{
	return _selected;
}

inline void DrawSelected::setSelected(Drawable* sel)
{
	_selected = sel;
}

}

/*!
 * \brief This class represents a tri-dimensional view of the World.
 */
class WorldView3D : public WorldView, public osg::Referenced {
public:
	//! Default constructor
	/*!
	 * \param world the World instance to be rendered
	 */
	WorldView3D(World* world, osgGA::GUIActionAdapter* action_adapter);

	//! Destructor
	~WorldView3D();

	//! Renders the World in 3D
	virtual void render();

	//! Notifies this class that the World has changed somehow.
	virtual void worldHasChanged();

	//! Resets the camera to a default position
	void reset(osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	//! Handle user events
	bool handle(osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	/*!
	 * \brief Sets the new Camera.
	 * \param cam The new Camera.
	 */
	void setCamera(const Orbis::Util::Camera& cam);

private:
	//! OSG's Scene Handler
	ref_ptr<SceneView> _scene_view;
	//! The GUI action adapter (GUI window)
	osgGA::GUIActionAdapter *_action_adapter;
	// indicates which drawable is selected
	ref_ptr<Drawable::DrawSelected> _selected;
	// trackball manipulator
	ref_ptr<osgGA::MatrixManipulator> _matrix_manipulator;
};

} // namespace declarations

#endif  // __ORBIS_WORLDVIEW3D_HPP__

