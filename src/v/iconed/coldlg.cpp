//========================= coldlg.cxx ==================================
// This the source for the color palette dialog class 
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//
// This file, while loosly tied to the iconed editor, serves as a pretty
// good example of what all can be done with a dialog. Some of the
// tricks are used here because each icon edit window needs its own
// color palette and color picker. So we do some fancy stuff to support
// that. First, the dialog itself is defined as an automatic variable
// in the init routing. This places the definition on the stack instead
// of in static data space. Then, space for a definition for each
// instance of the color dialog object is allocated, and the definitions
// of the dialog are copied. Finally, each color button in the dialog
// is dynamically pointed to the appropriate cell of the local copy
// of the Pal palette. Not all that hard once you see it done!
//=======================================================================

#include <iostream.h>
#include "coldlg.h"
#include "viedcmdw.h"
#include <v/vcolor.h>
#include <v/vapp.h>
#include <v/vwindow.h>
#include <v/vicon.h>
#include <v/vutil.h>
#include <v/vreply.h>

// Set the pen default colors

// Set Item Values for RGB
const ItemVal iRed = 1000;
const ItemVal iGreen = 1001;
const ItemVal iBlue = 1002;


//==========================>>> SetCmdObjItemList <<<==============================
  void vColorDialog::SetCmdObjItemList(CommandObject* cList, ItemVal id, void* ptr)
  {
    // This is called to change the attrs field of the item with given id

    // scan the entire list

    int ix;
    for (ix = 0 ; cList && (cList[ix].cmdType != C_EndOfList) ; ++ix)
      {
	if (cList[ix].cmdId == id)		// the one we want!
	  {
	    cList[ix].itemList = ptr;		// change
	    break;
	  }
      }
   }

//========================>>> vColorDialog::vColorDialog <<<====================
 vColorDialog::vColorDialog(vBaseWindow* bw, int All, myCanvasPane* cnv) :
    vDialog(bw, 0, "Select Color")
  {
    // Creates a color palette dialog window

    UserDebug(Constructor,"vColorDialog::vColorDialog(basewindow)\n")
    pWin = (vWindow*) bw;
    myCanvas = (myCanvasPane*) cnv;
    init();
  }

//========================>>> vColorDialog::init <<<====================
 void vColorDialog::init()
  {
    // Initializes the color palette dialog window

    static int sldR[2] = {0,255};	// Range for sliders

    // Define the color palette dialog window on the stack
    static CommandObject DefaultCmds[] =
    {
     {C_Label, lbL1, 0,"             ",NoList,CA_MainMsg,isSens,NoFrame, 0, 0},
	
     //----------- The set color frame ---------------------------------

     {C_Frame, frWhich, 0, "", NoList, CA_NoBorder, isSens,
	0, 0, lbL1},

     // this is used as a spacer to center the current foreground and backgroud
     {C_ColorButton, cbtPen, 1," ",0,CA_None,isSens,  // set to clrPen
	frWhich, 0, 0, 40, "Current Color"},
     {C_Button, btSetFG, 0, "Apply: Foreground",NoList, CA_None, isSens, frWhich,
	cbtPen, 0, 0, "Make Foreground Color"},
     {C_Button, btSetBG, 0, "Apply: Background",NoList, CA_None, isSens, frWhich,
        cbtPen, btSetFG, 0, "Make Background Color"},

// put in default color pallette
     {C_Frame, frPalette, 0, "", NoList, CA_None, isSens, NoFrame, 0, frWhich},

#define frColors 1499	// must be this
     {C_Frame,frColors,0,"",NoList,CA_NoBorder | CA_NoSpace,isSens,frPalette,0,0},

#define vC_Size 12
#include  "palclrs.h"


     // Add Slider frame
     {C_Frame, frSliders, 0, "Modify", NoList, CA_None, isSens, 
        NoFrame, 0, frPalette},

     // add red slider
     {C_ColorButton,iRed,0," ",(void*)&vStdColors[vC_Red],CA_None,isSens,
	frSliders,0,0, 0, "Click to enter Red value"},
     {C_Slider, Rslider, 0, "", (void*)&sldR[0], CA_None, isSens, frSliders,
        iRed, 0, 256, "Set Red Value"},
     {C_Text, Rtext, 0, "   ",(void*) "0  ", CA_None, isSens, frSliders, 
        Rslider, 0},

     // add green slider
     {C_ColorButton,iGreen,0," ",(void*)&vStdColors[vC_Green],CA_None,isSens,
	frSliders,0,Rslider, 0, "Click to enter Green value"},
     {C_Slider, Gslider, 0, "", (void*)&sldR[0], CA_None, isSens, frSliders,
        iGreen, Rslider, 256, "Set Green Value"},
     {C_Text, Gtext, 0, "", (void*)"0  ", CA_None, isSens, frSliders, 
        Gslider, Rslider},

     // add blue slider
     {C_ColorButton,iBlue,0," ",(void*)&vStdColors[vC_Blue],CA_None,isSens,
	frSliders,0,Gslider, 0, "Click to enter Blue value"},
     {C_Slider, Bslider, 0, "", (void*)&sldR[0], CA_None, isSens, frSliders,
        iBlue, Gslider, 256, "Set Blue Value"},
     {C_Text, Btext, 0, "",(void*) "0  ", CA_None, isSens, frSliders, 
        Bslider, Gslider},

     {C_Button, btReset, 0, "Reset",NoList, CA_None, isSens, NoFrame,
	frSliders, frPalette, 0, "Reset selected color to original color"},

     {C_EndOfList,0,0,0,0,CA_None,0,0,0}
  };

    vColor myPen;

    clrPen.Set(0,0,0);

    // Dynamically build the command list so that each dialog
    // can have its own set of colors
    int ix;
    for (ix = 0 ; DefaultCmds[ix].cmdType != C_EndOfList ; ++ix)
      ;
    
    myCmds = new CommandObject[ix+1];	// dynamic space

    for (int ij = 0 ; ij <= ix ; ++ij)
	myCmds[ij] = DefaultCmds[ij];
	

    for (int id =  0 ; id <= PalColors ; ++id)
	SetCmdObjItemList(myCmds, id+1500, (void*)&Pal[id]);

    SetCmdObjItemList(myCmds, cbtPen, (void*)&clrPen);

    AddDialogCmds(myCmds);		// add the predefined commands

    // message id of current color
    penColorMsg = 1500;

    // value id of current color
    penColorVal = 0;

    // Set color buttons
    saveColor = clrPen;
    SetValue(cbtPen,0,ChangeColor);
  }

