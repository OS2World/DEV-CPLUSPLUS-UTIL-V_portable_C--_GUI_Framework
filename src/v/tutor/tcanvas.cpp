//========================================================================
//      tcanvas.cpp - source for tutorial canvas
//
//      Copyright 1995,1996, Bruce E. Wampler, All Rights Reserved.
//========================================================================
//
// Each V application usually needs a canvas. In order to handle
// various events: mouse, redraw, resize, and scroll, you will need to
// derive your own canvas class. The base V vCanvasPane class can only
// draw -- it does not have any memory of what has been drawn on the
// screen (the vTextCanvasPane does handle redrawing, but is still
// limited). Thus, your class will usually be responsible for handling
// redrawing. This example is very simple. It lets the user draw
// lines - up to 200 - and will redraw the screen when it has been
// exposed.

// The example does not handle scrolling.

#include "tcanvas.h"            // include our header file

//====================>>> tCanvasPane::tCanvasPane <<<====================
  tCanvasPane::tCanvasPane()
  {
    // The constructor initializes our simple data structure.

    _mouseDown = 0; _nextpt = 0;
    _begx = -1; _begy = -1; _curx = -1; _cury = -1;
    _pt = new point[200];       // allocate only 200 lines
  }

//-===================>>> tCanvasPane::tCanvasPane <<<====================
  tCanvasPane::~tCanvasPane()
  {
    delete [] _pt;              // free the point array
  }

//======================>>> tCanvasPane::Clear <<<========================
  void tCanvasPane::Clear()
  {
    vCanvasPane::Clear();       // clear the canvas
    _nextpt = 0;                // start over at 0
  }

// This example does not handle scrolling, but a derived canvas would
// be likely to. Thus, we've included the derived scrolling methods,
// but simply call the superclass method for default handling, which
// is essentially a no op.

//======================>>> tCanvasPane::HPage <<<========================
  void tCanvasPane::HPage(int shown, int top)
  {
    vCanvasPane::HPage(shown, top);
  }

//======================>>> tCanvasPane::VPage <<<========================
  void tCanvasPane::VPage(int shown, int top)
  {
    vCanvasPane::VPage(shown, top);
  }

//======================>>> tCanvasPane::HScroll <<<======================
  void tCanvasPane::HScroll(int step)
  {
    vCanvasPane::HScroll(step);
  }

//======================>>> tCanvasPane::VScroll <<<======================
  void tCanvasPane::VScroll(int step)
  {
    vCanvasPane::VScroll(step);
  }

//=====================>>> tCanvasPane::MouseDown <<<=====================
  void tCanvasPane::MouseDown(int X, int Y, int button)
  {
   // Mouse down means the user is starting a line. We don't care which
   // button was pressed. There is nothing to draw until the mouse moves.

    _mouseDown = 1;                     // track mouse button
    _pt[_nextpt].x = _begx = _curx = X; // starting point
    _pt[_nextpt].y = _begy = _cury = Y;
    if (++_nextpt >= 200)               // set next point and do a simple
        _nextpt = 0;                    // minded storage allocation
  }

//======================>>> tCanvasPane::MouseMove <<<====================
  void tCanvasPane::MouseMove(int x, int y, int button)
  {
    // Mouse move means the user is drawing a line, so we have to draw
    // it on the screen. By drawing a Rubber Line, we can easily track
    // the user motions, and undraw the previous line.

    if (_begx != _curx || _begy != _cury) // Was there a previous line?
        DrawRubberLine(_begx, _begy, _curx, _cury);  // Undraw old line

    if (_begx != x || _begy != y)       // If we moved, draw new line
        DrawRubberLine(_begx, _begy, x, y);
    
    _curx = x; _cury = y;               // update positions
  }

//========================>>> tCanvasPane::MouseUp <<<====================
  void tCanvasPane::MouseUp(int X, int Y, int button)
  {
    // Mouse up means the user has ended a line, so we need to draw
    // a permanent line and update the data base.

    _mouseDown = 0;                     // Mouse down now
    if (_begx != X || _begy != Y)       // We drew a line
        DrawLine(_begx, _begy, X, Y);   // So draw permanent version

    _pt[_nextpt].x = X; _pt[_nextpt].y = Y;  // End point

    if (++_nextpt >= 200)               // set next point and do a simple
        _nextpt = 0;                    // minded storage allocation

    _begx = -1; _begy = -1; _curx = -1; _cury = -1;  // for next line
  }

//========================>>> tCanvasPane::Redraw <<<=====================
  void tCanvasPane::Redraw(int x, int y, int w, int h)
  {
    // This is a simple Redraw that just redraws everything.
    // Often, that will be more than fast enough, but the input
    // parameters can be used to make a more intelligent redraw.

    for (int i = 0 ; i < _nextpt ; i += 2)
        DrawLine(_pt[i].x, _pt[i].y, _pt[i+1].x, _pt[i+1].y);
  }

//======================>>> tCanvasPane::Resize <<<=======================
  void tCanvasPane::Resize(int w, int h)
  {
    // We also don't handle resizing in this example.
    vCanvasPane::Resize(w,h);
  }
