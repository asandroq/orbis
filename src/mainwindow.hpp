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

#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <gtkmm.h>

#include <viewarea.hpp>

/*!
 * \brief The application's main window.
 */
class MainWindow : public Gtk::Window {
public:
	//! Constructor
	MainWindow();

	//! Destructor
	~MainWindow();

	// action handlers
	void onFileNew();
	void onFileOpen();
	void onFileQuit();

private:
	// container for setting layout up
	Gtk::VBox _vbox;
	// main view area
	ViewArea *_view_area;
	// status bar
	Gtk::Statusbar _status;
	// ui manager
	Glib::RefPtr<Gtk::UIManager> _manager;
	// action group
	Glib::RefPtr<Gtk::ActionGroup> _actions;
};

#endif  // __MAINWINDOW_HPP__

