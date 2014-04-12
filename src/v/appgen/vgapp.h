//=======================================================================
//	vgapp.h:	Header for vgApp class
//=======================================================================

#ifndef VGAPP_H
#define VGAPP_H

// Include standard V files as needed

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include <v/vapp.h>
#include <v/vawinfo.h>

#include "vgdefs.h"
#include "vgcmdw.h"

    class vgApp : public vApp
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public

	vgApp(char* name, int sdi = 0);
	virtual ~vgApp();

	// Routines from vApp that are normally overridden

	virtual vWindow* NewAppWin(vWindow* win, VCONST char* name, int w, int h,
		vAppWinInfo* winInfo);

	virtual void Exit(void);

	virtual int CloseAppWin(vWindow*);

	virtual void AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType);

	virtual void KeyIn(vWindow*, vKey, unsigned int);

	// New routines for this particular app

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	vgCmdWindow* _vgCmdWin;		// Pointer to instance of first window

      };
#endif
