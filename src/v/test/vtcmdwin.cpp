//=======================================================================
//  vtcmdwin.cxx - testCmdWindow class
//  Copyright (C) 1995,1996, 1997, 1998  Bruce E. Wampler
//
//  This program is part of the V C++ GUI Framework example programs.
//
//  This program is free software; you can redistribute it and/or gmodify
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

//#define ALLTESTS	// for ugly list-in-a-command-bar test

#include <string.h>

#include <v/vcanvas.h>  // for Cursors
#include <v/vcolor.h>
#include <v/vicon.h>
#include <v/vutil.h>
#include <v/vfontsel.h>
#include <v/vfont.h>
#include <v/vprinter.h>

#include <v/vmemdc.h>
#include <v/vkeys.h>

#include "vtcmdwin.h"
#include "vtcw2.h"

    static char mess[] = "[A] Is this a good test of text?\n";
    static char newMenu[] = "[A] test very much bigger menu width";
    static int filterIndex = 0;
    static char* filter[] =
      {
        "*",
        "*.txt",
        "*.c;*.cpp;*.cxx;*.h",
        0
      };

//vbm1
#define copy_depth 1
#define copy_width 28
#define copy_height 12
static unsigned char copy_bits[] = {
 0xff,0xff,0xff,0xff,0x03,0x00,0xff,0xfd,0xfb,0x0f,0x09,0xfd,0x0b,0x08,0x19,
 0xfd,0xcb,0xff,0x3f,0xfd,0xcb,0xff,0x7f,0xfd,0xcb,0xff,0x3f,0xfd,0x0b,0x08,
 0x19,0xfd,0x0b,0x08,0x89,0xfd,0x0b,0x0c,0xff,0xfc,0xfb,0x07,0x00,0xfc,0xff,
 0xff,0xff,0xff};

    static vIcon copyIcon(&copy_bits[0], copy_height, copy_width);

#define DRAW_BRUCE
#ifdef DRAW_BRUCE
#include "bruce.vbm"    // bruce picture bitmap
    static vIcon bruceIcon(&bruce_bits[0], bruce_height, bruce_width,8);
#endif
#define color_width 16
#define color_height 24
static unsigned char color_bits[] = {
        2,              // 3 colors (offset by 1)
        255,0,0,        // 0 -> red
        0,255,0,        // 1 -> green
        0,0,255,        // 2 -> blue
        // a rgb "flag", 3 16x8 rows of each color
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // RRRRRRRRRRRRRRRR
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // RRRRRRRRRRRRRRRR
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,        // RRRRRRRRRRBBBBBR
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,        // RRRRRRRRRRBBBBBR
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,        // RRRRRRRRRRBBBBBR
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,        // RRRRRRRRRRBBBBBR
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // RRRRRRRRRRRRRRRR
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // RRRRRRRRRRRRRRRR
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // GGGGGGGGGGGGGGGG
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // GGGGGGGGGGGGGGGG
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // GGGGGGGGGGGGGGGG
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // GGGGGGGGGGGGGGGG
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // GGGGGGGGGGGGGGGG
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // GGGGGGGGGGGGGGGG
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // GGGGGGGGGGGGGGGG
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // GGGGGGGGGGGGGGGG
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // BBBBBBBBBBBBBBBB
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // BBBBBBBBBBBBBBBB
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // BBBBBBBBBBBBBBBB
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // BBBBBBBBBBBBBBBB
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // BBBBBBBBBBBBBBBB
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // BBBBBBBBBBBBBBBB
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,        // BBBBBBBBBBBBBBBB
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2         // BBBBBBBBBBBBBBBB
      };

    static vIcon colorIcon(&color_bits[0], color_height, color_width,8);

#define c24_height 15
#define c24_width 8
    static unsigned char c24_bits[] = {
     255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0, //RRRRRRRR
     255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,255,0,0,255,0, //RRRRRRGG
     255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,255,0,0,255,0, //RRRRRRGG
     255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,255,0,0,255,0, //RRRRRRGG
     255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0, //RRRRRRRR
     0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0, //GGGGGGGG
     0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0, //GGGGGGGG
     0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0, //GGGGGGGG
     0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0, //GGGGGGGG
     0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0, //GGGGGGGG
     0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255, //BBBBBBBB
     0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255, //BBBBBBBB
     0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255, //BBBBBBBB
     0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255, //BBBBBBBB
     0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255, //BBBBBBBB
     0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255  //BBBBBBBB
     };
    static vIcon c24Icon(&c24_bits[0], c24_height, c24_width,24);

    int copy_sens = 0;
    static int icon_sens = 1;


//#########################################################################

    static char sub[] = "SubA";
    static char title[] = "A Title(де)";
    static char blabel[] = "A Label(де)";

  enum {

	M_SimpleWin = 350,
	M_YNDialog,
	M_Dialog,
	M_CheckMe,
	M_CopySens,
	M_IconSens,
	M_ShowList,
	M_ChangeButton,
	M_ModalDialog,
	M_TextIn,
	M_Listx,
	M_Message,
	m_new2,
	M_ChgTitle,
	M_ChgAppTitle,
	M_Reply,
	M_AttrMessage,
	M_RevMessage,
	M_CenterCursor,
	M_SubMenu,
	M_HideCursor,
	M_ShowCursor,
	m_ChangeCursor,
	m_Hon,
	m_Hoff,
	m_Von,
	m_Voff,
	m_ScrollSub,
	m_HideStat,
	m_ShowStat,
	m_HideCmd,
	m_ShowCmd,
	m_HideCnv,
	m_ShowCnv,
	m_font,
	m_printTest,
        m_printTestScaled,
	M_DrawIcon,
	M_Mem2Canvas,
	M_ColorTest,
	M_PointTest,
	m_CopyAreaTest,
	m_grow,
	m_maketiny,
	m_setBG,
        m_changeMenu,
        m_AngleText,
	m_gettooltextin,

	M_Toggle,
	m_togbtn,
	m_togdlg,

	m_Timer,
	m_CurTime,
	m_Beep,
	LAST_ENUM_CMD
      };

