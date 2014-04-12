//	bnccnv.h:	Header for bncOGLCanvasPane class
//=======================================================================

#ifndef bncCNV_H
#define bncCNV_H

#include <v/vbglcnv.h>

    class bncOGLCanvasPane : public vBaseGLCanvasPane
      {
      public:		//---------------------------------------- public
	bncOGLCanvasPane(unsigned int vGLmode = vGL_Default, PaneType pt = P_Canvas);
	virtual ~bncOGLCanvasPane();

	virtual void graphicsInit(void);

  void TimerAnimate(void);	// for AuxTimer animation
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
