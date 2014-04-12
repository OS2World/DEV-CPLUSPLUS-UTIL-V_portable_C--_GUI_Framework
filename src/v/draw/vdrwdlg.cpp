//=======================================================================
//  vdrwdlg.cxx - Source file for vDrawDialog class
//  Copyright (C) 1995,1996  Bruce E. Wampler
//
//  This program is part of the V C++ GUI Framework example programs.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  (see COPYING) along with this program; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=======================================================================

#include "vdrwdlg.h"
#include "vdrwcmdw.h"
#include <v/vcolor.h>
#include <v/vapp.h>
#include <v/vwindow.h>
#include <v/vicon.h>

#include "rect.xbm"
#include "rdrect.xbm"
#include "ellipse.xbm"
#include "line.xbm"
#include "point.xbm"
#include "polygon.xbm"

    static vIcon rect(&rect_bits[0], rect_height, rect_width);
 //    = {BitMap, rect_height, rect_width, 1, &rect_bits[0]};

    static vIcon rdrect(&rdrect_bits[0], rdrect_height, rdrect_width);
 //    = {BitMap, rdrect_height, rdrect_width, 1, &rdrect_bits[0]};

    static vIcon ellipse(&ellipse_bits[0], ellipse_height, ellipse_width);
 //    = {BitMap, ellipse_height, ellipse_width, 1, &ellipse_bits[0]};

    static vIcon line(&line_bits[0], line_height, line_width);
  //   = {BitMap, line_height, line_width, 1, &line_bits[0]};

    static vIcon point(&point_bits[0], point_height, point_width);
  //   = {BitMap, point_height, point_width, 1, &point_bits[0]};

    static vIcon polygon(&polygon_bits[0], polygon_height, polygon_width);
 //    = {BitMap, polygon_height, polygon_width, 1, &polygon_bits[0]};

const int drawPoint = 0;	// same order as drawShap/drawVal in vdrwcmdw.cxx
const int drawLine = 1;
const int drawRect = 2;
const int drawRdRect = 3;
const int drawEllipse = 4;
const int drawPoly = 5;


const ItemVal lbL1 = 999;
const ItemVal lbL2 = 998;

const ItemVal frShapeBox = 104;
const ItemVal frShape = 105;
const ItemVal icPoint = 110;
const ItemVal icLine = 111;
const ItemVal icRect = 112;
const ItemVal icRdRect = 113;
const ItemVal icEllipse = 114;
const ItemVal icPolygon = 115;

const ItemVal frColors = 150;
const ItemVal frWhich = 151;
const ItemVal cbtPen = 152;
const ItemVal rbPen = 153;
const ItemVal cbPen = 154;
const ItemVal cbtBrush = 155;
const ItemVal rbBrush = 156;
const ItemVal cbBrush = 157;
const ItemVal cbtBG = 158;
const ItemVal rbBG = 159;