//      Sample Menu Item Definition
//      {"x", M_x, isSens, notChk, noKeyLbl, noKey, noSub},

    vMenu WindowSubMenu[] =
      {
        {"Center Cursor", M_CenterCursor,isSens, notChk, noKeyLbl, noKey, noSub},
        {"Change Cursor", m_ChangeCursor,isSens, notChk, noKeyLbl, noKey, noSub},
        {"Change Title", M_ChgTitle, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Change App Title", M_ChgAppTitle, isSens, notChk, noKeyLbl, noKey, noSub},
        {"CheckMe", M_CheckMe, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Copy Sensitive +/-", M_CopySens, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Draw Icon Sensitive +/-", M_IconSens, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Hide Cursor", M_HideCursor,isSens, notChk, noKeyLbl, noKey, noSub},
        {"Draw Icon",M_DrawIcon,isSens,notChk,noKeyLbl,noKey,noSub},
        {"MemDC Point Test",M_PointTest,isSens,notChk,noKeyLbl,noKey,noSub},
        {"MemDC to canvas",M_Mem2Canvas,isSens,notChk,noKeyLbl,noKey,noSub},
        {"MemDC CopyArea test",m_CopyAreaTest,isSens,notChk,noKeyLbl,noKey,noSub},
        {"Notice from vApp", 901, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Set BG color", m_setBG, isSens, notChk, "Ctrl-B", 'B'-'@', noSub},
        {"Show Cursor", M_ShowCursor,isSens, notChk, noKeyLbl, noKey, noSub},
        {"Show Window List", M_ShowList, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Shift Window",903, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Grow Canvas",m_grow,isSens, notChk, noKeyLbl, noKey, noSub},
        {"Tiny Canvas",m_maketiny,isSens, notChk, noKeyLbl, noKey, noSub},
        {"vWindow", M_SimpleWin, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Raise First Window", 902, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Timers off", 909, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Beep", m_Beep, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Get Tool TextIn",m_gettooltextin,isSens,notChk,noKeyLbl,noKey,noSub},
	
        {NULL}
      };

    vMenu ScrollMenu[] =
      {
        {"Show HScroll", m_Hon, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Hide HScroll", m_Hoff, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Show VScroll", m_Von, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Hide VScroll", m_Voff, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Hide Status", m_HideStat, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Show Status", m_ShowStat, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Hide CmdBar", m_HideCmd, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Show CmdBar", m_ShowCmd, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Hide Canvas", m_HideCnv, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Show Canvas", m_ShowCnv, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };

    vMenu TestMenu[] =
      {
        {"Misc Tests", M_SubMenu, isSens, notChk, noKeyLbl, noKey, WindowSubMenu},
        {"Hide/Show",m_ScrollSub, isSens,notChk,noKeyLbl,noKey, ScrollMenu},
        {"Change menu",m_changeMenu, isSens,notChk,noKeyLbl,noKey, noSub},
	{"&Dialog", M_Dialog, isSens, notChk, "Ctrl-D", 'D'-'@', noSub},
	{"&Toggle Dialog", m_togdlg, isSens, notChk, noKeyLbl, noKey, noSub},
        {"List ", M_Listx,     isSens, notChk, noKeyLbl, noKey, noSub},
        {"Modal Dialog", M_ModalDialog, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Set Font", m_font, isSens, notChk,noKeyLbl, noKey, noSub},
        {"Print Test", m_printTest, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Print Test Scaled", m_printTestScaled, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Text In", M_TextIn, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Reply", M_Reply, isSens, notChk, noKeyLbl, noKey, noSub},
        {"YN Dialog", M_YNDialog, isSens, notChk, noKeyLbl, noKey, noSub},
        {"CheckMe", M_CheckMe, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Test vCanvasPane", m_new2, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };

    vMenu FileMenu[] =
      {
        {"&New", M_New, isSens, notChk, noKeyLbl, noKey, noSub},
        {"&Open", M_Open, isSens, notChk, noKeyLbl, noKey, noSub},
        {"&Save", M_Save, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Save &As", M_SaveAs, isSens, notChk, noKeyLbl, noKey, noSub},
#ifdef vDEBUG
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"&Debug", M_SetDebug, isSens, notChk, noKeyLbl, noKey, noSub},
#endif
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"&Close",M_Close, isSens, notChk, noKeyLbl, noKey, noSub},
        {"E&xit", M_Exit, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };

    vMenu EditMenu[] =
      {
        {"Cut", M_Cut, isSens, notChk, "Ctrl-X", noKey, noSub},
        {"Copy", M_Copy, notSens, notChk, "Ctrl-C", noKey, noSub},
        {"Paste", M_Paste, notSens, notChk, "Ctrl-V", noKey, noSub},
        {NULL}
      };

    vMenu StandardMenu[] =
      {
        {"&File", M_File, isSens, notUsed, notUsed, noKey, &FileMenu[0]},
                // the File menu
        {"&Edit", M_Edit, isSens, notUsed, notUsed, noKey, &EditMenu[0]},
                // the Edit menu
        {"&Test", M_Test, isSens, notUsed, notUsed, noKey, &TestMenu[0]},
                // the Test menu
        {NULL}
      };

      char* testl[] =
      {
	"Test A",
        "Item B",
        "Item C",
        "Item D",
        "Item E",
        "Item F",
	"Item G",
//	"Item H",
//	"Item I",
//	"Item J",
//	"Item K",
        0
      };

  
#define vC_Size 8

    CommandObject CommandBar[] =
      {
        {C_Button, M_Copy, M_Copy, "Copy", NoList,CA_None,notSens,NoFrame, 0, 0, 0,
		"Copy Highlight"},
	{C_ToggleIconButton,M_Toggle , 1, "tog", &copyIcon,CA_None,isSens,NoFrame, 0, 0, 0,
		"Toggle Icon Test"},
	{C_ToggleButton,m_togbtn , 1, "+/-", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"Toggle Text Test"},
        {C_Button, M_Clear, M_Clear, "Clr", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"Clear screen"},
	{C_Button, M_Message, M_Message, "Msg", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"Show message in canvas"},
        {C_Button, M_RevMessage, M_RevMessage, "RevMsg", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"Reverse message in canvas"},

#include <v/vcb2x4.h>

	{C_ColorButton, M_Exit, M_Exit, "Exit", &vStdColors[vC_Red],CA_None,isSens,NoFrame, 0, 0, 0,
		"Exit Test"},
#ifdef ALLTESTS
	{C_List, 4200, 0, "A List", (void*)testl,
		 CA_None,isSens,NoFrame,0,0},
#endif
	{C_ComboBox,4201,0,"ComboBox",(void*)testl,
		 CA_Simple,isSens,NoFrame,0,0,0, "Combo Box test"},
        {C_TextIn,4202,0,"Test Text In", NoList,
                CA_None,isSens,NoFrame,0,0,0, "Test Text In"},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    CommandObject CommandBar2[] =
      {
        {C_Button, M_New, M_New, "New", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"New Command Window"},
        {C_Button, M_Close, M_Close, "Close", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"Close this window"},
        {C_Button, M_AttrMessage, 0, "Attr Message", NoList,CA_None,isSens,NoFrame,0,0,0,
		"Message with attributes"},
        {C_Button, M_ChangeButton, M_ChangeButton, "Change Me", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"Change text in this button"},
        {C_IconButton, M_DrawIcon, M_DrawIcon, "", &colorIcon,CA_None,isSens,NoFrame, 0, 0, 0,
		"Draw Icon on canvas"},
        {C_Button, M_ColorTest, 0, "Color Test", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"Draw colors"},
        {C_Slider, 888, 4, "",NoList,CA_Horizontal ,isSens,NoFrame, 0, 0, 0,
		"Slider Test"},
        {C_Text, 889, 4, "", (void*)" 04",CA_None,isSens,NoFrame, 0, 0},
        {C_Button, M_Dialog, 0, "Dialog", NoList,CA_None,isSens,NoFrame, 0, 0, 0,
		"Show Dialog Test"},
        {C_EndOfList}
      };

    vStatus StatBar[] =
      {
        {" Time: ", 305, CA_NoBorder, isSens, 0},
	{" 00:00:00 AM  dd mmm yyyy ",306, CA_None, isSens, 0},
	{"00000",307, CA_None, isSens, 0},
        {"Color:", 302, CA_NoBorder, isSens, 0 },
	{"   Black   ", 304, CA_None, isSens, 70},
	{"  +",311,CA_NoBorder,isSens,0},
	{" 20",313,CA_NoBorder,isSens,0},
	{"/",314,CA_NoBorder,isSens,0},
        {" 50",315,CA_NoBorder,isSens,0},
        {NULL,0,0,0,0}
      };

    char text_buff[256];        // big buffer for text

    CommandObject textInList[] =
      {
        {C_Label, 1, 1, "TI",NoList,
                CA_MainMsg,isSens,NoFrame, 0,0},
        {C_TextIn, 2, 2, "Default Test Text", NoList,
                CA_None,isSens,NoFrame,0,1},
        {C_TextIn, 3, 3, "12345", NoList,
                CA_None,isSens,NoFrame,0,2,5},
        {C_TextIn, 4, 4, "",NoList,CA_Password,isSens,NoFrame,3,2},
        {C_Button, M_OK, M_OK, " OK ", NoList,CA_DefaultButton,
                isSens,NoFrame,0,3},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    CommandObject ModalList[] =
      {
        {C_Label, 10, 10, "YND",
                 NoList, CA_MainMsg, isSens, NoFrame, 0,0},
        {C_Label, 99, 0, "Sample Label", NoList, CA_None,isSens,NoFrame, 10,0},


        {C_Blank, 11, 0, "     ", NoList, CA_None,isSens,NoFrame, 0,10},
        {C_Button, M_OK, M_OK, " OK ", NoList,
                CA_DefaultButton, isSens, NoFrame, 11, 10},
        {C_Button, 14, M_No, " No ", NoList,CA_None,isSens,NoFrame, M_OK, 10},
        {C_Button, M_Cancel, M_Cancel, " Cancel ", NoList,
		CA_None,isSens,NoFrame,14, 10},

        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static VCursor curCursor = VC_IBar;

    static vFont gfont(vfDefaultFixed);
    static long lp_lim = 0;
    static int memdcValid = 0;
    static int sliceOn = 0;


//====================>>> myTimer::TimerTick <<<====================
  void myTimer::TimerTick()
  {

   cmdw->WindowCommand(m_CurTime, m_CurTime, C_Button);                 // update clock
  }

//====================>>> testCmdWindow::testCmdWindow <<<====================
  testCmdWindow::testCmdWindow(VCONST char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {

    UserDebug3(Constructor,"testCmdWindow::testCmdWindow(%s,%d,%d) Constructor\n",name,width,height)

    nextWin = 0;
    nextCmdWin = 0;
    curDialog = 0;
    curTogDialog = 0;
    curListDialog = 0;
    _workCounter = 0;

    // The "Standard" window will consist of a menubar, a canvas,
    // an optional button bar, and an optional status bar.

    // First, create and add the proper panes to the CmdWindow
    // Note: there must be a correspondint delete in the destructor

    // The Menu Bar
    myMenu = new vMenuPane(StandardMenu);       // the standard menu
    AddPane(myMenu);                            // add pane to window

    // The command pane
    myCmdPane = new vCommandPane(CommandBar);
    AddPane(myCmdPane);

    // The command pane
    myCmdPane2 = new vCommandPane(CommandBar2);
    AddPane(myCmdPane2);

    // The Canvas
    myCanvas = new testTextCanvasPane(this);              // a new canvas pane
    AddPane(myCanvas);                          // add the pane to window

    // The Status Bar
    myStatus = new vStatusPane(StatBar);        // a new status bar
    AddPane(myStatus);                          // add pane to window

    // the timer
    _timer = new myTimer(this);
    _timer->TimerSet(1000); // 2 second intervals

    myMemDC = new vMemoryDC(256,256);           // a memory dc

    ShowWindow();

    myCanvas->ShowVScroll(1); myCanvas->SetVScroll(20,50);

    WindowCommand(m_CurTime, m_CurTime, C_Button);      // update clock

    theApp->EnableWorkSlice(50); sliceOn = 1;

  }

//====================>>> testCmdWindow::~testCmdWindow <<<====================
  testCmdWindow::~testCmdWindow()
  {
    UserDebug(Destructor,"testCmdWindow::~testCmdWindow() destructor\n")

    _timer->TimerStop();
    delete _timer;
    delete myMenu;
    delete myCanvas;
    delete myCmdPane;
    delete myCmdPane2;
    delete myStatus;
    if (curDialog)
	delete curDialog;
    if (curTogDialog)
	delete curTogDialog;
    if (curListDialog)
	delete curListDialog;

  }

//====================>>> testCmdWindow::CloseWin <<<====================
  void testCmdWindow::CloseWin()
  {
    vCmdWindow::CloseWin();
  }
  

//====================>>> testCmdWindow::SetShownTop <<<====================
  void testCmdWindow::SetShownTop(int shown, int top)
  {
    char buff[20];
    IntToStr(shown,buff);
    SetString(313,buff);
    IntToStr(top,buff);
    SetString(315,buff);
  }

//====================>>> testCmdWindow::WindowCommand <<<====================
  void testCmdWindow::WindowCommand(ItemVal id, ItemVal val, CmdType cType)
  {

    // route all commands through here - menus and buttons

    switch (id)
      {
        case M_Cut:
        case M_Copy:
          {
            theApp->ClipboardSetText("Greetings From the V Test Program\n");
            break;
          }

        case M_Paste:
          {
            char* pasteText = theApp->ClipboardGetText();
            if (pasteText != 0)
              {
                myCanvas->DrawText(pasteText);
                myCanvas->DrawText("\n");
              }
            break;
          }

        case M_New:
	  {
            // Create and register a new window
            if (theApp->simSDI())
                break;
            testCmdWindow* tmp = new testCmdWindow(sub,
		theApp->DefaultWidth(), theApp->DefaultHeight());
	    theApp->NewAppWin(tmp, sub, 400, 100);
	    sub[3]++;
            break;
          }

        case m_new2:
          {
            // Create and register a new window
	    // This test is required to test the MDI model to
	    // be sure the menus and tool bars change.
            if (theApp->simSDI())
                break;
            testCmdWindow2* tmp2 = new testCmdWindow2("Simple Window",
                300, 250);
	    theApp->NewAppWin(tmp2, "Simple Window", 400, 300);
            break;
          }

	case M_Open:
	  {
	    static char name[100] = ""; // start out with null name
	    vFileSelect fsel(this);     // make an instance

	    int oans = fsel.FileSelect("Open file",name,99,filter,filterIndex);

	    vNoticeDialog fsnote(this); // make an instance

	    if (oans && *name)
	      {
		SetTitle(name);
		(void)fsnote.Notice(name);
	      }
	    else
		(void)fsnote.Notice("No file name input.");

	    break;
	  }

	case M_SaveAs:
	  {
	    static char name[100] = "saveas.tmp"; // start out with null name
	    vFileSelect fsel(this);     // make an instance

	    int oans = fsel.FileSelectSave("Save file",name,99,filter,filterIndex);

	    vNoticeDialog fsnote(this); // make an instance

	    if (oans && *name)
	      {
		SetTitle(name);
		(void)fsnote.Notice(name);
	      }
	    else
		(void)fsnote.Notice("No file name input.");

	    break;
	  }

        case M_Close:           // Close not via close button
          {
            theApp->CloseAppWin(this);
            break;
          }

        case M_Exit:
          {
            theApp->Exit();
            break;
          }

        case M_CheckMe:                 // check me
          {
            ItemVal curval = GetValue(id);
            SetValue(M_CheckMe,!curval,Checked);
            if (curval)
                SetString(M_CheckMe,"Check Me");
            else
                SetString(M_CheckMe,"UnChk Me");
            break;
	  }

        case m_changeMenu:		// change menu item
          {
            SetString(m_changeMenu,newMenu);
            newMenu[1]++;
            break;
          }

	case m_togbtn:
          {
             ItemVal curval = GetValue(id);     // get current value
             if (curval)
                 SetString(311,"+");
             else
                 SetString(311,"-");
             break;
          }

        case M_CopySens:                        // Last
          {
            copy_sens = !copy_sens;
            SetValue(M_Copy,copy_sens,Sensitive);
            break;
          }


        case M_IconSens:                        // Last
          {
            icon_sens = !icon_sens;
            SetValue(M_DrawIcon,icon_sens,Sensitive);
            break;
          }

        case M_SimpleWin:
          {
            if (nextWin < MAX_WINDOWS)
              {
                Wins[nextWin] = new vWindow(sub,50,30);
                sub[3]++;               // make a new name
                Wins[nextWin]->ShowWindow();
                ++nextWin;
              }
           break;
          }
#ifdef vDEBUG
        case M_SetDebug:
          {
	    vDebugDialog debug(this);
            debug.SetDebug();
            break;
          }
#endif

        case M_YNDialog:
          {
	    vYNReplyDialog ynd(this);   // make an instance
            int ans;
            char ynmsg[] = "YN Dialog - try one.";

            ans = ynd.AskYN(ynmsg);
         //   fprintf(stderr,"YN answer value: %d\n",ans);
            break;
          }

        case M_ChgTitle:
          {
            SetTitle(title);
            ++title[0];
            break;
          }

        case M_ChgAppTitle:
          {
            theApp->SetAppTitle(title);
            ++title[0];
            break;
          }

        case M_Reply:
          {
            vReplyDialog rp(this);      // make an instance
            char out[120];
            char r[100];                // a buffer for reply

            int rv = rp.Reply("Please enter some text.",r,99,"Edit this text");

            vNoticeDialog note(this);   // make an instance

            if (rv == M_Cancel)
                (void)note.Notice("Cancel pressed");
            else if (*r)
              {
                strcpy(out,"Text was: ");
                strcat(out,r);
                (void)note.Notice(out);
              }
            else
                (void)note.Notice("No text input.");

            break;
          }

        case M_CenterCursor:            // test cursor motion
          {
            int r = myCanvas->GetRows();
            int c = myCanvas->GetCols();

            myCanvas->GotoRC(r/2, c/2);
            break;
          }

        case M_HideCursor:
          {
            myCanvas->HideTextCursor();
            break;
          }

        case M_ShowCursor:
          {
            myCanvas->ShowTextCursor();
            break;
          }

        case m_ChangeCursor:            // change canvas cursor
          {
            if (curCursor >= VC_LAST)   // change module # of cursors
                curCursor = 1;
            else
                ++curCursor;
            myCanvas->SetCursor(curCursor);  // Set the cursor
            break;
          }

        case m_Hon:
          {
            myCanvas->ShowHScroll(1);  // show the h scroll
	    myCanvas->SetHScroll(80,20);
            break;
          }

        case m_Hoff:
          {
            myCanvas->ShowHScroll(0);  // hide the h scroll
            break;
          }

        case m_Von:
          {
            myCanvas->ShowVScroll(1);  // show the v scroll
            break;
          }

        case m_Voff:
          {
            myCanvas->ShowVScroll(0);  // hide the v scroll
            break;
          }

        case m_HideStat:
          {
            ShowPane(myStatus,0);
            break;
          }

        case m_ShowStat:
          {
            ShowPane(myStatus,1);
            break;
          }

        case m_HideCmd:
          {
	    ShowPane(myCmdPane,0);
	    ShowPane(myCmdPane2,0);
            break;
          }

        case m_ShowCmd:
          {
	    ShowPane(myCmdPane,1);
	    ShowPane(myCmdPane2,1);
            break;
          }

        case m_HideCnv:
          {
            ShowPane(myCanvas,0);
            break;
          }

        case m_ShowCnv:
          {
            ShowPane(myCanvas,1);
            break;
          }

	case M_Dialog:
	  {
             char val[20];

	     if (!curDialog)
	      {
                curDialog = new testDialog(this);
	      }

	    curDialog->ShowDialog("This is a simple modeless dialog.");
	    int l,t,w,h;
	    GetPosition(l,t,w,h);	// get positon of main window
            myCanvas->DrawText("Main Window l,t,w, h: ");
            IntToStr(l,val); myCanvas->DrawText(val); myCanvas->DrawText(", ");
            IntToStr(t,val); myCanvas->DrawText(val); myCanvas->DrawText(", ");
            IntToStr(w,val); myCanvas->DrawText(val); myCanvas->DrawText(", ");
            IntToStr(h,val); myCanvas->DrawText(val); myCanvas->DrawText(", \n");


	    curDialog->SetDialogPosition(l+10,t+h+5);
            curDialog->GetDialogPosition(l,t,w,h);
            myCanvas->DrawText("Dialog l,t,w, h: ");
            IntToStr(l,val); myCanvas->DrawText(val); myCanvas->DrawText(", ");
            IntToStr(t,val); myCanvas->DrawText(val); myCanvas->DrawText(", ");
            IntToStr(w,val); myCanvas->DrawText(val); myCanvas->DrawText(", ");
            IntToStr(h,val); myCanvas->DrawText(val); myCanvas->DrawText(", \n");


	    break;
	  }

	case m_togdlg:
	  {
	    if (!curTogDialog)
	      {
		curTogDialog = new toggleDialog(this);
	      }

	    int dummy;
	    curTogDialog->ShowModalDialog("Toggle Dialog.",dummy);
	    break;
	  }

        case M_Listx:
          {
	    if (!curListDialog)
	      {
		curListDialog = new listDialog(this);      // create dynamically
	      }
  
             curListDialog->SetValue(200,8,Value);   // pre-select 8
            curListDialog->ShowDialog("List Dialog Test.");
            break;
	  }

	case 1353:
	  {
	    vNoticeDialog note(this);
	    note.Notice("Change Combo");
	    break;
	  }

        case M_Clear:                   // clear display
          {
            myCanvas->Clear();
            lp_lim = 0;
            break;
          }

        case M_Message:
          {
            myCanvas->DrawText(mess);
	    myCanvas->DrawText("\nYes\n");	// @@
            if (++mess[1] > 'Z')        // something new each time
                mess[1] = 'A';
            break;
          }

        case M_AttrMessage:
          {
            myCanvas->DrawAttrText("Red, ", ChRed);
            myCanvas->DrawAttrText("Green, ", ChGreen);
            myCanvas->DrawAttrText("RevGreen, ", ChGreen|ChReverse);
            myCanvas->DrawAttrText("HiGreen, ", ChGreen|ChHighlight);
            myCanvas->DrawAttrText("DimGreen, ", ChGreen|ChDimColor);
            myCanvas->DrawAttrText("reversed, ",ChReverse);
            myCanvas->DrawAttrText("and ", ChNormal);
            myCanvas->DrawAttrText("Blue.\n", ChBlue);
            break;
          }

        case M_RevMessage:
          {
            myCanvas->DrawAttrText(mess, ChReverse);
            if (++mess[1] > 'Z')        // something new each time
                mess[1] = 'A';
            break;
          }

        case M_ModalDialog:
          {
            vModalDialog md(this);      // try this one dynamically...
            int id, val;

            md.AddDialogCmds(ModalList);
            id = md.ShowModalDialog("This is a MODAL dialog.",val);
            break;
          }

        case M_ShowList:
          {
            theApp->ShowList();
            break;
          }

        case M_ChangeButton:
          {
            SetString(M_ChangeButton,blabel);
            ++blabel[0];
            break;
          }

        case m_font:
          {
            vFontSelect fd(this);

            (void) fd.FontSelect(gfont);
            myCanvas->SetFont(gfont);
            myCanvas->Clear();
            myCanvas->DrawText(mess);
            if (++mess[1] > 'Z')        // something new each time
                mess[1] = 'A';
            break;
          }

        case m_printTest:
          {
            PrintTest(0);
            break;
          }

        case m_printTestScaled:
          {
            PrintTest(1);
            break;
          }

        case m_grow:
          {
	    static int w = 200;
	    static int h = 100;
            myCanvas->SetWidthHeight(w,h);
	    w += 50; h += 50;
            break;
          }
	case m_maketiny:
	  {
	    myCanvas->SetWidthHeight(100,4);
	    break;
	  }

	case m_setBG:
  	  {
 	    vColor bgc = _pen.GetColor();
	    myCanvas->SetBackground(bgc);
	    break;
	  }

        case M_DrawIcon:
          {
	    memdcValid = 1;
            myMemDC->Clear();
            myCanvas->DrawLine(5,30,20,30);
            myMemDC->DrawLine(5,30,20,30);
            myCanvas->DrawIcon(20,30,copyIcon);
            myMemDC->DrawIcon(20,30,copyIcon);
            myCanvas->DrawIcon(80,30,c24Icon);
	    myMemDC->DrawIcon(80,30,c24Icon);
#ifdef DRAW_BRUCE
	    myCanvas->DrawIcon(120,2,bruceIcon);
            myMemDC->DrawIcon(120,2,bruceIcon);
#else
            myCanvas->DrawIcon(120,30,colorIcon);
            myMemDC->DrawIcon(120,30,colorIcon);
#endif
            break;
          }

        case M_Mem2Canvas:
          {
            if (!memdcValid)            // haven't done this yet
              {
                myMemDC->Clear();
                vBrush br;
                br.SetColor(255,0,0);
                myMemDC->SetBrush(br);
                myMemDC->DrawRectangle(0,0,256,256);
                memdcValid = 1;
              }
            myCanvas->CopyFromMemoryDC(myMemDC,10,20);
            break;
	  }

	case m_CopyAreaTest:
          {
            if (!memdcValid)            // haven't done this yet
              {
                myMemDC->Clear();
                vBrush br;
                br.SetColor(255,0,0);
                myMemDC->SetBrush(br);
                myMemDC->DrawRectangle(0,0,256,256);
                memdcValid = 1;
	      }
	    
#ifdef DRAW_BRUCE
	    myCanvas->CopyFromMemoryDC(myMemDC,10,30,120,2,bruce_width,bruce_height);
#else
	    myCanvas->CopyFromMemoryDC(myMemDC,10,30,120,30,color_width,color_height);
#endif
            break;
	  }

        // The following test attempts to see how fast we can draw
        case M_ColorTest:
          {
                    lp_lim = (long) GetValue(888);              // get value of slider
            vColor colorRow[256];
            // myMemDC->Clear();
            myCanvas->Clear();

            vBrush brush;
            vPen pen;

            for (long lps = 0 ; lps  < lp_lim ; ++lps)
              {
                for (long bar = 0 ; bar < 16 ; ++ bar)
                  {
                    brush.SetColor(vStdColors[(bar+lps) & 0x0F]);
                    pen.SetColor(vStdColors[(bar+lps) & 0x0F]);

                   // myMemDC->SetBrush(brush);
                   // myMemDC->DrawRectangle(0,bar*16,256,16);
                    myCanvas->SetBrush(brush);
                    myCanvas->SetPen(pen);
                    myCanvas->DrawRectangle(0,bar*16,256,16);
                  }
                theApp->CheckEvents();
              }
            break;
          }

        case M_PointTest:
          {
            memdcValid = 1;
            lp_lim = 8;    // get value of slider
            vColor colorRow[256];
            myMemDC->Clear();
            myCanvas->Clear();

            for (long lps = 0 ; lps  < lp_lim ; ++lps)
              {
                for (long bar = 0 ; bar < 16 ; ++ bar)
                  {
                    for (int col = 0 ; col < 256 ; ++col) // fill in color row
                        colorRow[col] = vStdColors[(bar+lps) & 0x0F];

                    for (int row = bar * 16 ; row < (bar*16)+16 ; ++row)
                      {
                        myMemDC->DrawColorPoints(0,row,256,colorRow);
//                      myCanvas->DrawColorPoints(0,row,256,colorRow);
                      }
                  }
                myCanvas->CopyFromMemoryDC(myMemDC,0,0);
                theApp->CheckEvents();
              }

            break;
          }


        case 888:
          {
            char buff[10];
            IntToStr(val,buff);
            SetString(889,buff);
            break;
          }

        case M_TextIn:
          {
            vModalDialog md(this);      // try this one dynamically...

            vNoticeDialog txtin(this);

            int ans, val;

            md.AddDialogCmds(textInList);

            ans = md.ShowModalDialog("Enter text.", val);

            text_buff[0] = 0;

            (void) md.GetTextIn(2, text_buff, 254);

            txtin.Notice(text_buff);
            break;
          }


	case m_gettooltextin:
 	  {
            vNoticeDialog txtin(this);
            text_buff[0] = 0;

            (void) GetTextIn(4202, text_buff, 254);

            txtin.Notice(text_buff);
            break;
	  }

	case m_CurTime:
          {
            char buff[40];
            char date[30];
            vGetLocalTime(buff);
	    vGetLocalDate(date);
	    strcat(buff,"  ");
            strcat(buff,date);
	    SetString(306,buff);
	    if (_workCounter < 1000)
	      {
		LongToStr(_workCounter,buff);
		SetString(307,buff);
	      }
	    else
	      {
		if (sliceOn)
		  theApp->EnableWorkSlice(0);
		sliceOn = 0;
	      }
	    break;
          }


	case m_Beep:
	  {
	    vBeep();
	    break;
	  }

	case 909:
	  {
	    _timer->TimerStop();
	    theApp->EnableWorkSlice(0);
	    sliceOn = 0;
	    break;
	  }

	default:
	  {
            if (id >= M_Black && id <= M_White) // A color button
              {
                _pen.SetColor(vStdColors[val]);
                myCanvas->SetPen(_pen);
                SetString(304,vColorNames[val]);
              }
            else
                vCmdWindow::WindowCommand(id, val, cType);
            break;
          }
      }
  }

//====================>>> testCmdWindow::PrintTest <<<======================
  void testCmdWindow::PrintTest(int scaled)
  {
    // Test what the text will do

    vPrintDC pdc(scaled);               // create a printDC object
    vPrinter printer;
    vFont font;
    int x;

    if (!printer.Setup("test.ps"))      // setup the printer
      {
	vBeep();
        return;
      }

    pdc.SetPrinter(printer);    // change to the printer we setup

    if (!pdc.BeginPrinting())   // print to test.ps
        return;

    int physW = pdc.GetPhysWidth();
    int physH = pdc.GetPhysHeight();

    // Assume 8.5" paper for this test. Calculate a logical inch in pixels.
    // The default gives about 72.

    int physInch = (int)((float)pdc.GetPhysWidth() / 8.5);  // a logical inch
    int logicalWidth = 612;
    int logicalHeight = 792;

    // Now, set the scale if in scaled mode.

    if (scaled)
        pdc.SetScale(physInch/4, 18);	// 1/72 = n/physInch
    else
      {
    	logicalWidth = pdc.GetPhysWidth();
        logicalHeight = pdc.GetPhysHeight();
      }

    // Have a valid drawing canvas, so draw a box around paper edges
//    for (int pg = 1 ; pg < 3 ; ++pg)    // print 2 pages to test everyting
    for (int pg = 1 ; pg < 2 ; ++pg)
      {

        vBrush pbrush(0,0,0,vTransparent);      // a transparent brush
        pdc.SetBrush(pbrush);
        if (pg == 1)
          {
            // We want 36 to be about 1/2" on 8.5" paper, which happens automatically
            // in the default mode. If we are in scaled mode, then 36 should be
            // 1/17th of the page width. Thus, to get real value, we need
            // to find n, where 36*n == PhysicalWidth / 17, or n = PhysWid / 612.

            pdc.DrawRectangle(36, 36, logicalWidth - 72,
                logicalHeight - 72);
            pdc.SetTranslate(0,36);     // and now leave 1/2 inch at top
          }
        // since we center the text, we don't
        // need the left margin

        // ----------- Here is the printing test.

        font.SetFontValues(vfDefaultFixed, 10, vfNormal, vfNormal, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "Default Fixed, 10 point, Normal", scaled);
        pdc.DrawLine(x, 12, x, 2);
        pdc.DrawText(x+1,12,"Default Fixed, 10 point, Normal");

        font.SetFontValues(vfDefaultVariable, 10, vfNormal, vfNormal, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "Default Variable, 10 point, Normal", scaled);
        pdc.DrawLine(x, 25, x, 15);
        pdc.DrawText(x+1,25,"Default Variable, 10 point, Normal");

        font.SetFontValues(vfFixed, 10, vfNormal, vfNormal, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "Fixed, 10 point, Normal", scaled);
        pdc.DrawLine(x, 40, x, 30);
        pdc.DrawText(x+1,40,"Fixed, 10 point, Normal");

        font.SetFontValues(vfSerif, 10, vfNormal, vfNormal, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "Serif, 10 point, Normal", scaled);
        pdc.DrawLine(x, 55, x, 45);
        pdc.DrawText(x+1,55,"Serif, 10 point, Normal");

        font.SetFontValues(vfSansSerif, 10, vfNormal, vfNormal, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "SansSerif, 10 point, Normal", scaled);
        pdc.DrawLine(x, 70, x, 60);
        pdc.DrawText(x+1,70,"SansSerif, 10 point, Normal");

        font.SetFontValues(vfSerif, 10, vfItalic, vfNormal, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "Serif, 10 point, Italic", scaled);
        pdc.DrawLine(x, 85, x, 75);
        pdc.DrawText(x+1,85,"Serif, 10 point, Italic");

        font.SetFontValues(vfSerif, 10, vfItalic, vfBold, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "Serif, 10 point, BoldItalic", scaled);
        pdc.DrawLine(x, 100, x, 90);
        pdc.DrawText(x+1,100,"Serif, 10 point, BoldItalic");

        font.SetFontValues(vfSansSerif, 10, vfItalic, vfNormal, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "SansSerif, 10 point, Italic", scaled);
        pdc.DrawLine(x, 115, x, 105);
        pdc.DrawText(x+1,115,"SansSerif, 10 point, Italic");

        font.SetFontValues(vfSansSerif, 10, vfItalic, vfBold, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "SansSerif, 10 point, BoldItalic", scaled);
        pdc.DrawLine(x, 130, x, 120);
        pdc.DrawText(x+1,130,"SansSerif, 10 point, BoldItalic");

        font.SetFontValues(vfFixed, 10, vfItalic, vfNormal, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "Fixed, 10 point, Italic", scaled);
        pdc.DrawLine(x, 145, x, 135);
        pdc.DrawText(x+1,145,"Fixed, 10 point, Italic");

        font.SetFontValues(vfFixed, 10, vfItalic, vfBold, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "Fixed, 10 point, BoldItalic", scaled);
        pdc.DrawLine(x, 160, x, 150);
        pdc.DrawText(x+1,160,"Fixed, 10 point, BoldItalic");

        font.SetFontValues(vfFixed, 20, vfItalic, vfBold, 0);
        pdc.SetFont(font);
        x = GetCenter( pdc, "BIG Fixed, 20 point, BoldItalic", scaled);
        pdc.DrawLine(x, 180, x, 160);
        pdc.DrawText(x+1,180,"BIG Fixed, 20 point, BoldItalic");

        pdc.SetFont(gfont);
        x = GetCenter( pdc, "Current Screen Font", scaled);
        pdc.DrawText(x+1,210,"Current Screen Font");

        font.SetFontValues(vfSansSerif, 24, vfNormal, vfBold, 1);
        pdc.SetFont(font);
        x = GetCenter( pdc, "The End", scaled);
        pdc.DrawLine(x, 310, x, 290);
        pdc.DrawText(x+1,310,"The End");

        pdc.DrawLine(60,72,72,72);
        pdc.DrawIcon(72,72,copyIcon);

        pdc.DrawLine(100,350,110,350);
        pdc.DrawIcon(110,350,bruceIcon);

        pdc.DrawLine(300,350,310,350);
        pdc.DrawIcon(310,350,c24Icon);

        // Test DrawColorPoints
        vColor useColor, colors[50];

        for (int cy = 16 ; cy < 32 ; ++cy)      // 16 bands of 16 lines
          {
            useColor.ResetColor(cy*8,cy*8,cy*8);        // set to a gray shade
            for (int rc = 0 ; rc < 50 ; ++rc)
                colors[rc] = useColor;          // set all to this color
            for (int ry = cy - 16 ; ry < 16 ; ++ry)     // the 16 lines
                pdc.DrawColorPoints(350,ry + cy + 500,50,colors);
          }

//        if (pg == 1)
//          {
//            pdc.EndPage();
//            pdc.BeginPage();
//@@          }
    }

    pdc.EndPrinting();
  }

//====================>>> testCmdWindow::GetCenter <<<======================
  int testCmdWindow::GetCenter(vPrintDC& pdc, char* str, int scale)
  {
    // Determine where the X is to center the text str
    // on the page, given a PrintDC

    int width = pdc.TextWidth(str);             // Real Width of the text
    int paperW = pdc.GetPhysWidth();            // Real paper width

    int center = (paperW - width) / 2;          // the x start to center!
    // The REAL center. Need to unscale if scaled

    if (scale)
      {
        int mult, div;
        pdc.GetScale(mult, div);
        center = (center/mult) * div;
      }
    if (width > paperW)                         // be reasonable
        center = 0;
    return center;
  }
