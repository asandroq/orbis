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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <world.hpp>
#include <textwindow.hpp>
#include <viewwindow.hpp>
#include <mainwindow.hpp>

#include <gridterrain.hpp>

/* XPM */
static const FXchar *filenew_data[] = {
"24 24 33 1",
" 	c None",
".	c #070707",
"+	c #555555",
"@	c #323232",
"#	c #050505",
"$	c #0E0E0E",
"%	c #151515",
"&	c #FFFFFF",
"*	c #959595",
"=	c #131313",
"-	c #2E2E2E",
";	c #0B0B0B",
">	c #212121",
",	c #585858",
"'	c #080808",
")	c #2F2F2F",
"!	c #818181",
"~	c #040404",
"{	c #414141",
"]	c #B3B3B3",
"^	c #808080",
"/	c #535353",
"(	c #EBEBEB",
"_	c #282828",
":	c #1F1F1F",
"<	c #161616",
"[	c #0F0F0F",
"}	c #010101",
"|	c #E4E4E4",
"1	c #BFBFBF",
"2	c #202020",
"3	c #2B2B2B",
"4	c #404040",
"                        ",
"                        ",
"     .+++++++@#$        ",
"     %&&&&&&&*=-;       ",
"     %&&&&&&&*>,-'      ",
"     %&&&&&&&*)!,-~     ",
"     %&&&&&&&*{]^,-~    ",
"     %&&&&&&&*/(]^,-'   ",
"     %&&&&&&&*$_:<['}   ",
"     %&&&&&&&|1111112   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     %&&&&&&&&&&&&&&3   ",
"     #44444444444444;   ",
"                        ",
"                        "};

