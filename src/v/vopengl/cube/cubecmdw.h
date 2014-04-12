//	cubecmdw.h:	Header for cubecmdw class
//=======================================================================

#ifndef cubeCMDW_H
#define cubeCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vutil.h>	// For V Utilities
#include <v/vcmdpane.h> // command pane

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "cubecnv.h"	// cubeOGLCanvasPane

    class cubeCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public
	cubeCmdWindow(char*, int, int);
	virtual ~cubeCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	// Standard elements
	vMenuPane* cubeMenu;		// For the menu bar
	cubeOGLCanvasPane* cubeCanvas;		// For the canvas
	vCommandPane* cubeCmdPane;	// for the command pane

	// Dialogs associated with CmdWindow


      };
#endif
