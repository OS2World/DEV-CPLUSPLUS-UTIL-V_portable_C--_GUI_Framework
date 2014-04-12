//	videcnv.h:	Header for vedCanvasPane class
//=======================================================================

#ifndef VIDECNV_H
#define VIDECNV_H

#define VCmdWindow videCmdWindow

#include <v/vtexted.h>

#include <fstream.h>

    class VCmdWindow;

    class vedTextEditor : public vTextEditor
      {
      public:		//---------------------------------------- public
	vedTextEditor(VCmdWindow* parent);
	virtual ~vedTextEditor();

	int ReadFile(char* name, int paintIt = 1);
	int SaveFile(char* name);

	virtual void ChangeLoc(long line, int col);
	virtual void ChangeInsMode(int IsInsMode, char* = 0);
	virtual void StatusMessage(char *msg);
	virtual void ErrorMsg(char *str);
 	virtual void EnterFocus();
	virtual void adjustLines(long line_1, int by);
	virtual void setColorScheme();

      protected:	//--------------------------------------- protected
          void paintLine(char* linout, int lineStart,
              int hiStart, int hiLast, long lineNum);
          virtual void TextMouseDown(int row, int col, int button);
          virtual void TextMouseUp(int row, int col, int button);


      private:		//--------------------------------------- private
          int parseC(char* linout, ChrAttr* attrs, 
		long lineNum, FileType ft);
          int parseFortran(char* linout, ChrAttr* attrs, long lineNum);
          int parseHTML(char* linout, ChrAttr* attrs, long lineNum);
          int parseTeX(char* linout, ChrAttr* attrs, long lineNum);
          int isCKeyWord(char* token);
          int isFortranKeyWord(char* token);
          int isPerlKeyWord(char* token);
          int isPerlOp(char* token);
          int isJavaKeyWord(char* token);
          int isHTMLKeyWord(char* token);
	  int curColors;
      };
#endif
