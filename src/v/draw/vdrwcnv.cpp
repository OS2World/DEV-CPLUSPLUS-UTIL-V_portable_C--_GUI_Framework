//================================================================
//  vdrwcnv.cxx -- myCanvasPane class defintion
//  Copyright (C) 1995,1996  Bruce E. Wampler
//
//  This SIMPLE canvas class just draws lines using the pen that
//  goes with this canvas. It doesn't handle any scrolling or
//  paging. It also only handles up to 200 lines.
//================================================================

#include "vdrwcmdw.h"
#include "vdrwcnv.h"

#include <v/vprinter.h>
#include <v/vprintdc.h>

//================>>> myCanvasPane::myCanvasPane <<<==============
  ostream& operator <<(ostream& s, Shape& sh)
  {
    return s << sh.drawShape << " " <<
		sh.p1.x << " " << sh.p1.y << " " <<
		sh.p2.x << " " << sh.p2.y << " " <<
		sh.sPen.GetColor().r() << " " <<
		sh.sPen.GetColor().g() << " " <<
		sh.sPen.GetColor().b() << " " <<
		sh.sPen.GetWidth() << " " <<
		sh.sPen.GetStyle() <<  " " <<

		sh.sBrush.GetColor().r() << " " <<
		sh.sBrush.GetColor().g() << " " <<
		sh.sBrush.GetColor().b() << " " <<
		sh.sBrush.GetFillMode() << " " <<
		sh.sBrush.GetStyle() <<  " " << endl;
  }

//================>>> myCanvasPane::myCanvasPane <<<==============
  istream& operator >>(istream& s, Shape& sh)
  {
    // Read in a file of points
    
    int r,g,b,ps, br, bg, bb, bfm, bs, pw;

    if (!s)
	return s;
    s >> sh.drawShape;
    if (!s)
	return s;
    s >> sh.p1.x >> sh.p1.y >>
	sh.p2.x >> sh.p2.y >> r >> g >> b >>
	pw >> ps >>
	br >> bg >> bb >> bfm >> bs;

    sh.sPen.SetWidth(pw);

    sh.sPen.SetColor(r,g,b); // can't read directly
    sh.sPen.SetStyle(ps);
    sh.sBrush.SetColor(br,bg,bb);
    sh.sBrush.SetFillMode(bfm);
    sh.sBrush.SetStyle(bs);
    sh.next = 0;
    return s;
  }

//================>>> myCanvasPane::myCanvasPane <<<==============
  myCanvasPane::myCanvasPane(myCmdWindow* cw)
  {
    // Initialize to known values

    _sList = 0;		// no shapes yet
    _DrawShape = m_DrawLine;	// draw lines
    _begx = -100;  _begy = -100;  _curx = -100;  _cury = -100;
    _changed = 0;	// not changed
    _maxx = 0; _maxy = 0;
    _dx = _dy = 0;	// no shift yet
    _cwin = cw;
  }

//==============>>> myCanvasPane::~myCanvasPane <<<================
  myCanvasPane::~myCanvasPane()
  {
    Shape* temp;

    for (Shape* sl = _sList ; sl != 0 ; )
      {
	temp = sl;
	sl = sl->next;
	delete temp;
      }
  }

//=================>>> myCanvasPane::ClearShapes <<<====================
  void myCanvasPane::ClearShapes()
  {
    Clear();	// clear the canvas

    _dx = _dy = 0;
    SetTranslate(_dx,_dy);
    _maxx = _maxy = 0;
    SetScrollBarPositions();
    for (Shape* sl = _sList ; sl != 0 ; )	// clear list
      {
	Shape* temp = sl;
	sl = sl->next;
	delete temp;
      }
    _sList = 0;
  }

//================>>> myCanvasPane::MouseDown <<<=================
  void myCanvasPane::MouseDown(int x, int y, int button)
  {
    // Mouse down means beginning to draw a shape

    Shape* newShape = new Shape;		// get a new shape

    newShape->next = _sList;		// point new item to rest of list
    _sList = newShape;			// and add in at front

    _begx = _curx = x - _dx;    _begy = _cury = y - _dy;
    _cwin->MouseXYStatus(_curx, _cury);

    newShape->p1.x = _begx; newShape->p1.y = _begy;	// add just first points
    newShape->sPen = GetPen();
    newShape->sBrush = GetBrush();
    newShape->drawShape = _DrawShape;
    if (_DrawShape == m_DrawPoint)
      {
	DrawPoint(_begx,_begy);
	_sList->p2.x = _begx;    _sList->p2.y = _begy;
	_changed = 1;	// changed now
      }
    UpdateMax(_begx,_begy);
  }

