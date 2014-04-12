//========================================================================
//  tutapp.cpp:     Source file for tutorial V application
//
//      Copyright 1995,1996 Bruce E. Wampler. All rights reserved.
//========================================================================
//
// This file is used to define the main application object. There
// will be exactly one instance of the application object. You will
// usually derive you own app class from the vApp class. This file
// defines a sample myApp class. The usual purpose of myApp is to
// start the initial window, and act as a central controller for
// your application. Rather than reading this file sequentially,
// you should skip to the end and read the comments surrounding the
// AppMain function.


//  Files required for tutorial minimal application:
//      tutapp.h:       Header for the min app
//      tutapp.cpp:     Source code for min app
//      tcmdwin.h:      Header code for sample command window
//      tcmdwin.cpp:    Source code for sample command window
//      tdialog.h:      Header for sample modeless dialog
//      tdialog.cpp:    Source for sample modeless dialog
//      tmodal.h:       Header for sample modal dialog
//      tmodal.cpp:     Source for sample modal dialog
//

// First #include header files we need to use.

#include "tutapp.h"     // our header file

//=========================>>> tutApp::NewAppWin <<<======================
  vWindow* tutApp::NewAppWin(vWindow* win, VCONST char* name, int w, int h,
    vAppWinInfo* winInfo)
  {
    // This version of NewAppWin is provided with the information
    // required to name and size a window. You can derive
    // 
    // Typically, this method would get a file name or other information
    // needed to setup the AppWinInfo class  specifically for the
    // application. Thus, each open window usually represents a view of
    // a file or data object.

    vWindow* thisWin = win;             // local copy to use
    vAppWinInfo* awinfo = winInfo;
    VCONST char *myname = name;                // local copy

    if (!name || !*name)
        myname = "Example";            // make up a name
        
    // The UserDebug macros are useful for tracking what is going on.
    // This shows we're building a window.

    UserDebug1(Build,"tutApp::NewAppWin(%s)\n",myname);

    // You may instantiate an instance of the window outside of
    // NewAppWin, or allow NewAppWin to create the instance.

    if (!thisWin)       // Didn't provide a window, so create one.
        thisWin = new tCmdWindow(myname, w, h);

    // The vAppWinInfo class is meant to serve as a database used by the
    // tutApp controller. If you use this feature, you will probably
    // derive your own myAppWinInfo class from vAppWinInfo. The instance
    // of vAppWinInfo created here will be automatically deleted when
    // this window instance is closed through CloseAppWin.

    if (!awinfo)        // Did caller provide an appinfo?
        awinfo = new vAppWinInfo(myname);

    // After you have created an instance of the window and an instance
    // of the AppWinInfo, you MUST call the base vApp::NewAppWin method.
    // You won't need to explicitly keep track of the pointer to
    // each new window -- unless it has to interact with other windows.
    // If that is the case, then you can use your derived vAppWinInfo
    // to coordinate the interaction.

    return vApp::NewAppWin(thisWin,name,w,h,awinfo);
  }

//===========================>>> tutApp::Exit <<<=========================
  void tutApp::Exit(void)
  {
    // This can be called to close all windows. If the app needs to do
    // something special, it can. Otherwise, it can call the general
    // vApp::Exit method, which will perform appropriate calls the the
    // specialized tutApp::CloseAppWin.

    UserDebug(Build,"tutApp::Exit()\n");

    vApp::Exit();       // easy default behavior
  }

//======================>>> tutApp::CloseAppWin <<<=======================
  int tutApp::CloseAppWin(vWindow* win)
  {
    // This will be called BEFORE a window has been unregistered or
    // closed. The app can do whatever it needs to to close down the
    // data associated with this window. (It is invoked explicitly by
    // you in response to a Close menu pick, for example, or when the
    // user clicks the close button. It can also be called by vApp::Exit().
    // After this method cleans up, it can then call the superclass
    // vApp::CloseAppWin to unregister and close this window. Note that
    // the win gives a handle that can be used with vApp::getAppWinInfo
    // to retrieve the AppWinInfo class.

    UserDebug(Build,"tutApp::CloseAppWin()\n");

    // Code to handle close of window (such as saving/closing
    // a file) would go here...

    return vApp::CloseAppWin(win);   // Unregister and close the window.
  }

//=====================>>> tutApp::AppCommand <<<=========================
  void tutApp::AppCommand(vWindow* win, ItemVal id, ItemVal val,
    CmdType cType)
  {
    // Any commands not processed by the window WindowCommand
    // method will be passed to here for default treatment.

    UserDebug1(Build,"tutApp::AppCmd(ID: %d)\n",id);
    vApp::AppCommand(win, id, val, cType);
  }

//=======================>>> tutApp::KeyIn <<<============================
  void tutApp::KeyIn(vWindow* win, vKey key, unsigned int shift)
  {
    // Any key strokes not processed by the window will be passed
    // along to here for default treatment.

    vApp::KeyIn(win, key, shift);
  }

//========================================================================
// Remember that any static references to an object are constructed by
// the C++ startup code before main or any other functions are called.
// Thus, the constructor for tutApp (and thus vApp) is invoked before
// anything else happens. This enables V to perform whatever
// initializations are required by the host GUI system - and frees you
// from having to worry about the typical gory details. All this means
// that EVERY V application needs a static instance of the tutApp to
// get things rolling. Note that the global variable theApp is set to
// point to this instance, and is the easiest way to access the vApp
// and tutApp methods (e.g., theApp->Exit()).
//========================================================================

  static tutApp tut_App("TutorApp");  // The single instance of the app

//===========================>>> AppMain <<<==============================
  int AppMain(int argc, char** argv)
  {
    // The V framework defines the instance of main. After some
    // processing of command line arguments, AppMain is called with
    // cleaned up command line arguments. Note that at this time, no
    // windows have been defined. Normally, AppMain is the place to
    // start up the first window. You can perform any initialization you
    // need to do here.

    (void) theApp->NewAppWin(0, "Tutorial V Example", 350, 100, 0);

    // At this point, the window is up, and all events are being
    // routed through its methods.

    // We MUST return 0 if the status is ok at this point.
    return 0;
  }
