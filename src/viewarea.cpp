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

#ifdef __GNUG__
#pragma implementation
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdexcept>

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
/*
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
*/
private:
	double _time;
	int _key, _button;
	EventType _event_type;
	float _x, _x_min, _x_max;
	float _y, _y_min, _y_max;
	unsigned _button_mask, _key_mask;
};

ViewArea::ViewArea()
{
	// configure OpenGL capable visual
	Glib::RefPtr<Gdk::GL::Config> glconfig;

	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
								Gdk::GL::MODE_DEPTH |
								Gdk::GL::MODE_DOUBLE);

	// if double-buffered mode fails, try single-buffered
	if(!glconfig) {
		glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
									Gdk::GL::MODE_DEPTH);
	}

	// if there is no visual, there is nothing more that can be done
	if(!glconfig) {
		throw std::runtime_error("No OpenGL visual found.");
	}

	set_gl_capability(glconfig);

	// window-system events
	add_events(Gdk::BUTTON1_MOTION_MASK	|
			Gdk::BUTTON1_MOTION_MASK		|
			Gdk::BUTTON_PRESS_MASK		|
			Gdk::VISIBILITY_NOTIFY_MASK);

	_viewer = new Orbis::WorldView3D(Orbis::World::instance(), this);
}

ViewArea::~ViewArea()
{
}

void ViewArea::requestRedraw()
{
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

	if(gldrawable->gl_begin(get_gl_context())) {
		_viewer->render();
		gldrawable->swap_buffers();
		gldrawable->gl_end();
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
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

	ea->adaptResize(0, 0, get_width(), get_height());

	if(gldrawable->gl_begin(get_gl_context())) {
		// resetting camera
		_viewer->reset(*ea, *this);

		// draw a frame
//		_viewer->render();
		gldrawable->swap_buffers();
		gldrawable->gl_end();
	}
}

void ViewArea::on_realize()
{
	Gtk::GL::DrawingArea::on_realize();

	reset();
}

bool ViewArea::on_configure_event(GdkEventConfigure* event)
{
	osg::ref_ptr<EventAdapter> ea = new EventAdapter;
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

	ea->adaptResize(0, 0, get_width(), get_height());

	if(gldrawable->gl_begin(get_gl_context())) {
		_viewer->handle(*ea, *this);
		_viewer->render();
		gldrawable->swap_buffers();
		gldrawable->gl_end();
	}

	return true;
}

bool ViewArea::on_expose_event(GdkEventExpose* event)
{
	requestRedraw();

	return true;
}

// Done.

