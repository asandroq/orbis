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

#ifndef __ORBIS_VIEWAREA_HPP__
#define __ORBIS_VIEWAREA_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <fox/fx.h>
#include <fox/fx3d.h>

#include <osgGA/GUIActionAdapter>

#include <worldview3d.hpp>

/*!
 * \brief This class represents the drawing surface.
 */
class ViewArea : public FXGLCanvas, public osgGA::GUIActionAdapter
{
	FXDECLARE(ViewArea)
public:
	enum {
		ID_CHORE = FXGLCanvas::ID_LAST,
		ID_MOVE,
		ID_ROTATE,
		ID_SELECT,
		ID_STATUS,
		ID_ZOOM,
		ID_LAST
	};

	//! Constructor
	ViewArea(FXComposite* parent, FXGLVisual* vis, FXGLCanvas* share = 0,
			FXObject *tgt = 0, FXSelector sel = 0, FXuint opts = 0);
	//! Destructor
	virtual ~ViewArea();

	//! Called when the server-side resources of this window were created
	virtual void create();

	/*!
	 * \brief Queries current frame rate.
	 * \return The frame rate.
	 */
	double frameRate() const;

	//! Puts in selection mode
	long onCmdSelect(FXObject *sender, FXSelector sel, void *data);

	//! Updates selection widgets
	long onUpdSelect(FXObject *sender, FXSelector sel, void *data);

	//! Puts in rotation mode
	long onCmdRotate(FXObject *sender, FXSelector sel, void *data);

	//! Updates rotation widgets
	long onUpdRotate(FXObject *sender, FXSelector sel, void *data);

	//! Puts in movement mode
	long onCmdMove(FXObject *sender, FXSelector sel, void *data);

	//! Updates movement widgets
	long onUpdMove(FXObject *sender, FXSelector sel, void *data);

	//! Puts in zoom mode
	long onCmdZoom(FXObject *sender, FXSelector sel, void *data);

	//! Updates zoom widgets
	long onUpdZoom(FXObject *sender, FXSelector sel, void *data);

	//! Updates the current status
	long onUpdStatus(FXObject *sender, FXSelector sel, void *data);

	//! Called when a key is pressed
	long onKeyPress(FXObject *sender, FXSelector sel, void *data);

	//! Called when a key is released
	long onKeyRelease(FXObject *sender, FXSelector sel, void *data);

	//! Called when the left mouse button is pressed
	long onLeftBtnPress(FXObject *sender, FXSelector sel, void *data);

	//! Called when the left mouse button is released
	long onLeftBtnRelease(FXObject *sender, FXSelector sel, void *data);

	//! Called when the middle mouse button is pressed
	long onMiddleBtnPress(FXObject *sender, FXSelector sel, void *data);

	//! Called when the middle mouse button is released
	long onMiddleBtnRelease(FXObject *sender, FXSelector sel, void *data);

	//! Called when the right mouse button is pressed
	long onRightBtnPress(FXObject *sender, FXSelector sel, void *data);

	//! Called when the right mouse button is released
	long onRightBtnRelease(FXObject *sender, FXSelector sel, void *data);

	//! Called when the mouse moves over the terrain
	long onMotion(FXObject *sender, FXSelector sel, void *data);

	//! Called whenever the window must be repainted
	long onPaint(FXObject *sender, FXSelector sel, void *data);

	//! Called when the window is resized
	long onConfigure(FXObject *sender, FXSelector sel, void *data);

	//! Called as an idle callback
	long onChore(FXObject *sender, FXSelector sel, void *data);

	//! Called when OSG needs to redraw the window
	void requestRedraw();

	//! Called when OSG needs to position the mouse pointer
	void requestWarpPointer(float, float);

	//! Called when OSG needs the window to continuously redraw itself
	void requestContinuousUpdate(bool = true);

	//! Resets to a default view
	void reset();

protected:
	//! Default constructor
	ViewArea() {}

private:
	// current state
	unsigned _state;
	// current frame rate
	double _frame_rate;
	// timer
	FXChore *_chore;
	// 3D viewer
	osg::ref_ptr<Orbis::WorldView3D> _viewer;
	// freaking hack
	static ViewArea *_share_group;
};

inline double ViewArea::frameRate() const
{
	return _frame_rate;
}

#endif // __ORBIS_VIEWAREA_HPP__
