//=======================================================================
//  vtcmdwin2.cxx - testCmdWindow class
//  Copyright (C) 1995,1996, 1997, 1998  Bruce E. Wampler
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

#include <v/vcanvas.h>  // for Cursors

#include "vtcw2.h"

    // Define a different menu and different status bars from
    // main cmdwindow to be sure things are updated correctly
    // for the MDI model on Windows, and that we really do
    // have independent menus and toolbars.

    static vMenu ShortFile[] =
      {
	{"Test Canvas", 200, isSens, notChk, noKeyLbl, noKey, noSub},
        {"E&xit", M_Exit, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };


    static CommandObject CommandBar[] =
      {
	{C_Button, 200,0,"Test Canvas", NoList,CA_None,isSens,NoFrame, 0, 0},
        {C_Button, M_Close, M_Close, "Close", NoList,CA_None,isSens,NoFrame, 0, 0},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static vStatus StatBar[] =
      {
	{"vCanvasPane Test Window", 808, CA_NoBorder, isSens, 0 },
        {NULL,0,0,0,0}
      };

    static vMenu StandardMenu[] =
      {
	{"&File", M_File, isSens, notUsed, notUsed, noKey, &ShortFile[0]},
	{NULL}
      };


//====================>>> testCmdWindow2::testCmdWindow2 <<<====================
  testCmdWindow2::testCmdWindow2(char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {

    // The "Standard" window will consist of a menubar, a canvas,
    // an optional button bar, and an optional status bar.

    // First, create and add the proper panes to the CmdWindow
    // Note: there must be a correspondint delete in the destructor

    // The Menu Bar
    myMenu = new vMenuPane(StandardMenu);		// A short menu
    AddPane(myMenu);                            // add pane to window

    // The command pane
    myCmdPane = new vCommandPane(CommandBar);
    AddPane(myCmdPane);

    // The Canvas
    myCanvas = new vCanvasPane();              // a new canvas pane
    AddPane(myCanvas);                          // add the pane to window

    // The Status Bar
    myStatus = new vStatusPane(StatBar);        // a new status bar
    AddPane(myStatus);                          // add pane to window

    ShowWindow();

  }

//====================>>> testCmdWindow::~testCmdWindow <<<====================
  testCmdWindow2::~testCmdWindow2()
  {
    UserDebug(Destructor,"testCmdWindow2::~testCmdWindow2() destructor\n")

    delete myMenu;
    delete myCanvas;
    delete myCmdPane;
    delete myStatus;

  }

//====================>>> testCmdWindow::WindowCommand <<<====================
  void testCmdWindow2::WindowCommand(ItemVal id, ItemVal val, CmdType cType)
  {

    // route all commands through here - menus and buttons
 
    switch (id)
      {
	case 200:
	  {
	    myCanvas->Clear();
            // The Ljyg is a visual pattern to line up lines with text
	    myCanvas->DrawText(20,40,"Ljyg: default font at 20,40");
	    myCanvas->DrawLine(20,10,20,40);
	    myCanvas->DrawLine(20,40,90,40);
            vFont font0(vfSansSerif,10,vfNormal,vfNormal,0,0);
            vFont font90(vfSansSerif,10,vfNormal,vfNormal,0,90);
            vFont font180(vfSansSerif,10,vfNormal,vfNormal,0,180);
            vFont font270(vfSansSerif,10,vfNormal,vfNormal,0,270);

            myCanvas->SetFont(font0);
            myCanvas->DrawText(200,150,"Angled Text 0");
            myCanvas->SetFont(font90);
            myCanvas->DrawText(200,150,"Angled Text 90");
            myCanvas->SetFont(font180);
            myCanvas->DrawText(200,150,"Angled Text 180");
            myCanvas->SetFont(font270);
            myCanvas->DrawText(200,150,"Angled Text 270");

            myCanvas->SetFont(font0);

	    break;
	  }

        case M_Close:           // Close not via close button
          {
            theApp->CloseAppWin(this);
            break;
          }
      }
  }
