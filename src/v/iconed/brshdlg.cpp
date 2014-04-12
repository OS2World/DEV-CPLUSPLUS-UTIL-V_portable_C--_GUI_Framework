//========================= brshdlg.cxx =================================
// This is the source file for the brush dialog class
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
// 
//=======================================================================
 
#include "brshdlg.h"
#include "viedcmdw.h"
#include <v/vcolor.h>
#include <v/vapp.h>
#include <v/vwindow.h>
#include <v/vicon.h>
 
//Brush image files
#include "brush1.vbm"
#include "brush2.vbm"
#include "brush3.vbm"
#include "brush4.vbm"
#include "brush5.vbm"
#include "brush6.vbm"
#include "brush7.vbm"
#include "brush8.vbm"
#include "brush9.vbm"
#include "brush10.vbm"


//Brush declarations
static vIcon brush1(&brush1_bits[0], brush1_height, brush1_width,brush1_depth);
static vIcon brush2(&brush2_bits[0], brush2_height, brush2_width,brush2_depth);
static vIcon brush3(&brush3_bits[0], brush3_height, brush3_width,brush3_depth);
static vIcon brush4(&brush4_bits[0], brush4_height, brush4_width,brush4_depth);
static vIcon brush5(&brush5_bits[0], brush5_height, brush5_width,brush5_depth);
static vIcon brush6(&brush6_bits[0], brush6_height, brush6_width,brush6_depth);
static vIcon brush7(&brush7_bits[0], brush7_height, brush7_width,brush7_depth);
static vIcon brush8(&brush8_bits[0], brush8_height, brush8_width,brush8_depth);
static vIcon brush9(&brush9_bits[0], brush9_height, brush9_width,brush9_depth);
static vIcon brush10(&brush10_bits[0], brush10_height, brush10_width,brush10_depth);

// Set Item Values for brushes
const ItemVal lbL1 = 2999;
const ItemVal frBrushBox = 104;
const ItemVal frBrush = 105;
const ItemVal ibBrush1 = 3101;
const ItemVal ibBrush2 = 3102;
const ItemVal ibBrush3 = 3103;
const ItemVal ibBrush4 = 3104;
const ItemVal ibBrush5 = 3105;
const ItemVal ibBrush6 = 3106;
const ItemVal ibBrush7 = 3107;
const ItemVal ibBrush8 = 3108;
const ItemVal ibBrush9 = 3109;
const ItemVal ibBrush10 = 3110;
 
// Define the brush dialog window
static CommandObject DefaultCmds[] =
  {
    {C_Label, lbL1, 0,"           ",NoList,CA_MainMsg,isSens,NoFrame, 0, 0},
    
    //----------- The brush frame ---------------------------------
    {C_Frame, frBrush, 0, "", NoList, CA_NoSpace, isSens, NoFrame, 0, lbL1},

    {C_ToggleIconButton, ibBrush1, 1, "", (void*)&brush1, CA_None, isSens,
            frBrush, 0, 0, 0, "Point"},
    {C_ToggleIconButton, ibBrush2, 0, "", (void*)&brush2, CA_None, isSens,
            frBrush, ibBrush1, 0, 0, "Medium Round"},
    {C_ToggleIconButton, ibBrush3, 0, "", (void*)&brush3, CA_None, isSens,
            frBrush, ibBrush2, 0, 0, "Large Round"},
    {C_ToggleIconButton, ibBrush4, 0, "", (void*)&brush4, CA_None, isSens,
            frBrush, ibBrush3, 0, 0, "Small Right Slash"},
    {C_ToggleIconButton, ibBrush5, 0, "", (void*)&brush5, CA_None, isSens,
            frBrush, ibBrush4, 0, 0, "Large Right Slash"},
    {C_ToggleIconButton, ibBrush6, 0, "", (void*)&brush6, CA_None, isSens,
            frBrush, 0, ibBrush1, 0, "Small Square"},
    {C_ToggleIconButton, ibBrush7, 0, "", (void*)&brush7, CA_None, isSens,
            frBrush, ibBrush6, ibBrush1, 0, "Medium Square"},
    {C_ToggleIconButton, ibBrush8, 0, "", (void*)&brush8, CA_None, isSens,
            frBrush, ibBrush7, ibBrush1, 0, "Large Square"},
    {C_ToggleIconButton, ibBrush9, 0, "", (void*)&brush9, CA_None, isSens,
            frBrush, ibBrush8, ibBrush1, 0, "Small Left Slash"},
    {C_ToggleIconButton, ibBrush10, 0, "", (void*)&brush10, CA_None, isSens,
            frBrush, ibBrush9, ibBrush1, 0, "Large Left Slash"},

    {C_EndOfList,0,0,0,0,CA_None,0,0,0}
  };
 
 
