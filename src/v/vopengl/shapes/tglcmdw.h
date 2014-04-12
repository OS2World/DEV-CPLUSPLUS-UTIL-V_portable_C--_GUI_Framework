//	tglcmdw.h:	Header for testGLcmdw class
//=======================================================================

#ifndef testGLCMDW_H
#define testGLCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vutil.h>	// For V Utilities
#include <v/vcmdpane.h> // command pane
#include <v/vstatusp.h>	// For the status pane
#include <v/vtimer.h>   // Timer

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "tglcnv.h"	// testGLCanvasPane

    class testGLCmdWindow;

    class testGLTimer : public vTimer
      {
      public:		//---------------------------------------- public
	testGLTimer(testGLCmdWindow* cw) { cmdw = cw; }
	~testGLTimer() {}
	virtual void TimerTick();
      private:		//--------------------------------------- private
	testGLCmdWindow* cmdw;
      };

    class testGLCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access
	friend class testGLTimer;

      public:		//---------------------------------------- public
	testGLCmdWindow(char*, int, int);
	virtual ~testGLCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	int _spinSpeed;		// speed when spinning 0-100

	// Standard elements
	vMenuPane* testGLMenu;		// For the menu bar
	testGLCanvasPane* testGLCanvas;		// For the canvas
	vCommandPane* testGLCmdPane;	// for the command pane
	vStatusPane* testGLStatus;		// For the status bar
	testGLTimer* _timer;    // Timer for spin

      };
#endif
