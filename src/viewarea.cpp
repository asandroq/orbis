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
 * $Id: viewarea.cpp,v 1.18 2004/01/16 21:15:53 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <osg/Timer>
#include <osgGA/GUIEventAdapter>

#include <world.hpp>
#include <viewarea.hpp>

// static timer that will give app's time 0
static osg::Timer _timer;
static osg::Timer_t _initial_time = _timer.tick();

// adapts events from Fox to OSG
class EventAdapter : public osgGA::GUIEventAdapter {
public:
	EventAdapter(MouseYOrientation myo = Y_INCREASING_DOWNWARDS)
		: GUIEventAdapter(myo)
	{
		_event_type = NONE;
		_key = _button = -1;
		_x = _x_min = _x_max = -1.0f;
		_y = _y_min = _y_max = -1.0f;
		_button_mask = _key_mask = 0;
		_time = _timer.delta_s(_initial_time, _timer.tick());
	}

	/*
	 * methods from GUIEventAdapter
	 */

	EventType getEventType() const
	{
		return _event_type;
	}

	int getKey() const
	{
		return _key;
	}

	int getButton() const
	{
		return _button;
	}

	float getXmin() const
	{
		return _x_min;
	}

	float getXmax() const
	{
		return _x_max;
	}

	float getYmin() const
	{
		return _y_min;
	}

	float getYmax() const
	{
		return _y_max;
	}

	float getX() const
	{
		return _x;
	}

	float getY() const
	{
		return _y;
	}

	unsigned getButtonMask() const
	{
		return _button_mask;
	}

	unsigned getModKeyMask() const
	{
		return _key_mask;
	}

	double time() const
	{
		return _time;
	}

	/*
	 * methods to adapt events
	 */

	void resize(int x1, int y1, int x2, int y2)
	{
		_x_min = x1;
		_x_max = x2;
		_y_min = y1;
		_y_max = y2;
	}

	void adaptFrame(int x, int y)
	{
		_x = x;
		_y = y;
		_event_type = FRAME;
	}

	void adaptResize(int x1, int y1, int x2, int y2)
	{
		resize(x1, y1, x2, y2);
		_event_type = RESIZE;
	}

	void adaptMouseMotion(FXEvent *e)
	{
		_event_type = MOVE;
		if(e->state & LEFTBUTTONMASK) {
			_event_type = DRAG;
			_button_mask |= LEFT_MOUSE_BUTTON;
		}
		if(e->state & MIDDLEBUTTONMASK) {
			_event_type = DRAG;
			_button_mask |= MIDDLE_MOUSE_BUTTON;
		}
		if(e->state & RIGHTBUTTONMASK) {
			_event_type = DRAG;
			_button_mask |= RIGHT_MOUSE_BUTTON;
		}

		adaptMouseDetails(e);
	}

	void adaptLeftBtnPress(FXEvent *e)
	{
		_event_type = PUSH;
		_button_mask |= LEFT_MOUSE_BUTTON;

		adaptMouseDetails(e);
	}

	void adaptLeftBtnRelease(FXEvent *e)
	{
		_event_type = RELEASE;
		_button_mask &= ~LEFT_MOUSE_BUTTON;

		adaptMouseDetails(e);
	}

	void adaptMiddleBtnPress(FXEvent *e)
	{
		_event_type = PUSH;
		_button_mask |= MIDDLE_MOUSE_BUTTON;

		adaptMouseDetails(e);
	}

	void adaptMiddleBtnRelease(FXEvent *e)
	{
		_event_type = RELEASE;
		_button_mask &= ~MIDDLE_MOUSE_BUTTON;

		adaptMouseDetails(e);
	}

	void adaptRightBtnPress(FXEvent *e)
	{
		_event_type = PUSH;
		_button_mask |= RIGHT_MOUSE_BUTTON;

		adaptMouseDetails(e);
	}

	void adaptRightBtnRelease(FXEvent *e)
	{
		_event_type = RELEASE;
		_button_mask &= ~RIGHT_MOUSE_BUTTON;

		adaptMouseDetails(e);
	}

	void adaptKeyPress(FXEvent *e)
	{
		_key = e->code;
		_event_type = KEYDOWN;
	}

	void adaptKeyRelease(FXEvent *e)
	{
		_key = e->code;
		_event_type = KEYUP;
	}

	void adaptMouseDetails(FXEvent *e)
	{
		_x = e->win_x;
		_y = e->win_y;
		_button = e->click_button;

		if(e->state & SHIFTMASK) {
			_key_mask |= MODKEY_SHIFT;
		}
		if(e->state & CONTROLMASK) {
			_key_mask |= MODKEY_CTRL;
		}
		if(e->state & ALTMASK) {
			_key_mask |= MODKEY_ALT;
		}
	}

private:
	double _time;
	int _key, _button;
	EventType _event_type;
	float _x, _x_min, _x_max;
	float _y, _y_min, _y_max;
	unsigned _button_mask, _key_mask;
};

ViewArea* ViewArea::_share_group = 0;

