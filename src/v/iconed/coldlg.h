//========================= coldlg.h ====================================
// This is the header file for the color palette dialog class.
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//=======================================================================

#ifndef COLDLG_H
#define COLDLG_H

#include <v/vdialog.h>	// We inherit from vDialog


#include "viedcnv.h"

const int PalColors = 64;


    class vApp;
    class vWindow;
    class myCanvasPane;
    class vColorDialog : public vDialog
      {
      public:		//---------------------------------------- public
	// Constructors
	vColorDialog(vBaseWindow* bw, int All = 1, myCanvasPane* cnv = NULL);

	virtual ~vColorDialog();	// Destructor

	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	void UpdatePalette(vColor* theColors, int numColors);


      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	void init();	// internal inintialization
        void UpdateColors(int mode, int value); // Updates current colors
        void UpdateSliders(vColor CurColor);	// Updates color sliders 
	void SetCmdObjItemList(CommandObject* cList, ItemVal id, void* ptr);

	vWindow* pWin;	// our parent
        myCanvasPane* myCanvas;         // For the canvas

	vColor saveColor;

	int penColorMsg;		// message id of current color
	int bgColorMsg;
	int penColorVal;		// value id of current color
	int bgColorVal;			// value of background color
	int drawWhatShape;		// active shape to draw
	ItemVal penStyleMsg;
	ItemVal brushStyleMsg;
	vColor Pal[PalColors];
	vColor clrPen;
	CommandObject* myCmds;
      };

// Declaration of Item Values
const ItemVal lbL1 = 999;
const ItemVal lbL2 = 998;

const ItemVal frPalette = 150;
const ItemVal frWhich = 151;
const ItemVal cbtPen = 152;
const ItemVal cbPen = 154;
const ItemVal cbtBrush = 155;
const ItemVal cbBrush = 157;
const ItemVal blank = 158;
const ItemVal dummy = 159;
const ItemVal frSliders = 160;

const ItemVal Rslider = 161;
const ItemVal Rtext = 162;
const ItemVal Bslider = 163;
const ItemVal Btext = 164;
const ItemVal Gslider = 165;
const ItemVal Gtext = 166;

const ItemVal blank1 = 170;
const ItemVal blank2 = 171;
const ItemVal blank3 = 172;
const ItemVal blank4 = 173;
const ItemVal blank5 = 174;


const ItemVal btClose = 190;
const ItemVal btSetFG = 191;
const ItemVal btSetBG = 192;
const ItemVal btReset = 193;

#endif
