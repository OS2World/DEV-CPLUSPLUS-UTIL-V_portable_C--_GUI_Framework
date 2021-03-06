//=======================================================================
//@V@:Note: This file generated by vgen V1.04 (09:06:20 22 Jun 1998).
//	cubeapp.cpp:	Source for cubeApp class
//=======================================================================

#include "cubeapp.h"		// Header file

//=========================>>> cubeApp::cubeApp <<<==========================
  cubeApp::cubeApp(char* name, int sdi, int h, int w) : vApp(name, sdi, h, w)
  {
    // Constructor

   _cubeCmdWin = 0;
  }

//=========================>>> cubeApp::cubeApp <<<==========================
  cubeApp::~cubeApp()
  {
    // Destructor
  }

//=====================>>> cubeApp::NewAppWin <<<==========================
  vWindow* cubeApp::NewAppWin(vWindow* win, char* name,
    int w, int h, vAppWinInfo* winInfo)
  {
    vAppWinInfo* awinfo = winInfo;
    char *appname = name;

    if (!*name)
      {
	 appname = "V/OpenGL Demo - Cube";		// Default name
      }
	
    UserDebug1(Build,"cubeApp::NewAppWin(%s)\n",appname);

    // Create the first window using provided CmdWindow

    _cubeCmdWin = (cubeCmdWindow*) win;
    if (!_cubeCmdWin)
      {
	_cubeCmdWin = new cubeCmdWindow(appname, w, h);
      }

    if (!awinfo)
	awinfo = new vAppWinInfo(appname);

    return vApp::NewAppWin(_cubeCmdWin, appname, w, h, awinfo);
  }

//============================>>> cubeApp::Exit <<<===========================
  void cubeApp::Exit(void)
  {
    // This is called to close all windows.

    UserDebug(Build,"cubeApp::Exit()\n");

    vApp::Exit();		// Default behavior
  }

//======================>>> cubeApp::CloseAppWin <<<===========================
  int cubeApp::CloseAppWin(vWindow* win)
  {
    // This will be called BEFORE a window has been unregistered or
    // closed.  Default behavior: unregister and close the window.

    UserDebug(Build,"cubeApp::CloseAppWin()\n");

    return vApp::CloseAppWin(win);
  }

//=====================>>> cubeApp::AppCommand <<<==============================
  void cubeApp::AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType)
  {
    // Commands not processed by the window will be passed here

    UserDebug1(Build,"cubeApp::AppCmd(ID: %d)\n",id);
    vApp::AppCommand(win, id, val, cType);
  }

//=========================>>> cubeApp::KeyIn <<<==============================
  void cubeApp::KeyIn(vWindow* win, vKey key, unsigned int shift)
  {
    // Key strokes not processed by the window will be passed here

    vApp::KeyIn(win, key, shift);
  }

//###########################################################################

  static cubeApp cube_App("V/OpenGL Demo - Cube",1);	// The instance of the app

//============================>>> AppMain <<<==============================
  int AppMain(int argc, char** argv)
  {
    // Use AppMain to create the main window

    (void) theApp->NewAppWin(0, "V/OpenGL Demo - Cube", 300, 300);

    return 0;
  }
