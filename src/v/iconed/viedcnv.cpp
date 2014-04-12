//========================= viedcnv.cxx =================================
// This is the source file for the icon editor picture class, the color
// palette class, and the canvas pane class.   -
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//=======================================================================

#include <fstream.h>
#include <ctype.h>
#include <v/vapp.h>
#include <v/vnotice.h>

#include "viedcmdw.h"
#include "viedcnv.h"
#include "coldlg.h"
#include "imageio.h"

//================>>> myCanvasPane::myCanvasPane <<<==============
  myCanvasPane::myCanvasPane(myCmdWindow* cw)
  {
    // Initialize items in the canvas pane to default values

    _DrawShape = m_DrawPoint;    // set draw tool to line
    _begx = -100;  _begy = -100;
    _curx = -100;  _cury = -100;
    _changed = 0;       	// not changed
    _dx = _dy = 0;      	// no shift yet
    _cwin = cw;
    _drawGrid = 1;
    _drawmode = DRAW;
    _rdh =  _rdw = 0;	// for smarter redraws


    // define an instance of the Picture class
    _myPicture = new Picture();	

    // define an instance of the Color Palette class
    _myColorPal = new Color_Pal();

    _PixSize = 8;

    _SelectbegX = -1;  _SelectbegY = -1;
    _SelectendX = -1;  _SelectendY = -1;
    _BrushOffX = 0;  _BrushOffY = 0;
    _BrushWidth = 1; _BrushHeight = 1;
    _BrushX = 0;     _BrushY = 0;
    _BrushPrevX = -1;  _BrushPrevY = -1;

    _myPicture->SetIndex(0,0,BrushLayer,-99);	// reset picture index table
    _prevind = -1;			// reset previous index
    _snap = 0;				// turn snap utility off
    _maxx = _PixSize * _myPicture->GetPicWidth();
    _maxy = _PixSize * _myPicture->GetPicHeight();
  }

//==============>>> myCanvasPane::~myCanvasPane <<<================
  myCanvasPane::~myCanvasPane()
  {
    // Destroys the myCanvasPane class instance
    if (_myPicture != 0)
	delete _myPicture;
    if (_myColorPal != 0)
	delete _myColorPal;
  }

//=================>>> myCanvasPane::ResizeCanvas <<<=====================
  void myCanvasPane::ResizeCanvas( int width, int height)
  {
    // Resizes canvas to given width and height

    int* oldPic;
    int oldH, oldW, x, y;
    long oldLim;

    if (width < 2 || height < 2)
	return; 

    oldH = _myPicture->_height;
    oldW = _myPicture->_width;
    if (oldH == height && oldW == width)	// no op
	return;
    
    _myPicture->Flatten();  // do flatten in case trans layer still holds op

    oldLim = (long)oldW * (long)oldH;
    oldPic = new int[oldLim];		// space for temp copy
    // Copy current pic
    for (x = 0 ; x < oldW ; ++x)
      {
	for (y = 0 ; y < oldH ; ++y)
	  {
	    oldPic[(x*oldH)+y] = 
		_myPicture->GetIndex(x,y,ImageLayer);
	  }
      }

    if (_myPicture != 0)
	delete _myPicture;
    _myPicture = new Picture(width, height);

    _myPicture->Clear(BrushLayer);
    _myPicture->Clear(TransLayer);

    // Now, copy upper left corner of old image into new one
    for (x = 0 ; x < oldW && x < width; ++x)
      {
	for (y = 0 ; y < oldH && y < height; ++y)
	  {
	    _myPicture->SetIndex(x, y, ImageLayer, 
		oldPic[(x*oldH)+y]);
	  }
      }

    delete [] oldPic;		// Free the storage
    setfg(0); setbg(1);
    _changed = 1;   // has changed now

    Clear();
    Redraw(0,0,0,0);
  }

//=================>>> myCanvasPane::NewCanvas <<<=====================
  void myCanvasPane::NewCanvas( int width, int height)
  {
    // Creates a new canvas with given width and height

    if (_myPicture != 0)
	delete _myPicture;
    _myPicture = new Picture(width, height);
    if (_myColorPal)
	_myColorPal->init_Palette();
//    Redraw(0,0,0,0);
  }

//=================>>> myCanvasPane::SetDrawShape <<<=====================
  void myCanvasPane::SetDrawShape(int dm)
  { 
    // Check if setting a brush
    if (dm == m_DrawSelect)
      {
	_myPicture->Clear(BrushLayer, _myColorPal->getbg());
	_myPicture->SetIndex(0,0,BrushLayer,-99);
	_BrushWidth = 1;
	_BrushHeight = 1;
	_BrushOffX = 0;
	_BrushOffY = 0;
      }
    else if (dm == m_DrawText)
      {
      }
    else if (dm == m_DrawPixel)
      {
	_myPicture->Clear(BrushLayer, _myColorPal->getbg());
	_myPicture->SetIndex(0,0,BrushLayer,-99);
	_BrushWidth = 1;
	_BrushHeight = 1;
	_BrushOffX = 0;
	_BrushOffY = 0;
      }
    // else a just shape
    
    _DrawShape = dm;
  }

//=================>>> myCanvasPane::SetFGColor <<<=====================
  void myCanvasPane::SetFGColor(vColor& color)
  {
    // Search current palette and set to BG color to that color.
    // If we don't have a match, then we will allocate a new color
    // at the end of the palette until we run out of colors

    int colorsUsed = _myColorPal->ColorsUsed();

    for (int ix = 0 ; ix < colorsUsed ; ++ix)
      {
	if (_myColorPal->_Palette[ix] == color)
	  {
	    setfg(ix);
	    return;
	  }
      }

    // Fall through means no match, so have to do more
    if (colorsUsed < 256)	// can add
      {
	// change next free color palette entry
	_myColorPal->_Palette[colorsUsed].ResetColor(color);
	_myColorPal->SetColorsUsed(colorsUsed+1);
	setfg(colorsUsed);
      }
    else
	setfg(0);    // @@ for now, just use 0 (need message)
  }

//=================>>> myCanvasPane::SetBGColor <<<=====================
  void myCanvasPane::SetBGColor(vColor& color)
  {
    // Search current palette and set to BG color to that color.
    // If we don't have a match, then we will allocate a new color
    // at the end of the palette until we run out of colors

    int colorsUsed = _myColorPal->ColorsUsed();

    for (int ix = 0 ; ix < colorsUsed ; ++ix)
      {
	if (_myColorPal->_Palette[ix] == color)
	  {
	    setbg(ix);
	    return;
	  }
      }
    // Fall through means no match, so have to do more
    if (colorsUsed < 256)	// can add
      {
	// change next free color palette entry
	_myColorPal->_Palette[colorsUsed].ResetColor(color);
	_myColorPal->SetColorsUsed(colorsUsed+1);
	setbg(colorsUsed);
      }
    else
	setbg(1);
  }

//=================>>> myCanvasPane::DrawGrid <<<=====================
  void myCanvasPane::DrawGrid()
  {
    //  This will draw the black grid and set scroll bars if needed

    int x, width, height;
    vPen gridPen;
    vBrush gridBrush;


    // set the pen and brush settings to draw the grid
    gridPen.SetColor(0,0,0);
    SetPen(gridPen);
    gridBrush.SetColor(255,255,255);
    gridBrush.SetStyle(vTransparent);
    SetBrush(gridBrush);

    // find width and height of icon in order to scale the grid
    width = _myPicture->GetPicWidth();
    height = _myPicture->GetPicHeight();

    if (_drawGrid)
      {
	// draw the grids vertical lines
	for (x = 0 ; x <= width ; x++)
	  {
	    DrawLine(x*_PixSize, 0, x*_PixSize, height*_PixSize);
	  }

	// draw the grids horizontal lines
	for (x=0; x<=height; x++)
	  {
	    DrawLine(0, x*_PixSize, width*_PixSize, x*_PixSize);
	  }
      }
    else		// Just a box
      {
	DrawLine(0, 0, 0, height*_PixSize+1);
	DrawLine(width*_PixSize+1, 0, width*_PixSize+1, height*_PixSize+1);
	DrawLine(0, 0, width*_PixSize+1, 0);
	DrawLine(0, height*_PixSize+1, width*_PixSize+1, height*_PixSize+1);
      }


    // Draw Border around small Icon (actual size icon)
    DrawLine(_myPicture->_width * _PixSize + 10 - 1, 10 - 1,
        (_myPicture->_width * _PixSize + 10 + 1) + _myPicture->_width, 10 - 1);
    DrawLine(_myPicture->_width * _PixSize + 10 - 1, 10 + _myPicture->_height,
        (_myPicture->_width * _PixSize + 10 + 1) + _myPicture->_width,
        10 + _myPicture->_height);

    DrawLine(_myPicture->_width * _PixSize + 10 - 1, 10,
        _myPicture->_width * _PixSize + 10 - 1, 10 + _myPicture->_height);

    DrawLine((_myPicture->_width * _PixSize + 10) + _myPicture->_width, 10,
        (_myPicture->_width * _PixSize + 10) + _myPicture->_width,
        10 + _myPicture->_height);

    // reset pen and brush to default settings
    _PixelPen.SetColor(0,0,0);
    _PixelPen.SetStyle(vTransparent);
    SetPen(_PixelPen);
    _PixelBrush.SetColor(255,255,255);
    _PixelBrush.SetStyle(vSolid);
    SetBrush(_PixelBrush);

    // set max values used by scroll bars
    _maxx = _PixSize * _myPicture->GetPicWidth();
    _maxy = _PixSize * _myPicture->GetPicHeight();;

    // set scroll bars
    SetScrollBarPositions();
  }

