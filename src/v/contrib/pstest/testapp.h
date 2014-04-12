//	testapp.h:	Header for testApp class
//=======================================================================

#ifndef testAPP_H
#define testAPP_H

// Include standard V files as needed

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include <v/vapp.h>
#include <v/vawinfo.h>

#include "testcmdw.h"	// we use testCommandWindow

    class testApp : public vApp
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public

	testApp(char* name, int sdi = 0, int h = 0, int w = 0);
	virtual ~testApp();

	// Routines from vApp that are normally overridden

	virtual vWindow* NewAppWin(vWindow* win, char* name, int h, int w,
		vAppWinInfo* winInfo);

	virtual void Exit(void);

	virtual void CloseAppWin(vWindow*);

	virtual void AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType);

	virtual void KeyIn(vWindow*, vKey, unsigned int);

	// New routines for this particular app

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	testCmdWindow* _testCmdWin;		// Pointer to instance of first window

      };
#endif