//==================>>> myCanvasPane::MouseMotion <<<===============
  void myCanvasPane::MouseMotion(int mx, int my)
  {
    _cwin->MouseXYStatus(mx - _dx, my - _dy);
  }

//==================>>> myCanvasPane::MouseMove <<<===============
  void myCanvasPane::MouseMove(int mx, int my, int button)
  {
    int h, w;
    int x = mx - _dx;		// shift to drawing space
    int y = my - _dy;

    // Draw rubber band shape on mouse move
    if (_begx != _curx || _begy != _cury)    // old line to clear?
      {
	h = _cury - _begy;
	w = _curx - _begx;
	if (h == 0) h = 1;
	if (w == 0) w = 1;
	switch (_DrawShape)
	  {
	    case m_DrawLine:
		DrawRubberLine(_begx, _begy, _curx, _cury);
		break;
	    case m_DrawPoly:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawRect:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawRdRect:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawEllipse:
		DrawRubberEllipse(_begx, _begy, w, h);
		break;
	    case m_DrawIcon:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	  }
      }

    if (_begx != x || _begy != y)	// draw new line
      {
	_cwin->MouseXYStatus(x, y);
	h = y - _begy;
	w = x - _begx;
	if (h == 0) h = 1;
	if (w == 0) w = 1;
	switch (_DrawShape)
	  {
	    case m_DrawLine:
		DrawRubberLine(_begx, _begy, x, y);
		break;
	    case m_DrawPoly:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawRect:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawRdRect:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawEllipse:
		DrawRubberEllipse(_begx, _begy, w, h);
		break;
	    case m_DrawIcon:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawPoint:
	      {
		Shape* newShape = new Shape;		// get a new shape
		newShape->next = _sList;		// point new item to rest of list
		_sList = newShape;			// and add in at front
		_begx = x;    _begy = y;
		newShape->p1.x = x; newShape->p1.y = y;	// add just first points
		newShape->p2.x = x; newShape->p2.y = y;
		newShape->sPen = GetPen();
		newShape->sBrush = GetBrush();
		newShape->drawShape = _DrawShape;
		DrawPoint(x,y);
		_changed = 1;	// changed now
		break;
	      }
	  }
      }

    _curx = x;    _cury = y;		// update
  }

//==================>>> myCanvasPane::MouseUp <<<=================
  void myCanvasPane::MouseUp(int mx, int my, int button)
  {
    // Finish drawing shape on mouse up

    int h, w;
    int x = mx - _dx;		// shift to drawing space
    int y = my - _dy;

    if (_DrawShape == m_DrawPoint)		// Point is no op
	return;

    if (_begx != _curx || _begy != _cury)    // old shape to clear?
      {
	h = _cury - _begy;
	w = _curx - _begx;
	if (h == 0) h = 1;
	if (w == 0) w = 1;
	switch (_DrawShape)
	  {
	    case m_DrawLine:
		DrawRubberLine(_begx, _begy, _curx, _cury);
		break;
	    case m_DrawPoly:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawRect:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawRdRect:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawEllipse:
		DrawRubberEllipse(_begx, _begy, w, h);
		break;
	    case m_DrawIcon:
		DrawRubberRectangle(_begx, _begy, w, h);
		break;
	  }
      }

    if (_begx != x || _begy != y)	// First time?
      {
	_cwin->MouseXYStatus(x, y);
	h = y - _begy;
	w = x - _begx;
	UpdateMax(x,y);			// find maximum range
	if (h == 0) h = 1;
	if (w == 0) w = 1;
	switch (_DrawShape)
	  {
	    case m_DrawLine:
		DrawLine(_begx, _begy, x, y);
		break;
	    case m_DrawPoly:
	      {
		vPoint tri[4];
		tri[0] = _sList->p1;
		tri[1].x = tri[0].x; tri[1].y = y;
		tri[2].x = x; tri[2].y = y;
		tri[3] = tri[0];
		DrawPolygon(4,tri);
		break;
	      }
	    case m_DrawRect:
		DrawRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawRdRect:
		DrawRoundedRectangle(_begx, _begy, w, h);
		break;
	    case m_DrawEllipse:
		DrawEllipse(_begx, _begy, w, h);
		break;
	    case m_DrawIcon:
		DrawRectangle(_begx, _begy, w, h);
		break;
	  }
      }

    _sList->p2.x = x;    _sList->p2.y = y;

    _changed = 1;	// changed now
    _begx = -100;  _begy = -100;  _curx = -100;  _cury = -100;
  }