//=================>>> myCanvasPane::DrawPixel <<<====================
  void myCanvasPane::DrawDot(int x, int y)
  {
    // draw zoomed pixel
    if (_drawGrid)
	DrawRectangle(_PixSize*x+1, _PixSize*y+1, _PixSize-1, _PixSize-1);
    else
	DrawRectangle(_PixSize*x+1, _PixSize*y+1, _PixSize, _PixSize);

    // draw actual pixel
    DrawRectangle(x + (_myPicture->_width * _PixSize) + 10, y + 10, 1,1);
  }

//=================>>> myCanvasPane::DrawPixel <<<====================
  void myCanvasPane::DrawPixel(int x, int y, int layer)
  {
    //  This will draw a zoomed pixel (scaled for the zoomed icon)
    //  and an actual pixel (icon on the side that is the actual sized icon)
    //  It draws the pixel from layer 'layer' at x,y

    int tempind;	// temporary index

    // set temp index to the index from x,y in the layer 'layer'
    tempind = _myPicture->GetIndex(x,y,layer);

    // check to see if it is an alpha (transparent)
    if( tempind != -1 )
      {  // not transparent

	// We found that V is slowing down the redraw so we try to
	// improve the speed by only changing the pen/brush color when
	// we had to.

	if ( tempind != _prevind )
	  {  // current color differs from needed
	    _PixelBrush.SetColor(_myColorPal->getColor(tempind));
	    SetBrush(_PixelBrush);
	    _prevind = tempind;  	// set this color as current color
	  }

	// draw zoomed pixel
	if (_drawGrid)
	    DrawRectangle(_PixSize*x+1, _PixSize*y+1, _PixSize-1, _PixSize-1);
	else
	    DrawRectangle(_PixSize*x+1, _PixSize*y+1, _PixSize, _PixSize);

	// draw actual pixel
	DrawRectangle(x + (_myPicture->_width * _PixSize) + 10, y + 10, 1,1);
      }
  }

//=================>>> myCanvasPane::DrawImage <<<====================
  void myCanvasPane::DrawImage()
  {
    //  Draw entire image - used on a redraw command.  It walks through the
    //  the entire array and draws the image and transparent layer only.

    int x, y, width, height, ind;

    // get icon height and width
    width = _myPicture->GetPicWidth();
    height = _myPicture->GetPicHeight();

    int colorsUsed = _myColorPal->ColorsUsed();
    int colorSet;

    // walk through array by color to optimize resetting of colors
    for (int c = 0 ; c < colorsUsed ; ++c)
      {
	colorSet = 0;
	for (x = 0 ; x < width ; x++)
	  {
	    for (y = 0 ; y < height ; y++)
	      {
		// set index to the x,y cord on transparent layer
		ind = _myPicture->GetIndex(x, y, TransLayer);
		if (ind == -1)
		    ind = _myPicture->GetIndex(x, y, ImageLayer);
		if (ind == c)	// found an instance of this color
		  {
		    // This is the major optimization - we will set
		    // the color only once per color
		    if (!colorSet)
		      {
			colorSet = 1;
			_PixelBrush.SetColor(_myColorPal->getColor(ind));
			SetBrush(_PixelBrush);
		      }
		    DrawDot(x,y);		// And draw the dot
		  }
	      }
	  }
      }
  }

//=================>>> myCanvasPane::ClearShapes <<<====================
  void myCanvasPane::ClearShapes()
  {
    //  This will clear all shapes and pixels on the canvas screen

    Clear();       // clear the canvas
    _prevind = -1; // force the pen to be updated next time DrawPixel is called
    DrawGrid();    // Draw the grid and set scroll bars

    // clear the image and transparent layers
    _myPicture->Clear(ImageLayer, _myColorPal->getbg());
    _myPicture->Clear(TransLayer);

    // set top and left start point to 0
    _dx = _dy = 0;
    SetTranslate(_dx,_dy);
  }

//================>>> myCanvasPane::MouseDown <<<=================
  void myCanvasPane::MouseDown(int x, int y, int button)
  {
    //  Everytime a mouse button is depressed run this command

    int ind;
    vColor clrPen;

    // Mouse down means beginning to draw a shape

    // first we flatten the transparent layer and redraw
    _myPicture->Flatten();
//    Redraw(0,0,0,0);

    // set foreground or background color as active
    if (button == 1)
      {
	_curPen = _myColorPal->getfg();
      }
    else if (button == 3)
      {
	_curPen = _myColorPal->getbg();
      }

    // set mouse cordinates
    _begx = _curx = x - _dx;
    _begy = _cury = y - _dy;
    _prex = _curx;
    _prey = _cury;

    _cwin->MouseXYStatus(_curx/_PixSize, _cury/_PixSize);

    // if text was selected set mode to point
    if (_DrawShape == m_DrawText)
	_DrawShape = m_DrawPoint;

    // if pixel was selected set mode to point
    if (_DrawShape == m_DrawPixel)
	_DrawShape = _PrevDrawShape;

    switch (_DrawShape)
      {
	case m_DrawPoint:		// Draw tool point is selected
	  {
	    // drawing a point is drawing a line at start and end
	    // values are the same
	    IconLine(_begx/_PixSize,_begy/_PixSize,_begx/_PixSize,_begy/_PixSize);
	    _changed = 1;   // has changed now
	    break;
	  }

	case m_DrawFill:         // mode tool fill is selected
	  {
	    _myPicture->Flatten();	// Flatten before we fill

	    // get index from image layer as test pixel
	    ind = _myPicture->GetIndex(_begx/_PixSize,_begy/_PixSize,ImageLayer);

	    FillTool(_begx/_PixSize,_begy/_PixSize,ind,button);  // Fill it
	    _changed = 1;   // has changed now
	    break;
	  }

	case m_DrawDropper:	// mode tool eye dropper is selected
	  {
	    if (button == 1)  // left button - set fg color
	      {
		setfg(_myPicture->GetIndex(_begx/_PixSize,_begy/_PixSize,ImageLayer));
	      }
	    else if (button == 3)   // right button - set bg color
	      {
		setbg(_myPicture->GetIndex(_begx/_PixSize,_begy/_PixSize,ImageLayer));
	      }
	    break;
	  }

	case m_DrawSelect:  // cut - copy - paste tool is selected
	  {
	    _SelectbegX = _begx/_PixSize;  // set start x position
	    _SelectbegY = _begy/_PixSize;  // set start y position
	    _changed = 1;   // has changed now
	    break;
	  }
      }
  }

//==================>>> myCanvasPane::MouseMotion <<<===============
  void myCanvasPane::MouseMotion(int mx, int my)
  {
    // everytime the mouse is in motion run this command

    // update brush cordinates
    _BrushPrevX = _BrushX;
    _BrushPrevY = _BrushY;
    _BrushX = (mx - _dx)/_PixSize;
    _BrushY = (my - _dy)/_PixSize;

    // if brush has left the prev pixel erase old brush
    if ( !( (_BrushPrevX == _BrushX ) && (_BrushPrevY == _BrushY ) ) )
      DrawBrush(_BrushPrevX, _BrushPrevY, 0);

    // update to current brush values
    _cwin->MouseXYStatus(_BrushX,_BrushY);

    // if brush has left prev pixel draw new brush
    if ( !( (_BrushPrevX == _BrushX ) && (_BrushPrevY == _BrushY ) ) )
	DrawBrush(_BrushX, _BrushY, 1);
  }

