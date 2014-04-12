//================================================================
//  vdrwcmdw.cxx:     Source file for drawApp cmdwin class
//  Copyright (C) 1995,1996  Bruce E. Wampler
//================================================================
#include <v/vnotice.h>  // so we can use notice
#include <v/vfilesel.h> // for file selection
#include <v/vutil.h>
#include <v/vynreply.h>
#include <v/vkeys.h>

#include "vdrwcmdw.h"   // our header
#include "vdrwcnv.h"            // myCanvasPane class
#include "vdrwdlg.h"    // for draw dialog

//-------------------------------------------------------------
// MENUS and COMMAND BARS
//-------------------------------------------------------------
    // Define the File and Edit pulldown menus
    static vMenu FileMenu[] =
      {
        {"&New", M_New, isSens, notChk, noKeyLbl, noKey, noSub},
        {"&Open...", M_Open, isSens, notChk, noKeyLbl, noKey, noSub},
        {"&Save", M_Save, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Save &as...", M_SaveAs, isSens,notChk,noKeyLbl,noKey,noSub},
        {"&Close", m_CloseFile, isSens,notChk,noKeyLbl,noKey,noSub},
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"&Print", M_Print, isSens, notChk, noKeyLbl, noKey, noSub},
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"E&xit", M_Exit, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };

    static vMenu EditMenu[] =
      {
        {"&Clear", m_Clr, isSens, notChk, noKeyLbl, noKey,noSub},
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"&Global Quick Pick", m_DrawAllDialog, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };

    static vMenu PenMenu[] =
      {
        {"&Solid", m_PenSolid, isSens, isChk, noKeyLbl, noKey,noSub},
        {"&Transparent", m_PenTransparent, isSens, notChk, noKeyLbl, noKey,noSub},
        {"&Dash", m_PenDash, isSens, notChk, noKeyLbl, noKey,noSub},
        {"D&ot", m_PenDot, isSens, notChk, noKeyLbl, noKey,noSub},
        {"D&ashDot", m_PenDashDot, isSens, notChk, noKeyLbl, noKey,noSub},
        {NULL}
      };

    static vMenu BrushMenu[] =
      {
        {"&Solid", m_BrushSolid, isSens, isChk, noKeyLbl, noKey,noSub},
        {"&Transparent", m_BrushTransparent, isSens, notChk, noKeyLbl, noKey,noSub},
        {"&Horizontal Hatch", m_BrushHoriz, isSens, notChk, noKeyLbl, noKey,noSub},
        {"&Vertical Hatch", m_BrushVert, isSens, notChk, noKeyLbl, noKey,noSub},
        {"&LeftDiag Hatch", m_BrushLeft, isSens, notChk, noKeyLbl, noKey,noSub},
        {"&RightDiag Hatch", m_BrushRight, isSens, notChk, noKeyLbl, noKey,noSub},
        {"&Cross Hatch", m_BrushCross, isSens, notChk, noKeyLbl, noKey,noSub},
        {"&DiagCross Hatch", m_BrushDiagCross, isSens, notChk, noKeyLbl, noKey,noSub},
        {NULL}
      };

    static vMenu DrawMenu[] =
      {
        {"&Point            ", m_DrawPoint, isSens, notChk, "Alt-P", 'p',noSub,VKM_Alt},
        {"&Line             ", m_DrawLine, isSens, isChk, "Alt-L", 'l', noSub, VKM_Alt},
        {"&Rectangle        ", m_DrawRect, isSens, notChk, "Alt-R",'r',noSub, VKM_Alt},
        {"R&ounded Rectangle", m_DrawRdRect, isSens, notChk, "Alt-O", 'o',noSub,VKM_Alt},
        {"&Ellipse          ", m_DrawEllipse, isSens, notChk, "Alt-E", 'e',noSub,VKM_Alt},
        {"&Triangle         ", m_DrawPoly, isSens, notChk, "Alt-T",'t',noSub,VKM_Alt},
#ifdef ICON
        {"Icon", m_DrawIcon, isSens, notChk, noKeyLbl, noKey,noSub},
#endif
        {NULL}
      };

    // Define the main menu
    vMenu MainMenu[] =
      {
        {"&File",M_File,isSens,notUsed,notUsed,noKey,&FileMenu[0]},
        {"&Edit",M_Edit,isSens,notUsed,notUsed,noKey,&EditMenu[0]},
        {"&Pen",90,isSens,notUsed,notUsed,noKey,&PenMenu[0]},
        {"&Brush",91,isSens,notUsed,notUsed,noKey,&BrushMenu[0]},
        {"&Draw",92,isSens,notUsed,notUsed,noKey,&DrawMenu[0]},
        {NULL}
      };

    // Define the command bar
    static vColor btnColor(0,0,0);

    static int minMaxStep[3] = {1, 20, 1};      // for line width

    static char *penbrush[] = {"Pen Color", "Brush Color", "Background", 0 };

    // IMPORTANT - this order must correspond to the order used in the
    // vDrawAllDialog class so SendWindowCommandAll can be used
    static char *drawShape[] =
      {
        "Point", "Line", "Rectangle", "Round Rect",
        "Ellipse", "Triangle", 0
      };
    static int drawVal[] =      // map val to command
      {
        m_DrawPoint, m_DrawLine, m_DrawRect, m_DrawRdRect,
        m_DrawEllipse, m_DrawPoly, 0
      };

    static CommandObject CommandBar[] =
      {
        {C_CheckBox, m_QuickPick, 0, "Quick Pick", NoList,
                CA_None,isSens,NoFrame,0,0},
        {C_ComboBox, m_PenBrushCombo, 0, "",(void*)penbrush,
                CA_None,isSens,NoFrame,0,0},
        {C_ColorButton, m_ColorBtn, 0, "", (void*)&btnColor,
                CA_None,isSens,NoFrame,0,0,16},
#define vC_Size 8               // size of color buttons
#include<v/vcb2x8.h>    // define a color selection box
        {C_Label, 999, 0 ,"Pen Width:", NoList,
                CA_None, isSens, NoFrame,0,0},

        {C_Spinner,m_LineWidth,1,"",(void*)&minMaxStep[0],
                CA_None,isSens,NoFrame,0,0},
        {C_ComboBox, m_DrawCombo, 1, "",(void*)drawShape,
                CA_None,isSens,NoFrame,0,0},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    vStatus StatBar[] =
      {
        {" Time: ", m_TimeLabel, CA_NoBorder, isSens, 0},
        {" 0:00:00 AM  00/00/00 ",m_Time, CA_None, isSens, 0},
        {"    Mouse X,Y: ", m_MousePosLbl, CA_NoBorder, isSens, 0 },
        {" 000 ", m_MousePosX, CA_None, isSens, 0},
        {" 000 ", m_MousePosY, CA_None, isSens, 0},
        {NULL,0,0,0,0}
      };

    static char* filter[] = {"*.drw","*", 0};   // file filter
    static int filterIndex = 0;

//================>>> myCmdWindow::myCmdWindow <<<================
  myCmdWindow::myCmdWindow(VCONST char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {
    // Create and add the proper panes to the CmdWindow

    *_fname = 0;                        // no name
    _brush.SetColor(0,0,0);             // black brush
    _pen.SetColor(0,0,0);               // black pen
    _bg.Set(255,255,255);               // white background
    curWidth = 1;
    _mx = _my = 0;
    changeColor = setPen;
    brushMenuId = m_BrushSolid;
    penMenuId = m_PenSolid;

    myMenu = new vMenuPane(MainMenu);   // Add the main menu
    AddPane(myMenu);

    myCmdPane = new vCommandPane(CommandBar); // Add command pane
    AddPane(myCmdPane);

    myCmdWindow* me  = this;
    myCanvas = new myCanvasPane(me);    // Add the canvas pane
    AddPane(myCanvas);
    myCanvas->SetDrawShape(m_DrawLine); // We will be drawing line
    curShape = m_DrawLine;

    // The Status Bar
    myStatus = new vStatusPane(StatBar); // a new status bar
    AddPane(myStatus);
    _timer = new myTimer(this);         // create timer
    _timer->TimerSet(1000);             // 2 second intervals


    myDrwDlg = 0;                       // no dialog yet
   
    ShowWindow();           // FINALLY, we must show the window!

    WindowCommand(m_CurTime, m_CurTime, C_Button);      // update clock

    myCanvas->ShowVScroll(1);  // And, now turn on scroll bars
    myCanvas->ShowHScroll(1);
  }

//===============>>> myCmdWindow::~myCmdWindow <<<================
  myCmdWindow::~myCmdWindow()
  {
    delete myMenu;      // Delete everything new'ed in
    delete myCanvas;    //   the constructor
    delete myCmdPane;
    delete myStatus;
    _timer->TimerStop();
    delete _timer;
    if (myDrwDlg)       // dialog?
        delete myDrwDlg;
  }

//==============>>> myCmdWindow::OpenFile <<<================
  void myCmdWindow::OpenFile(char* fname)
  {
    char fn[256];               // scratch copy
    int ix;

    if (*_fname)                // already have one!
        return;
    
    for (ix = 0 ; fname[ix] != 0 && ix < 255 ; ++ix)
      fn[ix] = fname[ix];       // safe copy
    fn[ix] = 0;

    if (!*fn)           // no file open yet
      {
        vFileSelect fsel(this);     // V file select dialog
        if (!fsel.FileSelect("Open V Draw File",
            fn,255,filter,filterIndex) || !*fn)
            return;
      }

    if (!myCanvas->Read(fn))    // Save in _fname
      {
        vNoticeDialog note(this);       // for user notification
        note.Notice("Unable to open file.");
        return;
      }
    SetTitle(fn);               // show on title bar
    strcpy(_fname,fn);          // keep copy
    myCanvas->Redraw(0,0,0,0);  // paint it
    myCanvas->SetChanged(0);    // not really changed
  }

//==============>>> myCmdWindow::CheckClose <<<================
  int myCmdWindow::CheckClose()
  {
    // return 1 if ok to close now, 0 to abort

    if (myCanvas->Changed())    // changes!
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

            if (!fsel.FileSelectSave("Save V Draw File As",
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
    char buff[12];

    _mx = x; _my = y;
    IntToStr(x,buff);   // convert x
    SetString(m_MousePosX,buff);
    IntToStr(y,buff);
    SetString(m_MousePosY,buff);
  }


//==============>>> myCmdWindow::WindowCommand <<<================
  void myCmdWindow::WindowCommand(ItemVal id, ItemVal val,
        CmdType cType)
  {
    // route all commands through here - menus and buttons

    vNoticeDialog note(this);   // for user notification

    switch (id)         // switch on id of commands
      {
        case M_New:     // new file
          {
            (void) theApp->NewAppWin(0,"V Draw - No Name",600,250,0);
            break;
          }

        case M_Open:                    // open existing file
          {
            if (*_fname)                // need to create a new window
              {
                // create new window, then open a file in it.
                myCmdWindow* cw = (myCmdWindow*)
                    theApp->NewAppWin(0,"V Draw - No Name",600, 250,0);
                cw->OpenFile("");       // open via dialog
                cw->RaiseWindow();      // bring to front
              }
            else
                OpenFile("");           // open a file using dialog
                
            break;
          }

        case m_CloseFile:               // Close current file
          {
            if (!myCanvas->Changed())   // not changed!
              {
                myCanvas->ClearShapes();
                *_fname = 0;
                SetTitle("V Draw - No Name");   // no file
                break;
              }
            // else fall through
          }

        case M_Save:                    // save current drawing
          {
            if (*_fname)                // already have a name
              {
                if (!myCanvas->Save(_fname))    // Save in _fname
                    note.Notice("Unable to save file");
                else if (id == m_CloseFile)
                  {
                    *_fname = 0;
                    SetTitle("V Draw - No Name");       // no file
                  }
                break;
              }
            // else fall through to SaveAs
          }

        case M_SaveAs:                  // save file as
          {
            vFileSelect fsel(this);     // V file select dialog

            if (!fsel.FileSelectSave("Save V Draw File As",
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
                SetTitle("V Draw - No Name");   // no file
              }
            else
                SetTitle(_fname);               // change title
            break;
          }

        case M_Exit:                    // quit all
          {
            if (CheckClose())           // ok to close
                theApp->Exit(); // Standard action for Exit
            break;
          }

        case m_LineWidth:               // change line width
          {
            _pen.SetWidth(val);         // set new pen width
            myCanvas->SetPen(_pen);
            break;
          }

        case m_Clr:                   // clear the screen
          {
            myCanvas->ClearShapes();
            break;
          }

        case M_Print:                   // print
          {
            myCanvas->Print();
            break;
          }

        case m_PenSolid:                // change to solid pen
          {
            _pen.SetStyle(vSolid);
            myCanvas->SetPen(_pen);
            SetValue(penMenuId,0,Checked);      // uncheck old style
            penMenuId = id;
            SetValue(penMenuId,1,Checked);      // check new style
            break;
          }

        case m_PenTransparent:          // transparent pen
          {
            _pen.SetStyle(vTransparent);
            myCanvas->SetPen(_pen);
            SetValue(penMenuId,0,Checked);
            penMenuId = id;
            SetValue(penMenuId,1,Checked);
            break;
          }

        case m_PenDash:                 // dashed pen
          {
            _pen.SetStyle(vDash);
            myCanvas->SetPen(_pen);
            SetValue(penMenuId,0,Checked);
            penMenuId = id;
            SetValue(penMenuId,1,Checked);
            break;
          }

        case m_PenDot:                  // dotted pen
          {
            _pen.SetStyle(vDot);
            myCanvas->SetPen(_pen);
            SetValue(penMenuId,0,Checked);
            penMenuId = id;
            SetValue(penMenuId,1,Checked);
            break;
          }

        case m_PenDashDot:              // dashed-dotted pen
          {
            _pen.SetStyle(vDashDot);
            myCanvas->SetPen(_pen);
            SetValue(penMenuId,0,Checked);
            penMenuId = id;
            SetValue(penMenuId,1,Checked);
            break;
          }

        case m_BrushSolid:              // solid brush
          {
            _brush.SetStyle(vSolid);
            myCanvas->SetBrush(_brush);
            SetValue(brushMenuId,0,Checked);
            brushMenuId = id;
            SetValue(brushMenuId,1,Checked);
            break;
          }

        case m_BrushTransparent:        // transparent brush
          {
            _brush.SetStyle(vTransparent);
            myCanvas->SetBrush(_brush);
            SetValue(brushMenuId,0,Checked);
            brushMenuId = id;
            SetValue(brushMenuId,1,Checked);
            break;
          }

        case m_BrushHoriz:              // horizontal brush
          {
            _brush.SetStyle(vHorizontalHatch );
            myCanvas->SetBrush(_brush);
            SetValue(brushMenuId,0,Checked);
            brushMenuId = id;
            SetValue(brushMenuId,1,Checked);
            break;
          }

        case m_BrushVert:               // vertical brush
          {
            _brush.SetStyle(vVerticalHatch);
            myCanvas->SetBrush(_brush);
            SetValue(brushMenuId,0,Checked);
            brushMenuId = id;
            SetValue(brushMenuId,1,Checked);
            break;
          }

        case m_BrushLeft:               // left diagonal hatch
          {
            _brush.SetStyle(vLeftDiagonalHatch );
            myCanvas->SetBrush(_brush);
            SetValue(brushMenuId,0,Checked);
            brushMenuId = id;
            SetValue(brushMenuId,1,Checked);
            break;
          }

        case m_BrushRight:              // right diagonal hatch
          {
            _brush.SetStyle(vRightDiagonalHatch );
            myCanvas->SetBrush(_brush);
            SetValue(brushMenuId,0,Checked);
            brushMenuId = id;
            SetValue(brushMenuId,1,Checked);
            break;
          }

        case m_BrushCross:              // cross hatch
          {
            _brush.SetStyle(vCrossHatch );
            myCanvas->SetBrush(_brush);
            SetValue(brushMenuId,0,Checked);
            brushMenuId = id;
            SetValue(brushMenuId,1,Checked);
            break;
          }

        case m_BrushDiagCross:          // cross diagonal
          {
            _brush.SetStyle(vDiagonalCrossHatch );
            myCanvas->SetBrush(_brush);
            SetValue(brushMenuId,0,Checked);
            brushMenuId = id;
            SetValue(brushMenuId,1,Checked);
            break;
          }

        case m_DrawLine:                // set what we are drawing
        case m_DrawPoly:
        case m_DrawRect:
        case m_DrawRdRect:
#ifdef ICON
        case m_DrawIcon:                        // icon
#endif
        case m_DrawPoint:
        case m_DrawEllipse:
          {
            SetValue(curShape,0,Checked);       // fix the menu check
            curShape = id;
            SetValue(curShape,1,Checked);
            myCanvas->SetDrawShape(id);
            for (int xx = 0 ; drawVal[xx] != 0 ; ++xx) // need to make combo
              {                                 // have same as menu
                if (drawVal[xx] == id)
                  {
                    SetValue(m_DrawCombo, xx, Value);
                    break;
                  }
              }
            break;
          }

        case m_PenBrushCombo:           // what color map represents
          {
            if (val == setPen)          // pen
              {
                changeColor = setPen;
                SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().r(),Red);
                SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().g(),Green);
                SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().b(),Blue);
              }
            else if (val == setBrush)   // brush
              {
                changeColor = setBrush;
                SetValue(m_ColorBtn,(ItemVal)_brush.GetColor().r(),Red);
                SetValue(m_ColorBtn,(ItemVal)_brush.GetColor().g(),Green);
                SetValue(m_ColorBtn,(ItemVal)_brush.GetColor().b(),Blue);
              }
            else                        // background
              {
                changeColor = setBG;
                SetValue(m_ColorBtn,(ItemVal)_bg.r(),Red);
                SetValue(m_ColorBtn,(ItemVal)_bg.g(),Green);
                SetValue(m_ColorBtn,(ItemVal)_bg.b(),Blue);
              }
            break;
          }

        case m_DrawCombo:               // select which shape to draw
          {
            SetValue(curShape,0,Checked);  // fix the menu check
            curShape = drawVal[val];       // map to shape
            SetValue(curShape,1,Checked);
            myCanvas->SetDrawShape(curShape);
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
            SetString(m_Time,buff);
            break;
          }

        case m_QuickPick:               // open the quick pick dialog
          {
            if (val)
              {
                if (!myDrwDlg)          // not created yet
                    myDrwDlg = new vDrawDialog(this, 0);
                myDrwDlg->ShowDialog("Drawing Attributes");
                int l,t,w,h;
                GetPosition(l,t,w,h);   // my position
                myDrwDlg->SetDialogPosition(l+w+2,t);   // put quick pick here
              }
            else                        // close the dialog
              {
                myDrwDlg->CloseDialog();
              }
            break;
          }

        default:                        // change pen color?
          {
            if (id >= M_Black && id <= M_White)  // color button
              {
                if (changeColor == setPen)      // what we are changing?
                  {
                    _pen.SetColor( vStdColors[val]);
                    myCanvas->SetPen(_pen);
                    SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().r(),Red);
                    SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().g(),Green);
                    SetValue(m_ColorBtn,(ItemVal)_pen.GetColor().b(),Blue);
                  }
                else if (changeColor == setBrush)
                  {
                    _brush.SetColor(vStdColors[val]);
                    myCanvas->SetBrush(_brush);
                    SetValue(m_ColorBtn,(ItemVal)_brush.GetColor().r(),Red);
                    SetValue(m_ColorBtn,(ItemVal)_brush.GetColor().g(),Green);
                    SetValue(m_ColorBtn,(ItemVal)_brush.GetColor().b(),Blue);
                  }
                else                    // set background
                  {
                    _bg.Set(vStdColors[val]);
                    myCanvas->SetBackground(_bg);
                    SetValue(m_ColorBtn,(ItemVal)_bg.r(),Red);
                    SetValue(m_ColorBtn,(ItemVal)_bg.g(),Green);
                    SetValue(m_ColorBtn,(ItemVal)_bg.b(),Blue);
                  }
              }
            else                        // pass up hierarchy
                vCmdWindow::WindowCommand(id, val, cType);
            break;
          }
      }
  }