//=======================>>> vColorDialog::~vColorDialog <<<====================
  vColorDialog::~vColorDialog()
  {
    // Destroys the class vColorDialog
    UserDebug(Destructor,"vColorDialog::~vColorDialog() destructor\n")
    delete myCmds;
  }

//===================>>> vColorDialog::UpdatePalette <<<=========================
  void vColorDialog::UpdatePalette(vColor* theColors, int numColors)
  {

    int lim = (numColors < PalColors) ? numColors : PalColors;

    // message id of current color
    penColorMsg = 1500;

    // value id of current color
    penColorVal = 0;

    // set color values
    for (int i = 0 ; i < lim ; i++)	// Copy supplied colors
      {
	Pal[i].ResetColor(theColors[i]);
	SetValue(1500+i,0,ChangeColor);
      }

    for (int ix = lim ; ix < PalColors ; ++ix)
      {
	Pal[ix].ResetColor(0,0,0);
	SetValue(1500+ix,0,ChangeColor);
      }

    // Set color buttons
    clrPen.ResetColor(Pal[0]);
    saveColor = clrPen;
    SetValue(cbtPen,0,ChangeColor);
  }

//===================>>> vColorDialog::UpdateColors <<<=========================
  void vColorDialog::UpdateColors(int mode, int value)
  {
    //  Update color buttons (current and color button index) after scroll
    //  bar update

    if (mode == Rslider)
      {
	/* set value into palette */

	// Reset color - the R is new
	Pal[penColorVal].ResetColor(value,
	    Pal[penColorVal].g(),
	    Pal[penColorVal].b());
      } 
    else if (mode == Gslider) 
      {
	// Reset color - the G is new
	Pal[penColorVal].ResetColor(Pal[penColorVal].r(),
	    value,
	    Pal[penColorVal].b());
      } 
    else 
      {
	// Reset color - the B is new
	Pal[penColorVal].ResetColor(Pal[penColorVal].r(),
	    Pal[penColorVal].g(),
	    value);
      }

    // update pen
    clrPen.ResetColor(Pal[penColorVal]);

    // update color buttons
    SetValue(penColorMsg,0,ChangeColor);
    SetValue(cbtPen, 0, ChangeColor);
  }

