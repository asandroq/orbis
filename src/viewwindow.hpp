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
 * $Id: viewwindow.hpp,v 1.2 2004/02/26 19:29:45 asandro Exp $
 */

#ifndef __VIEWWINDOW_HPP__
#define __VIEWWINDOW_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <fox/fx.h>

class ViewArea;

/*!
 * \brief Window that contains and manipulates a view.
 */
class ViewWindow : public FXMDIChild {
	FXDECLARE(ViewWindow)
public:
	//! Constructor
	ViewWindow(FXMDIClient *p, const FXString& name,
			FXIcon* ic = 0, FXPopup *pup = 0,
				FXuint opts = 0, FXint x = 0,
					FXint y = 0, FXint w = 0, FXint h = 0);

	//! Destructor
	virtual ~ViewWindow();

	//! Called when the server-side resources of this window are created
	virtual void create();

	//! Resets the viewing parameters
	void reset();

protected:
	//! Default constructor
	ViewWindow() {}

private:
	// main viewing area
	ViewArea *_view_area;
};

#endif // __VIEWWINDOW_HPP_