MainWindow::MainWindow(FXApp* app)
	: FXMainWindow(app, PACKAGE_NAME, 0, 0, DECOR_ALL, 0, 0, 800, 600),
		Orbis::Script::FoxActionAdapter(this),
				_console(0), _filenew_icon(0), _menu_bar(0),
					_world_menu(0), _window_menu(0), _drag_shell(0)
{
	// icons
	_filenew_icon = new FXXPMIcon(getApp(), filenew_data);

	// creating menu bar
//	_drag_shell = new FXToolBarShell(this, FRAME_RAISED);
	_menu_bar = new FXMenuBar(this, //_drag_shell,
				LAYOUT_SIDE_TOP | LAYOUT_FILL_X | FRAME_RAISED);
//	new FXToolBarGrip(_menu_bar, _menu_bar,
//				FXMenuBar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE);

	// creating layout manager
	FXHorizontalFrame *hframe =
		new FXHorizontalFrame(this, FRAME_SUNKEN | LAYOUT_SIDE_TOP |
						LAYOUT_FILL_X | LAYOUT_FILL_Y,
							0, 0, 0, 0, 0, 0, 0, 0);

	// creating and setting up main view area
	_mdi_client = new FXMDIClient(hframe, LAYOUT_FILL_X | LAYOUT_FILL_Y);

	// creating status bar
	_status_bar = new FXStatusBar(this,
		LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER);
	_status_bar->getStatusLine()->setTarget(this);
	_status_bar->getStatusLine()->setSelector(MainWindow::ID_STATUSBAR);

	// Make MDI Menu
	_mdi_menu = new FXMDIMenu(this, _mdi_client);

	// MDI buttons in menu - note the message ID's!!!!!
	// Normally, MDI commands are simply sensitized or desensitized;
	// Under the menubar, however, they're hidden if the MDI Client is
	// not maximized.  To do this, they must have different ID's.
	new FXMDIWindowButton(_menu_bar, _mdi_menu, _mdi_client,
				FXMDIClient::ID_MDI_MENUWINDOW, LAYOUT_LEFT);
	new FXMDIDeleteButton(_menu_bar, _mdi_client,
				FXMDIClient::ID_MDI_MENUCLOSE,
					FRAME_RAISED | LAYOUT_RIGHT);
	new FXMDIRestoreButton(_menu_bar, _mdi_client,
				FXMDIClient::ID_MDI_MENURESTORE,
					FRAME_RAISED | LAYOUT_RIGHT);
	new FXMDIMinimizeButton(_menu_bar, _mdi_client,
				FXMDIClient::ID_MDI_MENUMINIMIZE,
					FRAME_RAISED | LAYOUT_RIGHT);

	// creating the World menu
	FXMenuPane* _world_menu = new FXMenuPane(this);
	new FXMenuCommand(_world_menu,
				"&New\tCtrl-N\tNew world",
						_filenew_icon, this, ID_NEW);
	new FXMenuCommand(_world_menu,
				"&Import\tCtrl-I\tImports from a raster format",
						NULL, this, ID_IMPORT);
	new FXMenuSeparator(_world_menu);
	new FXMenuCommand(_world_menu,
				"&Start\tCtrl-S\tStarts the world",
						NULL, this, ID_START);
	new FXMenuCommand(_world_menu,
				"&Stop\tCtrl-T\tStops the world",
						NULL, this, ID_STOP);
	new FXMenuSeparator(_world_menu);
	new FXMenuCommand(_world_menu,
				"&Quit\tCtrl-Q\tQuit the application",
							NULL, this, ID_QUIT);
	new FXMenuTitle(_menu_bar, "&World", 0, _world_menu);

	// Window menu
	FXMenuPane* _window_menu = new FXMenuPane(this);
	new FXMenuCommand(_window_menu,
				"New Vie&w Window", NULL, this, ID_NEWVIEW);
	new FXMenuCommand(_window_menu,
				"New Te&xt Window", NULL, this, ID_NEWTEXT);
	new FXMenuCommand(_window_menu,
				"View Console", NULL, this, ID_VIEWCONSOLE);
	new FXMenuSeparator(_window_menu);
	new FXMenuCommand(_window_menu,
				"Tile &Horizontally", NULL, _mdi_client,
					FXMDIClient::ID_MDI_TILEHORIZONTAL);
	new FXMenuCommand(_window_menu,
				"Tile &Vertically", NULL, _mdi_client,
					FXMDIClient::ID_MDI_TILEVERTICAL);
	new FXMenuCommand(_window_menu,
				"C&ascade", NULL, _mdi_client,
					FXMDIClient::ID_MDI_CASCADE);
	new FXMenuCommand(_window_menu,
				"&Close", NULL, _mdi_client,
					FXMDIClient::ID_MDI_CLOSE);
//	new FXMenuCommand(_window_menu,
//				"Close All", NULL, _mdi_client,
//					FXMDIClient::ID_CLOSE_ALL_DOCUMENTS);
	FXMenuSeparator* sep = new FXMenuSeparator(_window_menu);
	sep->setTarget(_mdi_client);
	sep->setSelector(FXMDIClient::ID_MDI_ANY);
	new FXMenuCommand(_window_menu, NULL, NULL, _mdi_client,
						FXMDIClient::ID_MDI_1);
	new FXMenuCommand(_window_menu, NULL, NULL, _mdi_client,
						FXMDIClient::ID_MDI_2);
	new FXMenuCommand(_window_menu, NULL, NULL, _mdi_client,
						FXMDIClient::ID_MDI_3);
	new FXMenuCommand(_window_menu, NULL, NULL, _mdi_client,
						FXMDIClient::ID_MDI_4);
	new FXMenuTitle(_menu_bar, "&Window", NULL, _window_menu);

	// so we can receive continuous updates
	_chore = getApp()->addChore(this, ID_CHORE);

	// Lua error messages go to FoxActionAdapter
	Orbis::World::instance()->setLuaActionAdapter(this);

	// we are ready
	_status_bar->getStatusLine()->setText("Ready.");
}

MainWindow::~MainWindow()
{
	delete _world_menu;
	delete _window_menu;
	delete _filenew_icon;

	Orbis::World::shutdown();
}

void MainWindow::create()
{
	FXMainWindow::create();

	// show everything
	show();
}

void MainWindow::processEvents()
{
	FoxActionAdapter::processEvents();

	lock();

	if(!_messages.empty()) {
		if(_mdi_client->indexOfChild(_console) == -1) {
			_console = new ConsoleWindow(_mdi_client, "Console", 0,
										_mdi_menu, 0, 0, 0, 640, 480);

			_mdi_client->setActiveChild(_console);
			_console->create();
		}
		while(!_messages.empty()) {
			_console->addText(_messages.front());
			_messages.pop();
		}
	}

	unlock();
}

