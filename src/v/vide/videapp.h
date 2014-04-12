//=======================================================================
//	videapp.h:	Header for videApp class
//	date: 04 Mar 2000 11:44:06
//=======================================================================

#ifndef videAPP_H
#define videAPP_H

// Include standard V files as needed

#ifdef USE_V_DLL
#define VIDE_VERS "VIDE - 1.08/04Mar00!"
#else
#define VIDE_VERS "VIDE - 1.08/04Mar00"
#endif

#ifdef V_VersionX
#define stricmp(a,b) strcasecmp((a),(b))
#endif

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include <v/vapp.h>
#include <v/vawinfo.h>
#include <v/vtimer.h>

#include "videcmdw.h"	// we use videCommandWindow
#include "vdbcmdw.h"	// debug command window
#include "videlib.h"	// most of VIDE uses the lib

    enum Emulation
      {
        Generic,
        See,
        Vi,
        Emacs,
        Epsilon,
        WordStar
      };

    enum UpdateEvents
      {
	ChangeFont,	// have changed font
	ChangePrefs,	// editor prefs have changed
	ChangePC	// changing PC from debugger
      };

    enum {
	m_FirstCmd = 100, 		// Dummy Command
	m_Options,			// set options
        m_View,				// view a file R/O
	m_saveAll,
        m_SaveClose,
        m_Build,
        m_MakeAll,
        m_MakeCleanObj,
        m_MakeCleanAll,
        m_MakeCleanBin,
        m_MakeDebug,
        m_MakeArgs,
        m_StopMake,
        m_SelectMake,
	m_GenCtags,
	m_sendToBrowser,

	m_Project,           // Project menu
	m_ProjectNewC,        // ProjectDialog menu
	m_ProjectNewJava,        // ProjectDialog menu
	m_NewJavaApplet,
	m_NewJavaWin,
	m_NewJavaConsole,
	m_ProjectOpen, 	     // ProjectDialog menu
	m_ProjectEdit,
	m_ProjectClose,
	m_ProjectSaveAs,
	m_MakeMake,

        m_Tools,
        m_RunVgen,
        m_RunViconed,
        m_RunCmdArgs,
        m_RunProject,
        m_RunShell,

	m_CloseDebug,
	m_DebugMenu,
	m_Help,
	m_AboutVide,
	m_EditorPrefs,
	m_VidePrefs,

	videHelp,
	gnuHelp,
	win32Help,
	vguiHelp,
	javaHelp,
	javaAPIHelp,
       	htmlCSSHelp,
	htmlHelp,

	m_Debug,
	m_RunTo,
	m_StepOver,
	m_StepInto,
	m_StopDebug,
	m_SetBP,
	m_DeleteBP,
	m_DisableBP,
	m_CallStack,
	m_Inspect,
	m_InspectInd,
	m_Locals,
	m_ShowBreakpoints,
	m_DeleteBreakpoints,
	m_Continue,
	m_Kill,
	m_Up,
	m_Down,
	m_DBRun,
	m_Args,
	m_SetArgs,
	m_DBHelp,

	m_jdbLocals,
	m_jdbClasses,
	m_jdbStepUp,
	m_jdbMemory,
	
	m_lastDebug
      };
