//	tglapp.h:	Header for testGLApp class
//=======================================================================

#ifndef testGLAPP_H
#define testGLAPP_H

// Include standard V files as needed

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include <v/vapp.h>
#include <v/vawinfo.h>

#include "tglcmdw.h"	// we use testGLCommandWindow

    class testGLApp : public vApp
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public

	testGLApp(char* name, int sdi = 0, int h = 0, int w = 0);
	virtual ~testGLApp();

	// Routines from vApp that are normally overridden

	virtual vWindow* NewAppWin(vWindow* win, char* name, int h, int w,
		vAppWinInfo* winInfo);

	virtual void Exit(void);

	virtual int CloseAppWin(vWindow*);

	virtual void AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType);

	virtual void KeyIn(vWindow*, vKey, unsigned int);

	// New routines for this particular app

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	testGLCmdWindow* _testGLCmdWin;		// Pointer to instance of first window

      };
#endif
