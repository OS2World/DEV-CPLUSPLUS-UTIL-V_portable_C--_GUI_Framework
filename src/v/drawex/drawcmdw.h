//================================================================
//  mycmdwin.h:	Header file for minimal prototype V application
//  Copyright (C) 1995  Bruce E. Wampler
//================================================================
#ifndef DRAWCMDW_H
#define DRAWCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vcmdpane.h> // command pane
#include <v/vpen.h>	// for a pen

#include "drawcnv.h"		// myCanvasPane class

    class myCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);   // allow AppMain access

      public:		//--------------------------------- public
	myCmdWindow(VCONST char*, int, int);	// Constructor with size
	virtual ~myCmdWindow();		// Destructor
	virtual void WindowCommand(ItemVal id, ItemVal val,
		CmdType cType);

      protected:	//------------------------------ protected

      private:		//-------------------------------- private
	// Pointers to panes this window contains
	vMenuPane* myMenu;		// For the menu bar
	myCanvasPane* myCanvas;		// For the canvas
	vCommandPane* myCmdPane;	// for the command pane

	// other stuff
	vPen _pen;			// track pen color
	char _fname[100];		// file name
      };
#endif