//@V@:EndIDsi

    static const int maxBPs = 32;

    class myTimer : public vTimer
      {
      public:		//---------------------------------------- public
	myTimer() {}
	~myTimer() {}
	virtual void TimerTick();
      private:		//--------------------------------------- private
      };

  class BreakPoint
    {

    public:
	BreakPoint();
	~BreakPoint() {};	// nothing to clean up..
	BreakPoint(char* baseName, long line,BreakPoint* nxt);
	bool addBP(char* baseName, long line);
	bool checkIfBP(char* baseName, long line);
	bool deleteBP(char* baseName, long line);
	void sendForAllBreakPoints(ItemVal id);
	void deleteAllBreakPoints();
	void adjustLines(char* name,long line_1, int by);
	char* getFileName() {return bpFile;}
	char* getPCFileName() {return pcFile;}
	void setCurPC(char* name, long line);
	bool checkIfCurPC(char* name, long line)	// inline for speed
	  {
	    return line == pcLine && strcmp(pcFile,name) == 0;
	  }

	long bp[maxBPs];		// list of BPs for this file

    private:
	char bpFile[maxFileNameSize];	// filename only
	BreakPoint* next;		// link to next breakpoint
	static char pcFile[maxFileNameSize];	// filename only
	static long pcLine;

    };

    class videApp : public vApp
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public

	videApp(char* name, int sdi = 0, int h = 0, int w = 0);
	virtual ~videApp();

	// Routines from vApp that are normally overridden

	virtual vWindow* NewAppWin(vWindow* win, VCONST char* name, int w, int h,
		vAppWinInfo* winInfo = 0);

	virtual void Exit(void);

	virtual int CloseAppWin(vWindow*);

	virtual void AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType);

	virtual void KeyIn(vWindow*, vKey, unsigned int);

	virtual void DropFile(const char* fn);

 	// routines for videApp

        videCmdWindow* GetMsgWindow() { return _videCmdWin; }
	void splitFileName(const char* fn, char* fdir, char* fbase, char* fext);

        void SetMkFile(char* mkf);
        char* GetMkFile() { return _videMkFile; }

        Emulation GetEmulation() { return _emulation; }
        void SetEmulation(Emulation em) { _emulation = em; }

	char* getHelpPath() {return _videHelpPath;}
	char* getJavaPath() {return _videJavaPath;}
	char* getBCCPath() {return _videBCCPath;}
	char* getBrowser() {return _videBrowser;}
	char* getHelpViewer() {return _videHelpViewer;}
	char* getVideShell() {return _videShell;}
	char* getDebugger() {return _videDebugger;}
	char* getCtags() {return _videCtags;}
	void setHelpPath(const char* hp) {strcpy(_videHelpPath,hp);}
	void setJavaPath(const char* jp) {strcpy(_videJavaPath,jp);}
	void setBCCPath(const char* bcp) {strcpy(_videBCCPath,bcp);}
	void setBrowser(const char* bp) {strcpy(_videBrowser,bp);}
	void setHelpViewer(const char* bp) {strcpy(_videHelpViewer,bp);}
	void setVideShell(const char* bp) {strcpy(_videShell,bp);}
	void setDebugger(const char* bp) {strcpy(_videDebugger,bp);}
	void setCtags(const char* bp) {strcpy(_videCtags,bp);}
	int getAutoCtags() {return _autoCtags;}
	int setAutoCtags(int val) {_autoCtags = val;}
	int getAutoSave() {return _autoSave;}
	void setAutoSave(int v);
	int getBackup() {return _backup;}
	void setBackup(int v) {_backup = v;}
	int getAutoIndent() {return _autoin;}
	void setAutoIndent(int v) {_autoin = v;}
	int getColorScheme() { return _colorScheme; }
	void setColorScheme(int cs) { _colorScheme = cs; }
	int getDefaultRows() { return _winRows;}
	int getDefaultCols() { return _winCols;}
	void setDefaultRows(int r) { _winRows = r;}
	void setDefaultCols(int c) { _winCols = c;}

	int readPrefs();
	void savePrefs();
	void setEditLocals();

	// Debug helpers
	bool debuggerOpen() { return vdbCmdWin != 0;}
	void debugCommand(videCmdWindow* win, ItemVal id);
	void DebuggerClosed() {vdbCmdWin = 0;};
	void runDebug(videCmdWindow* win, const char* runName, const char* debugger);
	void setCurCmdWin(videCmdWindow* win);
	videCmdWindow* getCurCmdWin();
	vdbCmdWindow* getvdbCmdWin() { return vdbCmdWin; }
	bool isBP(char* bname, long line)
		{ return bpList->checkIfBP(bname,line); }
	BreakPoint* getBreakPoints() { return bpList; }

	// help
	void showHelp(int id);
	int findHelp(const char* name, const char* dir, const char* path,
		int showNotice = 1, int browser = 0);

	// make the default font easily available...

	vFont DefaultFont;

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

        Emulation _emulation;
	videCmdWindow* _videCmdWin;	// first window - the info console
	videCmdWindow* curCmdWin;	// current cmd window
	vdbCmdWindow* vdbCmdWin;	// The debug cmd window

	myTimer* _saveTimer;

	BreakPoint* bpList;		// list of breakpoints

	int _tabSpacing;
	int _backup;
	int _autoin;
	int _autoSave;
	int _wrapLim;
	int _braceIndent;
	int _winRows;
	int _winCols;
	int _colorScheme;
	int _autoCtags;
        char _videMkFile[maxFileNameSize];
        char _videHelpPath[maxFileNameSize];
        char _videJavaPath[maxFileNameSize];
        char _videBCCPath[maxFileNameSize];
        char _videBrowser[maxFileNameSize];
        char _videHelpViewer[maxFileNameSize];
        char _videShell[maxFileNameSize];
        char _videDebugger[maxFileNameSize];
        char _videCtags[maxFileNameSize];
      };
#endif
