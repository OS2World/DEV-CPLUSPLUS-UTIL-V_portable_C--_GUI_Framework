//================================================================
//  drawcnv.cxx -- myCanvasPane class defintion
//  Copyright (C) 1995  Bruce E. Wampler
//
//  This SIMPLE canvas class just draws lines using the pen that
//  goes with this canvas. It doesn't handle any scrolling or
//  paging. It also only handles up to 200 lines.
//================================================================

#include "drawcnv.h"

//================>>> myCanvasPane::myCanvasPane <<<==============
  myCanvasPane::myCanvasPane()
  {
    // Initialize to known values
    _mouseDown = 0;
    _begx = -1;  _begy = -1;  _curx = -1;  _cury = -1;
    _pt = new point[200];    // Just 200 lines for this example
    _nextpt = 0;
  }

//==============>>> myCanvasPane::myCanvasPane <<<================
  myCanvasPane::~myCanvasPane()
  {
    delete [] _pt;		// free the point array
  }

//=================>>> myCanvasPane::Clear <<<====================
  void myCanvasPane::Clear()
  {
    vCanvasPane::Clear();	// clear the canvas
    _nextpt = 0;		// and all the points
  }

//================>>> myCanvasPane::MouseDown <<<=================
  void myCanvasPane::MouseDown(int X, int Y, int button)
  {
    // Note beginning of line on mouse down
    _mouseDown = 1;
    _begx = _curx = X;    _begy = _cury = Y;

    _pt[_nextpt].x = X;  _pt[_nextpt].y = Y;
    _pt[_nextpt].pPen = GetPen();
    ++_nextpt;
    if (_nextpt >= 200)		// really dumb, but SIMPLE!
	_nextpt = 0;
  }

//==================>>> myCanvasPane::MouseUp <<<=================
  void myCanvasPane::MouseUp(int X, int Y, int button)
  {
    // Finish drawing line on mouse up
    _mouseDown = 0;
    if (_begx != X || _begy != Y)	// First time?
	DrawLine(_begx, _begy, X, Y);

    _pt[_nextpt].x = X;    _pt[_nextpt].y = Y;
    _pt[_nextpt].pPen = GetPen();
    ++_nextpt;

    if (_nextpt >= 200)		// really dumb, but SIMPLE!
	_nextpt = 0;

    _mouseDown = 0;		// ready for next line
    _begx = -1;  _begy = -1;  _curx = -1;  _cury = -1;
  }

//==================>>> myCanvasPane::MouseMove <<<===============
  void myCanvasPane::MouseMove(int x, int y, int button)
  {
    // Draw rubber band line on mouse move
    if (_begx != _curx || _begy != _cury)    // old line to clear?
	DrawRubberLine(_begx, _begy, _curx, _cury);

    if (_begx != x || _begy != y)	// draw new line
	DrawRubberLine(_begx, _begy, x, y);
    
    _curx = x;    _cury = y;		// update
  }

//===================>>> myCanvasPane::Redraw <<<=================
  void myCanvasPane::Redraw(int x, int y, int w, int h)
  {
    // This is a stupid Redraw that just redraws everything.
    // It also starts losing things after 200 points. This
    // is just sample code, remember!

    int x1, y1, x2, y2;		// work variables

    for (int ix = 0 ; ix < _nextpt ; ix += 2)
      {
	if (ix == 0 || _pt[ix].pPen != _pt[ix-2].pPen)
	    SetPen(_pt[ix].pPen);
	x1 = _pt[ix].x;      y1 = _pt[ix].y;
	x2 = _pt[ix+1].x;    y2 = _pt[ix+1].y;
	DrawLine(x1, y1, x2, y2);
      }
  }

//==================>>> myCanvasPane::Resize <<<==================
  void myCanvasPane::Resize(int w, int h)
  {
    // This simple example doesn't need to do anything special for
    // resize. The default will cause a Redraw.
    vCanvasPane::Resize(w,h);
  }

#include <fstream.h>		// to save/restore drawings

//==================>>> myCanvasPane::Read <<<====================
  int myCanvasPane::Read(char* name)
  {
    // Read in a file of points
    int r,g,b,ps,pw;

    if (!name || !*name) return 0;	// sanity check
    ifstream inFile(name);		// open the file to read

    if (!inFile) return 0;		// OK?
    
    inFile >> _nextpt;			// number of points
    for (int ix = 0 ; ix < _nextpt ; ++ix)  // read the points
      {
	inFile >> _pt[ix].x >> _pt[ix].y >> r >> g >> b
	    >> pw >> ps;
	  _pt[ix].pPen.SetColor(r,g,b); // can't read directly
	  _pt[ix].pPen.SetWidth(pw);
	  _pt[ix].pPen.SetStyle(ps);
      }
    inFile.close();			// finished with file
    return 1;
  }

//==================>>> myCanvasPane::Save <<<====================
  int myCanvasPane::Save(char* name)
  {
    // Save drawing to a file of points

    if (!name || !*name) return 0;	// sanity check
    ofstream outFile(name);		// open output file

    if (!outFile) return 0;		// OK?

    outFile << _nextpt << endl;		// number of points
    for (int ix = 0 ; ix < _nextpt ; ++ix) // write the points
      {
	outFile << _pt[ix].x << " " << _pt[ix].y << " "
		<< _pt[ix].pPen.GetColor().r() << " "
		<< _pt[ix].pPen.GetColor().g() << " "
		<< _pt[ix].pPen.GetColor().b() << " "
		<< _pt[ix].pPen.GetWidth() << " "
		<< _pt[ix].pPen.GetStyle() << endl;
      }
    outFile.close();			// done with file
    return 1;
  }