//==================>>> myCanvasPane::MouseMove <<<===============
  void myCanvasPane::MouseMove(int mx, int my, int button)
  {
    // everytime the mouse moves run this command

    int h, w;
    int x = mx - _dx;           // shift to drawing space
    int y = my - _dy;

    if (_DrawShape == m_DrawFill)
	return; 

    // Draw rubber band shape on mouse move
    if( !( (_prex == _curx) && (_prey == _cury) ) )
      {
	// do only if pointer leaves the pixel space
	if ( !( ( _prex/_PixSize == _curx/_PixSize ) &&
              ( _prey/_PixSize == _cury/_PixSize ) ) )
	  {
	    switch (_DrawShape)		// determine which draw tool we are using
	      {
		case m_DrawLine:		// using line draw tool
		  {
		    _drawmode = ERASE;  // tell to erase previous line
		    IconLine(_begx/_PixSize, _begy/_PixSize,
		    _prex/_PixSize, _prey/_PixSize);
		    // update previous x and y values
		    _prex = _curx;
		    _prey = _cury;
		    break;
		  }

		case m_DrawSelect:  	// using the select tool, select will mock the rectangle
		  {
		  }

		case m_DrawRect:		// using the rectangle draw tool
		  {
		    _drawmode = ERASE;  // tell to erase previous line
		    IconRect(_begx/_PixSize, _begy/_PixSize,
		    _prex/_PixSize, _prey/_PixSize, _snap);
		    // update previous x and y values
		    _prex = _curx;
		    _prey = _cury;
		    break;
		  }

		case m_DrawRdRect:		// using the rounded rectangle draw tool
		  {
		    _drawmode = ERASE;  // tell to erase previous line
		    IconRdRect(_begx/_PixSize, _begy/_PixSize,
		    _prex/_PixSize, _prey/_PixSize, _snap);
		    // update previous x and y values
		    _prex = _curx;
		    _prey = _cury;
		    break;
		  }


		case m_DrawEllipse:		// using the ellipse draw tool
		  {
		    _drawmode = ERASE;  // tell to erase previous line
		    IconEllipse(_begx/_PixSize, _begy/_PixSize,
		    _prex/_PixSize, _prey/_PixSize, _snap);
		    // update previous x and y values
		    _prex = _curx;
		    _prey = _cury;
		    break;
		  }

	      }
	  }
      }

    if (_begx != x || _begy != y)       // draw new line
      {
        _cwin->MouseXYStatus(x/_PixSize, y/_PixSize);
        h = y - _begy;
        w = x - _begx;
        if (h == 0) h = 1;
        if (w == 0) w = 1;

	switch (_DrawShape)			// makes call to draw with the tool selected
	  {
	    case m_DrawLine:			// line draw tool selected
	      {
		_drawmode = DRAW;  // tell to draw previous line
		IconLine(_begx/_PixSize, _begy/_PixSize,
		_curx/_PixSize, _cury/_PixSize);
		break;
	      }

	    case m_DrawSelect:  		// select tool selected, select mocks rectangle
	      {
	      }

	    case m_DrawRect:			// rectangle draw tool selected
	      {
		_drawmode = DRAW;  // tell to draw previous line
		IconRect(_begx/_PixSize, _begy/_PixSize,
		    _curx/_PixSize, _cury/_PixSize, _snap);
		break;
	      }

	    case m_DrawRdRect:		// rounded rectangle draw tool selected
	      {
		_drawmode = DRAW;  // tell to draw previous line
		IconRdRect(_begx/_PixSize, _begy/_PixSize,
		    _curx/_PixSize, _cury/_PixSize, _snap);
		break;
	      }

	    case m_DrawEllipse:		// ellipse draw tool selected
	      {
		_drawmode = DRAW;  // tell to draw previous line
		IconEllipse(_begx/_PixSize, _begy/_PixSize,
		    _curx/_PixSize, _cury/_PixSize, _snap);
		break;
	      }

	    case m_DrawPoint:			// point draw tool selected
	      {
		_begx = x;    _begy = y;
		IconLine(x/_PixSize, y/_PixSize, x/_PixSize, y/_PixSize);
		break;
	      }
	  }
      }
    _curx = x;    _cury = y;            // update position
  }

//==================>>> myCanvasPane::MouseUp <<<=================
  void myCanvasPane::MouseUp(int mx, int my, int button)
  {
    // Finish drawing shape on mouse up

    int h, w;
    int x = mx - _dx;           // shift to drawing space
    int y = my - _dy;
    int width, height;
    int selectx, selecty, newind;

    // get icon width and height
    width = _myPicture->GetPicWidth();
    height = _myPicture->GetPicHeight();

    // draw point
    if (_DrawShape == m_DrawPoint)
      {
	if ( (_prex == _curx) && (_prey == _cury) )
	    IconLine(_prex/_PixSize, _prey/_PixSize,
                   _prex/_PixSize, _prey/_PixSize);
//	Redraw(0,0,0,0);
	return;
      }

    if (_DrawShape == m_DrawFill)
      {
	Redraw(0,0,0,0);
	return;
      }

    // if select tool must copy selected area to the brush layer
    if (_DrawShape == m_DrawSelect)
      {
	// clear previous brush layer
	_myPicture->Clear(BrushLayer);

	// set selection end point
	_SelectendX = _curx/_PixSize + 1;
	_SelectendY = _cury/_PixSize + 1;

	// set custom brush width and height
	_BrushWidth  = MyAbs(_SelectendX - _SelectbegX);
	_BrushHeight = MyAbs(_SelectendY - _SelectbegY);

	// alway need a brush width/height
	if (_BrushWidth  == 0)
	    _BrushWidth  = 1;
	if (_BrushHeight == 0)
	    _BrushHeight = 1;

	// force mouse pointer to upper left corner of brush
	_BrushOffX = 0;
	_BrushOffY = 0;

	// check to see which way the area was selected (left to right or
       // right to left  etc...) and reajust accordingly
       if ( _SelectbegX < _SelectendX )
	   selectx = _SelectbegX;
       else
	   selectx = _SelectendX;

       if ( _SelectbegY < _SelectendY )
	   selecty = _SelectbegY;
       else
	   selecty = _SelectendY;

       // start storing brush into brush layer
       // NOTE: we will not pick up the background color
       for( int i=0; i<_BrushWidth; i++ )
	 {
	   for( int j=0; j<_BrushHeight; j++ )
	     {
	       // get pixel from image layer
	       newind = _myPicture->GetIndex(selectx+i, selecty+j, ImageLayer);

	       // erase mode so set to background color
	       if (button == 3)
		   _myPicture->SetIndex(selectx+i, selecty+j, ImageLayer,
			 _myColorPal->getbg());

	       // store into brush layer if not background color
	       if (newind != _myColorPal->_bg)
		   _myPicture->SetIndex(i, j, BrushLayer, newind);
	     }
	 }
     }

    // if mouse pointer has entered a new pixel
    if ( !( ( _prex/_PixSize == _curx/_PixSize ) &&
            ( _prey/_PixSize == _cury/_PixSize ) ) )
      {
	switch (_DrawShape)		// finish drawing shape selected
	  {
	    case m_DrawLine:		// line draw tool selected
	      {
		_drawmode = ERASE;  // tell to erase previous line
		IconLine(_begx/_PixSize, _begy/_PixSize,
		_prex/_PixSize, _prey/_PixSize);
		_prex = _curx;
		_prey = _cury;
		break;
	      }

	    case m_DrawRect:		// rectangle draw tool selected
	      {
		_drawmode = ERASE;  // tell to erase previous line
		IconRect(_begx/_PixSize, _begy/_PixSize,
		_prex/_PixSize, _prey/_PixSize, _snap);
		_prex = _curx;
		_prey = _cury;
		break;
	      }

	    case m_DrawRdRect:		// rounded rectangle draw tool selected
	      {
		_drawmode = ERASE;  // tell to erase previous line
		IconRdRect(_begx/_PixSize, _begy/_PixSize,
		_prex/_PixSize, _prey/_PixSize, _snap);
		_prex = _curx;
		_prey = _cury;
		break;
	      }

	    case m_DrawEllipse:		// ellipse draw tool selected
	      {
		_drawmode = ERASE;  // tell to erase previous line
		IconEllipse(_begx/_PixSize, _begy/_PixSize,
		_prex/_PixSize, _prey/_PixSize, _snap);
		_prex = _curx;
		_prey = _cury;
		break;
	      }
	  }
      }


    if (_begx != x || _begy != y)       // First time?
      {
	_cwin->MouseXYStatus(x/_PixSize, y/_PixSize);
	h = y - _begy;
	w = x - _begx;

	if (h == 0)
	    h = 1;
	if (w == 0)
	    w = 1;

	// clear transparent layer
	_myPicture->Clear(TransLayer);

	switch (_DrawShape)
          {
            case m_DrawLine:		// line draw tool selected
	      {
                _drawmode = DRAW;  // tell to draw previous line
                IconLine(_begx/_PixSize, _begy/_PixSize,
                         x/_PixSize, y/_PixSize);
                _PrevDrawShape = _DrawShape;
                break;
	      }

            case m_DrawRect:		// rectangle draw tool selected
	      {
                _drawmode = DRAW;  // tell to draw previous line
                IconRect(_begx/_PixSize, _begy/_PixSize,
                         x/_PixSize, y/_PixSize, _snap);
                _PrevDrawShape = _DrawShape;
                break;
	      }

            case m_DrawRdRect:		// rounded rectangle draw tool selected
	      {
                _drawmode = DRAW;  // tell to draw previous line
                IconRdRect(_begx/_PixSize, _begy/_PixSize,
                           x/_PixSize, y/_PixSize, _snap);
                _PrevDrawShape = _DrawShape;
                break;
	      }

            case m_DrawEllipse:		// ellipse draw tool selected
	      {
                _drawmode = DRAW;  // tell to draw previous line
                IconEllipse(_begx/_PixSize, _begy/_PixSize,
                            x/_PixSize, y/_PixSize, _snap);
                _PrevDrawShape = _DrawShape;
                break;
	      }

          }
      }

    _changed = 1;       // changed now
    _begx = -100;  _begy = -100;  _curx = -100;  _cury = -100;

    // force a redraw
    Redraw(0,0,0,0);

    if (_DrawShape == m_DrawSelect)
      {
	_DrawShape = m_DrawPoint;
	_cwin->WindowCommand(m_DrawPoint,1,C_Button);
      }
  }

