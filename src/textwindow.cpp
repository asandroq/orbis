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
 * $Id: textwindow.cpp,v 1.6 2004/01/06 20:14:42 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <fstream>

#include <world.hpp>
#include <textwindow.hpp>

#ifdef HAVE_LIBFXSCINTILLA
#include <fxscintilla/Scintilla.h>
#include <fxscintilla/SciLexer.h>
#include <fxscintilla/FXScintilla.h>
#endif

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

static const FXchar *open_xpm[] = {
/* columns rows colors chars-per-pixel */
"22 22 8 1",
"  c Gray100",
". c Yellow",
"X c #848200",
"o c Gray0",
"O c None",
"+ c Gray0",
"@ c Gray0",
"# c Gray0",
/* pixels */
"OOOOOOOOOOOOOOOOOOOOOO",
"OOOOOOOOOOOOOOOOOOOOOO",
"OOOOOOOOOOOOOOOOOOOOOO",
"OOOOOOOOOOOOooooOOOOoO",
"OOOOOOOOOOOoOOOOooOooO",
"OOOOOOOOOOOOOOOOOOoooO",
"OOOOOOOOOOOOOOOOOooooO",
"OooooOOOOOOOOOOOoooooO",
"o. . ooooooooooOOOOOOO",
"o . . . . . . oOOOOOOO",
"o. . . . . . .oOOOOOOO",
"o . . . . . . oOOOOOOO",
"o. . . ooooooooooooooo",
"o . . ooXXXXXXXXXXXXoo",
"o. . ooXXXXXXXXXXXXooO",
"o . ooXXXXXXXXXXXXooOO",
"o. ooXXXXXXXXXXXXooOOO",
"o ooXXXXXXXXXXXXooOOOO",
"oooXXXXXXXXXXXXooOOOOO",
"ooXXXXXXXXXXXXooOOOOOO",
"oooooooooooooooOOOOOOO",
"OOOOOOOOOOOOOOOOOOOOOO"
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

static const FXchar *run_xpm[] = {
/* columns rows colors chars-per-pixel */
"22 22 2 1",
" 	c None",
".	c #8080DA",
"            ......    ",
"          ..........  ",
"         ............ ",
"         ............ ",
"          ........... ",
"           ........   ",
"          ....        ",
"         .....        ",
"        .....         ",
"       ......         ",
"        .......       ",
"        .......       ",
"       ......         ",
"       ...            ",
"      .....           ",
"      ......          ",
"     ...  ...         ",
"............          ",
"........              ",
".   ..                ",
".  .....              ",
"                      "};

static const char lua_keywords[] =
	"and break do else elseif "
	"end false for function if "
	"in local nil not or "
	"repeat return then true until while";

TextWindow::TextWindow(FXMDIClient *p, const FXString& name,
					FXIcon* ic, FXPopup *pup, FXuint opts,
						FXint x, FXint y, FXint w, FXint h)
	: FXMDIChild(p, name, ic, pup, opts, x, y, w, h)
{
	_file_name = "";
	setTitle("untitled.lua");

	// creating toolbar icons
	FXIcon *new_icon = new FXXPMIcon(getApp(), new_xpm);
	FXIcon *open_icon = new FXXPMIcon(getApp(), open_xpm);
	FXIcon *save_icon = new FXXPMIcon(getApp(), save_xpm);
	FXIcon *run_icon = new FXXPMIcon(getApp(), run_xpm);

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
#ifdef HAVE_LIBFXSCINTILLA
	_text_area = new FXScintilla(vframe, 0, 0,
					LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
								LAYOUT_FILL_Y);
	_text_area->sendMessage(SCI_SETLEXER, SCLEX_LUA, 0);
	_text_area->sendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<long>(lua_keywords));
        _text_area->sendMessage(SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
	_text_area->sendMessage(SCI_STYLESETFONT, STYLE_DEFAULT, reinterpret_cast<long>("fixed"));
	_text_area->sendMessage(SCI_STYLECLEARALL, 0, 0);
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_DEFAULT, FXRGB(0x80, 0x80, 0x80));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_COMMENTLINE, FXRGB(0x00, 0x7f, 0x00));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_NUMBER, FXRGB(0x00, 0x7f, 0x7f));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_STRING, FXRGB(0x7f, 0x00, 0x7f));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_CHARACTER, FXRGB(0x7f, 0x00, 0x7f));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_WORD, FXRGB(0x00, 0x00, 0x7f));
	_text_area->sendMessage(SCI_STYLESETBOLD, SCE_P_WORD, 1);
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_TRIPLE, FXRGB(0x7f, 0x00, 0x00));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_TRIPLEDOUBLE, FXRGB(0x7f, 0x00, 0x00));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_CLASSNAME, FXRGB(0x00, 0x00, 0xff));
	_text_area->sendMessage(SCI_STYLESETBOLD, SCE_P_CLASSNAME, 1);
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_DEFNAME, FXRGB(0x00, 0x7f, 0x7f));
	_text_area->sendMessage(SCI_STYLESETBOLD, SCE_P_DEFNAME, 1);
	_text_area->sendMessage(SCI_STYLESETBOLD, SCE_P_OPERATOR, 1);
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_IDENTIFIER, FXRGB(0x7f, 0x7f, 0x7f));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_COMMENTBLOCK, FXRGB(0x7f, 0x7f, 0x7f));
	_text_area->sendMessage(SCI_STYLESETFORE, SCE_P_STRINGEOL, FXRGB(0x00, 0x00, 0x00));
	_text_area->sendMessage(SCI_STYLESETBACK, SCE_P_STRINGEOL, FXRGB(0xe0, 0xc0, 0xe0));
	_text_area->sendMessage(SCI_STYLESETEOLFILLED, SCE_P_STRINGEOL, 1);
	_text_area->sendMessage(SCI_STYLESETFORE, 34, FXRGB(0x00, 0x00, 0xff));
	_text_area->sendMessage(SCI_STYLESETBOLD, 34, 1);
	_text_area->sendMessage(SCI_STYLESETFORE, 35, FXRGB(0xff, 0x00, 0x00));
	_text_area->sendMessage(SCI_STYLESETBOLD, 35, 1);
