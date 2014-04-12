//=======================================================================
//	vdbcnv.h:	Header for vdbTextEditor class
//=======================================================================

#ifndef vdbCNV_H
#define vdbCNV_H

#include <v/vkeys.h>
#include <v/vtexted.h>


    class vdbCmdWindow;
    class vCmdWindow;

    class vdbTextEditor : public vTextEditor
      {
      public:		//---------------------------------------- public
	vdbTextEditor(vdbCmdWindow* parent);
	virtual ~vdbTextEditor();

	virtual void ChangeLoc(long line, int col);
	virtual void StatusMessage(char *msg);
	virtual void ErrorMsg(char *str);

      protected:	//--------------------------------------- protected


      private:		//--------------------------------------- private

      };

     class vdbCI     // easy class to override
      {

        public:
          vdbCI(vTextEditor* textEd, vCmdWindow* cw);
          virtual ~vdbCI();

          virtual int ProcessKey(vKey key, unsigned int shift);
          virtual vTextEditor* te() VCONST { return _textEd; }
          virtual vCmdWindow* cmdWin() VCONST { return _myCmdWindow; }
          virtual void InitCmdInterp();
          virtual void CmdInterpHelp();
	  virtual void MouseJump(int row, int col) { /* no-op by default */ }

        private:
          vTextEditor* _textEd;
          vCmdWindow* _myCmdWindow;
          long
            cmdCount;          // how many times to repeat command
          int
            countWait,  // if building a count
            metaChar,   // the meta char for commands
            metaWait;   // if waiting on a meta command
      };


#endif
