//================================================================
//  drawcmdw.cxx:     Source file for drawApp cmdwin class
//  Copyright (C) 1995  Bruce E. Wampler
//================================================================
#include <v/vnotice.h>	// so we can use notice
#include <v/vpen.h>	// for drawing pen
#include <v/vfilesel.h>	// for file selection

#include "drawcmdw.h"	// our header

//	Start defines for the main window with 100

  const ItemVal m_Clear = 100;
  const ItemVal m_LineWidth = 101;
  const ItemVal m_ChangePen = 102;
  const ItemVal m_ColorBtn = 103;

    // Define the File and Edit pulldown menus
    static vMenu FileMenu[] =
      {
	{"New", M_New, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Open", M_Open, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Save", M_Save, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Save As", M_SaveAs, isSens,notChk,noKeyLbl,noKey,noSub},
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Exit", M_Exit, isSens, notChk, noKeyLbl, noKey, noSub},
	{NULL}
      };

    static vMenu EditMenu[] =
      {
	{"Clear", m_Clear, isSens, notChk, noKeyLbl, noKey,noSub},
	{NULL}
      };

    // Define the main menu
    vMenu MainMenu[] =
      {
	{"File",M_File,isSens,notUsed,notUsed,noKey,&FileMenu[0]},
	{"Edit",M_Edit,isSens,notUsed,notUsed,noKey,&EditMenu[0]},
	{NULL}
      };

    // Define the command bar
    static int minMaxStep[3] = {1, 20, 1};	// for line width
    static vColor btnColor(0,0,0);
    static CommandObject CommandBar[] =
      {
	{C_Button, m_Clear, m_Clear, "Clear", NoList,
		CA_None,isSens,NoFrame,0,0},
	{C_Label, 999, 0 ," Pen Width: ", NoList,
		CA_None, isSens, NoFrame,0,0},
	{C_Spinner,m_LineWidth,1,"",(void*)&minMaxStep[0],
		CA_None,isSens,NoFrame,0,0},
	{C_Label, 999, 0, " Pen Color: ",NoList,
		CA_None,isSens,NoFrame,0,0},
	{C_ColorButton, m_ColorBtn, 0, "", (void*)&btnColor,
		CA_None,isSens,NoFrame,0,0,12},
#define vC_Size 12			// size of color buttons
#include<v/vcb2x8.h>	// define a color selection box
	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

//================>>> myCmdWindow::myCmdWindow <<<================
  myCmdWindow::myCmdWindow(VCONST char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {
    // Create and add the proper panes to the CmdWindow

    *_fname = 0;			// no name

    myMenu = new vMenuPane(MainMenu);	// Add the main menu
    AddPane(myMenu);

    myCmdPane = new vCommandPane(CommandBar); // Add command pane
    AddPane(myCmdPane);

    myCanvas = new myCanvasPane;	// Add the canvas pane
    AddPane(myCanvas);

    ShowWindow();	    // FINALLY, we must show the window!
  }

//===============>>> myCmdWindow::~myCmdWindow <<<================
  myCmdWindow::~myCmdWindow()
  {
    delete myMenu;	// Delete everything new'ed in
    delete myCanvas;	//   the constructor
    delete myCmdPane;
  }

//==============>>> myCmdWindow::WindowCommand <<<================
  void myCmdWindow::WindowCommand(ItemVal id, ItemVal val,
	CmdType cType)
  {
    // route all commands through here - menus and buttons

    static char* filter[] = {"*.drw","*", 0};	// file filter
    static int fi = 0;
    vNoticeDialog note(this);	// for user notification

    switch (id)		// switch on id of commands
      {
	case M_New:
	  {
	    // call our NewAppWin method
	    theApp->NewAppWin(0,"V Draw - No Name",500,250,0);
	    break;
	  }

	case M_Open:
	  {
	    if (*_fname)
	      {
		note.Notice("Create a New window first.");
		break;
	      }

            vFileSelect fsel(this);     // V file select dialog

            if (!fsel.FileSelect("Open V Draw File",
	      _fname,99,filter,fi) || !*_fname)
		break;			// ignore if no selection

	    if (!myCanvas->Read(_fname))	// Save in _fname
	      {
		note.Notice("Unable to read file");
		break;
	      }
	    SetTitle(_fname);		// show on title bar
	    myCanvas->Redraw(0,0,0,0);	// paint it
	    break;
	  }

	case M_Save:
	  {
	    if (*_fname)			// have a name
	      {
		if (!myCanvas->Save(_fname))	// Save in _fname
		    note.Notice("Unable to save file");
		break;
	      }
	    // else fall through to SaveAs
	  }

	case M_SaveAs:
	  {
            vFileSelect fsel(this);     // V file select dialog

            if (!fsel.FileSelectSave("Save V Draw File As",
	      _fname,99,filter,fi) || !*_fname)
		break;			// ignore if no selection

	    if (!myCanvas->Save(_fname))	// Save in _fname
	      {
		note.Notice("Unable to read file");
		break;
	      }
	    SetTitle(_fname);
	    break;
	  }

	case M_Exit:
	  {
	    theApp->Exit();	// Standard action for Exit
	    break;
	  }

	case m_LineWidth:
	  {
	    _pen.SetWidth(val);	// set new pen width
	    myCanvas->SetPen(_pen);
	    break;
	  }

	case m_Clear:			// clear the screen
	  {
	    myCanvas->Clear();
	    break;
	  }

	default:			// change pen color?
	  {
	    if (id >= M_Black && id <= M_White)  // color button
	      {
		_pen.SetColor(vStdColors[val]);
		myCanvas->SetPen(_pen);
		SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().r(),Red);
		SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().g(),Green);
		SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().b(),Blue);
	      }
	    else		// pass up hierarchy
		vCmdWindow::WindowCommand(id, val, cType);
	    break;
	  }
      }
  }
