//=======================================================================
//  vtcmdwin.h - testCmdWindow class header
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

#ifndef VTCMDWIN2_H
#define VTCMDWIN2_H

#include <stdlib.h>
#include <v/vpane.h>
#include <v/vstatusp.h>
#include <v/vcmdpane.h>
#include <v/vcmdwin.h>
#include <v/vcanvas.h>


    class testCmdWindow2 : public vCmdWindow
      {
      public:		//---------------------------------------- public
	testCmdWindow2(char*, int, int); // Constructor with size
	virtual ~testCmdWindow2();		// Destructor
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	vCanvasPane* myCanvas;	// For the canvas
	vStatusPane* myStatus;		// For the status bar
	vCommandPane* myCmdPane;	// for the command pane
	vMenuPane* myMenu;
      };
#endif
