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
#  include <config.h>
#endif

#include <iostream>

#include <mainwindow.hpp>

MainWindow::MainWindow()
	: _view_area(0)
{
	// setting up
	set_title("Orbis");
	set_default_size(600, 480);
	add(_vbox);

	// creating actions
	_actions = Gtk::ActionGroup::create();
	_actions->add(Gtk::Action::create("FileMenu", "_File"));
	_actions->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW),
			sigc::mem_fun(*this, &MainWindow::onFileNew));
	_actions->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
			sigc::mem_fun(*this, &MainWindow::onFileOpen));
	_actions->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
			sigc::mem_fun(*this, &MainWindow::onFileQuit));


	Glib::ustring ui_info =
		"<ui>"
		"	<menubar name='MenuBar'>"
		"		<menu action='FileMenu'>"
		"			<menuitem action='FileNew'/>"
		"			<menuitem action='FileOpen'/>"
		"			<separator/>"
		"			<menuitem action='FileQuit'/>"
		"		</menu>"
		"	</menubar>"
		"	<toolbar name='ToolBar'>"
		"		<toolitem action='FileNew'/>"
		"		<toolitem action='FileOpen'/>"
		"	</toolbar>"
		"</ui>";

	try {
		// ui manager
		_manager = Gtk::UIManager::create();
		_manager->insert_action_group(_actions);
		_manager->add_ui_from_string(ui_info);

		add_accel_group(_manager->get_accel_group());

		Gtk::Widget* menu_bar = _manager->get_widget("/MenuBar");
		if(menu_bar) {
			_vbox.pack_start(*menu_bar, Gtk::PACK_SHRINK);
		}
		Gtk::Widget* tool_bar = _manager->get_widget("/ToolBar");
		if(tool_bar) {
			_vbox.pack_start(*tool_bar, Gtk::PACK_SHRINK);
		}

		// setting up view area
		_view_area = new ViewArea;
		_vbox.pack_start(*_view_area);

		// setting up status bar
		_status.push("Ready.");
		_vbox.pack_start(_status, Gtk::PACK_SHRINK);

		show_all_children();
	} catch(std::exception& e) {
		Glib::ustring error_msg = "Application initialisation error: ";
		error_msg += e.what();

		// tell user of error
		Gtk::MessageDialog diag(*this, error_msg, false, Gtk::MESSAGE_ERROR);
		diag.run();

		// user was warned, handling deferred
		throw e;
	}
}

MainWindow::~MainWindow()
{
	delete _view_area;
}

void MainWindow::onFileNew()
{
	std::cout << "File new.\n";
}

void MainWindow::onFileOpen()
{
	std::cout << "File open.\n";
}

void MainWindow::onFileQuit()
{
	hide();
}

