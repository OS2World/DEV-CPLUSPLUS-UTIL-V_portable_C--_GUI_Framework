//========================= viedcnv.h ===================================
// This is the header file for the icon editor picture class definition,
// the color palette class definition, and the myCanvasPane class definition.
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//=======================================================================

#ifndef VIEDCANVAS_H
#define VIEDCANVAS_H
 
#include <v/vcanvas.h>  // derive from vCanvas
#include <v/vpen.h>     // contain a vPen 
#include <v/vcolor.h>
#include <math.h>
 
#include <fstream.h>            // to save/restore drawings
 
#define DRAW  0
#define ERASE 1
 
    class vApp;
    class vWindow;
    class myCmdWindow;
 
    const int defsize = 16;     // default icon size is 16x16
    const int deflayers = 3;    // default number of layers is 3

    const int ImageLayer = 0;
    const int TransLayer = 1;
    const int BrushLayer = 2;

    class myCmdWindow;
 
    class Picture       // simple class for our icon
      {
      public:           //--------------------------------- public
        Picture(int width = defsize, int height = defsize); // Constructor given size
        ~Picture();     		// Destructor
 
        int _width, _height;    // size of icon
        int _x;                 // x coordinate for grid
        int _y;                 // y coordinate for grid
        int _index;             // index from color table
	int *pixels[deflayers];            // pointer to 3-D pixel arrays
 
        // New methods for this class
        int  GetIndex(int x, int y, int layer);			// get index from color table
        void SetIndex(int x, int y, int layer, int index);	// set index to pixel
        void Flatten();         				// collapse layers into picture
        void Clear(int layer, int thecolor = 0);		// clear given layer
        int  GetPicWidth();					// get defined picture width
        int  GetPicHeight();					// get defined picture height
 
      protected:        //------------------------------ protected
 
      private:          //-------------------------------- private
	int geti(int x,int y,int z) { return (pixels[z][(y*_width)+x]); }
        void seti(int x,int y, int z, int val) { pixels[z][(y*_width)+x] = val;}
      };
 
      class Color_Pal	// color palette class
      {
        public:
          Color_Pal();              	// Constructor
          ~Color_Pal();             	// Destructor
	  void init_Palette();		// init palette
 
          void makefg(int index);	// make index as the foreground color
          void makebg(int index);	// make index as the background color
          int  getfg();			// get the current foreground color
          int  getbg();			// get current background color
          
          vColor getColor(int index);	// get color by given index number from the color palette
          void setColor(int index, vColor value);	//  set color to index array location
          void setRed(int index, int value);	//  set red color value
          void setGreen(int index, int value);	//  set green color value
          void setBlue(int index, int value);	//  set blue color value
	  void SetColorsUsed(int c) { _colorsUsed = c; }
	  int ColorsUsed() { return _colorsUsed; }


          int _fg, _bg;
          vColor _Palette[256];
 
        private:
	  int _colorsUsed;
        };
 
    class myCanvasPane : public vCanvasPane
      {
      public:           //--------------------------------- public
        myCanvasPane(myCmdWindow* cw);	// Constructor
        virtual ~myCanvasPane();	// Destructor
        void NewCanvas(int width = defsize, int height = defsize);	// create a new canvas
 
        // Methods we will override.
 
        virtual void MouseDown(int X, int Y, int button);
        virtual void MouseUp(int X, int Y, int button);
        virtual void MouseMove(int x, int y, int button);
        virtual void MouseMotion(int x, int y);
        virtual void Redraw(int x, int y, int w, int h);
        virtual void Undo();
        virtual void Resize(int w, int h);
        virtual void HPage(int shown, int top);
        virtual void HScroll(int step);
        virtual void VPage(int shown, int top);
        virtual void VScroll(int step);
	void ResetScroll();
 
	void ResizeCanvas( int width, int height);

        // New methods for this class
	void SetFGColor(vColor& color);
	void SetBGColor(vColor& color);

        void ClearShapes();			// clears the drawing canvas
        int Read(char* name);		

        int ReadIcon(char* name); 

        int Save(char* name);

	// General iconIO save
        int SaveIcon(char* filename, char* iconname, char* ext);

        void SetDrawShape(int dm);

	// Draw grid to canvas
        void DrawGrid();

        void DrawPixel(int x, int y, int layer);
	void DrawDot(int x, int y);
        void DrawImage();
        int Changed() { return _changed; }
        void SetChanged(int chg) { _changed = chg; }
 
        // Functions to draw shapes
        void IconLine(int begx, int begy, int curx, int cury);
        void IconRect(int begx, int begy, int curx, int cury, int mode);
        void IconRdRect(int begx, int begy, int curx, int cury, int mode);
        void IconEllipse(int begx, int begy, int curx, int cury, int mode);
        void FillGrid(int x, int y);
        void DrawBrush(int x, int y, int mode);
        void FillTool(int x, int y, int the_index, int button);
        Color_Pal *_myColorPal;
        vColor *myPalette;
 
	// Functions to set the foreground and background colors
        int  getCurPenInd();
        void setCurPenCol(int index);
        void setfg(int index);
        void setbg(int index);
        myCmdWindow* _cwin;
 
        int  _curPen;
        vPen _PixelPen;
        vBrush _PixelBrush;
        int _snap;
        int _PixSize;
	int _drawGrid;
        Picture* _myPicture; 

        int _PrevDrawWhat;
        int _PrevDrawShape;
 
        int   _BrushWidth;
        int   _BrushHeight;                
        int   _BrushOffX;
        int   _BrushOffY;
        int   _BrushX;
        int   _BrushY;
        int   _BrushPrevX;
        int   _BrushPrevY;
        
      protected:        //------------------------------ protected
 
      private:          //-------------------------------- private
        void pixelwrite(int x, int y, int val);
	int pixelread(int x, int y);
	void fill(int x, int y, int width, int height, 
		int ov, int fillcolor);
        int getBaseAndExt(char* name, char* base, char* ext);
        void SetScrollBarPositions();
        int _DrawShape;
        int _changed;
        int _maxx, _maxy;
        int _begx;  int _begy;  int _curx;  int _cury;  // coords
        int _prex; int _prey;
        int _dx, _dy;
        int  Round(float num);
        int  MyAbs(int value);
 
        int   _x;               // temp x;
        int   _y;               // temp y;
        int   _startval;        // start value for for loop
        int   _endval;          // end value for for loop
 
        float _m;               // slope
        float _b;               // y intercept for line and y rad for ellipse
        float _a;               // x rad for ellipse
       
	int _rdh, _rdw;  // for smarter redraw
	
        int   _drawmode;
        int   _prevind;
 
        int   _SelectbegX;
        int   _SelectbegY;
        int   _SelectendX;
        int   _SelectendY;
        
      };
 
#endif