//===================>>> myCanvasPane::Redraw <<<=================
  void myCanvasPane::Redraw(int x, int y, int w, int h)
  {
    // This is a redraw that draws everything on the canvas

    // force a pen update

//    if ( w != 0 || h != 0)	// caused by expose, etc.
//      {
	// We can handle the case of x,y == 0 and new h and w
	// less than last h and w (which seems to get generated
	// by X all the time, but maybe not Windows...
//	if (x == 0 && y == 0 && w <= _rdw && h <= _rdh)
//	    return;
//      }

    _rdh = h; _rdw = w;

    _prevind = -1;

    DrawGrid();   // draw grid
    DrawImage();  // draw image
  }

//===================>>> myCanvasPane::ResetScroll <<<=================
  void myCanvasPane::ResetScroll()
  {
    // reset to no scrolling
    _dx = _dy = 0;
    SetTransX(0);             // simple translation
    SetTransY(0);             // simple translation
    SetScrollBarPositions();
  }

//===================>>> myCanvasPane::HPage <<<=================
  void myCanvasPane::HPage(int shown, int top)
  {
    int dX;

    if (GetWidth() > 0 && shown != 100)
      {
        dX = (int)(((long)top*(long)(_maxx-GetWidth())) / (long)(100 - shown));
      }
    else
        dX = 0;

    _dx = -dX;
    SetTransX(_dx);             // simple translation
    Clear();            // clear the canvas
    Redraw(0,0,0,0);
    SetScrollBarPositions();
  }

//===================>>> myCanvasPane::HScroll <<<=================
  void myCanvasPane::HScroll(int step)
  {
    if (step > 0 && (GetWidth() - _dx) <= _maxx)        // scroll right
      {
        _dx -= 10;              // bump up by 10
        SetTransX(_dx);
        Clear();                // clear the canvas
        Redraw(0,0,0,0);
      }
    else if (step < 0 && _dx < 0)               // scroll left
      {
        _dx += 10;              // shift by 10
        if (_dx > 0)
            _dx = 0;
        SetTransX(_dx);         // simple translation
        Clear();                // clear the canvas
        Redraw(0,0,0,0);
      }
    SetScrollBarPositions();
  }

//===================>>> myCanvasPane::VPage <<<=================
  void myCanvasPane::VPage(int shown, int top)
  {

    int dY;

    if (GetHeight() > 0 && shown != 100)
        dY = (int)(((long)top*(long)(_maxy-GetHeight())) / (long)(100 - shown));
    else
        dY = 0;

    _dy = -dY;
    SetTransY(_dy);             // simple translation
    Clear();            // clear the canvas
    Redraw(0,0,0,0);
    SetScrollBarPositions();
  }

//===================>>> myCanvasPane::VScroll <<<=================
  void myCanvasPane::VScroll(int step)
  {
    if (step > 0 && (GetHeight() - _dy) <= _maxy )      // scroll up
      {
        _dy -= 10;              // bump up by 10
        SetTransY(_dy);
        Clear();                // clear the canvas
        Redraw(0,0,0,0);
      }
    else if (step < 0 && _dy < 0) // scroll down
      {
        _dy += 10;              // shift by 10
        if (_dy > 0)
            _dy = 0;
        SetTransY(_dy);         // simple translation
        Clear();                // clear the canvas
        Redraw(0,0,0,0);
      }
    SetScrollBarPositions();
  }

//=================>>> myCanvasPane::SetScrollBarPositions <<<=================
  void myCanvasPane::SetScrollBarPositions()
  {
    // Calculate the percentage shown, and how far down scrolls should go

    int vShown, hShown, vTop, hTop;

// LEAVE IN FOR NOW - FIRST ATTEMPT TO CORRECT SCROLL POSITION
/*
    if ((_myPicture->_height * _PixSize) > GetHeight())
      {
        vShown = (int)((long)100L *(long)GetHeight() /(long)(_myPicture->_height * _PixSize));
        if (((_myPicture->_height * _PixSize) - GetHeight()) != 0)
            vTop = 1;
        else
            vTop = 0;
*/
    int Iwidth = _PixSize * _myPicture->GetPicWidth();
    int Iheight = _PixSize * _myPicture->GetPicHeight();

    if (Iheight > 0 && Iheight > GetHeight())       // not 100% shown
      {
        vShown = (int)((long)(100L*(long)GetHeight())/(long)Iheight);
        if ((Iheight - GetHeight()) != 0)
            vTop = (int)((long)((long)(-_dy*100) / (long)Iheight));
        else
            vTop = 0;
      }
    else                                // 100% shown
      {
        vShown = 100; vTop = 0;
      }

    if (Iwidth > 0 && Iwidth > GetWidth())        // not 100%
      {
        hShown = (int)((long)(100L*(long)GetWidth())/(long)Iwidth);
        if ((Iwidth - GetWidth()) != 0)
            hTop = (int)((long) ((long)(-_dx*100) / (long)Iwidth));
        else
            hTop = 0;
      }
    else
      {
        hShown = 100; hTop = 0;
      }

    if (vShown > 100) vShown = 100;     // normalize to 100
    if (hShown > 100) hShown = 100;     // normalize to 100

    SetVScroll(vShown,vTop);
    SetHScroll(hShown,hTop);
  }

//==================>>> myCanvasPane::Undo <<<==================
  void myCanvasPane::Undo()
  {
    //  Undo last command by clearing the transparent
    //  layer and redrawing

    // clear transparent layer
    _myPicture->Clear(TransLayer);

    // force redraw
    Redraw(0,0,0,0);
  }

//==================>>> myCanvasPane::Resize <<<==================
  void myCanvasPane::Resize(int w, int h)
  {
    int needClear = 0;

    if (w > _maxx && _dx != 0)          // see if now big enough for
      {                                 // entire drawing
        needClear = 1; _dx = 0;
      }

    if (h > _maxy && _dy != 0)
      {
        needClear = 1; _dy = 0;
      }

    if (needClear)
      {
        SetTranslate(_dx,_dy);
        Clear();
      }

    SetScrollBarPositions();
    vCanvasPane::Resize(w,h);           // call default resize
  }

