/*
 * The Orbis world simulator
 * Copyright (C) 2001-2005 Alex Sandro Queiroz e Silva
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

#ifndef __ORBIS_VIEWAREA_HPP__
#define __ORBIS_VIEWAREA_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <gtkglmm.h>

#include <osgGA/GUIActionAdapter>

#include <worldview3d.hpp>

/*!
 * \brief This class represents the drawing surface.
 */
class ViewArea : public Gtk::GL::DrawingArea, public osgGA::GUIActionAdapter
{
public:

	//! Constructor
	ViewArea();

	//! Destructor
	virtual ~ViewArea();

	/*!
	 * \brief Queries current frame rate.
	 * \return The frame rate.
	 */
	double frameRate() const;

	//! Called when OSG needs to redraw the window
	void requestRedraw();

	//! Called when OSG needs to position the mouse pointer
	void requestWarpPointer(float x, float y);

	//! Called when OSG needs the window to continuously redraw itself
	void requestContinuousUpdate(bool = true);

	//! Resets to a default view
	void reset();

protected:
	// window syste, events
	void on_realize();
	bool on_configure_event(GdkEventConfigure* event);
	bool on_expose_event(GdkEventExpose* event);

private:
	// current state
	unsigned _state;
	// current frame rate
	double _frame_rate;
	// 3D viewer
	osg::ref_ptr<Orbis::WorldView3D> _viewer;
};

inline double ViewArea::frameRate() const
{
	return _frame_rate;
}

#endif // __ORBIS_VIEWAREA_HPP__
