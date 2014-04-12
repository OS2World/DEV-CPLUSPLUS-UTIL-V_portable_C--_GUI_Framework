//========================= brshdlg.h ===================================
// This is a header file for the brush dialog class
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//=======================================================================
 
#ifndef BRUSHDLG_H
#define BRUSHDLG_H

#include <v/vdialog.h> 	// We inherit from vDialog
#include "viedcnv.h"	
 
    class vApp;
    class vWindow;
    class myCanvasPane;
    class vBrshDialog : public vDialog
      {
      public:           //---------------------------------------- public
	// Constructors
        vBrshDialog(vBaseWindow* bw, int All = 1, myCanvasPane* cnv = NULL);
 
        virtual ~vBrshDialog();          // Destructor
        virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
        void AddDefaultCmds();          // to add the defined commands
 
      protected:        //--------------------------------------- protected
 
      private:          //--------------------------------------- private
 
        void init();    // internal inintialization
 
        vWindow* pWin;          	// our parent
        myCanvasPane* myCanvas;
 
        int drawWhatBrush;              // active shape to draw
        int toAll;                      // to all or only parent window
        int toggleBrush;                // toggles between brushes
      };
 
//Definitions for brushes 
const int brush_1 = 3001;
const int brush_2 = 3002;
const int brush_3 = 3003;
const int brush_4 = 3004;
const int brush_5 = 3005;
const int brush_6 = 3006;
const int brush_7 = 3007;
const int brush_8 = 3008;
const int brush_9 = 3009;
const int brush_10 = 3010;
 
#endif
