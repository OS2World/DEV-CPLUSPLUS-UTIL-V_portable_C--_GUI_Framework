//=======================================================================
//	vgcmdw.h:	Header for vgApp class
//=======================================================================

#ifndef VGCMDW_H
#define VGCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vcmdpane.h> // command pane
#include <v/vstatusp.h>	// For the status pane

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "vgdefs.h"	// for definitions
#include "vgcnv.h"	// vgCanvasPane
#include "vgmdlg.h"	// vgModalDialog
#include "vgdlmdlg.h"	// vgdlgModalDialog

    class vgCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public
	vgCmdWindow(VCONST char*, int, int);
	virtual ~vgCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	// Standard elements
	vMenuPane* vgMenu;		// For the menu bar
	vgTextCanvasPane* vgCanvas;		// For the canvas
	vCommandPane* vgCmdPane;	// for the command pane

	vgOptions opts;
      };
#endif
