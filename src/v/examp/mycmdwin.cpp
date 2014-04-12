//=======================================================================
//  mycmdwin.cxx:     Source file for app cmdwin class
//  Copyright (C) 1995  Bruce E. Wampler
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

#include <v/vnotice.h>	// so we can use notice
#include <v/vkeys.h>	// to map keys
#include <v/vpen.h>
#include <v/vfilesel.h>
#include <v/vutil.h>

#include "mycmdwin.h"	// our header

//	Start defines for the main window with 100

#define m_CheckMe 100
#define m_CopySens 101
#define m_Dialog 102
#define m_ModalDialog 103
#define m_Clear 104

#define m_LineWidth 105
#define m_ChangePen 106

    // Define the pulldown menus
    static vMenu FileMenu[] =
      {
	{"New", M_New, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Open", M_Open, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Save", M_Save, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Save As", M_SaveAs, notSens, notChk, noKeyLbl, noKey, noSub},
#ifdef vDEBUG
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Debug", M_SetDebug, isSens, notChk, noKeyLbl, noKey, noSub},
#endif
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Exit", M_Exit, isSens, notChk, noKeyLbl, noKey, noSub},
	{NULL}
      };

    static vMenu EditMenu[] =
      {
	{"Cut", M_Cut, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Copy", M_Copy, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Paste", M_Paste, notSens, notChk, noKeyLbl, noKey, noSub},
	{NULL}
      };

    static vMenu TestMenu[] =
      {
	{"CheckMe", m_CheckMe, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Copy Sensitive", m_CopySens, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Dialog", m_Dialog, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Modal Dialog", m_ModalDialog, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };

    // Define a standard menu
    vMenu StandardMenu[] =
      {
	{"File", M_File, isSens, notUsed, notUsed, noKey, &FileMenu[0]},
		// the File menu
	{"Edit", M_Edit, isSens, notUsed, notUsed, noKey, &EditMenu[0]},
		// the Edit menu
	{"Test", M_Test, isSens, notUsed, notUsed, noKey, &TestMenu[0]},
		// the Test menu
	{NULL}
      };

    static int minMaxStep[3] = {1, 20, 1};
    // Define a button bar, usually with something useful
    static CommandObject CommandBar[] =
      {
	{C_Button, M_Copy, M_Copy, "Copy",NoList,CA_None,notSens,NoFrame,0,0},
	{C_Button, m_Clear, m_Clear, "Clear",NoList,CA_None,isSens,NoFrame,0,0},

	{C_Label, 999, 0 ,"Width:",NoList,CA_None, isSens, NoFrame,0,0},
	{C_Spinner,m_LineWidth,1,"",(void*)&minMaxStep[0],CA_None,isSens,NoFrame,0,0},
	{C_Button, m_ChangePen, m_ChangePen, "      Pen: ",NoList,
		CA_None,isSens,NoFrame,0,0},

#define vC_Size 12
#include<v/vcb2x8.h>

	{C_ColorButton, M_Exit, M_Exit, "Exit", &vStdColors[vC_Red],CA_None,isSens,NoFrame, 0, 0},

	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

#define m_cmdMsg 110
#define m_cmdCount 111
#define m_keyMsg 112
#define m_keyVal 113

    // Define a simple status bar
    static vStatus StatBar[] =
      {
	{"Commands issued: ", m_cmdMsg, CA_NoBorder, isSens, 0},
	{"   ", m_cmdCount, CA_None, isSens, 0},
	{"Last keypress: ", m_keyMsg, CA_NoBorder, isSens, 0},
	{"   ", m_keyVal, CA_None, isSens, 0},
	{0,0,0,0,0}
      };

    static char* filter[] =
      {
        "*",
        "*.txt",
        "*.c *.cxx *.h",
        0
      };
    int fi = 0;
    int copy_sens = 0;			// for tracking copy sensitive

//====================>>> myCmdWindow::myCmdWindow <<<====================
  myCmdWindow::myCmdWindow(VCONST char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {
    UserDebug1(Constructor,"myCmdWindow::myCmdWindow(%s) Constructor\n",name)

    // The "Standard" window will consist of a menubar, a canvas,
    // an optional button bar, and an optional status bar.

    // First, create and add the proper panes to the CmdWindow
    // Note: there must be a correspondint delete in the destructor

    // The Menu Bar
    myMenu = new vMenuPane(StandardMenu);	// the standard menu
    AddPane(myMenu);				// add pane to window

    // The Canvas
    myCanvas = new myCanvasPane;		// a new canvas pane
    AddPane(myCanvas);				// add the pane to window

    // The command pane
    myCmdPane = new vCommandPane(CommandBar);
    AddPane(myCmdPane);

    // The Status Bar
    myStatus = new vStatusPane(StatBar);	// a new status bar
    AddPane(myStatus);			// add pane to window

    // Now, cerate whatever dialogs and windows this app defines.

    // Create instance of myDialog, and define default commands
    sampleDialog = new myDialog(this,"Example");

    // Create instance of myModalDialog, and define default commands
    sampleModalDialog = new myModalDialog(this);
    
    // FINALLY, after all the panes have been constructed and
    // added, we must show the window!

    changePen = 1;		// we are changing pen
    ShowWindow();
  }

//====================>>> myCmdWindow::myCmdWindow <<<====================
  myCmdWindow::myCmdWindow(const myCmdWindow& w) : vCmdWindow(w)
  {
    // This copy constructor is used to support the shallow destructor
    // convention used by the V framework.

    UserDebug(Constructor,"myCmdWindow::myCmdWindow() Copy constructor\n")
    vSysError("myCmdWindow: V does not support copy constructors");
  }

//====================>>> myCmdWindow::~myCmdWindow <<<====================
  myCmdWindow::~myCmdWindow()
  {
    UserDebug(Destructor,"myCmdWindow::~myCmdWindow() destructor\n")

    // Now put a delete for each new in the constructor.

    delete myMenu;
    delete myCanvas;
    delete myStatus;
    delete myCmdPane;
    delete sampleDialog;
    delete sampleModalDialog;
  }

//====================>>> myCmdWindow::KeyIn <<<====================
  void myCmdWindow::KeyIn(vKey keysym, unsigned int shift)
  {
    static char ctrl[] = "^X ";
    static char chr[] = " X ";

    if (vk_IsModifier(keysym))
	SetString(m_keyVal, "mod");	// change status bar
    else if (keysym < ' ')		// ctrl char
      {
	ctrl[1] = keysym + '@';
	SetString(m_keyVal, ctrl);	// change status bar
      }
    else if (keysym < 128)		// normal printable char
      {
	chr[1] = keysym;
	SetString(m_keyVal, chr);	// change status bar
      }
    else 
	SetString(m_keyVal, "+++");	// change status bar
  }

//====================>>> myCmdWindow::WindowCommand <<<====================
  void myCmdWindow::WindowCommand(ItemVal id, ItemVal val, CmdType cType)
  {
    // route all commands through here - menus and buttons

    static int cmdCount = 0;

    vNoticeDialog note(this);		// Used for default actions

    char buff[20];			// buff for status

    ++cmdCount;				// count commands that have been issued
    IntToStr(cmdCount,buff);
    SetString(m_cmdCount, buff);	// change status bar

    UserDebug1(CmdEvents,"myCmdWindow:WindowCommand(%d)\n",id)

    switch (id)
      {
	case M_New:
	  {
	    note.Notice("New");
	    theApp->NewAppWin(0,"",250,100);	// call our NewAppWin method
	    break;
	  }

	case M_Open:
	  {
	    static char name[100] = ""; // start out with null name
            vFileSelect fsel(this);     // make an instance
	

            int oans = fsel.FileSelect("Open file",name,99,filter,fi);

            vNoticeDialog fsnote(this); // make an instance

            if (oans && *name)
              {
                SetTitle(name);
                (void)note.Notice(name);
              }
            else
                (void)note.Notice("No file name input.");

	    break;
	  }

	case M_Save:
	  {
	    note.Notice("Save");
	    break;
	  }

	case M_SaveAs:
	  {
	    note.Notice("Save As");
	    break;
	  }

        // For case M_SetDebug, Vigr should generate following code,
	// including vDEBUG #ifdef

#ifdef vDEBUG
	case M_SetDebug:
	  {
	    vDebugDialog debug(this);
	    debug.SetDebug();		// select debug

	    break;
	  }
#endif

	case M_Exit:
	  {
	    theApp->Exit();		// Standard action for Exit
	    break;
	  }

	case M_Cut:
	  {
	    note.Notice("Cut");
	    break;
	  }

	case M_Copy:
	  {
	    note.Notice("Copy");
	    break;
	  }

	case M_Paste:
	  {
	    note.Notice("Paste");
	    break;
	  }

	case m_CheckMe:
	  {
	    // This code would be supplied by the user, not Vigr

	    ItemVal curval = GetValue(id);

	    SetValue(m_CheckMe,!curval,Checked);
	    if (curval)
		SetString(m_CheckMe,"Check Me");
	    else
		SetString(m_CheckMe,"UnChk Me");

	    break;
	  }

	case m_CopySens:
	  {
	    // This code would be supplied by the user, not Vigr
	    copy_sens = !copy_sens;
	    SetValue(M_Copy,copy_sens,Sensitive);

	    break;
	  }

	case m_Dialog:
	  {
	    // Ideally, Vigr will generate the following code, too:
	    if (!sampleDialog->IsDisplayed())	// not twice!
		sampleDialog->ShowDialog("Sample Modeless Dialog");
	    break;
	  }

	case m_ModalDialog:
	  {
	    // Ideally, Vigr will generate the following code, too:
	    ItemVal rval, rid;
	    if (!sampleModalDialog->IsDisplayed())	// not twice!
		rid = sampleModalDialog->ShowModalDialog("Sample Modal",rval);
	    break;
	  }

	case m_ChangePen:
	  {
	    if (changePen)
	      {
		SetString(m_ChangePen,"Background:");
		changePen = 0;
	      }
	    else
	      {
		SetString(m_ChangePen,"      Pen: ");
		changePen = 1;
	      }
	    break;
	  }

	case m_LineWidth:
	  {
	    _pen.SetWidth(val);
	    myCanvas->SetPen(_pen);
	    break;
	  }

	case m_Clear:
	  {
	    myCanvas->Clear();
	    break;
	  }

	default:		// route unhandled commands up
	  {
	    if (id >= M_Black && id <= M_White)
	      {
		if (changePen)
		  {
		    _pen.SetColor(vStdColors[val]);
		    myCanvas->SetPen(_pen);
		  }
		else
		    myCanvas->SetBackground(vStdColors[val]);
	      }
	    else
		vCmdWindow::WindowCommand(id, val, cType);
	    break;
	  }
	    
      }
  }
