//================================================================
//  drawapp.cxx:     Source file for simple draw V application
//  Copyright (C) 1995,1996  Bruce E. Wampler
//================================================================
//  Files required for draw application:
//      vdrwapp.h:      Header for the draw app
//      vdrwapp.cxx:   	Source code for draw app class
//	vdrwcmdw.h:	Header for draw command window class
//	vdrwcmdw.cxx:	Source code for draw command window
//	vdrwcnv.h:	Header for draw canvas class
//	vdrwcnv.cxx:	Sorce for draw canvas class
//
#include "vdrwapp.h"	// our header

//==================>>> drawApp::NewAppWin <<<====================
  vWindow* drawApp::NewAppWin(vWindow* win, VCONST char* name, int w, int h,
    vAppWinInfo* winInfo)
  {
    // Create a new Draw window

    vWindow* thisWin = win;		// local copy to use
    vAppWinInfo* awinfo = winInfo;

    if (!thisWin)			// need to new a window
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
    // Any commands not processed by the window will be passed
    // along to here for default treatment.

//xxyyxx    UserDebug1(Build,"drawApp::AppCommand(ID: %d)\n",id);

    if (id == m_DrawAllDialog)
      {
	if (_drawDialog == 0)
	    _drawDialog = new vDrawDialog(this);
	_drawDialog->ShowDialog("Drawing Attributes - All Windows");
      }

    vApp::AppCommand(win, id, val, cType);
  }

//======================>>> drawApp::Exit <<<=====================
  void drawApp::Exit(void)
  {
    // This is called to close all windows. 

    vApp::Exit();	// easy default behavior
  }

//====================>>> drawApp::CloseAppWin <<<================
  int drawApp::CloseAppWin(vWindow* win)
  {
    // Code to handle close of window (such as saving/closing
    // a file) would go here...

    myCmdWindow* cw = (myCmdWindow*)win;
    if (cw->CheckClose())
	return vApp::CloseAppWin(win);
    return 0;
  }

//***** EVERY V application needs to declare an app instance *****

  static drawApp draw_App("V Draw");	// The instance of the app

//========================>>> AppMain <<<=========================
  int AppMain(int argc, char** argv)
  {
    // Use AppMain to create the main window

    myCmdWindow* cw =		// open new window, remember it
	(myCmdWindow*) theApp->NewAppWin(0, "V Draw - No Name", 600, 250, 0);
    if (argc > 1)
      {
	cw->OpenFile(argv[1]);	// open a file
      }

    return 0;		// 0 means OK
  }
