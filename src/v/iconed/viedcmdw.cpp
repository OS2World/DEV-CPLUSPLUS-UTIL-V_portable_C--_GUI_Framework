//========================= viedcmdw.cxx ================================
// This is the source file for the command window class.
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//=======================================================================

#include <v/vnotice.h>          // use notice box
#include <v/vfilesel.h>         // for file selection
#include <v/vutil.h>            // use utilities
#include <v/vynreply.h>         // use yes/no reply box
#include <v/vreply.h>           // use reply box
#include <v/vpen.h>             // use a pen
#include <v/vicon.h>            // use v defined icons
#include <v/vmodald.h>

#include <iostream.h>
#include <ctype.h>
 
#include "viedapp.h"
#include "viedcmdw.h"           // our header
#include "viedcnv.h"            // myCanvasPane class
#include "coldlg.h"             // color palette dialog
#include "brshdlg.h"            // brush dialog

//-------------------------------------------------------------
// MENUS and COMMAND BARS
//-------------------------------------------------------------
// Define the File and Edit pulldown menus

// Note - The space padding is necessary for X Athena to come out
// looking right, and it is harmless for Windows
static vMenu FileMenu[] =
  {
    {"&New       ", M_New, isSens, notChk, "Ctrl-N", 'N'-'@', noSub},
    {"&Open...   ", M_Open, isSens, notChk, "Ctrl-O", 'O'-'@', noSub},
    {"&Save      ", M_Save, isSens, notChk, "Ctrl-S", 'S'-'@', noSub},
    {"Save &as...", M_SaveAs, isSens,notChk,noKeyLbl,noKey,noSub},
    {"C&lose     ", m_CloseFile, isSens,notChk,"Ctrl-L",'L'-'@',noSub},
    {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
    {"About V Icon Ed",M_About, isSens,notChk,noKeyLbl,noKey,noSub},
    {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
    {"E&xit      ", M_Exit, isSens, notChk, noKeyLbl, noKey, noSub},
    {NULL}
  };

static vMenu ZoomMenu[] =
  {
    {"&2", m_Zoom2, isSens, notChk, noKeyLbl, noKey,noSub},
    {"&4", m_Zoom4, isSens, notChk, noKeyLbl, noKey,noSub},
    {"&8", m_Zoom8, isSens, isChk, noKeyLbl, noKey,noSub},
    {"&16", m_Zoom16, isSens, notChk, noKeyLbl, noKey,noSub},
    {"&32", m_Zoom32, isSens, notChk, noKeyLbl, noKey,noSub},
    {NULL}
  };

static vMenu EditMenu[] =
  {
    {"&Undo         ", m_Undo, isSens, notChk, "Ctrl-Z", 'Z'-'@',noSub},
    {"&Clear", m_Clear, isSens, notChk, noKeyLbl, noKey,noSub},
    {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
    {"&Resize",m_Resize, isSens,notChk,noKeyLbl, noKey, noSub},
    {NULL}
  };
 
static vMenu DrawMenu[] =
  {
    {"&Point", m_DrawPoint, isSens, isChk, noKeyLbl, noKey,noSub},
    {"&Line", m_DrawLine, isSens, notChk, noKeyLbl, noKey, noSub},
    {"&Rectangle", m_DrawRect, isSens, notChk, noKeyLbl, noKey,noSub},
    {"R&ounded Rectangle", m_DrawRdRect, isSens, notChk, noKeyLbl, noKey,noSub},
    {"&Ellipse", m_DrawEllipse, isSens, notChk, noKeyLbl, noKey,noSub},
    {"Pick &Color", m_DrawDropper, isSens, notChk, noKeyLbl, noKey,noSub},
    {"&Fill", m_DrawFill, isSens, notChk, noKeyLbl, noKey,noSub},
    {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
    {"Refresh Image",m_DrawRefresh, isSens, notChk, noKeyLbl, noKey, noSub},
    {"Show &Grid        ",m_DrawGrid, isSens, isChk, "Ctrl-G", 'G'-'@', noSub},
    {NULL}
  };
 
static vMenu BrushMenu[] =
  {
    {"&Regular Brush", m_DrawPixel, isSens, isChk, noKeyLbl, noKey,noSub},
    {"&Text Brush", m_DrawText, isSens, notChk, noKeyLbl, noKey,noSub},
    {"&Cut/Paste Brush", m_DrawSelect, isSens, notChk, noKeyLbl, noKey,noSub},
    {NULL}
  };

// Define the main menu
vMenu MainMenu[] =
  {
    {"&File",M_File,isSens,notUsed,notUsed,noKey,&FileMenu[0]},
    {"&Edit",M_Edit,isSens,notUsed,notUsed,noKey,&EditMenu[0]},
    {"&Draw",92,isSens,notUsed,notUsed,noKey,&DrawMenu[0]},
    {"&Brush",93,isSens,notUsed,notUsed,noKey,&BrushMenu[0]},
    {"&Zoom",94,isSens,notUsed,notUsed,noKey,&ZoomMenu[0]},
    {NULL}
  };
 

// icon image files for command window toolbar
#include "palett.vbm"
#include "brushes.vbm"
#include "snap.vbm"
#include "clear.vbm"
#include "undo.vbm"

// define command window toolbar
static vIcon palett(&palett_bits[0], palett_height, palett_width, palett_depth,Transparent);
static vIcon brushes(&brushes_bits[0], brushes_height, brushes_width, brushes_depth,Transparent);
static vIcon snap(&snap_bits[0], snap_height, snap_width, snap_depth,Transparent);
static vIcon undo(&undo_bits[0], undo_height, undo_width, undo_depth);
static vIcon clear(&clear_bits[0], clear_height, clear_width, clear_depth);
    
#include "drwbnorm.vbm"
#include "drwbsel.vbm"
#include "drwbabc.vbm"

static vIcon drwbnorm(&drwbnorm_bits[0], drwbnorm_height, drwbnorm_width, drwbnorm_depth,Transparent);
static vIcon drwbsel(&drwbsel_bits[0], drwbsel_height, drwbsel_width, drwbsel_depth,Transparent);
static vIcon drwbabc(&drwbabc_bits[0], drwbabc_height, drwbabc_width, drwbabc_depth,Transparent);

static vColor fgBtnColor(0,0,0);
static vColor bgBtnColor(255,255,255);

// Define the tool command bar
static CommandObject CommandBar[] =
  {
    {C_Frame, m_fgbgFrame,0,"",NoList, CA_NoBorder | CA_NoSpace,isSens,NoFrame,0,0},
    {C_ColorButton,m_fgColor,0," ",(void*)&fgBtnColor,CA_None,isSens,m_fgbgFrame,0,0,
	20,"Current Foreground Color"},
    {C_ColorButton,m_bgColor,0," ",(void*)&bgBtnColor,CA_None,isSens,m_fgbgFrame,m_fgColor,
	0,20,"Current Background Color"},

    {C_Blank,999,0," ",NoList, CA_None,isSens,NoFrame,0,0},

    {C_ToggleIconButton, m_Snap, 0, "Snap", (void*)&snap,
        CA_None,isSens,NoFrame,0,0, 0, "Snap to square or circle"},

    {C_ToggleIconButton, m_BrshBox, 0, "BrushBox", (void*)&brushes,
        CA_None,isSens,NoFrame,0,0, 0, "Toggle Brush Selection Dialog"},

    {C_ToggleIconButton, m_ColorPalette, 0, "Palette", (void*)&palett,
        CA_None,isSens,NoFrame,0,0,0, "Toggle Color Palette Dialog"},

    {C_Blank,999,0," ",NoList, CA_None,isSens,NoFrame,0,0},
    {C_Frame, m_brushFrame,0,"",NoList, CA_NoSpace,isSens,NoFrame,0,0},
    {C_ToggleIconButton, m_DrawPixel, 1, "Norm Brush", (void*)&drwbnorm,
        CA_None,isSens,m_brushFrame,0,0, 0, "Use Shaped Brush"},
    {C_ToggleIconButton, m_DrawText, 0, "Text Brush", (void*)&drwbabc,
        CA_None,isSens,m_brushFrame,m_DrawPixel,0,0,"Use Text Brush"},
    {C_ToggleIconButton, m_DrawSelect, 0, "Select Brush", (void*)&drwbsel,
        CA_None,isSens,m_brushFrame,m_DrawText,0, 0, "Cut/Paste Brush"},


    {C_EndOfList,0,0,0,0,CA_None,0,0,0}
  };
 

// Shapes and tools icon images
#include "rect.vbm"
#include "rdrect.vbm"
#include "ellipse.vbm"
#include "line.vbm"
#include "point.vbm"
#include "fill.vbm"

#include "dropper.vbm"
 
// Shapes and tools icon declarations
static vIcon rect(&rect_bits[0], rect_height, rect_width, rect_depth);
static vIcon rdrect(&rdrect_bits[0], rdrect_height, rdrect_width, rdrect_depth);
static vIcon ellipse(&ellipse_bits[0], ellipse_height, ellipse_width, ellipse_depth);
static vIcon line(&line_bits[0], line_height, line_width, line_depth);
static vIcon point(&point_bits[0], point_height, point_width, point_depth);
static vIcon fill(&fill_bits[0], fill_height, fill_width, fill_depth);
static vIcon dropper(&dropper_bits[0], dropper_height, dropper_width, dropper_depth);

// Define shapes and tools icon item values
const ItemVal frShapeBox = 104;
const ItemVal frShape = 105;
 
  static CommandObject ToolBar[] =
    {
    //----------- The shape frame ---------------------------------
    {C_IconButton, m_Clear, 0, "Clear", (void*)&clear,
        CA_None,isSens,NoFrame,0,0,0,"Clear to Background color"},
    {C_IconButton, m_Undo, 0, "Undo", (void*)&undo,
        CA_None,isSens,NoFrame,0,0,0,"Undo last action"},

    {C_Frame, frShape, 0, "", NoList, CA_NoSpace, isSens, NoFrame, 0, lbL1},

    {C_ToggleIconButton, m_DrawPoint, 1, "", (void*)&point, CA_None, isSens, 
        frShape, 0, 0,0,"Draw Points"},
    {C_ToggleIconButton, m_DrawLine, 0, "", (void*)&line, CA_None, isSens, 
        frShape, m_DrawPoint, 0,0,"Draw Line"},
    {C_ToggleIconButton, m_DrawRect, 0, "", (void*)&rect, CA_None, isSens,   
        frShape, m_DrawLine, 0,0,"Draw Rectangle"},
    {C_ToggleIconButton, m_DrawRdRect, 0, "", (void*)&rdrect, CA_None, isSens,
        frShape, m_DrawRect, 0,0,"Draw Rounded Rectangle"},
    {C_ToggleIconButton, m_DrawEllipse, 0, "", (void*)&ellipse, CA_None, isSens,
        frShape, m_DrawRdRect,0,0,"Draw Ellipse"},
    {C_ToggleIconButton, m_DrawDropper, 0, "", (void*)&dropper, CA_None, isSens,
        frShape, m_DrawEllipse, 0,0,"Select color from current icon"},
    {C_ToggleIconButton, m_DrawFill, 0, "", (void*)&fill, CA_None, isSens,
        frShape, m_DrawDropper, 0, 0,"Area Fill"},
    {C_EndOfList,0,0,0,0,CA_None,0,0,0}
    };
 
// Define the Status bar
vStatus StatBar[] =
  {
    {"X,Y: ", m_MousePosLbl, CA_NoBorder, isSens, 0 },
    {" 000 ", m_MousePosX, CA_None, isSens, 0},
    {" 000 ", m_MousePosY, CA_None, isSens, 0},
    {NULL,0,0,0,0}
  };

#include "about.vbm"
static vIcon about(&about_bits[0], about_height, about_width, about_depth);
static CommandObject About[] =
  {
    {C_Icon,800,0,"",&about,CA_None,isSens,0,0,0},
    {C_Label,802,0,"   V Icon Editor Version" ,NoList,CA_None,isSens,0,800,0},
    {C_Label,803,0,viedVers ,NoList,CA_None,isSens,0,802,0},
    {C_Label,810,0,"Developed by Philip Eckenroth, Mike Tipping,",
	NoList,CA_None,isSens,0,0,800},
    {C_Label,820,0,"Marilee Padilla, and John Fredric Jr. Masciantoni,",
	NoList,CA_None,isSens,0,0,810},
    {C_Label,830,0,"for UNM CS-460 Software Engineering, Spring 1996.",
	NoList,CA_None,isSens,0,0,820},
    {C_Label,840,0,"Enhancements by Bruce E. Wampler. " ,
	NoList,CA_None,isSens,0,0,830},

    {C_Button,M_OK,0," OK ", NoList,CA_DefaultButton,isSens,0,840,840},
    {C_EndOfList,0,0,0,0,CA_None,0,0,0}
  };

static char* filter[] = {"*.vbm;*.bmp;*.xbm;*.xpm",
"*.vbm","*.bmp","*.xbm", "*.xpm","*", 0};   // file filter
static int filterIndex = 0;
 
//================>>> myCmdWindow::myCmdWindow <<<================
  myCmdWindow::myCmdWindow(VCONST char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {
    // Create and add the proper panes to the CmdWindow

    *_fname = 0;                        // no name
    _fgbrush.SetColor(0,0,0);           // black brush
    _pen.SetColor(0,0,0);               // black pen
    _bg.Set(255,255,255);               // white background
    curWidth = 1;                       // brush width
    _mx = _my = 0;
    brushMenuId = m_BrushSolid;
    penMenuId = m_PenSolid;
 
    myMenu = new vMenuPane(MainMenu);           // Add the main menu
    AddPane(myMenu);
 
    myCmdPane = new vCommandPane(CommandBar);   // Add command pane
    AddPane(myCmdPane);
 
    myToolPane = new vCommandPane(ToolBar);     // Add command pane
    AddPane(myToolPane);
 
    myCmdWindow* me  = this;
    myCanvas = new myCanvasPane(me);            // Add the canvas pane

    AddPane(myCanvas);

    myCanvas->SetDrawShape(m_DrawPoint);        // We will be drawing line
    curShape = m_DrawPoint;
    
    // Add the Status bar pane
    myStatus = new vStatusPane(StatBar); // a new status bar
    AddPane(myStatus);
 
    myColDlg = 0;                       // all dialog windows are closed
    myBrshDlg = 0;

    ShowWindow();           // FINALLY, we must show the window!
 
    myCanvas->ShowVScroll(1);  // And, now turn on scroll bars
    myCanvas->ShowHScroll(1);
    myCanvas->SetDrawShape(m_DrawPoint);
  }
 
//===============>>> myCmdWindow::~myCmdWindow <<<================
  myCmdWindow::~myCmdWindow()
  {
    // Destory the command window and everything in it

    delete myMenu;      // Delete main menu
    delete myCanvas;    // Delete the canvas
    delete myCmdPane;   // Delete the command pane
    delete myToolPane;  // Delete the Tool pane
    delete myStatus;    // Delete the status bar
    if (myColDlg)       // if color palette is open, delete it
        delete myColDlg;         
    if (myBrshDlg)       // if brushes box is open, delete it
        delete myBrshDlg;         
    
  }

//===================>>> myCmdWindow::SetFGBtn <<<====================
  void myCmdWindow::SetFGBtn(vColor& color)
  {
    fgBtnColor = color;
    SetValue(m_fgColor,0,ChangeColor);
  }

//===================>>> myCmdWindow::SetBGBtn <<<====================
  void myCmdWindow::SetBGBtn(vColor& color)
  {
    bgBtnColor = color;
    SetValue(m_bgColor,0,ChangeColor);
  }

//===================>>> myCmdWindow::GetText <<<====================
  int myCmdWindow::GetText(void)
  {
    // Opens a reply dialog box, which allows the user to enter text.
    // The text is then read from the input box, and the text is placed
    // in the current brush.

    vReplyDialog rp(this);
    char r[25];            // a buffer for the reply
    char temp_str[256];
    char letter;
    int location = 0;    // this gives x location only - top left of letters

    if ( rp.Reply("Enter Text for brush:",r,25) == M_Cancel)
        return 0;

    myCanvas->_myPicture->Clear(BrushLayer, myCanvas->_myColorPal->getbg());

    while (strlen(r) > 0)
      {
        letter = r[0];
        strcpy(temp_str, r+1);
        strcpy(r, temp_str);
        location = write_text(letter, location);

        // add the call to add the letter to the brush layer here
      }
    myCanvas->_BrushOffX = 0;
    myCanvas->_BrushOffY = 0;
    myCanvas->_BrushWidth = location;
    myCanvas->_BrushHeight = 7;
    return 1;
  }

//===================>>> myCmdWindow::write_text <<<=====================
  int myCmdWindow::write_text(char letter, int location)
  {
    // Converts text input into a bit sequence, and draws it on the canvas

    char bit_seq[46];   // text bit sequence
    int pos;
    int width = 5;      // width of our custom font
    int height = 7;     // height of our custom font
    char cur_bit;

    if ( (location + width) > myCanvas->_myPicture->GetPicWidth())  
      return location;

    switch(letter)
    {
      case 'A' : 
      {
        strcpy(bit_seq, "01110100011000111111100011000110001"); 
        break; 
      }
      case 'B' : 
      {
        strcpy(bit_seq, "11110100011000111110100011000111110"); 
        break;
      }
      case 'C' : 
      {
        strcpy(bit_seq, "01110100011000010000100001000101110"); 
        break;
      }
      case 'D' : 
      {
        strcpy(bit_seq, "11110100011000110001100011000111110"); 
        break;
      }
      case 'E' : 
      {
        strcpy(bit_seq, "11111100001000011110100001000011111"); 
        break;
      }
      case 'F' : 
      {
        strcpy(bit_seq, "11111100001000011110100001000010000"); 
        break;
      }
      case 'G' : 
      {
        strcpy(bit_seq, "01110100011000010000100111000101110"); 
        break;
      }
      case 'H' : 
      {
        strcpy(bit_seq, "10001100011000111111100011000110001"); 
        break;
      }
      case 'I' : 
      {
        strcpy(bit_seq, "11111001000010000100001000010011111"); 
        break;
      }
      case 'J' : 
      {
        strcpy(bit_seq, "11111001000010000100001001010011000"); 
        break;
      }
      case 'K' : 
      {
        strcpy(bit_seq, "10001100101010011000101001001010001"); 
        break;
      }
      case 'L' : 
      {
        strcpy(bit_seq, "10000100001000010000100001000011111"); 
        break;
      }
      case 'M' : 
      {
        strcpy(bit_seq, "10001110111010110101100011000110001"); 
        break;
      }
      case 'N' : 
      {
        strcpy(bit_seq, "10001110011010110101101011001110001"); 
        break;
      }
      case 'O' : 
      {
        strcpy(bit_seq, "01110100011000110001100011000101110"); 
        break;
      }
      case 'P' : 
      {
        strcpy(bit_seq, "11110100011000111110100001000010000"); 
        break;
      }
      case 'Q' : 
      {
        strcpy(bit_seq, "01110100011000110001101011001001101"); 
        break;
      }
      case 'R' : 
      {
        strcpy(bit_seq, "11110100011000111110101001001010001"); 
        break;
      }
      case 'S' : 
      {
        strcpy(bit_seq, "01110100011000001110000011000101110"); 
        break;
      }
      case 'T' : 
      {
        strcpy(bit_seq, "11111001000010000100001000010000100"); 
        break;
      }
      case 'U' : 
      {
        strcpy(bit_seq, "10001100011000110001100011000101110"); 
        break;
      }
      case 'V' : 
      {
        strcpy(bit_seq, "10001100010101001010010100010000100"); 
        break;
      }
      case 'W' : 
      {
        strcpy(bit_seq, "10001100011000110101101011010101110"); 
        break;
      }
      case 'X' : 
      {
        strcpy(bit_seq, "10001010100101000100010100101010001"); 
        break;
      }
      case 'Y' : 
      {
        strcpy(bit_seq, "10001010100101000100001000010000100"); 
        break;
      }
      case 'Z' : 
      {
        strcpy(bit_seq, "11111100010001000100010001000111111"); 
        break;
      }
      case '0' : 
      {
        strcpy(bit_seq, "01110100011000110001100011000101110"); 
        break;
      }
      case '1' : 
      {
        strcpy(bit_seq, "00100011001010000100001000010011111"); 
        break;
      }
      case '2' : 
      {
        strcpy(bit_seq, "01110100010000100010001000100011111"); 
        break;
      }
      case '3' : 
      {
        strcpy(bit_seq, "01110100010000100110000011000101110"); 
        break;
      }
      case '4' : 
      {
        strcpy(bit_seq, "10001100011000111111000010000100001"); 
        break;
      }
      case '5' : 
      {
        strcpy(bit_seq, "11111100001000011110000010000111110"); 
        break;
      }
      case '6' : 
      {
        strcpy(bit_seq, "00001000100010001110100011000101110"); 
        break;
      }
      case '7' : 
      {
        strcpy(bit_seq, "11111000010000100010001000100010000"); 
        break;
      }
      case '8' : 
      {
        strcpy(bit_seq, "01110100011000101110100011000101110"); 
        break;
      }
      case '9' : 
      {
        strcpy(bit_seq, "01110100011000101110000100010001000"); 
        break;
      }
      case '-' : 
      {
        strcpy(bit_seq, "00000000000000011111000000000000000"); 
        break;
      }
      case '_' : 
      {
        strcpy(bit_seq, "00000000000000000000000000000011111"); 
        break;
      }
      case '=' : 
      {
        strcpy(bit_seq, "00000000001111100000111110000000000"); 
        break;
      }
      case '+' : 
      {
        strcpy(bit_seq, "00000001000010011111001000010000000"); 
        break;
      }
      case '!' : 
      {
        strcpy(bit_seq, "00100001000010000100001000000000100"); 
        break;
      }
      case '@' : 
      {
        strcpy(bit_seq, "01110100011001110101101101000001110"); 
        break;
      }
      case '#' : 
      {
        strcpy(bit_seq, "00000010101111101010111110101000000"); 
        break;
      }
      case '$' : 
      {
        strcpy(bit_seq, "00100011101010001110001010111000100"); 
        break;
      }
      case '%' : 
      {
        strcpy(bit_seq, "01001101010101000100010101010110010"); 
        break;
      }
      case '^' : 
      {
        strcpy(bit_seq, "00100010101000100000000000000000000"); 
        break;
      }
      case '&' : 
      {
        strcpy(bit_seq, "01000101001010001000101011001001101"); 
        break;
      }
      case '*' : 
      {
        strcpy(bit_seq, "00000101010111011111011101010100000"); 
        break;
      }
      case '(' : 
      {
        strcpy(bit_seq, "00100010001000010000100000100000100"); 
        break;
      }
      case ')' : 
      {
        strcpy(bit_seq, "00100000100000100001000010001000100"); 
        break;
      }
      case '~' : 
      {
        strcpy(bit_seq, "01001101011001000000000000000000000"); 
        break;
      }
      case '`' : 
      {
        strcpy(bit_seq, "01100001000001000000000000000000000"); 
        break;
      }
      case '[' : 
      {
        strcpy(bit_seq, "01110010000100001000010000100001110"); 
        break;
      }
      case ']' : 
      {
        strcpy(bit_seq, "01110000100001000010000100001001110"); 
        break;
      }
      case '{' : 
      {
        strcpy(bit_seq, "00110010000010011000001000100000110"); 
        break;
      }
      case '}' : 
      {
        strcpy(bit_seq, "01100000100010000011001000001001100"); 
        break;
      }
      case ':' : 
      {
        strcpy(bit_seq, "00100011100010000000001000111000100"); 
        break;
      }
      case ';' : 
      {
        strcpy(bit_seq, "00100011100010000000001100010001000"); 
        break;
      }
      case '"' : 
      {
        strcpy(bit_seq, "01010010100101000000000000000000000"); 
        break;
      }
      case '\'' : 
      {
        strcpy(bit_seq, "00110001000100000000000000000000000"); 
        break;
      }
      case '<' : 
      {
        strcpy(bit_seq, "00000000110110010000011000001100000"); 
        break;
      }
      case '>' : 
      {
        strcpy(bit_seq, "00000110000011000001001101100000000"); 
        break;
      }
      case ',' : 
      {
        strcpy(bit_seq, "00000000000000000000001100010001000"); 
        break;
      }
      case '.' : 
      {
        strcpy(bit_seq, "00000000000000000000001000111000100"); 
        break;
      }
      case '/' : 
      {
        strcpy(bit_seq, "00001000010001000100010001000010000"); 
        break;
      }
      case '\\' : 
      {
        strcpy(bit_seq, "10000100000100000100000100000100001"); 
        break;
      }
      case '|' : 
      {
        strcpy(bit_seq, "00100001000010000000001000010000100"); 
        break;
      }
      case '?' : 
      {
        strcpy(bit_seq, "01110100010001000100001000000000100"); 
        break;
      }
      case 'a' : 
      {
        strcpy(bit_seq, "00000000000111000001011111000101111"); 
        break;
      }
      case 'b' : 
      {
        strcpy(bit_seq, "10000100001011011001100011100110110"); 
        break;
      }
      case 'c' : 
      {
        strcpy(bit_seq, "00000000000111010001100001000101110"); 
        break;
      }
      case 'd' : 
      {
        strcpy(bit_seq, "00001000010110110011100011001101101"); 
        break;
      }
      case 'e' : 
      {
        strcpy(bit_seq, "00000000000111010001111111000001110"); 
        break;
      }
      case 'f' : 
      {
        strcpy(bit_seq, "00110010010100011110010000100001000"); 
        break;
      }
      case 'g' : 
      {
        strcpy(bit_seq, "01101100100110010000011101000101110"); 
        break;
      }
      case 'h' : 
      {
        strcpy(bit_seq, "10000100001011011001100011000110001"); 
        break;
      }
      case 'i' : 
      {
        strcpy(bit_seq, "00100000000110000100001000010001110"); 
        break;
      }
      case 'j' : 
      {
        strcpy(bit_seq, "00010000000001000010100101001001100"); 
        break;
      }
      case 'k' : 
      {
        strcpy(bit_seq, "10000100001000110010111001001010001"); 
        break;
      }
      case 'l' : 
      {
        strcpy(bit_seq, "01100001000010000100001000010001110"); 
        break;
      }
      case 'm' : 
      {
        strcpy(bit_seq, "00000000001101010101101011010110001"); 
        break;
      }
      case 'n' : 
      {
        strcpy(bit_seq, "00000000001011011001100011000110001"); 
        break;
      }
      case 'o' : 
      {
        strcpy(bit_seq, "00000000000111010001100011000101110"); 
        break;
      }
      case 'p' : 
      {
        strcpy(bit_seq, "10110110011100110110100001000010000"); 
        break;
      }
      case 'q' : 
      {
        strcpy(bit_seq, "01101100111001101101000010000100001"); 
        break;
      }
      case 'r' : 
      {
        strcpy(bit_seq, "00000000001011011001100001000010000"); 
        break;
      }
      case 's' : 
      {
        strcpy(bit_seq, "00000000000111010000011100000111110"); 
        break;
      }
      case 't' : 
      {
        strcpy(bit_seq, "01000010001111001000010000100100110"); 
        break;
      }
      case 'u' : 
      {
        strcpy(bit_seq, "00000000001000110001100011001101101"); 
        break;
      }
      case 'v' : 
      {
        strcpy(bit_seq, "00000000001000110001010100101000100"); 
        break;
      }
      case 'w' : 
      {
        strcpy(bit_seq, "00000000001000110001101011010101010"); 
        break;
      }
      case 'x' : 
      {
        strcpy(bit_seq, "00000000001000101010001000101010001"); 
        break;
      }
      case 'y' : 
      {
        strcpy(bit_seq, "10001100011001101101000011000101110"); 
        break;
      }
      case 'z' : 
      {
        strcpy(bit_seq, "00000000001111100010001000100011111"); 
        break;
      }
      case ' ' : 
      {
        strcpy(bit_seq, "00000000000000000000000000000000000"); 
        break;
      }
   }

   for (int y=0; y < height; y++)
   {
      for (int x=0; x < width; x++)
      {
         pos = y * width + x;
         cur_bit = bit_seq[pos];
         if (cur_bit == '1')
            myCanvas->_myPicture->SetIndex(location+x, y, 2, -99);
      }
   }

   return location + width + 1;
}
 
//==============>>> myCmdWindow::OpenFile <<<================
  void myCmdWindow::OpenFile(char* fname)
  {
    // Opens a given file

    char fn[256];               // scratch copy
    int ix;
 
    if (*_fname)                // already have one!
        return;
    
    for (ix = 0 ; fname[ix] != 0 && ix < 255 ; ++ix)
      fn[ix] = fname[ix];       // safe copy
    fn[ix] = 0;
 
    if (!*fn)                   // no file open yet
      {
        vFileSelect fsel(this); // V file select dialog
        if (!fsel.FileSelect("Open V Icon File",
            fn,255,filter,filterIndex) || !*fn)
            return;
      }
 
    if (!myCanvas->Read(fn))    // Save in _fname
      {
        vNoticeDialog note(this);       // for user notification
        note.Notice("Unable to open file.");
        return;
      }

    if (myColDlg)
	myColDlg->UpdatePalette(myCanvas->_myColorPal->_Palette, 
	      myCanvas->_myColorPal->ColorsUsed());
    
    myCanvas->setfg(0);
    myCanvas->setbg(1);
    
    WindowCommand(m_DrawPixel,1,C_Button);
    WindowCommand(m_DrawPoint,1,C_Button);

    SetTitle(fn);               // show on title bar
    strcpy(_fname,fn);          // keep copy
    myCanvas->Redraw(0,0,0,0);  // paint it
    myCanvas->SetChanged(0);    // not really changed
  }
 
//==============>>> myCmdWindow::CheckClose <<<================
  int myCmdWindow::CheckClose()
  {
    // Checks to see if the user wants to save changes, if changes have been
    // done, before it closes the application.
    // Return 1 if ok to close now, 0 to abort
 
    if (myCanvas->Changed())    // changes have been made
      {
        vYNReplyDialog ynr(this);
        int ans = ynr.AskYN("Save new or changed drawing?");
        if (ans == 0)
          {
            myCanvas->SetChanged(0);
            return 1;           // don't want to save
          }
        if (ans == -1)          // cancel
            return 0;
 
        // ok, want to save changed file
        if (*_fname)                    // have a name
          {
            if (!myCanvas->Save(_fname))        // Save in _fname
              {
                vNoticeDialog note(this);       // for user notification
                note.Notice("Unable to save file");
                return 0;
              }
          }
        else                            // need to request name
          {
            vFileSelect fsel(this);     // V file select dialog
 
            if (!fsel.FileSelectSave("Save icon file as",
                _fname,99,filter,filterIndex) || !*_fname)
            return 0;
 
            if (!myCanvas->Save(_fname))        // Save in _fname
              {
                vNoticeDialog note(this);       // for user notification
                note.Notice("Unable to save file");
                return 0;
              }
          }
      }
    return 1;
  }
 
//==============>>> myCmdWindow::MouseXYStatus <<<================
  void myCmdWindow::MouseXYStatus(int x, int y)
  {
    // Get the current X, Y status of the mouse

    char buff[12];
 
    _mx = x; _my = y;
    IntToStr(x,buff);   // convert x coordinate to a string
    SetString(m_MousePosX,buff);
    IntToStr(y,buff);   // convert y coordinate to a string
    SetString(m_MousePosY,buff);
  }
 
//==============>>> myCmdWindow::GetDigit  <<<====================
  int myCmdWindow::GetDigit(char * digitstring)
  {
    // Get a digit from the string and make sure it is writable

    int add = 0;
    int x = 0;

    while (isdigit(digitstring[x]))
    {
      add = (add * 10) + (digitstring[x] - '0');
      x++;
    }
    return add;
  }
 
//====================>>> myCmdWindow::GetWH <<<==========================
  int myCmdWindow::GetWH(int& width, int& height)
  {
    // instantiate a dialog to ask for size of new icon.

    vReplyDialog rp(this);
    char r[32], tempr[32];        // a buffer for the reply

    r[0] = 0;
    if (rp.Reply("Please enter WIDTH and HEIGHT",r,30) == M_Cancel)
	return 0;

    if (r[0] == 0)
	return 0;

    // parse the returned size string - clean out crap and get
    // just the numbers.  If both nums are not there, punt and go
    // with the standard defaults settings.

    while (!(isdigit(r[0])) && (strlen(r) > 0))
      {
	strcpy(tempr, r+1);
	strcpy(r, tempr);
      }
    width = GetDigit(r);
    while (isdigit(r[0]) && (strlen(r) > 0))
      {
	strcpy(tempr, r+1);
	strcpy(r, tempr);
      }

    while (!(isdigit(r[0])) && (strlen(r) > 0))
      {
	strcpy(tempr, r+1);
	strcpy(r, tempr);
      }
    height = GetDigit(r);

    // Check to see that nums are within ranges 
    if (height > 150 || width > 150)
      {
        vNoticeDialog note(this);       // for user notification
        note.Notice("Maximum icon size is 150x150.");
	return 0;
      }
    return 1;
  }


//====================>>> myCmdWindow::WindowCommand <<<==========================
  void myCmdWindow::WindowCommand(ItemVal id, ItemVal val, CmdType cType)
  {
    // Implements the commands given from buttons and menu from the command window

    int width, height;

    // route all commands through here - menus and buttons
 
    vNoticeDialog note(this);   // for user notification
 
    switch (id)         // switch on id of commands
      {
        case M_New:     // start new icon 
          {
            if (!CheckClose())           // ok to close
                break;

	    if (!GetWH(width,height))
		break;

            // make call for a new canvas of size - default is height = 0
            if (height == 0)
                myCanvas->NewCanvas();
            else
                myCanvas->NewCanvas(width, height);

	    myCanvas->setfg(0);
	    myCanvas->setbg(1);

	    WindowCommand(m_DrawPixel,1,C_Button);
	    WindowCommand(m_DrawPoint,1,C_Button);

            myCanvas->ClearShapes();
	    if (myColDlg)
		myColDlg->UpdatePalette(myCanvas->_myColorPal->_Palette, 
	    	    myCanvas->_myColorPal->ColorsUsed());

            break;
          }

        case M_Open:                    // open existing file
          {
            if (*_fname || myCanvas->Changed()) // need to create a new window
              {
                // create new window, then open a file in it.
                myCmdWindow* cw = (myCmdWindow*)
                theApp->NewAppWin(0,"V Icon Edit",400,200);
                cw->OpenFile("");       // open via dialog
                cw->RaiseWindow();      // bring to front
              }
            else
                OpenFile("");           // open a file using dialog

	    myCanvas->setfg(0);
	    myCanvas->setbg(1);

	    WindowCommand(m_DrawPixel,1,C_Button);
	    WindowCommand(m_DrawPoint,1,C_Button);
            break;
          }
 
        case m_CloseFile:               // Close current file
          {
            if (!CheckClose())           // ok to close
                break;
	    myCanvas->ClearShapes();
	    myCanvas->Redraw(0,0,0,0);
	    *_fname = 0;
	    SetTitle("Icon Edit - No Name");   // no file
	    break;
          }
 
        case M_Save:                    // save current drawing
          {
            if (*_fname)                // already have a name
              {
                if (!myCanvas->Save(_fname))    // Save in _fname
		  {
                    note.Notice("Unable to save file");
		  }
                else if (id == m_CloseFile)
                  {
                    *_fname = 0;
                    SetTitle("Icon Edit - No Name");       // no file
                  }
                break;
              }
            // else fall through to SaveAs
          }

        case M_SaveAs:                  // save file as
          {
            vFileSelect fsel(this);     // V file select dialog
 
            if (!fsel.FileSelectSave("Save icon file as",
              _fname,99,filter,filterIndex) || !*_fname)
                break;                  // ignore if no selection
 
            if (!myCanvas->Save(_fname)) // Save in _fname
              {
                note.Notice("Unable to save file");
                break;
              }
            if (id == m_CloseFile)
              {
                *_fname = 0;
                SetTitle("Icon Edit - No Name");   // no file
              }
            else
                SetTitle(_fname);               // change title
            break;
          }
 
	case M_About:
	  {
	    vModalDialog about(this,"About V Icon Editor");
	    ItemVal dummy;
	    about.AddDialogCmds(About);
	    about.ShowModalDialog("",dummy);
	    break;
	  }

        case M_Exit:                    // quit all
          {
            if (CheckClose())           // ok to close
                theApp->Exit(); // Standard action for Exit
            break;
          }
 
        case m_Undo:
          {
            myCanvas->Undo();
            break;
          }
 
        case m_Clear:                   // clear the screen
          {
            myCanvas->ClearShapes();
	    myCanvas->Redraw(0,0,0,0);
	    myCanvas->SetChanged(1);
            break;
          }
 
	case m_Resize:
          {
  	    int h, w;
	    if (!GetWH(w,h))
		break;
	    myCanvas->ResizeCanvas(w,h);

	    WindowCommand(m_DrawPixel,1,C_Button);
	    WindowCommand(m_DrawPoint,1,C_Button);

            break;
          }

	case m_Zoom2:			// note: these values MUST be 700-732
	case m_Zoom4:
	case m_Zoom8:
	case m_Zoom16:
	case m_Zoom32:
	  {
	    SetValue(m_Zoom2,0,Checked);
	    SetValue(m_Zoom4,0,Checked);
	    SetValue(m_Zoom8,0,Checked);
	    SetValue(m_Zoom16,0,Checked);
	    SetValue(m_Zoom32,0,Checked);
	    SetValue(id,1,Checked);		// turn on ours
	    myCanvas->_PixSize = id - m_Zoom0;
	    myCanvas->ResetScroll();		// reset any scrolling
	    myCanvas->Clear();
	    myCanvas->Redraw(0,0,0,0);
	    break;
	  }

        case m_DrawDropper:
        case m_DrawLine:                // set what we are drawing
	case m_DrawPoint:
        case m_DrawRect:
        case m_DrawRdRect:
        case m_DrawEllipse:
        case m_DrawFill:
          {
            SetValue(curShape,0,Checked);       // fix the menu check
            curShape = id;
            SetValue(curShape,1,Checked);
            myCanvas->SetDrawShape(id);
            break;
          }
 
        case m_DrawText:                        // Text Brush
          {
            if (myCanvas->_cwin->GetText())     // Read text
              {
                SetValue(m_DrawText,1,Checked);
                SetValue(m_DrawSelect,0,Checked);
                SetValue(m_DrawPixel,0,Checked);
                myCanvas->SetDrawShape(id);
              }
            else
              {
                SetValue(m_DrawText,0,Checked);
                SetValue(m_DrawSelect,0,Checked);
                SetValue(m_DrawPixel,1,Checked);
                myCanvas->SetDrawShape(m_DrawPixel);
              }

//            myCanvas->Redraw(0,0,0,0);  // paint it
            break;
          }

        case m_DrawSelect:
          {
            SetValue(m_DrawText,0,Checked);
            SetValue(m_DrawSelect,1,Checked);
            SetValue(m_DrawPixel,0,Checked);
	    myCanvas->SetDrawShape(id);
//            myCanvas->Redraw(0,0,0,0);  // paint it
            break;
          }

        case m_DrawPixel:
          {
            SetValue(m_DrawText,0,Checked);
            SetValue(m_DrawSelect,0,Checked);
            SetValue(m_DrawPixel,1,Checked);

            myCanvas->SetDrawShape(id);
//            myCanvas->Redraw(0,0,0,0);  // paint it
            break;
          }

        case m_ColorPalette:            // open the color palette dialog
          {
            if (val)
              {
                if (!myColDlg)          // not created yet
                    myColDlg = new vColorDialog(this, 0, myCanvas);
		myColDlg->UpdatePalette(myCanvas->_myColorPal->_Palette,
		    myCanvas->_myColorPal->ColorsUsed());
                myColDlg->ShowDialog("Color Selection");
                int l,t,w,h;
                GetPosition(l,t,w,h);   // my position
                myColDlg->SetDialogPosition(l+w+2,t);   // put quick pick here
              }
            else                        // close the dialog
              {
                myColDlg->CloseDialog();
              }
            break;
          }
 
	case m_DrawGrid:
	  {
	    myCanvas->_drawGrid = !myCanvas->_drawGrid;  // toggle
	    SetValue(m_DrawGrid,myCanvas->_drawGrid,Checked); // fix menu
	    myCanvas->Clear();
	    myCanvas->Redraw(0,0,0,0);  // paint it
	    break;
	  }

	case m_DrawRefresh:
	  {
	    myCanvas->Clear();
	    myCanvas->Redraw(0,0,0,0);  // paint it
	    break;
	  }

        case m_BrshBox:                 // open the brush box dialog
          {
            if (val)
              {
                if (!myBrshDlg)          // not created yet
                    myBrshDlg = new vBrshDialog(this, 0, myCanvas);
                myBrshDlg->ShowDialog("*Brushes*");
                int l,t,w,h;
                GetPosition(l,t,w,h);   // my position
                myBrshDlg->SetDialogPosition(l+w+2,t+280);
              }
            else                        // close the dialog
              {
                myBrshDlg->CloseDialog();
              }
            break;
          }
 
         case m_Snap:                   // enable the Snap utility
         {
           if (myCanvas->_snap == 0)    
               myCanvas->_snap = 1;
           else
               myCanvas->_snap = 0;
         }

        default:                        // change pen color?
          {
            vCmdWindow::WindowCommand(id, val, cType);
            break;
          }
      }
  }
