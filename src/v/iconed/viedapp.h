//================================================================
//  viedapp.h:  Header file for V Icon Editor application
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//================================================================
#ifndef VIEDAPP_H
#define VIEDAPP_H
 
#include <v/vapp.h>
#include <v/vawinfo.h>  // for app info
 
#ifdef USE_V_DLL
#define viedVers "1.02!"
#else
#define viedVers "1.02"
#endif
   
#include "viedcmdw.h"   // we user our cmdwin class
#include "coldlg.h"     // we have a color dialog
#include "brshdlg.h"    // we have a brushbox dialog
 
    class drawApp : public vApp
      {
        friend int AppMain(int, char**);   // allow AppMain access
 
      public:           //--------------------------------- public
        drawApp(char* name) : vApp(name) {
                                          _colorDialog = 0;
                                          _brshDialog = 0;}
                                        // just call vApp
        virtual ~drawApp() { 
                             if (_colorDialog != 0) delete _colorDialog;
                             if (_brshDialog != 0) delete _brshDialog;}
 
        // Routines from vApp that are normally overridden
        virtual vWindow* NewAppWin(vWindow* win, VCONST char* name, int w = 420,
                int h = 270 , vAppWinInfo* winInfo = 0);
        void AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType);
        virtual void Exit(void);
        virtual int CloseAppWin(vWindow*);
 
      protected:        //------------------------------ protected
 
      private:          //-------------------------------- private
 
        vColorDialog* _colorDialog;
        vBrshDialog* _brshDialog;
        myCmdWindow* _curWindow;        // newest window created
      };
#endif