//=========================>>> vBrshDialog::vBrshDialog <<<====================
 vBrshDialog::vBrshDialog(vBaseWindow* bw, int All, myCanvasPane* cnv) :
    vDialog(bw, 0, "Brush Box")
  {
    // Creates the brush dialog window.

    UserDebug(Constructor,"vBrshDialog::vBrshDialog(basewindow)\n")
    toAll = All;
    pWin = (vWindow*) bw;
    myCanvas = (myCanvasPane*)cnv;
    toggleBrush = ibBrush1;
    init();
  }
 
//=========================>>> vBrshDialog::vBrshDialog <<<====================
 void vBrshDialog::init()
  {
    // Initializes the brush dialog window.

    AddDialogCmds(DefaultCmds);         // add the predefined commands
 
    drawWhatBrush = brush_1;
  }
 
//========================>>> vBrshDialog::~vBrshDialog <<<====================
  vBrshDialog::~vBrshDialog()
  {
    // Brush Dialog destructor function

    UserDebug(Destructor,"vBrshDialog::~vBrshDialog() destructor\n")
  }
 
//====================>>> vBrshDialog::DialogCommand <<<=======================
  void vBrshDialog::DialogCommand(ItemVal id, ItemVal val, CmdType ctype)
  {
    // After the user has selected a command from the dialog, this routine is 
    // called with the value.  This code would be generated by Vigr.

    UserDebug2(CmdEvents,"vBrshDialog::DialogCommand(id:%d, val:%d)\n",id, val)

    int i,j;
 
    switch (id)         // We will draw in a different brush depending on value
        // toggleBrush is used to keep track of which item
        // from the Brush box is currently being used.
        // Only one brush can be used at any one time.
 
      {
        case ibBrush1:	// Style 1 brush chosen
	 {
           if (toggleBrush == ibBrush1) //toggle with other patterns
	       SetValue(ibBrush1,1, Value);
            else
	      {
		SetValue(toggleBrush, 0,Value);
		toggleBrush = ibBrush1;
	      }

	    // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            myCanvas->_myPicture->SetIndex(0,0,2,-99);
            myCanvas->_BrushWidth = 1;
            myCanvas->_BrushHeight = 1;
            myCanvas->_BrushOffX = 0;
            myCanvas->_BrushOffY = 0;
            drawWhatBrush = brush_1;
            break;
	 }

        case ibBrush2:	// Style 2 brush chosen
	 {
           if (toggleBrush == ibBrush2) //toggle with other patterns 
                    SetValue(ibBrush2,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush2;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            myCanvas->_myPicture->SetIndex(1,0,2,-99);
            myCanvas->_myPicture->SetIndex(2,0,2,-99);
            myCanvas->_myPicture->SetIndex(0,1,2,-99);
            myCanvas->_myPicture->SetIndex(1,1,2,-99);
            myCanvas->_myPicture->SetIndex(2,1,2,-99);
            myCanvas->_myPicture->SetIndex(3,1,2,-99);
            myCanvas->_myPicture->SetIndex(0,2,2,-99);
            myCanvas->_myPicture->SetIndex(1,2,2,-99);
            myCanvas->_myPicture->SetIndex(2,2,2,-99);
            myCanvas->_myPicture->SetIndex(3,2,2,-99);
            myCanvas->_myPicture->SetIndex(1,3,2,-99);
            myCanvas->_myPicture->SetIndex(2,3,2,-99);
 
            myCanvas->_BrushWidth = 4;
            myCanvas->_BrushHeight = 4;
            myCanvas->_BrushOffX = 2;
            myCanvas->_BrushOffY = 2;
            drawWhatBrush = brush_2;
            break;
	 }

        case ibBrush3:	// Style 3 brush chosen
	 {
           if (toggleBrush == ibBrush3) //toggle with other patterns 
                    SetValue(ibBrush3,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush3;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            myCanvas->_myPicture->SetIndex(2,0,2,-99);
            myCanvas->_myPicture->SetIndex(3,0,2,-99);
            myCanvas->_myPicture->SetIndex(4,0,2,-99);
            myCanvas->_myPicture->SetIndex(1,1,2,-99);
            myCanvas->_myPicture->SetIndex(2,1,2,-99);
            myCanvas->_myPicture->SetIndex(3,1,2,-99);
            myCanvas->_myPicture->SetIndex(4,1,2,-99);
            myCanvas->_myPicture->SetIndex(5,1,2,-99);
            myCanvas->_myPicture->SetIndex(0,2,2,-99);
            myCanvas->_myPicture->SetIndex(1,2,2,-99);
            myCanvas->_myPicture->SetIndex(2,2,2,-99);
            myCanvas->_myPicture->SetIndex(3,2,2,-99);
            myCanvas->_myPicture->SetIndex(4,2,2,-99);
            myCanvas->_myPicture->SetIndex(5,2,2,-99);
            myCanvas->_myPicture->SetIndex(6,2,2,-99);
            myCanvas->_myPicture->SetIndex(0,3,2,-99);
            myCanvas->_myPicture->SetIndex(1,3,2,-99);
            myCanvas->_myPicture->SetIndex(2,3,2,-99);
            myCanvas->_myPicture->SetIndex(3,3,2,-99);
            myCanvas->_myPicture->SetIndex(4,3,2,-99);
            myCanvas->_myPicture->SetIndex(5,3,2,-99);
            myCanvas->_myPicture->SetIndex(6,3,2,-99);
            myCanvas->_myPicture->SetIndex(1,4,2,-99);
            myCanvas->_myPicture->SetIndex(2,4,2,-99);
            myCanvas->_myPicture->SetIndex(3,4,2,-99);
            myCanvas->_myPicture->SetIndex(4,4,2,-99);
            myCanvas->_myPicture->SetIndex(5,4,2,-99);
            myCanvas->_myPicture->SetIndex(2,5,2,-99);
            myCanvas->_myPicture->SetIndex(3,5,2,-99);
            myCanvas->_myPicture->SetIndex(4,5,2,-99);
 
            myCanvas->_BrushWidth = 7;
            myCanvas->_BrushHeight = 7;
            myCanvas->_BrushOffX = 4;
            myCanvas->_BrushOffY = 4;
            drawWhatBrush = brush_3;
            break;
	 }

        case ibBrush4:	// Style 4 brush chosen
	 {
           if (toggleBrush == ibBrush4) //toggle with other patterns 
                    SetValue(ibBrush4,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush4;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            myCanvas->_myPicture->SetIndex(1,0,2,-99);
            myCanvas->_myPicture->SetIndex(0,1,2,-99);
 
            myCanvas->_BrushWidth = 2;
            myCanvas->_BrushHeight = 2;
            myCanvas->_BrushOffX = 1;
            myCanvas->_BrushOffY = 1;
            drawWhatBrush = brush_4;
            break;
	 }

        case ibBrush5:	// Style 5 brush chosen
	 {
           if (toggleBrush == ibBrush5) //toggle with other patterns 
                    SetValue(ibBrush5,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush5;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            myCanvas->_myPicture->SetIndex(4,0,2,-99);
            myCanvas->_myPicture->SetIndex(3,1,2,-99);
            myCanvas->_myPicture->SetIndex(2,2,2,-99);
            myCanvas->_myPicture->SetIndex(1,3,2,-99);
            myCanvas->_myPicture->SetIndex(0,4,2,-99);
 
            myCanvas->_BrushWidth = 5;
            myCanvas->_BrushHeight = 5;
            myCanvas->_BrushOffX = 3;
            myCanvas->_BrushOffY = 3;
            drawWhatBrush = brush_5;
            break;
	 }

        case ibBrush6:	// Style 6 brush chosen
	 {
           if (toggleBrush == ibBrush6) //toggle with other patterns 
                    SetValue(ibBrush6,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush6;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            myCanvas->_myPicture->SetIndex(0,0,2,-99);
            myCanvas->_myPicture->SetIndex(1,0,2,-99);
            myCanvas->_myPicture->SetIndex(0,1,2,-99);
            myCanvas->_myPicture->SetIndex(1,1,2,-99);
 
            myCanvas->_BrushWidth = 2;
            myCanvas->_BrushHeight = 2;
            myCanvas->_BrushOffX = 1;
            myCanvas->_BrushOffY = 1;
            drawWhatBrush = brush_6;
            break;
	 }

        case ibBrush7:	// Style 7 brush chosen
	 {
           if (toggleBrush == ibBrush7) //toggle with other patterns 
                    SetValue(ibBrush7,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush7;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            for (i = 0; i < 5; i++)
              for (j = 0; j < 5; j++)
                myCanvas->_myPicture->SetIndex(i,j,2,-99);            
                                              
            myCanvas->_BrushWidth = 5;
            myCanvas->_BrushHeight = 5;
            myCanvas->_BrushOffX = 3;
            myCanvas->_BrushOffY = 3;
            drawWhatBrush = brush_7;
            break;
	 }

        case ibBrush8:	// Style 8 brush chosen
	 {
           if (toggleBrush == ibBrush8) //toggle with other patterns 
                    SetValue(ibBrush8,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush8;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            for (i = 0; i < 8; i++)
              for (j = 0; j < 8; j++)
                myCanvas->_myPicture->SetIndex(i,j,2,-99);            
 
            myCanvas->_BrushWidth = 8;
            myCanvas->_BrushHeight = 8;
            myCanvas->_BrushOffX = 3;
            myCanvas->_BrushOffY = 3;
            drawWhatBrush = brush_8;
            break;
	 }

        case ibBrush9:	// Style 9 brush chosen
	 {
           if (toggleBrush == ibBrush9) //toggle with other patterns 
                    SetValue(ibBrush9,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush9;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            myCanvas->_myPicture->SetIndex(0,0,2,-99);
            myCanvas->_myPicture->SetIndex(1,1,2,-99);
 
            myCanvas->_BrushWidth = 2;
            myCanvas->_BrushHeight = 2;
            myCanvas->_BrushOffX = 1;
            myCanvas->_BrushOffY = 1;
            
            drawWhatBrush = brush_9;
            break;
	 }

        case ibBrush10:	// Style 10 brush chosen
	 {
           if (toggleBrush == ibBrush10) //toggle with other patterns 
                    SetValue(ibBrush10,1, Value);
            else    {
                     SetValue(toggleBrush, 0,Value);  
                     toggleBrush = ibBrush10;  
                    }

            // Draw selected brush
            myCanvas->_myPicture->Clear(2, myCanvas->_myColorPal->getbg());
            myCanvas->_myPicture->SetIndex(0,0,2,-99);
            myCanvas->_myPicture->SetIndex(1,1,2,-99);
            myCanvas->_myPicture->SetIndex(2,2,2,-99);
            myCanvas->_myPicture->SetIndex(3,3,2,-99);
            myCanvas->_myPicture->SetIndex(4,4,2,-99);
 
            myCanvas->_BrushWidth = 5;
            myCanvas->_BrushHeight = 5;
            myCanvas->_BrushOffX = 3;
            myCanvas->_BrushOffY = 3;
            drawWhatBrush = brush_10;
            break;
	 }
 
      }
    myCanvas->Redraw(0,0,0,0);	// Do an automatic redraw clear old brush on the canvas
  }
