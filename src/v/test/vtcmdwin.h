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

#ifndef VTCMDWIN_H
#define VTCMDWIN_H

#include <stdio.h>
#include <stdlib.h>
#include <v/vapp.h>
#include <v/vcmdwin.h>
#include <v/vmodald.h>
#include <v/vynreply.h>
#include <v/vdebug.h>
#include <v/vnotice.h>
#include <v/vpane.h>
#include <v/vstatusp.h>
#include <v/vcmdpane.h>
#include <v/vfont.h>
#include <v/vreply.h>
#include <v/vfilesel.h>
#include <v/vtimer.h>
#include <v/vprintdc.h>
#include <v/vmemdc.h>

#include "vtcanvas.h"		// The test canvas
#include "vtdialog.h"		// The test dialog
#include "vttogdlg.h"		// The toggle dialog


    class testCmdWindow;

    class myTimer : public vTimer
      {
      public:		//---------------------------------------- public
	myTimer(testCmdWindow* cw) { cmdw = cw; }
	~myTimer() {}
	virtual void TimerTick();
      private:		//--------------------------------------- private
	testCmdWindow* cmdw;
      };


    class testCmdWindow : public vCmdWindow
      {
      public:		//---------------------------------------- public
	testCmdWindow(VCONST char*, int, int); // Constructor with size
	virtual ~testCmdWindow();		// Destructor
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
        void SetShownTop(int shown, int top);
        virtual void CloseWin();

	virtual void WorkSlice() { ++_workCounter; }

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	long _workCounter;
	vMenuPane* myMenu;		// For the menu bar
	testTextCanvasPane* myCanvas;	// For the canvas
	vStatusPane* myStatus;		// For the status bar
	vCommandPane* myCmdPane;	// for the command pane
	vCommandPane* myCmdPane2;	// for the command pane
        vCommandPane* myCmdPane3;	// for the command pane

	vMemoryDC* myMemDC;

 	void PrintTest(int scaled = 0);
	int GetCenter( vPrintDC& pdc, char* str, int scaled);

#define MAX_WINDOWS 4
	int nextCmdWin;

 	testCmdWindow* CmdWins[MAX_WINDOWS];
	
	int nextWin;
 	vWindow* Wins[MAX_WINDOWS];

	testDialog* curDialog;	// so we construct dialog only once
	toggleDialog* curTogDialog;
	vDialog* curListDialog;

	vPen _pen;
	myTimer* _timer;		// a timer
      };
#endif