//==================>>> myCanvasPane::ReadIcon <<<====================
  int myCanvasPane::ReadIcon(char* name)
  {
    // reads in a VBM file and parses

    int x, y, ix;
    int height, width;

    int colors;
    unsigned char *r, *g, *b;
    unsigned char* pix;		// for pixels

    char strip_name[256], ext[256];

    if (!name || !*name)
	return 0;

    getBaseAndExt(name, strip_name, ext);

    imageIO vbmIO;

    if (strcmp(ext,"vbm") == 0 || strcmp(ext,"VBM") == 0)
      {
	if (!vbmIO.ReadVBM(name))		// Read a VBM image
	    return 0;
      }
    else if (strcmp(ext,"xpm") == 0 || strcmp(ext,"XPM") == 0)
      {
	if (!vbmIO.ReadXPM(name))		// Read a VBM image
	    return 0;
      }
    else if (strcmp(ext,"xbm") == 0 || strcmp(ext,"XBM") == 0)
      {
	if (!vbmIO.ReadXBM(name))		// Read a XBM image
	    return 0;
      }
    else if (strcmp(ext,"bmp") == 0 || strcmp(ext,"BMP") == 0)
      {
	if (!vbmIO.ReadBMP(name))		// Read a VBM image
	    return 0;
      }
    else
	return 0;

    if (vbmIO.Depth() > 8)
      {
	vNoticeDialog note(theApp);
	note.Notice("Can't handle 24-bit Files.");
	return 0;
      }

    r = vbmIO.RedMap(); g = vbmIO.GreenMap(); b = vbmIO.BlueMap();

    colors = vbmIO.Colors();
    width = vbmIO.Width();
    height = vbmIO.Height();

    for (ix = 0; ix < colors; ix++)	// fetch the color list
      {
	_myColorPal->_Palette[ix].Set(r[ix],g[ix],b[ix]);
      }

    _myColorPal->SetColorsUsed(colors);

    // Put color pallet clear and fill here
    if (_myPicture != 0)
	delete _myPicture;
    _myPicture = new Picture(width, height);

    _myPicture->Clear(ImageLayer, _myColorPal->getbg());
    _myPicture->Clear(TransLayer);
    _myPicture->Clear(BrushLayer);
    Clear();


    pix = vbmIO.Pixels();
    ix = 0;			// start reading pixels at 0
    for (y = 0; y < height; y++)
      {
	for (x = 0; x < width; x++)
	  {
	    _myPicture->SetIndex(x, y, ImageLayer, pix[ix++]);
	  }
      }

//    Redraw(0,0,0,0);
    return 1;
  }

//==================>>> myCanvasPane::Read <<<====================
  int myCanvasPane::Read(char* name)
  {
    // load in a file


    if (!name || !*name)
        return 0;       // sanity check

    return ReadIcon(name);
  }

//==================>>> myCanvasPane::SaveIcon <<<====================
  int myCanvasPane::SaveIcon(char* filename, char* iconname, char* ext)
  {
    // Save an image. The process is relatively simple by using
    // the imageIO package. We Flatten the image to make sure it
    // is up to date. Then we create an imageIO color map, copy
    // our color map to it. Then we set depth, height, width, and
    // number of colors used. Then, we copy our pixel array
    // to the imageIO pixel array. Finally, we call the imageIO
    // routine to optimize the color map. Then we call the appropriate
    // output converter based on the extension.

    unsigned char *r, *g, *b, *pix;
    long px;

    _myPicture->Flatten();  // do flatten in case trans layer still holds op
//    Redraw(0,0,0,0);

    imageIO vbmIO;

    // 1. Set up color map

    vbmIO.CreateColorMap(256);	// Create an empty color map
    r = vbmIO.RedMap();
    g = vbmIO.GreenMap();
    b = vbmIO.BlueMap();

    for (int ix = 0 ; ix < _myColorPal->ColorsUsed() ; ++ix)
      {
	r[ix] = (unsigned char) _myColorPal->getColor(ix).r();
	g[ix] = (unsigned char) _myColorPal->getColor(ix).g();
	b[ix] = (unsigned char) _myColorPal->getColor(ix).b();
      }

    // 2. set various attributes

    vbmIO.SetDepth(8);
    vbmIO.SetColors(_myColorPal->ColorsUsed());
    vbmIO.SetWidth(_myPicture->_width);
    vbmIO.SetHeight(_myPicture->_height);

   // 3. Copy the image array

    pix = vbmIO.CreatePixels((long)(_myPicture->_height * _myPicture->_width));

    px = 0;
    for (int ir = 0 ; ir < _myPicture->_height ; ++ir)
      {
	for (int ic = 0 ; ic < _myPicture->_width ; ++ic)
	  {
	    pix[px++] = (unsigned char)_myPicture->GetIndex(ic,ir,ImageLayer);
	  }
      }

    // 4. Optimize color map

    vbmIO.OptimizeColorMap();		// squish color map

    // 5. Write out in appropriate format


    if (strcmp(ext,"vbm") == 0 || strcmp(ext,"VBM") == 0)
	return vbmIO.WriteVBM(filename,iconname);
    else if (strcmp(ext,"xpm") == 0 || strcmp(ext,"XPM") == 0)
	return vbmIO.WriteXPM(filename,iconname);
    else if (strcmp(ext,"bmp") == 0 || strcmp(ext,"BMP") == 0)
	return vbmIO.WriteBMP(filename,iconname);
    else if (strcmp(ext,"xbm") == 0 || strcmp(ext,"XBM") == 0)
      {
        if (vbmIO.Colors() > 2)
	  {
	    vNoticeDialog note(theApp);
	    note.Notice("Too many colors for .XBM.");
	  }
	else 
	    return vbmIO.WriteXBM(filename, iconname);
      }

    return 0;
  }

//==================>>> myCanvasPane::getBaseAndExt <<<====================
  int myCanvasPane::getBaseAndExt(char* name, char* base, char* ext)
  {
    char temp_str[256];

    strcpy(temp_str,name);

    int ix;
    for (ix = strlen(temp_str) - 1 ; ix >= 0 ; --ix)
      {
	if (temp_str[ix] == '.')	// found break
	  {
	    strcpy(ext,&temp_str[ix+1]);
	    temp_str[ix] = 0;
	    break;
	  }
	if (temp_str[ix] == '/' || temp_str[ix] == '\\')
	    break;
      }

    for ( ; ix >= 0 ; --ix)
      {
	if (temp_str[ix] == '/' || temp_str[ix] == '\\')
	  {
	    strcpy(base,&temp_str[ix+1]);
	    break;
	  }
      }

    if (ix <= 0 &&
	temp_str[0] != '/' && temp_str[0] != '\\')
      {
	strcpy(base,&temp_str[0]);
      }

    for (char *cp = base ; *cp ; ++cp)
	*cp = tolower(*cp);
    return 1;
  }

//==================>>> myCanvasPane::Save <<<====================
  int myCanvasPane::Save(char* name)
  {
    char strip_name[256];
    char extension[32];

    if (!name || !*name)
	return 0;      // sanity check

    // crop the full path name to extract filename - extension

    strcpy(extension,"vbm");		// vbm by default

    getBaseAndExt(name, strip_name, extension);

    int retv = SaveIcon(name, strip_name, extension);
    if (retv)
	_changed = 0;		// not changed anymore
    return retv;
  }

