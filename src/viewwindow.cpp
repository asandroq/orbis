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
 * $Id: viewwindow.cpp,v 1.2 2003/10/28 22:16:57 asandro Exp $
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <viewarea.hpp>
#include <viewwindow.hpp>

/* XPM */
static const FXchar *move_xpm[] = {
"25 25 7 1",
" 	c None",
".	c #000000",
"+	c #6B9063",
"@	c #F0FFEE",
"#	c #5B7950",
"$	c #ADC8AD",
"%	c #506B46",
"                         ",
"           .             ",
"          .+.            ",
"         .@+#.           ",
"        .@$+##.          ",
"       ....+....         ",
"          .+.            ",
"     .    .+.    .       ",
"    ..    .+.    ..      ",
"   .@.    .+.    .$.     ",
"  .@$......+......$$.    ",
" .+++++++++++++++++++.   ",
"  .##......+......#%.    ",
"   .#.    .+.    .%.     ",
"    ..    .+.    ..      ",
"     .    .+.    .       ",
"          .+.            ",
"       ....+....         ",
"        .$$+#%.          ",
"         .$+%.           ",
"          .+.            ",
"           .             ",
"                         ",
"                         ",
"                         "};

/* XPM */
static const FXchar *rotate_xpm[] = {
"24 24 11 1",
" 	c None",
".	c #000000",
"+	c #CBE4C7",
"@	c #6B9063",
"#	c #506B46",
"$	c #4F6B46",
"%	c #58774F",
"&	c #628359",
"*	c #6B8F63",
"=	c #8AAC84",
"-	c #AAC7A5",
"                        ",
"        .......         ",
"      ..+++++++..       ",
"     .++@@@@@@@@@.      ",
"    .+@@@###$%&@@@.     ",
"   .+@@##.....@@@@@.    ",
"  .+@@#..     ..@@@@.   ",
"  .+@#.         .@@@.   ",
" .+@@#.         .@*@#.  ",
" .+@#.           .=@#.  ",
" .+@#.           .-@#.  ",
" .+@#.           .+@#.  ",
" .+@#.           .+@#.  ",
" .+@#.           .+@#.  ",
" .+@@@.      .  .+@@#.  ",
"  .@@@.     ..  .+@#.   ",
"  .@@@#.   .+...+@@#.   ",
"   .@@#.  .+@+++@@#.    ",
"    .#.  .+@@@@@@#.     ",
"     .    .@@@@##.      ",
"           .@@#..       ",
"            .#.         ",
"             .          ",
"                        "};

/* XPM */
static const FXchar *scale_xpm[] = {
"24 24 79 1",
" 	c None",
".	c #000000",
"+	c #ECF4EB",
"@	c #C6DCC2",
"#	c #BBD5B7",
"$	c #B6CDB1",
"%	c #8D9F8B",
"&	c #8EA08B",
"*	c #B8D1B4",
"=	c #BBD6B7",
"-	c #AAC4A5",
";	c #627D59",
">	c #89A983",
",	c #71916A",
"'	c #51664B",
")	c #5E7758",
"!	c #779971",
"~	c #73956C",
"{	c #57734D",
"]	c #72916A",
"^	c #40573A",
"/	c #4F6948",
"(	c #63855B",
"_	c #54714A",
":	c #B5CDB1",
"<	c #4A6243",
"[	c #506C47",
"}	c #8D9F8A",
"|	c #384C31",
"1	c #A9AFA8",
"2	c #AFC0AC",
"3	c #AABEA7",
"4	c #A7BBA4",
"5	c #83957F",
"6	c #CEE1CB",
"7	c #C0D9BB",
"8	c #BCD6B7",
"9	c #B0CBAA",
"0	c #799771",
"a	c #3D5238",
"b	c #AFC1AC",
"c	c #89AA82",
"d	c #7B9E74",
"e	c #74966D",
"f	c #597750",
"g	c #42593A",
"h	c #6B9063",
"i	c #66895E",
"j	c #A6BBA3",
"k	c #AFCBAB",
"l	c #62845A",
"m	c #536F49",
"n	c #425839",
"o	c #82957F",
"p	c #789771",
"q	c #5A7750",
"r	c #4D6843",
"s	c #384A31",
"t	c #425A3A",
"u	c #415839",
"v	c #384B31",
"w	c #80927D",
"x	c #7D9078",
"y	c #6C8764",
"z	c #779970",
"A	c #959F92",
"B	c #7E9A77",
"C	c #779870",
"D	c #4A6343",
"E	c #7C9078",
"F	c #7D9B77",
"G	c #66885E",
"H	c #53704A",
"I	c #B8D1B3",
"J	c #68845F",
"K	c #54714B",
"L	c #6C8664",
"M	c #537049",
"N	c #506C46",
"                        ",
" ........     ........  ",
" .+@#$%.       .&*=-;.  ",
" .@>,'.         .)!~{.  ",
" .#]^.           ./(_.  ",
" .:'.             .<[.  ",
" .}.               .|.  ",
" ..      .....      ..  ",
" .      .12345.      .  ",
"       .167890a.        ",
"       .b7cdefg.        ",
"       .38dhi_g.        ",
"       .jkeilmn.        ",
"       .opq_mrs.        ",
" .      .atguv.      .  ",
" ..      .....      ..  ",
" .&.               .w.  ",
" .*).             .xy.  ",
" .8z/.           .AB{.  ",
" .#C(D.         .EFGH.  ",
" .IJK[|.       .wL{MN.  ",
" ........     ........  ",
"                        ",
"                        "};