const ItemVal btClose = 200;
const ItemVal btApply = 201;


    static vColor clrPen(0,0,0);
    static vColor clrBrush(0,0,0);
    static vColor clrBG(255,255,255);

    static char *penStyles[] =
      {
	"Solid", "Transparent", "Dash", "Dot", "DashDot", 0
      };

    static ItemVal penMap[] =
      {
	m_PenSolid, m_PenTransparent, m_PenDash, m_PenDot, m_PenDashDot, 0
      };

    static char *brushStyles[] =
      {
	"Solid", "Transparent", "Horizontal", "Vertical",
	"Left Diag", "Right Diag", "Cross", "Diag Cross", 0
      };

    static ItemVal brushMap[] =
      {
	m_BrushSolid, m_BrushTransparent, m_BrushHoriz,  m_BrushVert,
	m_BrushLeft, m_BrushRight, m_BrushCross, m_BrushDiagCross, 0
      };

    static CommandObject DefaultCmds[] =
      {
	{C_Label, lbL1, 0,"                           ",NoList,CA_MainMsg,isSens,NoFrame, 0, 0},
	
	//----------- The set color frame ---------------------------------
	{C_Frame, frColors, 0, "", NoList, CA_None, isSens, NoFrame, 0, lbL1},
	{C_Frame, frWhich, 0, "", NoList, CA_NoSpace | CA_NoBorder, isSens,
		frColors, 0, 0},
	{C_ColorButton, cbtPen, 0," ",(void*)&clrPen,CA_None,isSens,
		frWhich, 0, 0},
	{C_RadioButton, rbPen, 1, "Pen  ", NoList, CA_None, isSens,
		frWhich, cbtPen, 0},
	{C_ComboBox, cbPen,0,"",(void*)penStyles, CA_None, isSens,
		frWhich, rbPen, 0},
	{C_ColorButton, cbtBrush, 0," ",(void*)&clrBrush,CA_None,isSens,
		frWhich, 0, cbPen},
	{C_RadioButton, rbBrush, 0, "Brush", NoList, CA_None, isSens,
		frWhich, cbtBrush, cbPen},
	{C_ComboBox, cbBrush,0,"",(void*)brushStyles, CA_None, isSens,
		frWhich, rbBrush, cbPen},
	{C_ColorButton, cbtBG, 0," ",(void*)&clrBG,CA_None,isSens,
		frWhich, 0, cbBrush},
	{C_RadioButton, rbBG, 0, "Background", NoList, CA_None, isSens,
		frWhich, cbtBG, cbBrush},
#define vC_Size 12
#define vC_Frame frColors
#define vC_Below frWhich
#include <v/vcb2x8.h>

	//----------- The shape frame ---------------------------------
//	{C_Frame, frShape, 0, "", NoList, CA_NoSpace | CA_NoBorder, isSens,
	{C_Frame, frShape, 0, "", NoList, CA_NoBorder, isSens,
		frColors, 0, M_ColorFrame},

	{C_ToggleIconButton, icPoint, 0, "", (void*)&point, CA_None, isSens, frShape, 0, 0},
	{C_ToggleIconButton, icLine, 0, "", (void*)&line, CA_None, isSens, frShape, 0,
		icPoint},

	{C_ToggleIconButton, icRect, 1, "", (void*)&rect, CA_None, isSens, frShape,
		icPoint, 0},
	{C_ToggleIconButton, icRdRect, 0, "", (void*)&rdrect, CA_None, isSens, frShape,
		icPoint, icRect},

	{C_ToggleIconButton, icEllipse, 0, "", (void*)&ellipse, CA_None, isSens, frShape,
		icRect, 0},
	{C_ToggleIconButton, icPolygon, 0, "", (void*)&polygon, CA_None, isSens, frShape,
		icRect, icEllipse},
	
	{C_Button, btClose, 0, "Close",NoList, CA_None, isSens, NoFrame,
		0, frColors},
	{C_Button, btApply, 0, "Apply",NoList, CA_None, isSens, NoFrame,
		btClose, frColors},

	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static ItemVal curDrawType = icRect;

//==========================>>> SetCmdObjAttr <<<==============================
  void SetCmdObjAttr(CommandObject* cList, ItemVal id, CmdAttribute newAttr)
  {
    // This is called to change the attrs field of the item with given id

    // scan the entire list

    int ix;
    for (ix = 0 ; cList && (cList[ix].cmdType != C_EndOfList) ; ++ix)
      {
	if (cList[ix].cmdId == id)		// the one we want!
	  {
	    cList[ix].attrs = newAttr;		// change
	    break;
	  }
      }
   }

//==========================>>> SetCmdObjCmdType <<<===========================
  void SetCmdObjCmdType(CommandObject* cList, ItemVal id, CmdType ct)
  {
    // This is called to change the attrs field of the item with given id

    // scan the entire list

    int ix;
    for (ix = 0 ; cList && (cList[ix].cmdType != C_EndOfList) ; ++ix)
      {
	if (cList[ix].cmdId == id)		// the one we want!
	  {
	    cList[ix].cmdType = ct;		// change
	    break;
	  }
      }                                      	// special case - changing last
    if (cList[ix].cmdId == id)
	cList[ix].cmdType = ct;
   }

//=========================>>> vDrawDialog::vDrawDialog <<<====================
 vDrawDialog::vDrawDialog(vBaseWindow* bw, int All) :
    vDialog(bw, 0, "Quick Pick")
  {
    UserDebug(Constructor,"vDrawDialog::vDrawDialog(basewindow)\n")
    toAll = All;
    pWin = (vWindow*) bw;
    init();
  }

//=========================>>> vDrawDialog::vDrawDialog <<<====================
 vDrawDialog::vDrawDialog(vApp* aw) :
    vDialog(aw, 0, "Global Quick Pick")
  {
    UserDebug(Constructor,"vDrawDialog::vDrawDialog(vapp)\n")
    toAll = 1;
    pWin = (vWindow*) NULL;
    init();
  }

//=========================>>> vDrawDialog::vDrawDialog <<<====================
 void vDrawDialog::init()
  {

    if (!toAll)			// State of Close and Apply depend on parent
      {
	SetCmdObjCmdType(DefaultCmds, btClose, C_EndOfList);
      }
    else
      {
	SetCmdObjCmdType(DefaultCmds, btClose, C_Button);
      }

    AddDialogCmds(DefaultCmds);		// add the predefined commands

    setWhatColor = setPen;
    drawWhatShape = drawLine;
    penColorMsg = M_Black;		// message id of current color
    brushColorMsg = M_Black;		// for pen, brush, and bg
    bgColorMsg = M_White;
    penColorVal = vC_Black;		// message id of current color
    brushColorVal = vC_Black;		// for pen, brush, and bg
    bgColorVal = vC_White;
    penStyleMsg = m_PenSolid;		// solid pen
    brushStyleMsg = m_BrushSolid;	// solid brush
  }

//========================>>> vDrawDialog::~vDrawDialog <<<====================
  vDrawDialog::~vDrawDialog()
  {
    UserDebug(Destructor,"vDrawDialog::~vDrawDialog() destructor\n")
  }

//====================>>> vDrawDialog::DialogCommand <<<=======================
  void vDrawDialog::DialogCommand(ItemVal id, ItemVal val, CmdType ctype)
  {
    // After the user has selected a command from the dialog,
    // this routine is called with the value.  This code would be generated
    // by Vigr.

    UserDebug2(CmdEvents,"vDrawDialog::DialogCommand(id:%d, val:%d)\n",id, val)

    switch (id)		// We will do some things depending on value
      {
	// Draw Shape Radio Buttons
	case icPoint:
	    SetValue(curDrawType,0,Value);
	    curDrawType = id;
	    SetValue(curDrawType,1,Value);
	    drawWhatShape = drawPoint;
	    break;
	case icLine:
	    SetValue(curDrawType,0,Value);
	    curDrawType = id;
	    SetValue(curDrawType,1,Value);
	    drawWhatShape = drawLine;
	    break;
	case icRect:
	    SetValue(curDrawType,0,Value);
	    curDrawType = id;
	    SetValue(curDrawType,1,Value);
	    drawWhatShape = drawRect;
	    break;

	case icRdRect:
	    SetValue(curDrawType,0,Value);
	    curDrawType = id;
	    SetValue(curDrawType,1,Value);
	    drawWhatShape = drawRdRect;
	    break;

	case icEllipse:
	    SetValue(curDrawType,0,Value);
	    curDrawType = id;
	    SetValue(curDrawType,1,Value);
	    drawWhatShape = drawEllipse;
	    break;

	case icPolygon:
	    SetValue(curDrawType,0,Value);
	    curDrawType = id;
	    SetValue(curDrawType,1,Value);
	    drawWhatShape = drawPoly;
	    break;

	// Pen/Brush/BG Radio Buttons
	case rbPen:
	    if (val)
	      {
		setWhatColor = setPen;
	      }
	    break;
	case rbBrush:
	    if (val)
	      {
		setWhatColor = setBrush;
	      }
	    break;
	case rbBG:
	    if (val)
	      {
		setWhatColor = setBG;
	      }
	    break;

	case btClose:		// Close the dialog
	  {
	    CloseDialog();
	    break;
	  }

	case cbPen:
	  {
	    penStyleMsg = penMap[val];	// map to correct command
	    break;
	  }

	case cbBrush:
	  {
	    brushStyleMsg = brushMap[val];
	    break;
	  }

	case btApply:		// Apply settings to all windows
	  {
	    if (!toAll)
		break;

	    // Implementing this command requires close coordination
	    // with the vDrawCommandWindow class so messages can be
	    // handled appropriately.

	    // First, set the color of the pen, brush, and background.
	    // Change window to set each one, then send the color
	    theApp->SendWindowCommandAll(m_PenBrushCombo, setPen, C_ComboBox);
	    theApp->SendWindowCommandAll(penColorMsg, penColorVal, C_ColorButton);
	    theApp->SendWindowCommandAll(m_PenBrushCombo, setBrush, C_ComboBox);
	    theApp->SendWindowCommandAll(brushColorMsg, brushColorVal, C_ColorButton);
	    theApp->SendWindowCommandAll(m_PenBrushCombo, setBG, C_ComboBox);
	    theApp->SendWindowCommandAll(bgColorMsg, bgColorVal, C_ColorButton);
	    theApp->SendWindowCommandAll(penStyleMsg, 1, C_Button);
	    theApp->SendWindowCommandAll(brushStyleMsg, 1, C_Button);


	    // And finally set the set mode to whatever ours is, both to
	    // the WindowCommand method, and to the combo box
	    theApp->SendWindowCommandAll(m_PenBrushCombo, setWhatColor, C_ComboBox);
	    theApp->SetValueAll(m_PenBrushCombo, setWhatColor, Value);

	    // Next, set the shape
	    theApp->SendWindowCommandAll(m_DrawCombo,drawWhatShape,C_ComboBox);
	    theApp->SetValueAll(m_DrawCombo,drawWhatShape,Value);

	    break;
	  }

	default:		// set a color...

	    if (id >= M_Black && id <= M_White)  // color button
	      {
		if (setWhatColor == setPen)
		  {
		    penColorMsg = id;
		    penColorVal = val;
		    clrPen.Set(vStdColors[val]);
		    SetValue(cbtPen,(ItemVal)clrPen.r(),Red);
		    SetValue(cbtPen,(ItemVal)clrPen.g(),Green);
		    SetValue(cbtPen,(ItemVal)clrPen.b(),Blue);
		  }
		else if (setWhatColor == setBrush)
		  {
		    brushColorMsg = id;
		    brushColorVal = val;
		    clrBrush.Set( vStdColors[val]);
		    SetValue(cbtBrush,(ItemVal)clrBrush.r(),Red);
		    SetValue(cbtBrush,(ItemVal)clrBrush.g(),Green);
		    SetValue(cbtBrush,(ItemVal)clrBrush.b(),Blue);
		  }
		else
		  {
		    bgColorMsg = id;
		    bgColorVal = val;
		    clrBG.Set(vStdColors[val]);
		    SetValue(cbtBG,(ItemVal)clrBG.r(),Red);
		    SetValue(cbtBG,(ItemVal)clrBG.g(),Green);
		    SetValue(cbtBG,(ItemVal)clrBG.b(),Blue);
		  }
		break;
	      }
      }

    if (!toAll && pWin)		// update what we did
      {

	// Implementing this command requires close coordination
	// with the vDrawCommandWindow class so messages can be
	// handled appropriately.

	// First, set the color of the pen, brush, and background.
	// Change window to set each one, then send the color
	pWin->WindowCommand(m_PenBrushCombo, setPen, C_ComboBox);
	pWin->WindowCommand(penColorMsg, penColorVal, C_ColorButton);
	pWin->WindowCommand(m_PenBrushCombo, setBrush, C_ComboBox);
	pWin->WindowCommand(brushColorMsg, brushColorVal, C_ColorButton);
	pWin->WindowCommand(m_PenBrushCombo, setBG, C_ComboBox);
	pWin->WindowCommand(bgColorMsg, bgColorVal, C_ColorButton);
	pWin->WindowCommand(penStyleMsg, 1, C_Button);
	pWin->WindowCommand(brushStyleMsg, 1, C_Button);

	// And finally set the set mode to whatever ours is, both to
	// the WindowCommand method, and to the combo box
	pWin->WindowCommand(m_PenBrushCombo, setWhatColor, C_ComboBox);
	pWin->SetValue(m_PenBrushCombo, setWhatColor, Value);

	// Next, set the shape
	pWin->WindowCommand(m_DrawCombo,drawWhatShape,C_ComboBox);
	pWin->SetValue(m_DrawCombo,drawWhatShape,Value);
      }

    // All commands should also route through the parent handler
    vDialog::DialogCommand(id,val,ctype);
  }
