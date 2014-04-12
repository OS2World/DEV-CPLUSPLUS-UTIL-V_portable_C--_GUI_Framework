//================================================================
//  vdrwapp.h:	Header file for V draw app
//  Copyright (C) 1995,1996  Bruce E. Wampler
//================================================================
#ifndef DRAWAPP_H
#define DRAWAPP_H

#include <v/vapp.h>
#include <v/vawinfo.h>	// for app info

#include "vdrwcmdw.h"	// we user our cmdwin class
#include "vdrwdlg.h"	// we have a draw dialog

    class drawApp : public vApp
      {
	friend int AppMain(int, char**);   // allow AppMain access

      public:		//--------------------------------- public
	drawApp(char* name) : vApp(name) {_drawDialog = 0;}	// just call vApp
	virtual ~drawApp() { if (_drawDialog != 0) delete _drawDialog;}

	// Routines from vApp that are normally overridden
	virtual vWindow* NewAppWin(vWindow* win, VCONST char* name, int w,
		int h, vAppWinInfo* winInfo);
	void AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType);
	virtual void Exit(void);
	virtual int CloseAppWin(vWindow*);

      protected:	//------------------------------ protected

      private:		//-------------------------------- private

 	vDrawDialog* _drawDialog;
	myCmdWindow* _curWindow;	// newest window created

      };
#endif
