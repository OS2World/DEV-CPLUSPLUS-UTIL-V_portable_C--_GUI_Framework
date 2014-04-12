//	vtcnv.h:	Header for testTextCanvasPane class
//=======================================================================
#ifndef testTCNV_H
#define testTCNV_H

#include <v/vtextcnv.h>

    class testCmdWindow;

    class testTextCanvasPane : public vTextCanvasPane
      {
      public:		//---------------------------------------- public

	testTextCanvasPane(testCmdWindow* cw);
	virtual ~testTextCanvasPane();

	// Scrolling
	virtual void HPage(int, int);
	virtual void VPage(int, int);

	virtual void HScroll(int);
	virtual void VScroll(int);

	// Events
	virtual void FontChanged(VCONST vFont& newFont);
	virtual void ResizeText(const int rows, const int cols);
	virtual void Redraw(int x, int y, int height , int width);
	virtual void TextMouseDown(int row, int col, int button);
	virtual void TextMouseUp(int row, int col, int button);
	virtual void TextMouseMove(int row, int col, int button);

      protected:	//--------------------------------------- protected
      private:		//--------------------------------------- private
	testCmdWindow* _myCmdw;

      };
#endif

