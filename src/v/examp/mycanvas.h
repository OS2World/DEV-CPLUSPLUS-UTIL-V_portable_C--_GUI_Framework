//=======================================================================
//  mycanvas.h -- myCanvasPane class defintion header
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

#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <v/vcanvas.h>
#include <v/vpen.h>

    typedef struct point
      {
	int x;
	int y;
	vPen pPen;
      } point;

    class myCanvasPane : public vCanvasPane
      {
      public:		//---------------------------------------- public
	myCanvasPane();
	virtual ~myCanvasPane();

	// Windows
	virtual void Clear();

	// Scrolling
	virtual void HPage(int, int);
	virtual void VPage(int, int);

	virtual void HScroll(int);
	virtual void VScroll(int);

	// Events
	virtual void MouseDown(int, int, int);
	virtual void MouseUp(int, int, int);
	virtual void MouseMove(int, int, int);

	virtual void Redraw(int, int, int, int); // Expose/redraw event
	virtual void Resize(int, int);		// Resize event

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private
	int _mouseDown;
	int _begx;
	int _begy;
	int _curx;
	int _cury;

	point *pt;
	int nextpt;
      };
#endif
