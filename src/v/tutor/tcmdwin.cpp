//========================================================================
//  tcmdwin.cpp:     Source file for tutorial cmdwin class
//
//      Copyright 1995,1996  Bruce E. Wampler. All rights reserved.
//========================================================================

// This file contains the source code for a typical command window
// derived from the vCmdWindow class. It will contain the definitions
// of the menu bar and command and status bars. It represents the main
// interaction point with the user.
// 
// We start out with the #includes needed to define this class plus
// any V utility dialogs such as vNotice we use.

#include <v/vnotice.h>  // so we can use notice
#include <v/vkeys.h>    // to map keys
#include <v/vutil.h>    // for utilities
#include <v/vfilesel.h> // for file select

#include "tcmdwin.h"            // our header file

// Now, we define static arrays for the menus, command bars, and
// status bars used by this window. This consists of defining the
// constants needed for IDs, followed by the static declarations of
// the menu and command arrays. Note that V predefines quite a few
// standard IDs which you can use instead of defining your own.

  // Start ID defines for the main window at 100

  enum {m_CheckMe = 100,	// for CheckMe command
        m_CopySens,		// for Set Copy Sensitive
        m_Dialog,		// to pop up the dialog
	m_ModalDialog,		// for modal dialog
	m_Clear};		// Clear screen

// Now, the static declarations of the menu arrays. You first define
// the pulldown menus, one for each main menu bar label.

    static vMenu FileMenu[] =    // Items for File menu
      {
        {"New",M_New,isSens,notChk,noKeyLbl,noKey,noSub},
        {"Open",M_Open,isSens,notChk,noKeyLbl,noKey,noSub},
        {"Save",M_Save,notSens,notChk,noKeyLbl,noKey,noSub},
        {"Save As",M_SaveAs,notSens,notChk,noKeyLbl,noKey,noSub},
        {"Close",M_Close,isSens,notChk,noKeyLbl,noKey,noSub},
#ifdef vDEBUG                   // Defines V debug code
        {"-",M_Line,notSens,notChk,noKeyLbl,noKey,noSub},
        {"Debug",M_SetDebug,isSens,notChk,noKeyLbl,noKey,noSub},
#endif
        {"-",M_Line,notSens,notChk,noKeyLbl,noKey,noSub},
        {"Exit",M_Exit,isSens,notChk,noKeyLbl,noKey,noSub},
        {NULL}
      };

    static vMenu EditMenu[] =    // Items for Edit menu
      {
        {"Cut",M_Cut,notSens,notChk,noKeyLbl,noKey,noSub},
        {"Copy",M_Copy,notSens,notChk,noKeyLbl,noKey,noSub},
        {"Paste",M_Paste,notSens,notChk,noKeyLbl,noKey,noSub},
        {NULL}
      };

    static vMenu TestMenu[] =   // Items for Test menu
      {
        {"CheckMe",m_CheckMe,isSens,notChk,noKeyLbl,
            noKey,noSub},
        {"Copy Sensitive",m_CopySens,isSens,notChk,noKeyLbl,
            noKey,noSub},
        {"Dialog",m_Dialog,isSens,notChk,noKeyLbl,
            noKey,noSub},
        {"Modal Dialog",m_ModalDialog,isSens,notChk,noKeyLbl,
            noKey,noSub},
        {NULL}
      };

    // Now, define the items on the menu bar

    vMenu StandardMenu[] =     // The menu bar with three items
      {
        {"File",M_File,isSens,notUsed,notUsed,noKey,&FileMenu[0]},
        {"Edit",M_Edit,isSens,notUsed,notUsed,noKey,&EditMenu[0]},
        {"Test",M_Test,isSens,notUsed,notUsed,noKey,&TestMenu[0]},
        {NULL}
      };

