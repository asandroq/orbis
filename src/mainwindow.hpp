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

#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <fox/fx.h>

#include <consolewindow.hpp>
#include <foxactionadapter.hpp>

/*!
 * \brief The application's main window.
 */
class MainWindow : public FXMainWindow, public Orbis::Script::FoxActionAdapter {
	FXDECLARE(MainWindow)
public:
	enum {
		ID_NEW  = FXMainWindow::ID_LAST,
		ID_IMPORT,
		ID_START,
		ID_STOP,
		ID_NEWVIEW,
		ID_NEWTEXT,
		ID_VIEWCONSOLE,
		ID_QUIT,
		ID_STATUSBAR,
		ID_CHORE,
		ID_LAST
	};

	//! Constructor
	MainWindow(FXApp *app = 0);

	//! Destructor
	~MainWindow();

	//! Creates this widget
	virtual void create();

	/*!
	 * \brief Process pending Lua events.
	 */
	void processEvents();

	// menu commands
	long onCmdNew(FXObject* obj, FXSelector sel, void*);
	long onCmdImport(FXObject* obj, FXSelector sel, void*);
	long onCmdStart(FXObject* obj, FXSelector sel, void*);
	long onCmdStop(FXObject* obj, FXSelector sel, void*);
	long onCmdQuit(FXObject* obj, FXSelector sel, void*);

	// window menu commands
	long onCmdNewView(FXObject* obj, FXSelector sel, void*);
	long onCmdNewText(FXObject* obj, FXSelector sel, void*);
	long onCmdViewConsole(FXObject* obj, FXSelector sel, void*);

	long onChore(FXObject* obj, FXSelector sel, void*);

private:
	ConsoleWindow *_console;
	FXIcon *_filenew_icon;
	FXChore *_chore;
	FXToolBar *_tool_bar;
	FXMenuBar *_menu_bar;
	FXMenuPane *_world_menu;
	FXMenuPane *_window_menu;
	FXMDIMenu *_mdi_menu;
	FXMDIClient *_mdi_client;
	FXStatusBar *_status_bar;
	FXToolBarShell *_drag_shell;
};

#endif  // __MAINWINDOW_HPP__

