//================================================================
//  drawapp.h:	Header file for minimal prototype V application
//  Copyright (C) 1995  Bruce E. Wampler
//================================================================
#ifndef DRAWAPP_H
#define DRAWAPP_H

#include <v/vapp.h>
#include <v/vawinfo.h>	// for app info

#include "drawcmdw.h"	// we user our cmdwin class

    class drawApp : public vApp
      {
	friend int AppMain(int, char**);   // allow AppMain access

      public:		//--------------------------------- public
	drawApp(char* name) : vApp(name) {}	// just call vApp
	virtual ~drawApp() {}

	// Routines from vApp that are normally overridden
	virtual vWindow* NewAppWin(vWindow* win, VCONST char* name, int w,
		int h, vAppWinInfo* winInfo);
	virtual void Exit(void);
	virtual int CloseAppWin(vWindow*);

      protected:	//------------------------------ protected

      private:		//-------------------------------- private

      };
#endif