//==================>>> myCanvasPane::FillGrid <<<=====================
  void myCanvasPane::FillGrid(int x, int y)
  {
    //  fill the pixel into the grid(array) called from drawline, etc

    int tempind;
    int newx, newy;
    int layer;

    // draw the brush so walk through brush layer
    for (int i = 0; i < _BrushWidth; i++)
      {
	for (int j = 0; j < _BrushHeight; j++)
	  {
	    // set new x and y cord
	    newx = x-_BrushOffX+i;
	    newy = y-_BrushOffY+j;

	    // make sure we are in bounds of icon
	    if ( (newx>=0) && (newx<_myPicture->_width)
		&& (newy>=0) && (newy<_myPicture->_height) )
	      {
		// set temp index to brush layer at x,y
		tempind = _myPicture->GetIndex(i, j, BrushLayer);
		// set to transparent layer
		layer = TransLayer;

		// transparent don't draw
		if (tempind != -1)
		  {
		    // if we are in draw mode . . . draw
		    if (_drawmode == DRAW)
		      {
			// if -99 then the brush is default
			// so use fg/bg colors
			if (tempind == -99)
			  {
			    tempind = getCurPenInd();
			  }
			else
			  { // custom vrush - read brush's colors
			    // set alyer to brush
			    layer = BrushLayer;

			    // if right button pressed - erase
			    if (_myColorPal->getbg() == _curPen)
				tempind = _myColorPal->getbg();
			  }
		      }
		    else
		      {  // ERASE
			// set temp index to image x,y
			tempind = _myPicture->GetIndex(newx,newy,ImageLayer);

			// make sure layer is set to transparent
			layer = TransLayer;
		      }

		    // set pixel into array
		    _myPicture->SetIndex(newx, newy, TransLayer, tempind);
		    // draw pixel to screen
		    DrawPixel(newx, newy, layer);
		  }
	      }
	  }
      }
  }

//==================>>> myCanvasPane::DrawBrush <<<=====================
  void myCanvasPane::DrawBrush(int x, int y, int mode)
  {  //  draw the brush selected (default or custom)

    int tempind;
    int newx, newy;
    int layer;

    if ( (x <0 ) || (y < 0) ) 
	return;

    // walk through brush layer
    for (int i = 0; i < _BrushWidth; i++)
      {
	for (int j = 0; j < _BrushHeight; j++)
	  {
	    // set new x and y
	    newx = x-_BrushOffX+i;
	    newy = y-_BrushOffY+j;

	    // if in bounds of icon
	    if( (newx>=0) && (newx<_myPicture->_width)
		&& (newy>=0) && (newy<_myPicture->_height) )
	      {

		// set temp index to brush layer
		tempind = _myPicture->GetIndex(i, j, BrushLayer);
		layer = TransLayer;

		// transparent??? do nothing
		if (tempind != -1 && (tempind < 256 && tempind > 0))
		  {
		    if (mode)  // draw
		      {
			// -99 is a default brush so use bg/fg color
			if (tempind == -99)
			  {
			    tempind = getCurPenInd();
			  } 
			else
			  {  // custom brush better read color
			    layer = BrushLayer;
			  }
		      }
		    else
		      {  // erase
			// read from transparent layer
			tempind = _myPicture->GetIndex(newx,newy,TransLayer);

			// if transparent read from image layer
			if (tempind == -1  && (tempind < 256 && tempind > 0))
			    tempind = _myPicture->GetIndex(newx,newy,ImageLayer);

			// layer = transparent
			layer = TransLayer;
		      }

			// draw if it not transparent
		    if ( tempind != -1 && (tempind < 256 && tempind > 0))
		      {
			// update pen only if needed
			if ( tempind != _prevind && (tempind < 256 && tempind > 0))
			  {
			    _PixelBrush.SetColor(_myColorPal->getColor(tempind));
			    SetBrush(_PixelBrush);
			    _prevind = tempind;
			  }

			// draw pixel - no sotarge into array needed
			DrawRectangle(_PixSize*newx+1, _PixSize*newy+1,
			_PixSize-1, _PixSize-1);
		      }
		  }
	      }
	  }
      }
  }

//==================>>> myCanvasPane::FillTool <<<======================
  void myCanvasPane::FillTool(int x, int y, int the_index, int button)
  {  
//#define RECURSIVE_FILL

#ifdef RECURSIVE_FILL
    //  recursive fill tool 

    // Make sure we are not out of our image
    if ( (x >=0) && (x < _myPicture->_width) &&
       (y >=0) && (y < _myPicture->_height) )
      {
	// get pixels index on transparent layer
	int tempind = _myPicture->GetIndex(x,y,TransLayer);

	if (tempind == -1)          // transparent -> look at image layer
	    tempind = _myPicture->GetIndex(x,y,ImageLayer);

	if ( tempind == the_index)  // is original color, so fill
	  {
	    if (button == 1)
		_myPicture->SetIndex(x, y, TransLayer, _myColorPal->getfg());  // set pixel index
	    else
		_myPicture->SetIndex(x, y, TransLayer, _myColorPal->getbg());  // set pixel index

	    // start recursive calls
	    if (y > 0)
		FillTool(x, y-1, the_index,button);  // fill up
	    if (x+1 < _myPicture->_width)
		FillTool(x+1, y, the_index,button);  // fill right
	    if (y + 1 < _myPicture->_height)
		FillTool(x, y+1, the_index,button);  // fill down
	    if (x > 0)
		FillTool(x-1, y, the_index,button);  // fill left
	  }
      }
#else
    // stack based fill
    int fillcolor;
    if (button == 1)
	fillcolor = _myColorPal->getfg();	// fill with fg
    else
	fillcolor = _myColorPal->getbg();	// fill with bg
    fill(x, y, _myPicture->_width, _myPicture->_height, the_index, fillcolor);

#endif
  }

#ifndef RECURSIVE_FILL
//
// A Seed Fill Algorithm
// by Paul Heckbert
// from "Graphics Gems", Academic Press, 1990
//

typedef struct {short y, xl, xr, dy;} Segment;
//
// Filled horizontal segment of scanline y for xl<=x<=xr.
// Parent segment was on line y-dy.  dy=1 or -1
//

#define STACKSIZE 8000    // max depth of stack (< 64K DOS SEGMENT)

// push new segment on stack
#define PUSH(Y, XL, XR, DY) \
    if (sp<stack+STACKSIZE && Y+(DY)>=0 && Y+(DY)<height) \
    {sp->y = Y; sp->xl = XL; sp->xr = XR; sp->dy = DY; sp++;}

// pop segment off stack
#define POP(Y, XL, XR, DY) \
    {sp--; Y = sp->y+(DY = sp->dy); XL = sp->xl; XR = sp->xr;}

//
// fill: set the pixel at (x,y) and all of its 4-connected neighbors
// with the same pixel value to the new pixel value nv.
// A 4-connected neighbor is a pixel above, below, left, or right of a pixel.
//

//======================>>> myCanvasPane::pixelwrite <<<===========================
  void myCanvasPane::pixelwrite(int x, int y, int val)
  {
    _myPicture->SetIndex(x,y,TransLayer,val);
  }

//======================>>> myCanvasPane::pixelread <<<===========================
  int myCanvasPane::pixelread(int x, int y)
  {
    int tmp = _myPicture->GetIndex(x,y,TransLayer);
    if (tmp == -1)
	tmp = _myPicture->GetIndex(x,y,ImageLayer);
    return tmp;
  }

//======================>>> myCanvasPane::fill <<<===========================
  void myCanvasPane::fill(int x, int y, int width, int height, 
	int ov, int fillcolor)
  {
    int start, x1, x2, dy = 0;

    if (fillcolor == ov)		// nothing to do
	return;

    Segment* stack = new Segment[STACKSIZE]; // allocate the stack
    Segment* sp = stack;

    PUSH(y, x, x, 1);			// needed in some cases
    PUSH(y+1, x, x, -1);		// seed segment (popped 1st)

    while (sp > stack)
      {
	// pop segment off stack and fill a neighboring scan line
	POP(y, x1, x2, dy);

	// segment of scan line y-dy for x1<=x<=x2 was previously filled,

	for (x = x1; x >= 0 && pixelread(x, y) == ov; x--)
	    pixelwrite(x, y,fillcolor);
	if (x >= x1) 
	  {
	    for (x++; x<=x2 && pixelread(x, y)!=ov; x++)
		;
	    start = x;
	    if (x > x2)
		continue;
	  } 
	else
	  {
	    start = x+1;
	    if (start<x1)
		PUSH(y, start, x1-1, -dy);       // leak on left?
	    x = x1+1;
	  }
	do 
	  {
	 //   for (; x<width && pixelread(x, y)==ov; x++)
	    for (; x < width && pixelread(x, y)==ov; x++)
		pixelwrite(x, y, fillcolor);
	    PUSH(y, start, x-1, dy);
	    if (x>x2+1) 
		PUSH(y, x2+1, x-1, -dy);       // leak on right?
	    for (x++; x<=x2 && pixelread(x, y)!=ov; x++)
		;
	    start = x;
	  } 
	while (x<=x2)
	    ;
      }
    delete [] stack;
  }
