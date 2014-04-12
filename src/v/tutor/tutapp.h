//========================================================================
//      tutapp.h:  Header file for tutorial V application
//
//      Copyright 1995,1996 Bruce E. Wampler. All rights reserved.
//========================================================================

#ifndef TUTAPP_H                // Standard technique for avoiding
#define TUTAPP_H                // problems with multiple #includes

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include <v/vapp.h>     // We are derived from vApp
#include <v/vawinfo.h>  // Need for app info

#include "tcmdwin.h"            // we use our tCmdWindow class

    class tutApp : public vApp
      {
        friend int AppMain(int, char**); // allow AppMain access

      public:           //---------------------------------------- public

        tutApp(char* name) : vApp(name) {}      // just call vApp
        virtual ~tutApp() {}

        // Routines from vApp that are normally overridden

        virtual vWindow* NewAppWin(vWindow* win, VCONST char* name, int w, int h,
           vAppWinInfo* winInfo);
        virtual void Exit(void);
        virtual int CloseAppWin(vWindow* win);
        virtual void AppCommand(vWindow* win, ItemVal id,
                ItemVal val, CmdType cType);
        virtual void KeyIn(vWindow* win, vKey key,
            unsigned int shift);

        // New routines for this particular app go here

      protected:        //------------------------------------- protected

      private:          //--------------------------------------- private

      };
#endif
