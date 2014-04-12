//	tglcnv.h:	Header for testGLCanvasPane class
//=======================================================================

#ifndef testGLCNV_H
#define testGLCNV_H

//#include "../vbglcnv.h"
#include <v/vbglcnv.h>

    class testGLCanvasPane : public vBaseGLCanvasPane
      {
      public:		//---------------------------------------- public
	testGLCanvasPane();
	virtual ~testGLCanvasPane();

	virtual void graphicsInit(void);

	void Spin();

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
	int _what;
      };
#endif
