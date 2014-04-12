//=======================================================================
//	vdbcmdw.h:	Header for vdbCmdw class
//=======================================================================

#ifndef vdbCmdW_H
#define vdbCmdW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vutil.h>	// For V Utilities
#include <v/vcmdpane.h> // command pane
#include <v/vstatusp.h>	// For the status pane
#include <v/vfont.h>
#include <v/vtimer.h>	// timer

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

//#include "vdbcnv.h"	// vdbTextEditor
#include <v/vtexted.h>	// standard text editor window
#include "gdbdlg.h"
#include "jdbdlg.h"
#include "videcmdw.h"

    enum DebugModel {dbGDB, dbJDB, dbNone};
    const int dbOutMax = 255;

#ifdef V_VersionX
    const int pREAD = 0;
    const int pWRITE = 1;
#endif

    class vdbCmdWindow;

    class dbAuxTimer : public vTimer
      {
      public:		//---------------------------------------- public
	dbAuxTimer(vdbCmdWindow* cw) { cmdw = cw; }
	~dbAuxTimer() {}
	virtual void TimerTick();
      private:		//--------------------------------------- private
	vdbCmdWindow* cmdw;
      };

    class vdbCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public

	vdbCmdWindow(char*, int, int);
	virtual ~vdbCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);
	void ChangeLoc(long line, int col);
	void StatusMessage(char* msg);

	dbAuxTimer* getDbAuxTimer() { return auxTimer; }
	bool OpenDebugger(const char* debugger, const char* exe);
	videCmdWindow* GotoDebugLine(char* info);

        void dbSendCmd(char* cmd, int echo = 1);
        bool dbShowOut(bool atBottom = false);
	void debugCmd(videCmdWindow* win, ItemVal id);
        void breakCmd(char* fname, long line, ItemVal id);
	void SetToolBar(ItemVal id, int indx, int val);
	DebugModel getDebugModel() { return debugModel; }

      protected:	//--------------------------------------- protected

        void doBreakCmd(videCmdWindow* win, char* bcmd);

      private:		//--------------------------------------- private

	// Standard elements
	vTextEditor* vdbCanvas;		// For the canvas

	vMenuPane* vdbMenu;		// For the menu bar
	vCommandPane* vdbCmdPane;	// for the command pane
	vStatusPane* vdbStatus;		// For the status bar
	vFont vdbFont;
	dbAuxTimer* auxTimer;		// aux timer

	gdbDialog* gdbDlg;		// our popup window
        jdbDialog* jdbDlg;		// for now...

	DebugModel debugModel;

	// Dialogs associated with CmdWindow

	// Stuff to interface to gdb
	int dbOutNext;
        char dbOutLine[dbOutMax+1];
        char dbCmd[256];
        char* dbCmdPtr;

#ifdef V_VersionWindows
	HANDLE dbProcessHandle;
	DWORD dbProcessId;
	HANDLE hChildStdinRd, hChildStdinWr, hChildStdinWrDup, 
	  hChildStdoutRd, hChildStdoutWr, 
	  hInputFile, hSaveStdin, hSaveStdout; 
#endif

#ifdef V_VersionX
    int to_child[2];		// pipes
    int to_parent[2];
    int to_parent_error[2];

    FILE *_inputfp;	    	// read from child
    FILE *_outputfp;	    	// write to child
    FILE *_errorfp;	    	// read errors from child

    pid_t _pid;			// child's process id
#endif

      };
#endif
