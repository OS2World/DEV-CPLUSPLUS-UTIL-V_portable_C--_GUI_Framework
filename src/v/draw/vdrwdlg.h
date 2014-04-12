//=======================================================================
//  vdrwdlg.h - Header file for vDrawDialog class
//  Copyright (C) 1995,1996  Bruce E. Wampler
//
//  This program is part of the V C++ GUI Framework example programs.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  (see COPYING) along with this program; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=======================================================================

#ifndef VDRWDLG_H
#define VDRWDLG_H
#include <v/vdialog.h>

    class vApp;
    class vWindow;
    class vDrawDialog : public vDialog
      {
      public:		//---------------------------------------- public
	vDrawDialog(vBaseWindow* bw, int All = 1);
	vDrawDialog(vApp* aw);

	virtual ~vDrawDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	void AddDefaultCmds();		// to add the defined commands

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	void init();	// internal inintialization

	vWindow* pWin;		// our parent

	int setWhatColor;		// active thing we are setting color of
	int penColorMsg;		// message id of current color
	int brushColorMsg;		// for pen, brush, and bg
	int bgColorMsg;
	int penColorVal;		// value id of current color
	int brushColorVal;		// for pen, brush, and bg
	int bgColorVal;
	int drawWhatShape;		// active shape to draw
	int toAll;			// to all or only parent window
	ItemVal penStyleMsg;
	ItemVal brushStyleMsg;

      };
#endif
