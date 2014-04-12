//	testcnv.h:	Header for testCanvasPane class
//=======================================================================

#ifndef testCNV_H
#define testCNV_H

#include <v/vcanvas.h>

class vDC;

    class testCanvasPane : public vCanvasPane
      {
      public:		//---------------------------------------- public
	testCanvasPane();
	virtual ~testCanvasPane();

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

   void draw(vDC*);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private
      };
#endif