// We now define a command bar. Command bars are optional, and there
// may be more than one. You can place any CommandObject you want on a
// command bar.

    static CommandObject CommandBar[] =  // A simple command bar
      {
        {C_Label,999,0 ,"Command Bar",NoList,CA_None,
            isSens,NoFrame,0,0},
        {C_Button,M_Copy,M_Copy,"Copy",NoList,CA_None,
            notSens,NoFrame,0,0},
        {C_Button,m_Dialog,m_Dialog,"Dialog",NoList,CA_None,
            isSens,NoFrame,0,0},
        {C_Button,m_Clear,m_Clear,"Clear",NoList,CA_None,
            isSens,NoFrame,0,0},
        {C_Button,M_Exit,M_Exit,"Exit",NoList,CA_None,
            isSens,NoFrame,0,0},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}  // This ends list
      };

// Sometimes it is easier to define IDs near the definition of
// the dialog or status bar using them.

  enum {m_cmdMsg = 110, m_cmdCount, m_keyMsg, m_keyVal};

    static vStatus StatBar[] =    // Define a simple status bar
      {
        {"Commands issued: ",m_cmdMsg,CA_NoBorder,isSens,0},
        {" ",m_cmdCount,CA_None,isSens,0},
        {"Last keypress: ",m_keyMsg,CA_NoBorder,isSens,0},
        {"   ",m_keyVal,CA_None,isSens,0},
        {0,0,0,0,0}           // This ends list
      };

    static int copy_sens = 0;   // local for tracking sensitive

