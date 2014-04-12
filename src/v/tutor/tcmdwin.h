//========================================================================
//      tcmdwin.h:  Header file for tutorial V command window
//
//      Copyright 1995,1996  Bruce E. Wampler. All rights reserved.
//========================================================================
//
// Derive a window from the vCmdWindow class

#ifndef TCMDWIN_H
#define TCMDWIN_H

#include <v/vcmdwin.h>  // So we can use vCmdWindow
#include <v/vmenu.h>    // For the menu pane
#include <v/vstatusp.h> // For the status pane
#include <v/vcmdpane.h> // command pane

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "tdialog.h"    // user defined: tDialog
#include "tmodal.h"     // user defined: tModalDialog
#include "tcanvas.h"    // user defined: tCanvasPane

    class tCmdWindow : public vCmdWindow
      {
        friend int AppMain(int, char**);        // allow AppMain access

      public:           //---------------------------------------- public
        tCmdWindow(VCONST char*, int, int);    // Constructor with size
        virtual ~tCmdWindow();          // Destructor
        virtual void WindowCommand(ItemVal id,ItemVal val,CmdType cType);
        virtual void KeyIn(vKey keysym, unsigned int shift);

      protected:        //------------------------------------- protected

      private:          //--------------------------------------- private

        // Each user CmdWindow should conform to a "Standard" window,
        // which includes a menu bar, a canvas, an optional command bar,
        // and an optional status bar.

        vMenuPane* myMenu;              // For the menu bar
        tCanvasPane* myCanvas;          // For the canvas
        vStatusPane* myStatus;          // For the status bar
        vCommandPane* myCmdPane;        // for the command pane

        // Each user CmdWindow will probably have some dialogs and
        // subwindows. Declare pointers to each instance here.

        tDialog* sampleDialog;
        tModalDialog* sampleModalDialog;
      };
#endif
