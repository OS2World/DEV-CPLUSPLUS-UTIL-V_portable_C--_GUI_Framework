//================================================================
//  viedapp.cxx:     Source file for V Icon Editor application
//
//    Copyright (C) 1995,1996  Bruce E. Wampler
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//================================================================

#include "viedapp.h"    // our header
 
//==================>>> drawApp::NewAppWin <<<====================
  vWindow* drawApp::NewAppWin(vWindow* win, VCONST char* name, int w, int h,
                              vAppWinInfo* winInfo)
  {
    // Create a new Icon Editor window
    vWindow* thisWin = win;             // local copy to use
    vAppWinInfo* awinfo = winInfo;
 
    if (!thisWin)                       // need to new a window
      {
        thisWin = new myCmdWindow(name, w, h);
      }
 
    if (!awinfo)
        awinfo = new vAppWinInfo(name);
 
    return vApp::NewAppWin(thisWin, name, w, h, awinfo);
  }
 
//======================>>> drawApp::AppCommand <<<=====================
  void drawApp::AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType)
  {
    // Any commands not processed by the window will be passed along to here for
    // default treatment.

    UserDebug1(Build,"drawApp::AppCommand(ID: %d)\n",id);
 
    vApp::AppCommand(win, id, val, cType);
  }
 
//======================>>> drawApp::Exit <<<=====================
  void drawApp::Exit(void)
  {
    // This is called to close all windows.

    vApp::Exit();       // easy default behavior
  }
 
//====================>>> drawApp::CloseAppWin <<<================
  int drawApp::CloseAppWin(vWindow* win)
  {
    // Closes the application window 

    myCmdWindow* cw = (myCmdWindow*)win;
    if (cw->CheckClose())
        return vApp::CloseAppWin(win);
    else
        return 0;
  }
 

//***** EVERY V application needs to declare an app instance *****
static drawApp draw_App("Icon Edit");    // The instance of the app

 
//========================>>> AppMain <<<=========================
  int AppMain(int argc, char** argv)
  {
    // Use AppMain to create the main window

    myCmdWindow* cw =           // open new window, remember it
        (myCmdWindow*) theApp->NewAppWin(0, "V Icon Edit - No Name",400,200);
    if (argc > 1)
      {
        theApp->CheckEvents();  // make sure events done before opening
        cw->OpenFile(argv[1]);  // open a file
      }
 
    return 0;           // 0 means OK
  }
