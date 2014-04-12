//================================================================
//  drawcnv.h -- myCanvasPane class defintion header
//  Copyright (C) 1995  Bruce E. Wampler
//================================================================
#ifndef DRAWCANVAS_H
#define DRAWCANVAS_H
#include <v/vcanvas.h>	// derive from vCanvas
#include <v/vpen.h>	// contain a vPen

    typedef struct point	// simple struct for points
      {
	int x;  int y;		// coordinates
	vPen pPen;		// pen to use
      } point;

    class myCanvasPane : public vCanvasPane
      {
      public:		//--------------------------------- public
	myCanvasPane();
	virtual ~myCanvasPane();

	// Methods we will override.

	virtual void Clear();
  	virtual void MouseDown(int X, int Y, int button);
	virtual void MouseUp(int X, int Y, int button);
	virtual void MouseMove(int x, int y, int button);
	virtual void Redraw(int x, int y, int w, int h);
	virtual void myCanvasPane::Resize(int w, int h);

	// New methods for this class
	int Read(char* name);
	int Save(char* name);

      protected:	//------------------------------ protected

      private:		//-------------------------------- private
	int _mouseDown;			// state of mouse
	int _begx;  int _begy;  int _curx;  int _cury;  // coords
	point *_pt;			// for point list
	int _nextpt;			// next free point
      };
#endif
