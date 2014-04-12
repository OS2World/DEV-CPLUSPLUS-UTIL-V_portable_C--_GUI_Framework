//	bnccmdw.h:	Header for bnccmdw class
//=======================================================================

#ifndef bncCMDW_H
#define bncCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vutil.h>	// For V Utilities
#include <v/vcmdpane.h> // command pane
#include <v/vtimer.h>	// Timer

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "bnccnv.h"	// bncOGLCanvasPane

    class bncCmdWindow;

    class bncAuxTimer : public vTimer
      {
      public:		//---------------------------------------- public
	bncAuxTimer(bncCmdWindow* cw) { cmdw = cw; }
	~bncAuxTimer() {}
	virtual void TimerTick();
      private:		//--------------------------------------- private
	bncCmdWindow* cmdw;
      };

    class bncCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public
	bncCmdWindow(char*, int, int);
	virtual ~bncCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);
	bncAuxTimer* GetbncAuxTimer() {return _auxTimer;}// Aux Timer

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	// Standard elements
	vMenuPane* bncMenu;		// For the menu bar
	bncOGLCanvasPane* bncCanvas;		// For the canvas
	vCommandPane* bncCmdPane;	// for the command pane
	bncAuxTimer* _auxTimer;	// Aux Timer

	// Dialogs associated with CmdWindow


      };
#endif