long MainWindow::onCmdNew(FXObject* obj, FXSelector sel, void*)
{
	ViewWindow *view_window = 0;

	// resetting world
	Orbis::World* world = Orbis::World::instance();
	world->reset();

	// if there is no view windows, we create one
	// to show us the brand new world
	if(_mdi_client->numChildren() == 0) {
		view_window = new ViewWindow(_mdi_client, "View 1", 0,
						_mdi_menu, 0, 0, 0, 640, 480);
		_mdi_client->setActiveChild(view_window);
		view_window->create();
	}

	// hack, take me out here!
	if(view_window) {
		view_window->reset();
	}

	return 1;
}

long MainWindow::onCmdImport(FXObject* obj, FXSelector sel, void*)
{
	const char patterns[] = "All files (*)";

	FXString filename =
		FXFileDialog::getOpenFilename(this,
			PACKAGE_NAME, FXFile::getHomeDirectory(), patterns);

	osg::ref_ptr<Orbis::Drawable::GridTerrain>
			terrain = new Orbis::Drawable::GridTerrain( filename.text());

	Orbis::World::instance()->addDrawable(terrain.get(), 0, "terrain");

	return 1;
}

long MainWindow::onCmdStart(FXObject* obj, FXSelector sel, void*)
{
	Orbis::World::instance()->start();

	return 1;
}

long MainWindow::onCmdStop(FXObject* obj, FXSelector sel, void*)
{
	Orbis::World::instance()->stop();

	return 1;
}

long MainWindow::onCmdQuit(FXObject* obj, FXSelector sel, void*)
{
	_chore = getApp()->removeChore(_chore);

	// simply exit by now
	getApp()->stop();

	return 1;
}

long MainWindow::onCmdNewView(FXObject* obj, FXSelector sel, void*)
{
	char buffer[10];
	// TODO: checks world
	ViewWindow *view_window = 0;

	sprintf(buffer, "View %d", _mdi_client->numChildren() + 1);

	view_window =
		new ViewWindow(_mdi_client,
				buffer, 0, _mdi_menu, 0, 0, 0, 640, 480);

	_mdi_client->setActiveChild(view_window);
	view_window->create();
	view_window->reset();

	return 1;
}

long MainWindow::onCmdNewText(FXObject* obj, FXSelector sel, void*)
{
	TextWindow *text_window = 0;

	text_window = new TextWindow(_mdi_client, "untitled", 0,
								_mdi_menu, 0, 0, 0, 640, 480);

	_mdi_client->setActiveChild(text_window);
	text_window->create();

	return 1;
}

long MainWindow::onCmdViewConsole(FXObject* obj, FXSelector sel, void*)
{
	if(_mdi_client->indexOfChild(_console) != -1) {
		_mdi_client->setActiveChild(_console);
	} else {
		_console = new ConsoleWindow(_mdi_client, "Console", 0,
										_mdi_menu, 0, 0, 0, 640, 480);

		_mdi_client->setActiveChild(_console);
		_console->create();
	}

	return 1;
}

long MainWindow::onChore(FXObject* obj, FXSelector sel, void*)
{
	processEvents();

	// restoring chore
	_chore = getApp()->addChore(this, ID_CHORE);

	return 1;
}

// Message Map
FXDEFMAP(MainWindow) MainWindowMap[]={
  //________Message_Type_________ID_____________________Message_Handler_____
  FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_NEW,        MainWindow::onCmdNew),
  FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_IMPORT,     MainWindow::onCmdImport),
  FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_START,      MainWindow::onCmdStart),
  FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_STOP,       MainWindow::onCmdStop),
  FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_QUIT,       MainWindow::onCmdQuit),
  FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_NEWVIEW,    MainWindow::onCmdNewView),
  FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_NEWTEXT,    MainWindow::onCmdNewText),
  FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_VIEWCONSOLE,    MainWindow::onCmdViewConsole),
  FXMAPFUNC(SEL_CHORE,     MainWindow::ID_CHORE,      MainWindow::onChore)
};

FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))
