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

#ifndef __CONSOLEWINDOW_HPP__
#define __CONSOLEWINDOW_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <config.h>

#include <string>
#include <fox/fx.h>

/*!
 * \brief Console window for showing output messages.
 */
class ConsoleWindow : public FXMDIChild {
	FXDECLARE(ConsoleWindow)
public:
	enum {
		ID_CLEAR = FXMDIChild::ID_LAST,
		ID_SAVE,
		ID_LAST
	};

	//! Constructor
	ConsoleWindow(FXMDIClient *p,
			const FXString& name,
			FXIcon* ic = 0,
			FXPopup *pup = 0,
			FXuint opts = 0,
			FXint x = 0,
			FXint y = 0,
			FXint w = 0,
			FXint h = 0);

	//! Destructor
	virtual ~ConsoleWindow();

	//! Called when the server-side resources of this window are created.
	virtual void create();

	/*!
	 * \brief Adds text to the console.
	 * \param text The text to be added.
	 */
	void addText(const std::string& text);

	//! Clears the text in the window.
	long onCmdClear(FXObject* obj, FXSelector sel, void*);

	//! Saves text to file.
	long onCmdSave(FXObject* obj, FXSelector sel, void*);

protected:
	//! Default constructor.
	ConsoleWindow() {}

private:
	//! Text area.
	FXText *_text_area;
	//! Console text
	static std::string _text;
};

#endif // __CONSOLEWINDOW_HPP__