//===================>>> myCanvasPane::Print <<<=================
  void myCanvasPane::Print()
  {
    // This is a stupid Redraw that just redraws everything.

    vPrintDC pdc;		// create a printDC object
    vPrinter printer;

    if (!printer.Setup("test.ps"))	// setup the printer
	return;

    pdc.SetPrinter(printer);	// change to the printer we setup

    if (!pdc.BeginPrinting())	// print to test.ps
	return;

    // Have a valid drawing canvas, so draw a box around paper edges

    vBrush pbrush(0,0,0,vTransparent);	// a transparent brush
    pdc.SetBrush(pbrush);
    pdc.DrawRectangle(36,36, pdc.GetPhysWidth() - 72,
	pdc.GetPhysHeight() - 72);

    pdc.SetTranslate(36,36);	// and now leave 1/2 inch margins

    DrawShapes(&pdc);

    pdc.EndPrinting();
  }

//===================>>> myCanvasPane::Redraw <<<=================
  void myCanvasPane::Redraw(int x, int y, int w, int h)
  {
    // This is a stupid Redraw that just redraws everything.

    DrawShapes(GetDC());

  }

//===================>>> myCanvasPane::HPage <<<=================
  void myCanvasPane::HPage(int shown, int top)
  {
    int dX;

    if (GetWidth() > 0 && shown != 100)
      {
	dX = (int)( ((long)top*(long)(_maxx-GetWidth())) /
		    (long)(100 - shown));
      }
    else
	dX = 0;

    _dx = -dX;
    SetTransX(_dx);		// simple translation
    Clear();		// clear the canvas
    Redraw(0,0,0,0);
    SetScrollBarPositions();
  }

//===================>>> myCanvasPane::HScroll <<<=================
  void myCanvasPane::HScroll(int step)
  {
    if (step > 0 && (GetWidth() - _dx) <= _maxx)	// scroll right
      {
	_dx -= 10;		// bump up by 10
	SetTransX(_dx);
	Clear();		// clear the canvas
	Redraw(0,0,0,0);
      }
    else if (step < 0 && _dx < 0)		// scroll left
      {
	_dx += 10;		// shift by 10
	if (_dx > 0)
	    _dx = 0;
	SetTransX(_dx);		// simple translation
	Clear();		// clear the canvas
	Redraw(0,0,0,0);
      }
    SetScrollBarPositions();
  }

//===================>>> myCanvasPane::VPage <<<=================
  void myCanvasPane::VPage(int shown, int top)
  {

    int dY;

    if (GetHeight() > 0 && shown != 100)
      {
	dY = (int)( ((long)top*(long)(_maxy-GetHeight())) /
		    (long)(100 - shown));
      }
    else
	dY = 0;

    _dy = -dY;
    SetTransY(_dy);		// simple translation
    Clear();		// clear the canvas
    Redraw(0,0,0,0);
    SetScrollBarPositions();
  }

//===================>>> myCanvasPane::VScroll <<<=================
  void myCanvasPane::VScroll(int step)
  {
    if (step < 0 && (GetHeight() - _dy) <= _maxy )	// scroll up
      {
	_dy -= 10;		// bump up by 10
	SetTransY(_dy);
	Clear();		// clear the canvas
	Redraw(0,0,0,0);
      }
    else if (step > 0 && _dy < 0) // scroll down
      {
	_dy += 10;		// shift by 10
	if (_dy > 0)
	    _dy = 0;
	SetTransY(_dy);		// simple translation
	Clear();		// clear the canvas
	Redraw(0,0,0,0);
      }
    SetScrollBarPositions();
  }

//=================>>> myCanvasPane::SetScrollBarPositions <<<=================
  void myCanvasPane::SetScrollBarPositions()
  {
    // Calculate the percentage shown, and how far down

    int vShown, hShown, vTop, hTop;

    if (_maxy > 0 && _maxy > GetHeight())	// not 100% shown
      {
	vShown = (int)((long)(100L*(long)GetHeight())/(long)_maxy);
	if ((_maxy - GetHeight()) != 0)
	    vTop = (int)((long)((long)(-_dy*100) /
			(long)_maxy));
	else
	    vTop = 0;
      }
    else				// 100% shown
      {
	vShown = 100; vTop = 0;
      }

    if (_maxx > 0 && _maxx > GetWidth())	// not 100%
      {
	hShown = (int)((long)(100L*(long)GetWidth())/(long)_maxx);
	if ((_maxx - GetWidth()) != 0)
	    hTop = (int)((long) ((long)(-_dx*100) / (long)_maxx));
	else
	    hTop = 0;
      }
    else
      {
	hShown = 100; hTop = 0;
      }

    if (vShown > 100) vShown = 100;	// normalize to 100
    if (hShown > 100) hShown = 100;	// normalize to 100

    SetVScroll(vShown,vTop);
    SetHScroll(hShown,hTop);
  }