#endif
//==================>>> myCanvasPane::MyAbs <<<==========================
  int myCanvasPane::MyAbs(int value)
  {  // This function mimics the absolute math funtion

    if (value < 0) 
	return -value;
    else 
	return value;
  }

//==================>>> myCanvasPane::Round <<<==========================
  int myCanvasPane::Round(float num)
  {  // This function will round the float and return an integer

    int sign;                     // sign for positive or negative num
    float temp;                   // temp val

    if (num < 0)
	sign = -1;    // compute sign
    else 
	sign = 1;

    temp = sign * (num - (int)num);       // set temp to remainer

    return (int)(num + (sign * temp));    // return rounded value
  }

//==================>>> myCanvasPane::IconLine <<<======================
  void myCanvasPane::IconLine(int begx, int begy, int curx, int cury)
  {  // This function will draw a line from begx,begy to curx,cury

    if ( (curx == begx) && (cury == begy) )       // draw point
      {
	_x = curx;
	_y = cury;
	FillGrid(_x, _y);
      }

    if (curx != begx)           // not a vertical line
      {
	_m = ((float)(cury - begy) / (float)(curx - begx)); // set slope
	_b = (float)(cury - (_m * curx));                   // set y intercept

	if (curx >= begx) // set begin and end for for loop, this depends
	  {                 // on which way to walk through the for loop
	    _endval = curx;
	    _startval = begx;
	  }
	else 
	  {
	    _endval = begx;
	    _startval = curx;
	  }

	for (_x = _startval; _x <= _endval; _x++) // walk along x axis
	  {
	    _y = Round( ((_m * _x) + _b) );         // equation for a line;
	    FillGrid(_x, _y);                       // make call to fill the grid
	  }
      }

    if (cury != begy)                   // not a horizontal line
      {
	if (begx != curx)                 // set slope
	    _m = ((float)(cury - begy) / (float)(curx - begx));

	_b = (float)(cury - (_m * curx));         // set y intercept
	if (cury >= begy)                 // find which way the line runs
	  {
	    _endval = cury;
	    _startval = begy;
	  } 
	else 
	  {
	    _endval = begy;
	    _startval = cury;
	  }

	for (_y=_startval; _y <= _endval; _y++)   // walk down y axis
	  {
	    if (begx != curx)
		_x = Round( ((_y - _b) / _m) );
	    else
		_x = curx;
	    FillGrid(_x, _y);                   // make call to grid
	  }
      }
  }

//==================>>> myCanvasPane::IconRect <<<======================
  void myCanvasPane::IconRect(int begx, int begy, int curx, int cury, int mode)
  {  // This function will draw a rectangle with opposite corners at begx,begy
  // to curx,cury

    if (mode == 1)                            // draw square for snap mode
      {

	// check to see how drawn - (left to right vs right to left,
	//                           up to down vd down to up)
	if (MyAbs(curx-begx) > MyAbs(cury-begy) )
	  {
	    if ((cury-begy) < 0)
		cury = begy - MyAbs(curx-begx);
	    else
		cury = begy + MyAbs(curx-begx);
	  }
	else if (MyAbs(cury-begy) > MyAbs(curx-begx) )
	  {
	    if ((curx-begx) < 0)
		curx = begx - MyAbs(cury-begy);
	    else
		curx = begx + MyAbs(cury-begy);
	  }
      }

    if ( (curx == begx) && (cury == begy) )       // draw point
	FillGrid(curx, cury);

    // Draw the rectangle according to begx, begy, curx, cury

    IconLine(begx, begy, begx+(curx-begx), begy);
    IconLine(begx, begy, begx, begy+(cury-begy));
    IconLine(begx+(curx-begx), begy, begx+(curx-begx), begy+(cury-begy));
    IconLine(begx, begy+(cury-begy), begx+(curx-begx), begy+(cury-begy));
  }

//==================>>> myCanvasPane::IconRdRect <<<====================
  void myCanvasPane::IconRdRect(int begx, int begy, int curx, int cury, int mode)
  {  // This function will draw a rectangle like above but with rounded corners

    int smallest;                 // smallest side of rectangle

    if (mode == 1)                            // draw square for snap mode
      {
	// check to see how drawn - (left to right vs right to left,
	//                           up to down vd down to up)
	if (MyAbs(curx-begx) > MyAbs(cury-begy) )
	  {
	    if ((cury-begy) < 0)
		cury = begy - MyAbs(curx-begx);
	    else
		cury = begy + MyAbs(curx-begx);
	  }
	else if (MyAbs(cury-begy) > MyAbs(curx-begx) )
	  {
	    if ((curx-begx) < 0)
		curx = begx - MyAbs(cury-begy);
	    else
		curx = begx + MyAbs(cury-begy);
	  }
      }

    if ( (curx == begx) && (cury == begy) )       // draw point
	FillGrid(curx, cury);

    // find smallest side
    if (MyAbs(begy-cury) > MyAbs(begx-curx) )
	smallest = MyAbs(curx-begx);
    else
	smallest = MyAbs(cury-begy);

    // no need to draw rounded corners
    if (smallest <= 1)
      {
	IconLine(begx, begy, curx, begy);
	IconLine(begx, cury, curx, cury);
	IconLine(begx, begy, begx, cury);
	IconLine(curx, begy, curx, cury);
      }
    else if (smallest <= 5)    // draw rounded corners
      {
	// right to left
	if (begx > curx)
	  {
	    //  top to bottom
	    if (begy > cury)
	      {
		IconLine(begx-1, begy, curx+1, begy);
		IconLine(begx-1, cury, curx+1, cury);
		IconLine(begx, begy-1, begx, cury+1);
		IconLine(curx, begy-1, curx, cury+1);
	      }
	    else	    // bottom to top
	      {
		IconLine(begx-1, begy, curx+1, begy);
		IconLine(begx-1, cury, curx+1, cury);
		IconLine(begx, begy+1, begx, cury-1);
		IconLine(curx, begy+1, curx, cury-1);
	      }
	  }
	else	// left to right
	  {
	    // top to bottom
	    if (begy > cury)
	      {
		IconLine(begx+1, begy, curx-1, begy);
		IconLine(begx+1, cury, curx-1, cury);
		IconLine(begx, begy-1, begx, cury+1);
		IconLine(curx, begy-1, curx, cury+1);
	      }
	    else      // bottom to top
	      {
		IconLine(begx+1, begy, curx-1, begy);
		IconLine(begx+1, cury, curx-1, cury);
		IconLine(begx, begy+1, begx, cury-1);
		IconLine(curx, begy+1, curx, cury-1);
	      }
	  }
      }
    else //Bigger then 5 in smallest (smallest <= 5)
      {
	// same checking of drawing orientation
	if (begx > curx)
	  {
	    if (begy > cury)
	      {
		IconLine(begx-2, begy, curx+2, begy);
		IconLine(begx-2, cury, curx+2, cury);
		IconLine(begx, begy-2, begx, cury+2);
		IconLine(curx, begy-2, curx, cury+2);
		FillGrid(begx-1, begy-1);  FillGrid(curx+1, cury+1);
		FillGrid(begx-1, cury+1);  FillGrid(curx+1, begy-1);
	      }
	    else // if (begy < cury)
	      {
		IconLine(begx-2, begy, curx+2, begy);
		IconLine(begx-2, cury, curx+2, cury);
		IconLine(begx, begy+2, begx, cury-2);
		IconLine(curx, begy+2, curx, cury-2);
		FillGrid(begx-1, begy+1);  FillGrid(curx+1, cury-1);
		FillGrid(begx-1, cury-1);  FillGrid(curx+1, begy+1);
	      }
	  }
	else
	  {
	    if (begy > cury)
	      {
		IconLine(begx+2, begy, curx-2, begy);
		IconLine(begx+2, cury, curx-2, cury);
		IconLine(begx, begy-2, begx, cury+2);
		IconLine(curx, begy-2, curx, cury+2);
		FillGrid(begx+1, begy-1);  FillGrid(curx-1, cury+1);
		FillGrid(begx+1, cury+1);  FillGrid(curx-1, begy-1);
	      }
	    else // if (begy < cury)
	      {
		IconLine(begx+2, begy, curx-2, begy);
		IconLine(begx+2, cury, curx-2, cury);
		IconLine(begx, begy+2, begx, cury-2);
		IconLine(curx, begy+2, curx, cury-2);
		FillGrid(begx+1, begy+1);  FillGrid(curx-1, cury-1);
		FillGrid(begx+1, cury-1);  FillGrid(curx-1, begy+1);
	      }
	  }
      }
  }

