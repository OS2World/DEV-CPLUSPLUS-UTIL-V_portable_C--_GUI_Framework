//	testcmdw.h:	Header for testcmdw class
//=======================================================================

#ifndef testCMDW_H
#define testCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vutil.h>	// For V Utilities
#include <v/vcmdpane.h> // command pane
#include <v/vstatusp.h>	// For the status pane
#include <v/vtimer.h>	// Timer

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "testcnv.h"	// testCanvasPane

    class testCmdWindow;


    class testCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public
	testCmdWindow(char*, int, int);
	virtual ~testCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	// Standard elements
	vMenuPane* testMenu;		// For the menu bar
	testCanvasPane* testCanvas;		// For the canvas


      };
#endif
