//================================================================
//  tdrwcnv.h -- myCanvasPane class defintion header
//  Copyright (C) 1995,1996  Bruce E. Wampler
//================================================================
#ifndef DRAWCANVAS_H
#define DRAWCANVAS_H

#include <v/vcanvas.h>	// derive from vCanvas
#include <v/vpen.h>	// contain a vPen

#include <fstream.h>		// to save/restore drawings

    class Shape			// simple class for our shapes
      {
      public:		//--------------------------------- public
	Shape() {}
	~Shape() {}

	int drawShape;
	vPoint p1;		// handles all shapes we will draw
	vPoint p2;
	vPen sPen;		// pen to use
	vBrush sBrush;		// brush to use
	Shape* next;		// ptr to next shape

	friend ostream& operator <<(ostream& s, Shape& sh);
	friend istream& operator >>(istream& s, Shape& sh);
      };
    class myCmdWindow;
    class myCanvasPane : public vCanvasPane
      {
      public:		//--------------------------------- public
	myCanvasPane(myCmdWindow* cw);
	virtual ~myCanvasPane();

	// Methods we will override.

  	virtual void MouseDown(int X, int Y, int button);
	virtual void MouseUp(int X, int Y, int button);
	virtual void MouseMove(int x, int y, int button);
        virtual void MouseMotion(int x, int y);
	virtual void Redraw(int x, int y, int w, int h);
	virtual void Resize(int w, int h);
	virtual void HPage(int shown, int top);
	virtual void HScroll(int step);
  	virtual void VPage(int shown, int top);
	virtual void VScroll(int step);

	// New methods for this class
	void ClearShapes();
	int Read(char* name);
	int Save(char* name);
  	void SetDrawShape(int dm) { _DrawShape = dm; }
	int Changed() { return _changed; }
	int Empty() { return _sList == 0; }
	void SetChanged(int chg) { _changed = chg; }

	void Print();
	void DrawShapes(vDC* cp);

      protected:	//------------------------------ protected

      private:		//-------------------------------- private
        void UpdateMax(int x, int y);
	void SetScrollBarPositions();
	Shape* _sList;
	int _DrawShape;
	int _changed;
	int _maxx, _maxy;
	int _begx;  int _begy;  int _curx;  int _cury;  // coords
	int _dx, _dy;
	myCmdWindow* _cwin;
      };
#endif
