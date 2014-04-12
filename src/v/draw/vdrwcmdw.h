//================================================================
//  vdrwcmdw.h:	Header file for V draw command window
//  Copyright (C) 1995,1996  Bruce E. Wampler
//================================================================
#ifndef DRAWCMDW_H
#define DRAWCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vcmdpane.h> // command pane
#include <v/vpen.h>	// for a pen
#include <v/vbrush.h>	// for a brush
#include <v/vstatusp.h>	// status pane
#include <v/vtimer.h>	// timer

//	Start defines for the main window with 100

    const int setPen = 0;
    const int setBrush = 1;
    const int setBG = 2;

    const ItemVal m_Clr = 100;
    const ItemVal m_LineWidth = 101;
    const ItemVal m_ChangePen = 102;
    const ItemVal m_ColorBtn = 103;
    const ItemVal m_LineUp = 104;
    const ItemVal m_LineDown = 105;
    const ItemVal m_DrawAttrSub = 106;
    const ItemVal m_CloseFile = 107;

    const ItemVal m_PenBrushCombo = 150;
    const ItemVal m_DrawCombo = 151;
    const ItemVal m_QuickPick = 152;

    const ItemVal m_DrawLine = 200;
    const ItemVal m_DrawPoly = 201;
    const ItemVal m_DrawRect = 202;
    const ItemVal m_DrawRdRect = 203;
    const ItemVal m_DrawEllipse = 204;
    const ItemVal m_DrawIcon = 205;
    const ItemVal m_DrawPoint = 206;

    const ItemVal m_PenSolid = 301;
    const ItemVal m_PenTransparent = 302;
    const ItemVal m_PenDash = 303;
    const ItemVal m_PenDot = 304;
    const ItemVal m_PenDashDot = 305;

    const ItemVal m_BrushSolid = 310;
    const ItemVal m_BrushTransparent = 311;
    const ItemVal m_BrushHoriz = 312;
    const ItemVal m_BrushVert = 313;
    const ItemVal m_BrushLeft = 314;
    const ItemVal m_BrushRight = 315;
    const ItemVal m_BrushCross = 316;
    const ItemVal m_BrushDiagCross = 317;

    const ItemVal m_TimeLabel = 400;
    const ItemVal m_Time = 401;
    const ItemVal m_MousePosLbl = 402;
    const ItemVal m_MousePosX = 403;
    const ItemVal m_MousePosY = 404;
    const ItemVal m_CurTime = 405;

    const ItemVal m_DrawAllDialog = 900;

    class vDrawDialog;
    class myCanvasPane;
    class myTimer;

    class myCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);   // allow AppMain access

      public:		//--------------------------------- public
	myCmdWindow(VCONST char*, int, int);	// Constructor with size
	virtual ~myCmdWindow();		// Destructor
	virtual void WindowCommand(ItemVal id, ItemVal val,
		CmdType cType);
	int CheckClose();
	void OpenFile(char* fname);
	void MouseXYStatus(int x, int y);
	
      protected:	//------------------------------ protected

      private:		//-------------------------------- private
	// Pointers to panes this window contains
	vMenuPane* myMenu;		// For the menu bar
	myCanvasPane* myCanvas;		// For the canvas
	vCommandPane* myCmdPane;	// for the command pane
	vStatusPane* myStatus;		// for status pane

	myTimer* _timer;

	vDrawDialog* myDrwDlg;

	int curWidth;
	int curShape;
	int changeColor;
	// other stuff
        int _mx, _my;			// Mouse coordinates
	vPen _pen;			// track pen color
	vBrush _brush;
	vColor _bg;			// background color
	ItemVal brushMenuId;
	ItemVal penMenuId;
	char _fname[100];		// file name
      };

    class myTimer : public vTimer
      {
      public:		//---------------------------------------- public
	myTimer(myCmdWindow* cw) { cmdw = cw; }
	~myTimer() {}
	virtual void TimerTick() {cmdw->WindowCommand(m_CurTime, m_CurTime, C_Button);}
      private:		//--------------------------------------- private
	myCmdWindow* cmdw;
      };
#endif