ViewArea::ViewArea(FXComposite* parent, FXGLVisual* vis, FXGLCanvas* share,
				FXObject *tgt, FXSelector sel, FXuint opts)
	: FXGLCanvas(parent, vis, _share_group, tgt, sel, opts),
					_state(ID_SELECT), _chore(0)
{
	if(_share_group == 0) {
		_share_group = this;
	}

	_chore = getApp()->addChore(this, ID_CHORE);
}

ViewArea::~ViewArea()
{
}

void ViewArea::create()
{
	FXGLCanvas::create();

	// now that the server-side resources for this window were created
	_viewer = new Orbis::WorldView3D(Orbis::World::instance(), this);
}

long ViewArea::onCmdSelect(FXObject *sender, FXSelector sel, void *data)
{
	_state = ID_SELECT;

	return 1;
}

long ViewArea::onUpdSelect(FXObject *sender, FXSelector sel, void *data)
{
	FXuint msg = _state == ID_SELECT ? ID_CHECK : ID_UNCHECK;

	sender->handle(this, MKUINT(ID_SHOW,   SEL_COMMAND), NULL);
	sender->handle(this, MKUINT(ID_ENABLE, SEL_COMMAND), NULL);
	sender->handle(this, MKUINT(msg,       SEL_COMMAND), NULL);

	return 1;
}

long ViewArea::onCmdRotate(FXObject *sender, FXSelector sel, void *data)
{
	_state = ID_ROTATE;

	return 1;
}

long ViewArea::onUpdRotate(FXObject *sender, FXSelector sel, void *data)
{
	FXuint msg = _state == ID_ROTATE ? ID_CHECK : ID_UNCHECK;

	sender->handle(this, MKUINT(ID_SHOW,   SEL_COMMAND), NULL);
	sender->handle(this, MKUINT(ID_ENABLE, SEL_COMMAND), NULL);
	sender->handle(this, MKUINT(msg,       SEL_COMMAND), NULL);

	return 1;
}

long ViewArea::onCmdMove(FXObject *sender, FXSelector sel, void *data)
{
	_state = ID_MOVE;

	return 1;
}

long ViewArea::onUpdMove(FXObject *sender, FXSelector sel, void *data)
{
	FXuint msg = _state == ID_MOVE ? ID_CHECK : ID_UNCHECK;

	sender->handle(this, MKUINT(ID_SHOW,   SEL_COMMAND), NULL);
	sender->handle(this, MKUINT(ID_ENABLE, SEL_COMMAND), NULL);
	sender->handle(this, MKUINT(msg,       SEL_COMMAND), NULL);

	return 1;
}

long ViewArea::onCmdZoom(FXObject *sender, FXSelector sel, void *data)
{
	_state = ID_ZOOM;

	return 1;
}

long ViewArea::onUpdZoom(FXObject *sender, FXSelector sel, void *data)
{
	FXuint msg = _state == ID_ZOOM ? ID_CHECK : ID_UNCHECK;

	sender->handle(this, MKUINT(ID_SHOW,   SEL_COMMAND), NULL);
	sender->handle(this, MKUINT(ID_ENABLE, SEL_COMMAND), NULL);
	sender->handle(this, MKUINT(msg,       SEL_COMMAND), NULL);

	return 1;
}

long ViewArea::onUpdStatus(FXObject* obj, FXSelector sel, void*)
{
	return 1;
}

long ViewArea::onKeyPress(FXObject *sender, FXSelector sel, void *data)
{
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptKeyPress(e);
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);

	return 1;
}

long ViewArea::onKeyRelease(FXObject *sender, FXSelector sel, void *data)
{
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptKeyRelease(e);
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);

	return 1;
}

long ViewArea::onLeftBtnPress(FXObject *sender, FXSelector sel, void *data)
{
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	// I'm gonna fool OSG
	switch(_state) {
		case ID_SELECT:
			// pass for now
			break;
		case ID_ROTATE:
			ea->adaptLeftBtnPress(e);
			break;
		case ID_MOVE:
			ea->adaptMiddleBtnPress(e);
			break;
		case ID_ZOOM:
			ea->adaptRightBtnPress(e);
			break;
	}
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);

	return 1;
}

long ViewArea::onLeftBtnRelease(FXObject *sender, FXSelector sel, void *data)
{
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptLeftBtnRelease(e);
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);

	return 1;
}

long ViewArea::onMiddleBtnPress(FXObject *sender, FXSelector sel, void *data)
{
/*
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptMiddleBtnPress(e);
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);
*/

	return 1;
}

long ViewArea::onMiddleBtnRelease(FXObject *sender, FXSelector sel, void *data)
{
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptMiddleBtnRelease(e);
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);

	return 1;
}

long ViewArea::onRightBtnPress(FXObject *sender, FXSelector sel, void *data)
{
/*
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptRightBtnPress(e);
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);
*/

	return 1;
}

long ViewArea::onRightBtnRelease(FXObject *sender, FXSelector sel, void *data)
{
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptRightBtnRelease(e);
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);

	return 1;
}

