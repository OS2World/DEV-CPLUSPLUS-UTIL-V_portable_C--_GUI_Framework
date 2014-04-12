//	sboxapp.h:	Header for sboxApp class
//=======================================================================

#ifndef sboxAPP_H
#define sboxAPP_H

// Include standard V files as needed

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include <v/vapp.h>
#include <v/vawinfo.h>

#include "sboxcmdw.h"	// we use sboxCommandWindow

    class sboxApp : public vApp
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public

	sboxApp(char* name, int sdi = 0, int h = 0, int w = 0);
	virtual ~sboxApp();

	// Routines from vApp that are normally overridden

	virtual vWindow* NewAppWin(vWindow* win, char* name, int w, int h,
		vAppWinInfo* winInfo);

	virtual void Exit(void);

	virtual int CloseAppWin(vWindow*);

	virtual void AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType);

	virtual void KeyIn(vWindow*, vKey, unsigned int);

	// New routines for this particular app

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	sboxCmdWindow* _sboxCmdWin;		// Pointer to instance of first window

      };
#endif