//=====================>>> vColorDialog::UpdateSliders <<<======================
  void vColorDialog::UpdateSliders(vColor CurColor)
  {
  //  Update Sliders with color

    char strbuff[20];
    int index;

    // Set red value slider
    index = CurColor.r();
//    index = (int) ((float)index / (float)255 * (float)100);
    SetValue(Rslider, index, Value);
    IntToStr(CurColor.r(),strbuff);
    SetString(Rtext,strbuff);

    // Set green value slider
    index = CurColor.g();
//    index = (int) ((float)index / (float)255 * (float)100);
    SetValue(Gslider, index, Value);
    IntToStr(CurColor.g(),strbuff);
    SetString(Gtext,strbuff);

    // Set blue value slider
    index = CurColor.b();
//    index = (int) ((float)index / (float)255 * (float)100);
    SetValue(Bslider, index, Value);
    IntToStr(CurColor.b(),strbuff);
    SetString(Btext,strbuff);
  }

//====================>>> vColorDialog::DialogCommand <<<=======================
  void vColorDialog::DialogCommand(ItemVal id, ItemVal val, CmdType ctype)
  {
    // After the user has selected a command from the dialog, this routine is
    // called with the value.  This code would be generated by Vigr.

    UserDebug2(CmdEvents,"vColorDialog::DialogCommand(id:%d, val:%d)\n",id, val)

    char buff[32];

    switch (id)		// We will do some things depending on value
      {
        case Rslider:	// set value of red scale
          {
            int index = (int) val;

	    IntToStr(index,buff);
            SetString(Rtext,buff);       // set string
          
            UpdateColors(Rslider, index);

            break;
          }

        case Gslider:	// set value of green scale
          {
            int index = (int) val;

	    IntToStr(index,buff);
            SetString(Gtext,buff);       // set string

            UpdateColors(Gslider, index);

            break;
          }

        case Bslider:	// set value of blue scale
          {
            int index = (int) val;

	    IntToStr(index,buff);
            SetString(Btext,buff);       // set string

            UpdateColors(Bslider, index);

            break;
          }

	case iBlue:
	  {
	    int index;
	    vReplyDialog rp(pWin);
	    if (rp.Reply("Enter Blue value 0-255:",buff,30) == M_Cancel)
		break;
	    index = (int) StrToLong(buff);
	    if (index < 0 || index > 255)
		break;
	    IntToStr(index,buff);
            SetString(Btext,buff);       // set string
	    SetValue(Bslider,(index*100)/255,Value);
            UpdateColors(Bslider, index);
            break;
	  }

	case iRed:
	  {
	    int index;
	    vReplyDialog rp(pWin);
	    if (rp.Reply("Enter Red value 0-255:",buff,30) == M_Cancel)
		break;
	    index = (int) StrToLong(buff);
	    if (index < 0 || index > 255)
		break;
	    IntToStr(index,buff);
            SetString(Rtext,buff);       // set string
	    SetValue(Rslider,(index*100)/255,Value);
            UpdateColors(Rslider, index);
            break;
	  }

	case iGreen:
	  {
	    int index;
	    vReplyDialog rp(pWin);
	    if (rp.Reply("Enter Green value 0-255:",buff,30) == M_Cancel)
		break;
	    index = (int) StrToLong(buff);
	    if (index < 0 || index > 255)
		break;
	    IntToStr(index,buff);
	    SetValue(Gslider,(index*100)/255,Value);
            SetString(Gtext,buff);       // set string
            UpdateColors(Gslider, index);
            break;
	  }

	case btSetBG:
	    myCanvas->SetBGColor(clrPen);
	    break;

	case btSetFG:
	    myCanvas->SetFGColor(clrPen);
	    break;

	case btReset:
	    Pal[penColorVal].ResetColor(saveColor);
	    // update color buttons
	    clrPen.ResetColor(saveColor);
	    SetValue(penColorMsg,0,ChangeColor);
	    SetValue(cbtPen, 0, ChangeColor);
	    UpdateSliders(clrPen);
	    break;


	case btClose:		// Close the dialog
	  {
	    CloseDialog();
	    break;
	  }

	default:		// set a color...

	    if (id >= 1500 && id <= 1499+PalColors)  // color button
	      {
		penColorMsg = id;
		penColorVal = val;
		saveColor = Pal[val];
		clrPen.ResetColor(Pal[val]);
		SetValue(cbtPen,0,ChangeColor);
		UpdateSliders(clrPen);
		break;
	      }
      }

    // All commands should also route through the parent handler
    vDialog::DialogCommand(id,val,ctype);
  }
