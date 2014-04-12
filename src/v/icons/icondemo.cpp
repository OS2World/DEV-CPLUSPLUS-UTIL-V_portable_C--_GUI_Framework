//=======================================================================
//  icondemo.cpp:     Source file for icon demo
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

#include "icondemo.h"	// our header
#include <v/vicon.h>

// This program is used to show the sample 16x16 icons provided with the V
// distribution. It doesn't do anything else.

//****************************************************************************

    // Define a  menu and some command bars

    static vMenu ShortFile[] =
      {
        {"E&xit", M_Exit, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };

#include "bold.vbm"
    static vIcon boldI(&bold_bits[0],bold_height,bold_width);
#include "brush.vbm"
    static vIcon brushI(&brush_bits[0],brush_height,brush_width);
#include "check.vbm"
    static vIcon checkI(&check_bits[0],check_height,check_width);
#include "copy.vbm"
    static vIcon copyI(&copy_bits[0],copy_height,copy_width);
#include "cut.vbm"
    static vIcon cutI(&cut_bits[0],cut_height,cut_width);
#include "draw.vbm"
    static vIcon drawI(&draw_bits[0],draw_height,draw_width);
#include "ellip.vbm"
    static vIcon ellipI(&ellip_bits[0],ellip_height,ellip_width);
#include "help.vbm"
    static vIcon helpI(&help_bits[0],help_height,help_width);
#include "italic.vbm"
    static vIcon italicI(&italic_bits[0],italic_height,italic_width);
#include "line.vbm"
    static vIcon lineI(&line_bits[0],line_height,line_width);
#include "new.vbm"
    static vIcon newI(&new_bits[0],new_height,new_width);
#include "open.vbm"
    static vIcon openI(&open_bits[0],open_height,open_width);
#include "paste.vbm"
    static vIcon pasteI(&paste_bits[0],paste_height,paste_width);
#include "pencil.vbm"
    static vIcon pencilI(&pencil_bits[0],pencil_height,pencil_width);
#include "poly.vbm"
    static vIcon polyI(&poly_bits[0],poly_height,poly_width);
#include "preview.vbm"
    static vIcon previewI(&preview_bits[0],preview_height,preview_width);
#include "print.vbm"
    static vIcon printI(&print_bits[0],print_height,print_width);
#include "rect.vbm"
    static vIcon rectI(&rect_bits[0],rect_height,rect_width);
#include "redo.vbm"
    static vIcon redoI(&redo_bits[0],redo_height,redo_width);
#include "save.vbm"
    static vIcon saveI(&save_bits[0],save_height,save_width);
#include "underln.vbm"
    static vIcon underlnI(&underln_bits[0],underln_height,underln_width);
#include "undo.vbm"
    static vIcon undoI(&undo_bits[0],undo_height,undo_width);
#include "zoom.vbm"
    static vIcon zoomI(&zoom_bits[0],zoom_height,zoom_width);

#include "alpha.vbm"
    static vIcon alphaI(&alpha_bits[0],alpha_height,alpha_width);
#include "back.vbm"
    static vIcon backI(&back_bits[0],back_height,back_width);
#include "book.vbm"
    static vIcon bookI(&book_bits[0],book_height,book_width);
#include "cursor.vbm"
    static vIcon cursorI(&cursor_bits[0],cursor_height,cursor_width);
#include "down.vbm"
    static vIcon downI(&down_bits[0],down_height,down_width);
#include "drawline.vbm"
    static vIcon drawlineI(&drawline_bits[0],drawline_height,drawline_width);
#include "fill.vbm"
    static vIcon fillI(&fill_bits[0],fill_height,fill_width);
#include "find.vbm"
    static vIcon findI(&find_bits[0],find_height,find_width);
#include "findagn.vbm"
    static vIcon findagnI(&findagn_bits[0],findagn_height,findagn_width);
#include "forward.vbm"
    static vIcon forwardI(&forward_bits[0],forward_height,forward_width);
#include "left.vbm"
    static vIcon leftI(&left_bits[0],left_height,left_width);
#include "page.vbm"
    static vIcon pageI(&page_bits[0],page_height,page_width);
#include "right.vbm"
    static vIcon rightI(&right_bits[0],right_height,right_width);
#include "up.vbm"
    static vIcon upI(&up_bits[0],up_height,up_width);

#include "moreicons.h"

    static CommandObject CommandBar[] =
      {
	{C_Button, M_Exit, 1, "Exit", NoList,CA_None, isSens, 0, 0, 0},
	{C_IconButton,20,20,"",&newI,CA_None,isSens,0,0,0,0,"new.vbm"},
	{C_IconButton,21,21,"",&openI,CA_None,isSens,0,0,0,0,"open.vbm"},
	{C_IconButton,29,29,"",&saveI,CA_None,isSens,0,0,0,0,"save.vbm"},
	{C_IconButton,14,14,"",&cutI,CA_None,isSens,0,0,0,0,"cut.vbm"},
	{C_IconButton,13,13,"",&copyI,CA_None,isSens,0,0,0,0,"copy.vbm"},
	{C_IconButton,22,22,"",&pasteI,CA_None,isSens,0,0,0,0,"paste.vbm"},
	{C_IconButton,31,31,"",&undoI,CA_None,isSens,0,0,0,0,"undo.vbm"},
	{C_IconButton,28,28,"",&redoI,CA_None,isSens,0,0,0,0,"redo.vbm"},
	{C_IconButton,26,26,"",&printI,CA_None,isSens,0,0,0,0,"print.vbm"},
	{C_IconButton,25,25,"",&previewI,CA_None,isSens,0,0,0,0,"preview.vbm"},
	{C_IconButton,44,44,"",&pageI,CA_None,isSens,0,0,0,0,"page.vbm"},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static CommandObject CommandBar2[] =
      {
	{C_IconButton,33,33,"",&alphaI,CA_None,isSens,0,0,0},
	{C_Frame,90,0,"",NoList,CA_NoSpace | CA_NoBorder,isSens,0,0,0},
	{C_ToggleIconButton,10,0,"",&boldI,CA_None,isSens,90,0,0,0,"bold.vbm"},
	{C_ToggleIconButton,18,0,"",&italicI,CA_None,isSens,90,10,0,0,"italic.vbm"},
	{C_ToggleIconButton,30,0,"",&underlnI,CA_None,isSens,90,18,0,0,"underln.vbm"},
	{C_IconButton,15,15,"",&drawI,CA_None,isSens,0,0,0,0,"draw.vbm"},
	{C_IconButton,11,11,"",&brushI,CA_None,isSens,0,0,0,0,"brush.vbm"},
	{C_IconButton,23,23,"",&pencilI,CA_None,isSens,0,0,0,0,"pencil.vbm"},
	{C_IconButton,38,38,"",&drawlineI,CA_None,isSens,0,0,0,0,"drawline.vbm"},
	{C_IconButton,39,39,"",&fillI,CA_None,isSens,0,0,0,0,"fill.vbm"},
	{C_IconButton,19,19,"",&lineI,CA_None,isSens,0,0,0,0,"line.vbm"},
	{C_IconButton,27,27,"",&rectI,CA_None,isSens,0,0,0,0,"rect.vbm"},
	{C_IconButton,16,16,"",&ellipI,CA_None,isSens,0,0,0,0,"ellip.vbm"},
	{C_IconButton,24,24,"",&polyI,CA_None,isSens,0,0,0,0,"poly.vbm"},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static CommandObject CommandBar3[] =
      {
	{C_IconButton,40,40,"",&findI,CA_None,isSens,0,0,0,0,"find.vbm"},
	{C_IconButton,41,41,"",&findagnI,CA_None,isSens,0,0,0,0,"findagn.vbm"},
	{C_IconButton,34,34,"",&backI,CA_None,isSens,0,0,0,0,"back.vbm"},
	{C_IconButton,42,42,"",&forwardI,CA_None,isSens,0,0,0,0,"forward.vbm"},
	{C_IconButton,46,46,"",&upI,CA_None,isSens,0,0,0,0,"up.vbm"},
	{C_IconButton,37,37,"",&downI,CA_None,isSens,0,0,0,0,"down.vbm"},
	{C_IconButton,43,43,"",&leftI,CA_None,isSens,0,0,0,0,"left.vbm"},
	{C_IconButton,45,45,"",&rightI,CA_None,isSens,0,0,0,0,"right.vbm"},
	{C_IconButton,36,36,"",&cursorI,CA_None,isSens,0,0,0,0,"cursor.vbm"},
	{C_IconButton,35,35,"",&bookI,CA_None,isSens,0,0,0,0,"book.vbm"},
	{C_IconButton,12,12,"",&checkI,CA_None,isSens,0,0,0,0,"check.vbm"},
	{C_IconButton,32,32,"",&zoomI,CA_None,isSens,0,0,0,0,"zoom.vbm"},
	{C_IconButton,17,17,"",&helpI,CA_None,isSens,0,0,0,0,"help.vbm"},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static vMenu StandardMenu[] =
      {
	{"&File", M_File, isSens, notUsed, notUsed, noKey, &ShortFile[0]},
	{NULL}
      };


//====================>>> iconCmdWindow::iconCmdWindow <<<====================
  iconCmdWindow::iconCmdWindow(char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {

    // The "Standard" window will consist of a menubar, a canvas,
    // an optional button bar, and an optional status bar.

    // First, create and add the proper panes to the CmdWindow
    // Note: there must be a correspondint delete in the destructor

    // The Menu Bar
    myMenu = new vMenuPane(StandardMenu);		// A short menu
    AddPane(myMenu);                            // add pane to window

    // The command pane
    myCmdPane = new vCommandPane(CommandBar);
    AddPane(myCmdPane);

    // The command pane (2)
    myCmdPane2 = new vCommandPane(CommandBar2);
    AddPane(myCmdPane2);

    // The command pane (3)
    myCmdPane3 = new vCommandPane(CommandBar3);
    AddPane(myCmdPane3);

    // The command pane (4)
    myCmdPane4 = new vCommandPane(CB1);
    AddPane(myCmdPane4);

    // The command pane (5)
    myCmdPane5 = new vCommandPane(CB2);
    AddPane(myCmdPane5);

    // The command pane (6)
    myCmdPane6 = new vCommandPane(CB3);
    AddPane(myCmdPane6);

    // The command pane (7)
    myCmdPane7 = new vCommandPane(CB4);
    AddPane(myCmdPane7);

    // The command pane (8)
    myCmdPane8 = new vCommandPane(CB5);
    AddPane(myCmdPane8);

    // The Canvas
    myCanvas = new vCanvasPane;              // a new canvas pane
    AddPane(myCanvas);                          // add the pane to window

    ShowWindow();

  }

//====================>>> testCmdWindow::~testCmdWindow <<<====================
  iconCmdWindow::~iconCmdWindow()
  {
    UserDebug(Destructor,"iconCmdWindow::~iconCmdWindow() destructor\n")

    delete myMenu;
    delete myCanvas;
    delete myCmdPane;
    delete myCmdPane2;
    delete myCmdPane3;
    delete myCmdPane4;
    delete myCmdPane5;
    delete myCmdPane6;
    delete myCmdPane7;
    delete myCmdPane8;
  }

//====================>>> testCmdWindow::WindowCommand <<<====================
  void iconCmdWindow::WindowCommand(ItemVal id, ItemVal val, CmdType cType)
  {

    // route all commands through here - menus and buttons
    char buff[20];

    switch (id)
      {
        case M_Exit:
          {
            theApp->Exit();
            break;
          }

      }
  }
//****************************************************************************
//=========================>>> iconApp::NewAppWin <<<==========================
  vWindow* iconApp::NewAppWin(vWindow* win, char* name, int w, int h, 
			      vAppWinInfo* winInfo)
  {
    // This version of NewAppWin is provided with the information
    // required to name and size a window.

    // Typically, these would get a file name or other information
    // that will setup the AppWinInfo class for information
    // specific to the application. Thus, each open window
    // usually represents a view of a file or data object.

    vWindow* thisWin = win;		// local copy to use
    vAppWinInfo* awinfo = winInfo;

    char *myname = name;

    if (!*name)
      {
	myname = "Icon Demo";		// make up a name
      }
	
    UserDebug1(Build,"iconApp::NewAppWin(%s)\n",myname);

    if (!thisWin)			// need to new a window
      {
	thisWin = new iconCmdWindow(myname, w, h);
      }

    // The real app will no doubt have its own AppWinInfo class.

    if (!awinfo)
	awinfo = new vAppWinInfo(myname);

    return vApp::NewAppWin(thisWin, myname, w, h, awinfo);
  }

//========================>>> iconApp::Exit <<<======================
  void iconApp::Exit(void)
  {
    // This is called to close all windows. If the app needs to
    // do something special, it can. Otherwise, it can call the
    // general vApp::Exit method, which will generate
    // appropriate calls the the specialized iconApp::CloseAppWin

    UserDebug(Build,"iconApp::Exit()\n");

    vApp::Exit();	// easy default behavior
  }

//=======================>>> iconApp::CloseAppWin <<<===========================
  int iconApp::CloseAppWin(vWindow* win)
  {
    // This will be called BEFORE a window has been unregistered or
    // closed.  The app can do whatever it needs to to close down
    // the data associated with this window. Then it can call the
    // general vApp::CloseAppWin to unregister and close this window.
    // Note that the win gives a handle that can be used with
    // vApp::getAppWinInfo to retrieve the AppWinInfo class.

    // Code to handle close of window (such as saving/closing a file)
    // would go here...

    // Now unregister and close the window.

    UserDebug(Build,"iconApp::CloseAppWin()\n");

    return vApp::CloseAppWin(win);
  }

//========================>>> iconApp::AppCommand <<<==============================
  void iconApp::AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType)
  {
    // Any commands not processed by the window will be passed
    // along to here for default treatment.

    UserDebug1(Build,"iconApp::AppCmd(ID: %d)\n",id);
    vApp::AppCommand(win, id, val, cType);
  }

//=========================>>> iconApp::KeyIn <<<==============================
  void iconApp::KeyIn(vWindow* win, vKey key, unsigned int shift)
  {
    // Any key strokes not processed by the window will be passed
    // along to here for default treatment.

    vApp::KeyIn(win, key, shift);
  }

//###########################################################################

  // EVERY V application needs the equivalent of the following line

  static iconApp icon_App("Icon Demo App");	// The instance of the app

//============================>>> AppMain <<<==============================
  int AppMain(int argc, char** argv)
  {
    // Use AppMain to create the main window

    (void) theApp->NewAppWin(0, "Icon Demo", 450, 50);

    return 0;
  }
