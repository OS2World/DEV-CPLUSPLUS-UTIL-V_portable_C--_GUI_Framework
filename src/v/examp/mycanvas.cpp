//=======================================================================
//  mycanvas.cxx -- myCanvasPane class defintion
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

#include "mycanvas.h"

//===================>>> myCanvasPane::myCanvasPane <<<====================
  myCanvasPane::myCanvasPane()
  {
    _mouseDown = 0;
    _begx = -1;
    _begy = -1;
    _curx = -1;
    _cury = -1;
    pt = new point[200];		// 200 lines
    nextpt = 0;
  }

//===================>>> myCanvasPane::myCanvasPane <<<====================
  myCanvasPane::~myCanvasPane()
  {
    delete [] pt;		// Zap the point array
  }

//======================>>> myCanvasPane::Clear <<<========================
  void myCanvasPane::Clear()
  {
    point *np;

    vCanvasPane::Clear();

    nextpt = 0;

  }

//======================>>> myCanvasPane::HPage <<<========================
  void myCanvasPane::HPage(int shown, int top)
  {
    vCanvasPane::HPage(shown, top);
  }

//======================>>> myCanvasPane::VPage <<<========================
  void myCanvasPane::VPage(int shown, int top)
  {
    vCanvasPane::VPage(shown, top);
  }

//=======================>>> myCanvasPane::HScroll <<<======================
  void myCanvasPane::HScroll(int step)
  {
    vCanvasPane::HScroll(step);
  }

//======================>>> myCanvasPane::VScroll <<<======================
  void myCanvasPane::VScroll(int step)
  {
    vCanvasPane::VScroll(step);
  }

//======================>>> myCanvasPane::MouseDown <<<======================
  void myCanvasPane::MouseDown(int X, int Y, int button)
  {
    _mouseDown = 1;
    _begx = _curx = X;
    _begy = _cury = Y;

    pt[nextpt].x = X;
    pt[nextpt].y = Y;
    pt[nextpt].pPen = GetPen();
    ++nextpt;
    if (nextpt >= 200)		// really dumb!
	nextpt = 0;
  }

//========================>>> myCanvasPane::MouseUp <<<======================
  void myCanvasPane::MouseUp(int X, int Y, int button)
  {
    _mouseDown = 0;
    if (_begx != X || _begy != Y)
      {
	DrawLine(_begx, _begy, X, Y);
      }

    pt[nextpt].x = X;
    pt[nextpt].y = Y;
    pt[nextpt].pPen = GetPen();
    ++nextpt;

    if (nextpt >= 200)		// really dumb!
	nextpt = 0;

    _mouseDown = 0;
    _begx = -1;
    _begy = -1;
    _curx = -1;
    _cury = -1;
  }

//======================>>> myCanvasPane::MouseMove <<<======================
  void myCanvasPane::MouseMove(int x, int y, int button)
  {
    if (_begx != _curx || _begy != _cury)
      {
	DrawRubberLine(_begx, _begy, _curx, _cury);	// erase old one
      }

    if (_begx != x || _begy != y)
      {
	DrawRubberLine(_begx, _begy, x, y);
      }
    
    _curx = x;
    _cury = y;
  }

//=========================>>> myCanvasPane::Redraw <<<======================
  void myCanvasPane::Redraw(int x, int y, int w, int h)
  {

    // This is a stupid Redraw that just redraws everything.
    // It also starts losing things after 200 points. This
    // is just sample code, remember!

    int x1, y1, x2, y2;

    for (int i = 0 ; i < nextpt ; i += 2)
      {
	if (i == 0 || pt[i].pPen != pt[i-2].pPen)
	    SetPen(pt[i].pPen);
	x1 = pt[i].x;
	y1 = pt[i].y;
	x2 = pt[i+1].x;
	y2 = pt[i+1].y;

	int xlim = x + w;
	int ylim = y + h;

	DrawLine(x1, y1, x2, y2);
      }
  }

//======================>>> myCanvasPane::Resize <<<======================
  void myCanvasPane::Resize(int w, int h)
  {
    vCanvasPane::Resize(w,h);
  }
