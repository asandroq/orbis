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
 * $Id: textwindow.hpp,v 1.6 2004/02/26 19:29:45 asandro Exp $
 */

#ifndef __TEXTWINDOW_HPP__
#define __TEXTWINDOW_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <config.h>

#include <fox/fx.h>

#ifdef HAVE_LIBFXSCINTILLA
class FXScintilla;
#endif

/*!
 * \brief Scripting editing and running window.
 */
class TextWindow : public FXMDIChild {
	FXDECLARE(TextWindow)
public:
	enum {
		ID_NEW = FXMDIChild::ID_LAST,
		ID_OPEN,
		ID_SAVE,
		ID_RUN,
		ID_LAST
	};

	//! Constructor
	TextWindow(FXMDIClient *p,
			const FXString& name,
			FXIcon* ic = 0,
			FXPopup *pup = 0,
			FXuint opts = 0,
			FXint x = 0,
			FXint y = 0,
			FXint w = 0,
			FXint h = 0);

	//! Destructor
	virtual ~TextWindow();

	//! Called when the server-side resources of this window are created
	virtual void create();

	//! Starts a new text
	long onCmdNew(FXObject* obj, FXSelector sel, void*);

	//! Loads text from file
	long onCmdOpen(FXObject* obj, FXSelector sel, void*);

	//! Saves text to file
	long onCmdSave(FXObject* obj, FXSelector sel, void*);

	//! Runs script in text file
	long onCmdRun(FXObject* obj, FXSelector sel, void*);

protected:
	//! Default constructor
	TextWindow() {}

private:
	//! Current file name.
	FXString _file_name;
	//! Text area.
#ifdef HAVE_LIBFXSCINTILLA
	FXScintilla *_text_area;
#else
	FXText *_text_area;
#endif
};

#endif // __TEXTWINDOW_HPP__

