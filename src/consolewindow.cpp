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

#include <fstream>

#include <consolewindow.hpp>

static const FXchar *new_xpm[] = {
/* columns rows colors chars-per-pixel */
"22 22 8 1",
"  c Gray100",
". c Gray76",
"X c Gray53",
"o c Gray36",
"O c Gray18",
"+ c Gray0",
"@ c None",
"# c Gray0",
/* pixels */
"@@@@@@@@@@@@@@@@@@@@@@",
"@@@@++++++++++@@@@@@@@",
"@@@@+       +O+@@@@@@@",
"@@@@+       +oO+@@@@@@",
"@@@@+       +XoO+@@@@@",
"@@@@+       +.XoO+@@@@",
"@@@@+       + .XoO+@@@",
"@@@@+       +++++++@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+             +@@@",
"@@@@+++++++++++++++@@@",
"@@@@@@@@@@@@@@@@@@@@@@",
"@@@@@@@@@@@@@@@@@@@@@@"
};

static const FXchar *save_xpm[] = {
/* columns rows colors chars-per-pixel */
"22 22 8 1",
"  c Gray100",
". c #cab5d1",
"X c #c1c1c1",
"o c #848200",
"O c Gray0",
"+ c None",
"@ c Gray0",
"# c Gray0",
/* pixels */
"++++++++++++++++++++++",
"+OOOOOOOOOOOOOOOOOOOO+",
"+OooOXXXXXXXXXXXXOXXO+",
"+OooOXXXXXXXXXXXXOXXO+",
"+OooOXXXXXXXXX.XXOOOO+",
"+OooOXXX..XXXXXXXOooO+",
"+OooOXXX..XXXXXXXOooO+",
"+OooOXXXXXXXXXXXXOooO+",
"+OooOXXXXXXXXXXXXOooO+",
"+OooOXXXXXXXXXXXXOooO+",
"+OooOXXXXXXXXXXXXOooO+",
"+OoooOOOOOOOOOOOOoooO+",
"+OooooooooooooooooooO+",
"+OooooooooooooooooooO+",
"+OoooOOOOOOOOOOOOOooO+",
"+OoooOOOOOOOOOXXXOooO+",
"+OoooOOOOOOOOOXXXOooO+",
"+OoooOOOOOOOOOXXXOooO+",
"+OoooOOOOOOOOOXXXOooO+",
"+OoooOOOOOOOOOXXXOooO+",
"++OOOOOOOOOOOOOOOOOO++",
"++++++++++++++++++++++"
};

std::string ConsoleWindow::_text = "";

ConsoleWindow::ConsoleWindow(FXMDIClient *p, const FXString& name,
						FXIcon* ic, FXPopup *pup, FXuint opts,
							FXint x, FXint y, FXint w, FXint h)
	: FXMDIChild(p, name, ic, pup, opts, x, y, w, h)
{
	setTitle("Console");

	// creating toolbar icons
	FXIcon *new_icon = new FXXPMIcon(getApp(), new_xpm);
	FXIcon *save_icon = new FXXPMIcon(getApp(), save_xpm);

	// creating layout manager
	FXVerticalFrame *vframe =
			new FXVerticalFrame(this,
				LAYOUT_FILL_X | LAYOUT_FILL_Y,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	// creating toolbar
	FXToolBarShell *dragshell = new FXToolBarShell(this, FRAME_RAISED);
	FXToolBar *toolbar = new FXToolBar(vframe, dragshell,
				LAYOUT_SIDE_TOP | LAYOUT_FILL_X | FRAME_RAISED,
							0,0,0,0, 4,4,4,4, 0,0);
	new FXToolBarGrip(toolbar, 0, //toolbar,
			FXToolBar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE);

	// creating text area
	_text_area = new FXText(vframe, 0, 0,
						LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	_text_area->setEditable(false);
	_text_area->setText(_text.c_str(), _text.length());

	// populating tool bar
	new FXButton(toolbar, "\tClear\tClear text",
			new_icon, this, ID_CLEAR,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP | LAYOUT_LEFT);
	new FXButton(toolbar, "\tSave\tSave the text to a file",
			save_icon, this, ID_SAVE,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP | LAYOUT_LEFT);
}

ConsoleWindow::~ConsoleWindow()
{
}

void ConsoleWindow::create()
{
	FXMDIChild::create();

	show();
}

void ConsoleWindow::addText(const std::string& text)
{
	_text += text;
	_text_area->appendText(text.c_str(), text.length());
}

long ConsoleWindow::onCmdClear(FXObject* obj, FXSelector sel, void*)
{
	_text.clear();
	_text_area->setText("\0", 1);

	return 1;
}

long ConsoleWindow::onCmdSave(FXObject* obj, FXSelector sel, void*)
{
	FXString filename;
	const char patterns[] = "Text file (*.txt)\n"
						"All files (*)";

	filename = FXFileDialog::getSaveFilename(this, PACKAGE_NAME,
							FXFile::getHomeDirectory(), patterns);

	if(!filename.empty()) {
		std::ofstream outfile(filename.text());

		if(!outfile.is_open()) {
			return 1;
		}
		outfile << _text;
	}

	return 1;
}

// Message Map
FXDEFMAP(ConsoleWindow) ConsoleWindowMap[] = {
  //________Message_Type_________ID_____________________Message_Handler_____
  FXMAPFUNC(SEL_COMMAND,   ConsoleWindow::ID_CLEAR,  ConsoleWindow::onCmdClear),
  FXMAPFUNC(SEL_COMMAND,   ConsoleWindow::ID_SAVE,   ConsoleWindow::onCmdSave)
};

FXIMPLEMENT(ConsoleWindow, FXMDIChild,
						ConsoleWindowMap, ARRAYNUMBER(ConsoleWindowMap))