//======================>>> tCmdWindow::tCmdWindow <<<====================
  tCmdWindow::tCmdWindow(VCONST char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {
    // This is the constructor for tCmdWindow. 
    
    UserDebug1(Constructor,"tCmdWindow::tCmdWindow(%s) Constructor\n",name)

    // The "Standard" window will consist of a menubar, a canvas, an
    // optional button bar, and an optional status bar.
    // 
    // First, create and add the proper panes to the CmdWindow
    // Note: there must be a corresponding delete in the destructor

    // Create and add the standard Menu Bar to this window
    myMenu = new vMenuPane(StandardMenu);
    AddPane(myMenu);

    // Create and add the command pane to this window
    myCmdPane = new vCommandPane(CommandBar);
    AddPane(myCmdPane);

    // Create and add our Canvas pane to this window
    myCanvas = new tCanvasPane;
    AddPane(myCanvas);

    // Create and add the Status Bar to this window
    myStatus = new vStatusPane(StatBar);
    AddPane(myStatus);

    // In the V model, a window may have dialogs. Each dialog used
    // by a window must have an instance pointer. The easiest way
    // to create dialogs is to construct each one using a new here
    // which only defines the dialog - you need to use its
    // ShowDialog method at the appropriate time to display it).
    // You delete dialogs in the destructor for this window.
    // 
    // Now, create whatever dialogs this app defines:
    // instances of tDialog and tModalDialog

    sampleDialog = new tDialog(this);
    sampleModalDialog = new tModalDialog(this);

    // FINALLY, after all the panes have been constructed and
    // added, we must show the window!

    ShowWindow();
  }

//=====================>>> tCmdWindow::~tCmdWindow <<<====================
  tCmdWindow::~tCmdWindow()
  {
    UserDebug(Destructor,"tCmdWindow::~tCmdWindow() destructor\n")

    // Now put a delete for each new in the constructor.

    delete myMenu;
    delete myCanvas;
    delete myStatus;
    delete myCmdPane;
    delete sampleDialog;
    delete sampleModalDialog;
  }

//========================>>> tCmdWindow::KeyIn <<<=======================
  void tCmdWindow::KeyIn(vKey keysym, unsigned int shift)
  {
    // Keystrokes are routed to this window. This example code shows very
    // simple processing of keystrokes, and how to update the m_keyVal
    // field of the status bar.

    static char ctrl[] = "^X ";
    static char chr[] = " X ";

    if (vk_IsModifier(keysym))
        SetString(m_keyVal, "mod");     // change status bar
    else if (keysym < ' ')              // ctrl char
      {
        ctrl[1] = keysym + '@';
        SetString(m_keyVal, ctrl);      // change status bar
      }
    else if (keysym < 128)              // normal printable char
      {
        chr[1] = keysym;
        SetString(m_keyVal, chr);       // change status bar
      }
    else 
        SetString(m_keyVal, "+++");     // change status bar
  }

//====================>>> tCmdWindow::WindowCommand <<<===================
  void tCmdWindow::WindowCommand(ItemVal id, ItemVal val, CmdType cType)
  {
    // All commands generated from this window's menus and dialog bars
    // are routed through here.  The easiest way to handle commands is to
    // use a single, sometimes large switch. Each time you add a command
    // to a menu or command bar, add a case to the switch here. In this
    // example, we will use the V Notice dialog to indicate commands have
    // been entered.

    static int cmdCount = 0;    // Used for sample status update
    vNoticeDialog note(this);   // Used for default actions
    char buff[20];              // buffer for status bar

    ++cmdCount;                 // count commands that have been issued
    IntToStr(cmdCount,buff);    // Use V utility routine to get string
    SetString(m_cmdCount, buff);        // change status bar

    UserDebug1(CmdEvents,"tCmdWindow:WindowCommand(%d)\n",id)

    switch (id)                 // The main switch to handle commands
      {
        // File Menu commands ------------------------------------

        case M_New:             // For this example, we will open a
          {                     // new window using our NewAppWin.
            (void)theApp->NewAppWin(0,"",250, 100);
            break;
          }

        case M_Open:            // This demos vFileSelect dialog
          {
            char name[100] = "";        // start out with null name
            vFileSelect fsel(this);     // an instance of vFileSelect
	    int fI = 0;			// Filter index
            static char* filter[] = {   // Filter for file select
                "*", "*.txt", "*.c *.cpp *.cxx *.h", 0 };

            // Show the file select dialog
            int ans = fsel.FileSelect("Open file",name,99,filter,fI);

            if (ans && *name)   // User picked a file name
              {
                SetTitle(name); // Set title of window to name
                note.Notice(name);  // Show the name
              }
            else                // Notify no name selected
                note.Notice("No file name selected.");
	    break;
          }

        case M_Save:            // This would usually save a file
          {
            note.Notice("Save");
            break;
          }

        case M_SaveAs:          // Save to a specified name
          {
            note.Notice("Save As");
            break;
          }

	case M_Close:
	  {
	    theApp->CloseAppWin(this);
	    break;
	  }

#ifdef vDEBUG                   // Include debugging like this
        case M_SetDebug:
          {
            vDebugDialog debug(this);   // an instance of debug 
            debug.SetDebug();           // dialog - let user set
            break;
          }
#endif

        case M_Exit:            // Standard exit command
          {                     // Invoke the standard app Exit
            theApp->Exit();     // to close all windows
            break;              // will never get here
          }

        // Edit Menu commands ------------------------------------
        case M_Cut:             // Standard items for Edit menu
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

        // Test Menu commands ------------------------------------
        case m_CheckMe:         // Demonstrate using a checked menu
          {
            ItemVal curval = GetValue(id); // Get current status
            SetValue(m_CheckMe,!curval,Checked); // Toggle check

            if (curval)                 // Change menu label
                SetString(m_CheckMe,"Check Me");
            else
                SetString(m_CheckMe,"UnChk Me");
            break;
          }

        case m_CopySens:        // Demo changing sensitivity
          {
            copy_sens = !copy_sens;     // toggle
            // This will change both menu and command button
            SetValue(M_Copy,copy_sens,Sensitive);
            break;
          }

        case m_Dialog:          // Invoke our dialog
          {
            if (!sampleDialog->IsDisplayed())   // not twice!
                sampleDialog->ShowDialog("Sample Modeless Dialog");
            break;
          }

        case m_ModalDialog:     // Invoke our modal dialog
          {
            ItemVal val, id;
            id = sampleModalDialog->ShowModalDialog("Sample Modal",val);
            // Now do something useful with id and val ...
            break;
          }

        case m_Clear:           // Clear the canvas
          {
            myCanvas->Clear();  // Invoke the canvas Clear
            break;
          }

        default:                // route unhandled commands up
          {                     // to superclass
            vCmdWindow::WindowCommand(id, val, cType);
            break;
          }
      }
  }
