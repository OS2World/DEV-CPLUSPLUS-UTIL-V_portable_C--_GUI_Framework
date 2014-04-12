//========================= viedcmdw.h ==================================
// This is the header file for the command window class.
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//=======================================================================

#ifndef VIEDCMDW_H
#define VIEDCMDW_H
 
#include <v/vcmdwin.h>  // So we can use vCmdWindow
#include <v/vmenu.h>    // For the menu pane
#include <v/vcmdpane.h> // command pane
#include <v/vpen.h>     // for a pen
#include <v/vbrush.h>   // for a brush
#include <v/vstatusp.h> // status pane
 
// Start defines for the main window with 100
 
    const int setPen = 0;
    const int setBrush = 1;
    const int setBG = 2;
 
// Declare Item values for window objects
    const ItemVal m_Clear = 100;
    const ItemVal m_CloseFile = 107;
    const ItemVal m_Undo = 108;
    const ItemVal m_Resize = 109;

    const ItemVal m_Zoom0 = 700;	// These values must correspond
    const ItemVal m_Zoom2 = 702;
    const ItemVal m_Zoom4 = 704;
    const ItemVal m_Zoom8 = 708;
    const ItemVal m_Zoom16 = 716;
    const ItemVal m_Zoom32 = 732;
 
    const ItemVal m_fgbgFrame = 2100;
    const ItemVal m_fgColor = 2101;
    const ItemVal m_bgColor = 2102;
    const ItemVal m_brushFrame = 2120;

    const ItemVal m_DrawCombo = 2151;
    const ItemVal m_QuickPick = 2152;
    const ItemVal m_ColorPalette = 2153;
    const ItemVal m_Snap = 2154;
    const ItemVal m_ToolBox = 2155;
    const ItemVal m_BrshBox = 2156;
    
    const ItemVal m_DrawPoint = 200;
    const ItemVal m_DrawLine = 201;
    const ItemVal m_DrawRect = 202;
    const ItemVal m_DrawRdRect = 204;
    const ItemVal m_DrawEllipse = 206;
    const ItemVal m_DrawZoom = 208;
    const ItemVal m_DrawFill = 209;
    const ItemVal m_DrawClear = 210;
    const ItemVal m_DrawUndo = 211;
    const ItemVal m_DrawText = 212;
    const ItemVal m_DrawDropper = 213;
    const ItemVal m_DrawSelect = 214;
    const ItemVal m_DrawPixel = 215;
    const ItemVal m_DrawGrid = 216;
    const ItemVal m_DrawRefresh = 217;
 
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
 
    const ItemVal m_MousePosLbl = 402;
    const ItemVal m_MousePosX = 403;
    const ItemVal m_MousePosY = 404;
 
    const ItemVal m_DrawAllDialog = 900;
    const ItemVal m_ColorAllDialog = 901;
    const ItemVal m_ToolAllDialog = 902;
    const ItemVal m_BrshAllDialog = 903;
    const ItemVal m_FillAllDialog = 904;
 
    class vColorDialog;
    class vToolDialog;
    class vBrshDialog;
    class myCanvasPane;
    class Picture;
    class myColor_Pal;
 
    class myCmdWindow : public vCmdWindow
      {
        friend int AppMain(int, char**);   // allow AppMain access
 
      public:           //--------------------------------- public
        myCmdWindow(VCONST char*, int, int);   	// Constructor with size
        virtual ~myCmdWindow();         	// Destructor
        virtual void WindowCommand(ItemVal id, ItemVal val,
                CmdType cType);			// executes commands from window
        int CheckClose();	// Checks is changes have been made to file before closing
        void OpenFile(char* fname);		// Opens a given file to the canvas
        void MouseXYStatus(int x, int y);	// Get the current X, Y status of the mouse
        int GetText(void);		// Allows user to enter text into a dialog box
        int write_text(char letter, int location); // writes text input on the canvas

	void SetFGBtn(vColor& color);
	void SetBGBtn(vColor& color);

        vColorDialog* myColDlg;		// pointer to our color dialog window
        
      protected:        //------------------------------ protected
 
      private:          //-------------------------------- private

        // Pointers to panes this window contains
        vMenuPane* myMenu;              // For the menu bar
        myCanvasPane* myCanvas;         // For the canvas
        vCommandPane* myCmdPane;        // for the command pane
        vCommandPane* myToolPane;       // for the command pane

        int GetDigit(char * digitstring); 	// Get a digit from the string 
	int GetWH(int& width, int& height);

        Picture *_myPicture;
        myColor_Pal *myColPal;
        vStatusPane* myStatus;          // for status pane
 
        vBrshDialog* myBrshDlg;		// pointer to brush box dialog
        
        int curWidth;			// current brush width		
        int curShape;			// current brush shape

        int _mx, _my;                   // Mouse coordinates
        vPen _pen;                      // track pen color
        vBrush _fgbrush;		// brush with foreground color
        vBrush _bgbrush;		// brush with background color
        vColor _bg;                     // background color
        ItemVal brushMenuId;
        ItemVal penMenuId;
        char _fname[100];               // file name
      };
#endif
