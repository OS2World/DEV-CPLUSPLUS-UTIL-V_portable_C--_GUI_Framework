//================================================================
//  drawapp.cxx:     Source file for simple draw V application
//  Copyright (C) 1995  Bruce E. Wampler
//================================================================
//  Files required for draw application:
//      drawapp.h:      Header for the draw app
//      drawapp.cxx:    Source code for draw app class
//      drawcmdw.h:     Header for draw command window class
//      drawcmdw.cxx:   Source code for draw command window
//      drawcnv.h:      Header for draw canvas class
//      drawcnv.cxx:    Sorce for draw canvas class
//
#include "drawapp.h"    // our header

//==================>>> drawApp::NewAppWin <<<====================
  vWindow* drawApp::NewAppWin(vWindow* win, VCONST char* name, int w, int h,
    vAppWinInfo* winInfo)
  {
    // Create a new Draw window

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

//======================>>> drawApp::Exit <<<=====================
  void drawApp::Exit(void)
  {
    // This is called to close all windows. 

    vApp::Exit();       // easy default behavior
  }

//====================>>> drawApp::CloseAppWin <<<================
  int drawApp::CloseAppWin(vWindow* win)
  {
    // Code to handle close of window (such as saving/closing
    // a file) would go here...

    return vApp::CloseAppWin(win);
  }

//***** EVERY V application needs to declare an app instance *****

  static drawApp draw_App("V Draw");    // The instance of the app

//========================>>> AppMain <<<=========================
  int AppMain(int argc, char** argv)
  {
    // Use AppMain to create the main window

    theApp->NewAppWin(0, "V Draw - No Name", 500, 250, 0);
    return 0;           // 0 means OK
  }
