//	cubecnv.h:	Header for cubeOGLCanvasPane class
//=======================================================================

#ifndef cubeCNV_H
#define cubeCNV_H

#include <v/vbglcnv.h>

    class cubeOGLCanvasPane : public vBaseGLCanvasPane
      {
      public:		//---------------------------------------- public
	cubeOGLCanvasPane();
	virtual ~cubeOGLCanvasPane();

	virtual void graphicsInit(void);

	// Scrolling
	virtual void HPage(int shown, int top);
	virtual void VPage(int shown, int top);

	virtual void HScroll(int step);
	virtual void VScroll(int step);

	// Events
	virtual void MouseDown(int x, int y, int button);
	virtual void MouseUp(int x, int y, int button);
	virtual void MouseMove(int x, int y, int button);

	virtual void Redraw(int x, int y, int width, int height);
	virtual void Resize(int newW, int newH);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private
        int initDone;
      };
#endif