//===================>>> myCanvasPane::DrawShapes <<<=================
  void myCanvasPane::DrawShapes(vDC* cp)
  {
    int x1, y1, x2, y2;		// work variables
    int w, h;

    vPen oldPen = cp->GetPen();	// need to remember these
    vBrush oldBrush = cp->GetBrush();

    for (Shape* sl = _sList ; sl != 0 ; sl = sl->next)
      {
	cp->SetPen(sl->sPen);	// change to shape's pen
	cp->SetBrush(sl->sBrush);	// and brush
	x1 = sl->p1.x;      y1 = sl->p1.y;	// get coords
	x2 = sl->p2.x;      y2 = sl->p2.y;
	w = x2 - x1;
	h = y2 - y1;
	if (w == 0) w = 1;	// adjust so none are 0
	if (h == 0) h = 1;
	switch (sl->drawShape)	// and draw shape
	  {
	    case m_DrawLine:
		cp->DrawLine(x1, y1, x2, y2);
		break;
	    case m_DrawPoly:
	      {
		vPoint tri[4];
		tri[0].x = x1; tri[0].y = y1;
		tri[1].x = tri[0].x; tri[1].y = y2;
		tri[2].x = x2; tri[2].y = y2;
		tri[3] = tri[0];
		cp->DrawPolygon(4,tri);
		break;
	      }
	    case m_DrawRect:
		cp->DrawRectangle(x1, y1, w, h);
		break;
	    case m_DrawRdRect:
		cp->DrawRoundedRectangle(x1, y1, w, h);
		break;
	    case m_DrawEllipse:
		cp->DrawEllipse(x1, y1, w, h);
		break;
	    case m_DrawIcon:
		cp->DrawRectangle(x1, y1, w, h);
		break;
	    case m_DrawPoint:
		cp->DrawPoint(x1,y1);
		break;
	  }
      }
    cp->SetPen(oldPen);
    cp->SetBrush(oldBrush);
  }

//==================>>> myCanvasPane::Resize <<<==================
  void myCanvasPane::Resize(int w, int h)
  {
    // This simple example doesn't need to do anything special for
    // resize. The default will cause a Redraw.

    int needClear = 0;

    if (w > _maxx && _dx != 0)		// see if now big enough for
      {					// entire drawing
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
    vCanvasPane::Resize(w,h);		// call default resize
  }

//==================>>> myCanvasPane::Read <<<====================
  int myCanvasPane::Read(char* name)
  {
    // Read in a file of points

    Shape curSh;
    Shape* newShape;

    if (!name || !*name)
	return 0;	// sanity check

    ifstream inFile(name);		// open the file to read

    if (!inFile) return 0;		// OK?
    
    inFile >> _maxx;			// read max x,y
    inFile >> _maxy;
    Clear();				// Clear out anything old
    _sList = 0;
    while (inFile >> curSh)
      {
	newShape = new Shape;		// get a new shape
	curSh.next = _sList;		// point item to rest of list
	*newShape = curSh;		// copy
	_sList = newShape;		// and add in at front
      }
    inFile.close();			// finished with file
    Redraw(0,0,0,0);
    SetScrollBarPositions();
    _changed = 1;			// changed now
    return 1;
  }

//==================>>> myCanvasPane::Save <<<====================
  int myCanvasPane::Save(char* name)
  {
    // Save drawing to a file of points

    if (!name || !*name) return 0;	// sanity check

    ofstream outFile(name);		// open output file

    if (!outFile) return 0;		// OK?

    outFile << _maxx << " " << _maxy << "\n";
    for (Shape* sl = _sList ; sl != 0 ; sl = sl->next)
      {
	outFile << *sl;
      }
    outFile.close();			// done with file
    Redraw(0,0,0,0);
    _changed = 0;			// saved now
    return 1;
  }

//==================>>> myCanvasPane::UpdateMax <<<====================
  void myCanvasPane::UpdateMax(int x, int y)
  {
    if (x > _maxx)
	_maxx = x;
    if (y > _maxy)
	_maxy = y;
  }