#else
#endif

	// populating tool bar
	new FXButton(toolbar, "\tNew\tNew text",
			new_icon, this, ID_NEW,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP
								| LAYOUT_LEFT);
	new FXButton(toolbar, "\tOpen\tOpen a text file",
			open_icon, this, ID_OPEN,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP
								| LAYOUT_LEFT);
	new FXButton(toolbar, "\tSave\tSave the text to a file",
			save_icon, this, ID_SAVE,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP
								| LAYOUT_LEFT);
	new FXButton(toolbar, "\tRun\tRun the script",
			run_icon, this, ID_RUN,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP
								| LAYOUT_LEFT);
}

TextWindow::~TextWindow()
{
}

void TextWindow::create()
{
	FXMDIChild::create();

	show();
}

long TextWindow::onCmdNew(FXObject* obj, FXSelector sel, void*)
{
	_file_name = "";
	setTitle("untitled.lua");
	_text_area->sendMessage(SCI_CLEARALL, 0, 0);

	return 1;
}

long TextWindow::onCmdOpen(FXObject* obj, FXSelector sel, void*)
{
	const char patterns[] = "Lua scripts (*.lua)\n"
				"XML descriptions (*.xml)\n"
				"All files (*)";

	FXString filename =
		FXFileDialog::getOpenFilename(this,
			PACKAGE_NAME, FXFile::getHomeDirectory(), patterns);

	if(!filename.empty()) {
		char buffer[0x100];
		std::ifstream infile(filename.text());

		_file_name = filename;
		if(!infile.is_open()) {
			return 1;
		}
		FXString contents;
		while(infile) {
			infile.getline(buffer, 0x100);
			contents.append(buffer);
			contents.append('\n');
		}
		setTitle(_file_name.text());
		_text_area->sendMessage(SCI_SETTEXT, 0,
				reinterpret_cast<long>(contents.text()));
	}

	return 1;
}

long TextWindow::onCmdSave(FXObject* obj, FXSelector sel, void*)
{
	FXString filename;
	const char patterns[] = "Lua scripts (*.lua)\n"
				"XML descriptions (*.xml)\n"
				"All files (*)";

	if(_file_name.empty()) {
		filename = FXFileDialog::getSaveFilename(this, PACKAGE_NAME,
					FXFile::getHomeDirectory(), patterns);
	} else {
		filename = _file_name;
	}

	if(!filename.empty()) {
		int len;
		char buffer[0x100];
		std::ofstream outfile(filename.text());

		if(!outfile.is_open()) {
			return 1;
		}
		_file_name = filename;
		setTitle(_file_name.text());
		for(int i = 0;; i++) {
			len = _text_area->sendMessage(SCI_LINELENGTH, i, 0);
			if(len == 0) {
				break;
			}
			_text_area->sendMessage(SCI_GETLINE,
					i, reinterpret_cast<long>(buffer));
			buffer[len] = '\0';
			outfile << buffer;
		}
	}

	return 1;
}

long TextWindow::onCmdRun(FXObject* obj, FXSelector sel, void*)
{
	// gives a chance to save current buffer
	if(_file_name.empty()) {
		handle(this, FXSEL(SEL_COMMAND, ID_SAVE), NULL);
	}

	if(!_file_name.empty()) {
		   Orbis::World::instance()->runFile(_file_name.text());
	}

	return 1;
}

// Message Map
FXDEFMAP(TextWindow) TextWindowMap[] = {
  //________Message_Type_________ID_____________________Message_Handler_____
  FXMAPFUNC(SEL_COMMAND,   TextWindow::ID_NEW,     TextWindow::onCmdNew),
  FXMAPFUNC(SEL_COMMAND,   TextWindow::ID_OPEN,    TextWindow::onCmdOpen),
  FXMAPFUNC(SEL_COMMAND,   TextWindow::ID_SAVE,	   TextWindow::onCmdSave),
  FXMAPFUNC(SEL_COMMAND,   TextWindow::ID_RUN,	   TextWindow::onCmdRun)
};

FXIMPLEMENT(TextWindow, FXMDIChild, TextWindowMap, ARRAYNUMBER(TextWindowMap))

