//	sboxcmdw.h:	Header for sboxcmdw class
//=======================================================================

#ifndef sboxCMDW_H
#define sboxCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vutil.h>	// For V Utilities
#include <v/vcmdpane.h> // command pane
#include <v/vtimer.h>	// Timer

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "sboxcnv.h"	// sboxOGLCanvasPane

    class sboxCmdWindow;

    class sboxAuxTimer : public vTimer
      {
      public:		//---------------------------------------- public
	sboxAuxTimer(sboxCmdWindow* cw) { cmdw = cw; }
	~sboxAuxTimer() {}
	virtual void TimerTick();
      private:		//--------------------------------------- private
	sboxCmdWindow* cmdw;
      };

    class sboxCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public
	sboxCmdWindow(char*, int, int);
	virtual ~sboxCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);
	sboxAuxTimer* GetsboxAuxTimer() {return _auxTimer;}// Aux Timer

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	// Standard elements
	vMenuPane* sboxMenu;		// For the menu bar
	sboxOGLCanvasPane* sboxCanvas;		// For the canvas
	vCommandPane* sboxCmdPane;	// for the command pane
	sboxAuxTimer* _auxTimer;	// Aux Timer

	// Dialogs associated with CmdWindow


      };
#endif