/* XPM */
static const FXchar *select_xpm[] = {
"24 24 52 1",
" 	c None",
".	c #000000",
"+	c #93A590",
"@	c #A3B99E",
"#	c #4C5A47",
"$	c #AFC8A9",
"%	c #6E8867",
"&	c #344730",
"*	c #B3CDAD",
"=	c #7E9E78",
"-	c #567450",
";	c #31442D",
">	c #B4CFAE",
",	c #83A47B",
"'	c #688B60",
")	c #53724C",
"!	c #31442C",
"~	c #84A67C",
"{	c #6C9164",
"]	c #65885D",
"^	c #53714C",
"/	c #30432C",
"(	c #84A67D",
"_	c #6E9266",
":	c #698E61",
"<	c #61845A",
"[	c #4A6644",
"}	c #283725",
"|	c #B3CEAE",
"1	c #82A47C",
"2	c #688D60",
"3	c #5C7D55",
"4	c #3C5336",
"5	c #283924",
"6	c #1C291A",
"7	c #B0CAAA",
"8	c #7A9973",
"9	c #63855B",
"0	c #64875C",
"a	c #4F6B49",
"b	c #A0B69B",
"c	c #596D54",
"d	c #445C3F",
"e	c #567650",
"f	c #486343",
"g	c #697666",
"h	c #3A4E35",
"i	c #40573B",
"j	c #273524",
"k	c #222D1F",
"l	c #212D1F",
"m	c #222E1F",
"                        ",
"                        ",
"      .                 ",
"      ..                ",
"      .+.               ",
"      .@#.              ",
"      .$%&.             ",
"      .*=-;.            ",
"      .>,')!.           ",
"      .>~{]^/.          ",
"      .>(_:<[}.         ",
"      .|1{23456.        ",
"      .7890a...         ",
"      .bcdef..          ",
"      .g..hi.           ",
"      ... .j.           ",
"           .k.          ",
"           .l.          ",
"            .m.         ",
"            ...         ",
"                        ",
"                        ",
"                        ",
"                        "};

ViewWindow::ViewWindow(FXMDIClient *p, const FXString& name,
			FXIcon* ic, FXPopup *pup, FXuint opts, FXint x,
						FXint y, FXint w, FXint h)
	: FXMDIChild(p, name, ic, pup, opts, x, y, w, h),
		_select_icon(0), _rotate_icon(0), _move_icon(0), _zoom_icon(0)
{
	// creating toolbar icons
	_move_icon = new FXXPMIcon(getApp(), move_xpm);
	_zoom_icon = new FXXPMIcon(getApp(), scale_xpm);
	_rotate_icon = new FXXPMIcon(getApp(), rotate_xpm);
	_select_icon = new FXXPMIcon(getApp(), select_xpm);

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

	// creating viewing area
	FXGLVisual *visual = new FXGLVisual(getApp(), VISUAL_DOUBLEBUFFER);
	_view_area = new ViewArea(vframe, visual, 0, 0, 0,
					LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
								LAYOUT_FILL_Y);

	// populating tool bar
	new FXButton(toolbar, "\tSelect\tSelect object",
			_select_icon, _view_area, ViewArea::ID_SELECT,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP
								| LAYOUT_LEFT);
	new FXButton(toolbar, "\tRotate\tRotate the World",
			_rotate_icon, _view_area, ViewArea::ID_ROTATE,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP
								| LAYOUT_LEFT);
	new FXButton(toolbar, "\tMove\tMove the World",
			_move_icon, _view_area, ViewArea::ID_MOVE,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP
								| LAYOUT_LEFT);
	new FXButton(toolbar, "\tZoom\tZoom into the World",
			_zoom_icon, _view_area, ViewArea::ID_ZOOM,
				BUTTON_TOOLBAR | FRAME_RAISED | LAYOUT_TOP
								| LAYOUT_LEFT);

}

ViewWindow::~ViewWindow()
{
	delete _select_icon;
	delete _rotate_icon;
	delete _move_icon;
	delete _zoom_icon;
}

void ViewWindow::create()
{
	FXMDIChild::create();
	show();
}

void ViewWindow::reset()
{
	_view_area->reset();
}

FXIMPLEMENT(ViewWindow, FXMDIChild, 0, 0)
