//=======================================================================
//      vgapp.cpp:      Source for vgApp class
//=======================================================================

#include "vgapp.h"              // Header file

//=========================>>> vgApp::vgApp <<<==========================
  vgApp::vgApp(char* name, int sdi) : vApp(name, sdi)
  {
    // Constructor

   _vgCmdWin = 0;

  }

//=========================>>> vgApp::vgApp <<<==========================
  vgApp::~vgApp()
  {
    // Desstructor
   

  }

//=====================>>> vgApp::NewAppWin <<<==========================
  vWindow* vgApp::NewAppWin(vWindow* win, VCONST char* name, 
    int w, int h, vAppWinInfo* winInfo)
  {
    vAppWinInfo* awinfo = winInfo;
    VCONST char *vgname = name;

    if (!*name)
      {
        vgname = "V Shell App Generator";               // Default name
      }
        
    UserDebug1(Build,"vgApp::NewAppWin(%s)\n",vgname);

    // Create the first window using provided CmdWindow

    _vgCmdWin = (vgCmdWindow*) win;
    if (!_vgCmdWin)
      {
        _vgCmdWin = new vgCmdWindow(vgname, w, h);
      }

    if (!awinfo)
        awinfo = new vAppWinInfo(vgname);

    return vApp::NewAppWin(_vgCmdWin, vgname, w, h, awinfo);
  }

//============================>>> vgApp::Exit <<<===========================
  void vgApp::Exit(void)
  {
    // This is called to close all windows.

    UserDebug(Build,"vgApp::Exit()\n");

    vApp::Exit();               // Default behavior
  }

//======================>>> vgApp::CloseAppWin <<<===========================
  int vgApp::CloseAppWin(vWindow* win)
  {
    // This will be called BEFORE a window has been unregistered or
    // closed.  Default behavior: unregister and close the window.

    UserDebug(Build,"vgApp::CloseAppWin()\n");

    return vApp::CloseAppWin(win);
  }

//=====================>>> vgApp::AppCommand <<<==============================
  void vgApp::AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType)
  {
    // Commands not processed by the window will be passed here

    UserDebug1(Build,"vgApp::AppCmd(ID: %d)\n",id);
    vApp::AppCommand(win, id, val, cType);
  }

//=========================>>> vgApp::KeyIn <<<==============================
  void vgApp::KeyIn(vWindow* win, vKey key, unsigned int shift)
  {
    // Key strokes not processed by the window will be passed here

    vApp::KeyIn(win, key, shift);
  }

//###########################################################################

  static vgApp vg_App("V Shell App Generator",1);       // The instance of the app

//============================>>> AppMain <<<==============================
  int AppMain(int argc, char** argv)
  {
    // Use AppMain to create the main window

    (void) theApp->NewAppWin(0, "V Shell App Generator",600, 300);

    return 0;
  }