//==================>>> myCanvasPane::IconEllipse <<<====================
  void myCanvasPane::IconEllipse(int begx, int begy, int curx, int cury, int mode)
  {  // This function will draw an elipse with center at begx,begy
    //  The x axis radius = |curx-begx|
    //  The y axis radius = |cury-begy|

    if (mode == 1)           // draw circle
      {
	// check to see how drawn - (left to right vs right to left,
	//                           up to down vd down to up)
	if (MyAbs(curx-begx) > MyAbs(cury-begy) )
	  {
	    cury = curx;
	    _a = MyAbs(curx - begx);      // find a ->x axis radius
	    _b = _a;                      // find b ->y axis radius
	  }
	else if ( (curx == begx) && (cury == begy) )
	  {
	    _x = curx;
	    _y = cury;
	    FillGrid(_x, _y);
	  }
	else
	  {
	    curx = cury;
	    _b = MyAbs(cury - begy);      // find b ->y axis radius
	    _a = _b;
	  }
      }
    else
      {
	_a = MyAbs(curx - begx);        // find a ->x axis radius
	_b = MyAbs(cury - begy);        // find b ->y axis radius
      }

    if ( (curx == begx) && (cury == begy) )       // draw point
      {
	_x = curx;
	_y = cury;
	FillGrid(_x, _y);

      }
    else if (_a == 0)
      {             // vertical line
	IconLine(begx, begy + (begy-cury), curx, cury);
      }
    else if (_b == 0)
      {             // horizonta line
	IconLine(begx + (begx-curx), begy, curx, cury);
      }
    else
      {

	_startval = -(int)_a;            // set start val for loop
	_endval = (int)_a;               // set end val for loop

	for (_x = _startval; _x <= _endval; _x++)   // walk down x axis
	  {
	    _y = Round( (float)sqrt( (float)(_b*_b) * // equation for elipse
	    ( 1.0 - ((float)(_x*_x)/(float)(_a*_a)) ) ) );
	    FillGrid(_x + begx, _y + begy);
	    FillGrid(_x + begx, -_y + begy);
	  }

	_startval = -(int)_b;               // set start val for loop
	_endval = (int)_b;                  // set end val for loop

	for (_y = _startval; _y <= _endval; _y++)   // walk down x axis
	  {
	    _x = Round( (float)sqrt( (float)(_a*_a) * // equation for elipse
	    ( 1.0 - ((float)(_y*_y)/(float)(_b*_b)) ) ) );
	    FillGrid(_x + begx, _y + begy);
	    FillGrid(-_x + begx, _y + begy);
	  }
      }
  }

//================>>> Picture::Picture <<<==============
  Picture::Picture(int width, int height)
  {
    // Allocate and Initialize the reset picture array with preset width and
    // height

    int i, j, k;

    for (i = 0 ; i < deflayers ; ++i)
	pixels[i] = new int[width*height];

    _width = width;
    _height = height;
    for (i = 0 ; i < deflayers ; i++)
      {
	for (j=0; j < _height; j++)
	  {
	    for (k=0; k < _width; k++)
	      {
		seti(k, j, i, 1);       // initialize pixels to white
	      }
	  }
      }
  }

//==============>>> Picture::~Picture <<<================
  Picture::~Picture()
  {
     // Deallocate the picture array
     for (int i = 0 ; i < deflayers ; ++i)
	delete [] pixels[i];
  }

//=================>>> Picture::GetIndex <<<====================
  int Picture::GetIndex(int x, int y, int layer)
  {
     // Get the index to the color table of the specified pixel

     return (int) geti(x, y, layer);
  }

//================>>> Picture::SetIndex <<<=================
  void Picture::SetIndex(int x, int y, int layer, int index)
  {
     // Set the specified pixel with an index value from the color table

     seti(x, y, layer, index);
  }

//==================>>> Picture::GetPicWidth <<<===============
  int Picture::GetPicWidth()
  {
     // return the current width of the picture

    return _width;
  }

//==================>>> Picture::GetPicHeight <<<===============
  int Picture::GetPicHeight()
  {
     // return the current height of the picture

     return _height;
  }

//==================>>> Picture::Flatten <<<===============
  void Picture::Flatten()
  {
    // Copy the transparent layer (1) to the base layer (0) if a change has been made in
    // the top layer, -1 indicates no new changes have been made

    int i, j, value;

    for (i=0; i < _height; i++)
      {
	for (j=0; j < _width; j++)
	  {
	    value = geti(j, i, TransLayer);
	    if (value != -1)
		seti(j, i, ImageLayer, value);
	  }
      }
  }

//==================>>> Picture::Clear <<<===============
  void Picture::Clear(int layer, int thecolor)
  {
    // Reset given layer to new value in order to clear it. If top layer,
    // clear to null values to indicate no change.
    // If base layer, clear to white.

     int i, j, value;

     if (layer == ImageLayer)
        value = thecolor;	// if base layer then set to background
     else
        value = -1;             // if other layer then set to null

    for (i=0; i < _height; i++)
      {
	for (j=0; j < _width; j++)
	  {
	    seti(j, i, layer, value);       // reset pixels to value
	  }
      }
  }

//==================>>> Color_Pal::Color_Pal <<<=======================
  Color_Pal::Color_Pal()
  {
    // setup the color palette

    init_Palette();
  }

//==================>>> Color_Pal::~Color_Pal <<<======================
  Color_Pal::~Color_Pal()
  {
    // destroy color palette instances
  }

//==================>>> Color_Pal::init_Palette <<<=======================
  void Color_Pal::init_Palette()
  {
    // setup the color palette

#include "paldecla.h"
    _colorsUsed = 64;
    _fg = 0;
    _bg = 1;
  }

//==================>>> Color_Pal::makefg <<<==========================
  void Color_Pal::makefg(int index)
  {
    //  make index as the foreground color

    _fg = index;
  }

//==================>>> Color_Pal::makebg <<<==========================
  void Color_Pal::makebg(int index)
  {
    //  make index as the background color

    _bg = index;
  }

//==================>>> Color_Pal::getfg <<<===========================
  int  Color_Pal::getfg()
  {  // get the current foreground color

  return _fg;
  }

//==================>>> Color_Pal::getbg <<<===========================
  int  Color_Pal::getbg()
  {  //  get current background color

  return _bg;
  }

//==================>>> myCanvasPane::setfg <<<========================
  void myCanvasPane::setfg(int index)
  {  //  set foreground color on the color palette

    _myColorPal->_fg = index;
    _cwin->SetFGBtn(_myColorPal->_Palette[index]);

  }

//==================>>> myCanvasPane::setbg <<<========================
  void myCanvasPane::setbg(int index)
  {  //  set background color on the color palette

    _myColorPal->_bg = index;
    _cwin->SetBGBtn(_myColorPal->_Palette[index]);
  }

//==================>>> myCanvasPane::getCurPenInd <<<================
  int myCanvasPane::getCurPenInd()
  {  //  get index of the curent foreground pen settings

    return _curPen;
  }

//==================>>> Color_Pal::getColor <<<=======================
  vColor  Color_Pal::getColor(int index)
  {  //  get color by given index number from the color palette

    return _Palette[index];
  }

//==================>>> myCanvasPane::setCurPenCol <<<================
  void  myCanvasPane::setCurPenCol(int index)
  {  //  set foreground to index number (current foreground color)

    _curPen = index;
  }

//==================>>> Color_Pal::setColor <<<=======================
  void Color_Pal::setColor(int index, vColor value)
  {  //  set color to index array location

    _Palette[index] = value;
  }

//==================>>> Color_Pal::setRed <<<=========================
  void Color_Pal::setRed(int index, int value)
  {  //  set red color value

    _Palette[index].SetR(value);
  }

//==================>>> Color_Pal::setGreen <<<=======================
  void Color_Pal::setGreen(int index, int value)
  {  //  set green color value

    _Palette[index].SetG(value);
  }

//==================>>> Color_Pal::setBlue <<<========================
  void Color_Pal::setBlue(int index, int value)
  {  //  set blue color value

    _Palette[index].SetB(value);
  }