long ViewArea::onMotion(FXObject *sender, FXSelector sel, void *data)
{
	FXEvent *e = (FXEvent*) data;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	// I'm gonna fool OSG
	if(e->state & LEFTBUTTONMASK) {
		switch(_state) {
			case ID_SELECT:
				e->state = 0;
				break;
			case ID_ROTATE:
				e->state = LEFTBUTTONMASK;
				break;
			case ID_MOVE:
				e->state = MIDDLEBUTTONMASK;
				break;
			case ID_ZOOM:
				e->state = RIGHTBUTTONMASK;
				break;
		}
	} else {
		e->state = 0;
	}

	ea->adaptMouseMotion(e);
	ea->resize(0, 0, getWidth(), getHeight());

	_viewer->handle(*ea, *this);

	return 1;
}

long ViewArea::onPaint(FXObject *sender, FXSelector sel, void *data)
{
	if(makeCurrent()) {
		_viewer->render();
		swapBuffers();
		makeNonCurrent();
	}

	return 1;
}

long ViewArea::onConfigure(FXObject *sender, FXSelector sel, void *data)
{
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptResize(0, 0, getWidth(), getHeight());

	if(makeCurrent()) {
		_viewer->handle(*ea, *this);
		_viewer->render();
		swapBuffers();
		makeNonCurrent();
	}

	return 1;
}

long ViewArea::onChore(FXObject *sender, FXSelector sel, void *data)
{
	int x, y;
	unsigned buttons;
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	getCursorPosition(x, y, buttons);
	
	ea->adaptFrame(x, y);
	if(makeCurrent()) {
		_viewer->handle(*ea, *this);
		_viewer->render();
		swapBuffers();
		makeNonCurrent();
	}

	// restoring chore
	_chore = getApp()->addChore(this, ID_CHORE);

	return 1;
}

void ViewArea::requestRedraw()
{
	if(makeCurrent()) {
		_viewer->render();
		swapBuffers();
		makeNonCurrent();
	}
}

void ViewArea::requestWarpPointer(float x, float y)
{
}

void ViewArea::requestContinuousUpdate(bool needed)
{
/*
	if(needed) {
		if(_chore == 0) {
			_chore = getApp()->addChore(this, ID_CHORE);
		}
	} else {
		if(_chore) {
			getApp()->removeChore(_chore);
			_chore = 0;
		}
	}
*/
}

void ViewArea::reset()
{
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;

	ea->adaptResize(0, 0, getWidth(), getHeight());

	if(makeCurrent()) {
		// resetting camera
		_viewer->reset(*ea, *this);

		// draw a frame
		_viewer->render();
		swapBuffers();
		makeNonCurrent();
	}
}

FXDEFMAP(ViewArea) ViewAreaMap[] = {
	FXMAPFUNC(SEL_COMMAND,	ViewArea::ID_SELECT,	ViewArea::onCmdSelect),
	FXMAPFUNC(SEL_UPDATE,	ViewArea::ID_SELECT,	ViewArea::onUpdSelect),
	FXMAPFUNC(SEL_COMMAND,	ViewArea::ID_ROTATE,	ViewArea::onCmdRotate),
	FXMAPFUNC(SEL_UPDATE,	ViewArea::ID_ROTATE,	ViewArea::onUpdRotate),
	FXMAPFUNC(SEL_COMMAND,	ViewArea::ID_MOVE,	ViewArea::onCmdMove),
	FXMAPFUNC(SEL_UPDATE,	ViewArea::ID_MOVE,	ViewArea::onUpdMove),
	FXMAPFUNC(SEL_COMMAND,	ViewArea::ID_ZOOM,	ViewArea::onCmdZoom),
	FXMAPFUNC(SEL_UPDATE,	ViewArea::ID_ZOOM,	ViewArea::onUpdZoom),
	FXMAPFUNC(SEL_UPDATE,	ViewArea::ID_STATUS,	ViewArea::onUpdStatus),
	FXMAPFUNC(SEL_KEYPRESS,		0,		ViewArea::onKeyPress),
	FXMAPFUNC(SEL_KEYRELEASE,	0,		ViewArea::onKeyRelease),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,	0,		ViewArea::onLeftBtnPress),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,		ViewArea::onLeftBtnRelease),
	FXMAPFUNC(SEL_MIDDLEBUTTONPRESS,0,		ViewArea::onMiddleBtnPress),
	FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE,0,		ViewArea::onMiddleBtnRelease),
	FXMAPFUNC(SEL_RIGHTBUTTONPRESS,	0,		ViewArea::onRightBtnPress),
	FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,0,		ViewArea::onRightBtnRelease),
	FXMAPFUNC(SEL_MOTION,		0,		ViewArea::onMotion),
	FXMAPFUNC(SEL_PAINT,		0,		ViewArea::onPaint),
	FXMAPFUNC(SEL_CONFIGURE,	0,		ViewArea::onConfigure),
	FXMAPFUNC(SEL_CHORE,	ViewArea::ID_CHORE,	ViewArea::onChore)
};

FXIMPLEMENT(ViewArea, FXGLCanvas, ViewAreaMap, ARRAYNUMBER(ViewAreaMap))

// Done.
