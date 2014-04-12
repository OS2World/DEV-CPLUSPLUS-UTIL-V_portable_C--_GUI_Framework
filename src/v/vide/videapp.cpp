//=======================================================================
//	videapp.cpp:	Source for videApp class
//	date: 05 Mar 2000 04:17:22 PM
//=======================================================================

#include "videapp.h"		// Header file
#include <v/vos.h>	// <v/vos.h>
#include <v/vutil.h>
#include <v/vnotice.h>
#include <v/vynreply.h>
#include "vprefs.h"	// <v/vprefs.h>
#include "vrundlg.h"
#include <fstream.h>


    char BreakPoint::pcFile[maxFileNameSize];
    long BreakPoint::pcLine;

//====================>>> BreakPoint::BreakPoint <<<====================
  BreakPoint::BreakPoint()
  {
    for (int ix = 0 ; ix < maxBPs ; ++ix)
	bp[ix] = -1;
    bpFile[0] = 0;
    next = 0;
    pcFile[0] = 0;	// no current breakpoint
    pcLine = -1;
  }

//====================>>> BreakPoint::BreakPoint <<<====================
  BreakPoint::BreakPoint(char* path, long line,	BreakPoint* nxt)
  {
    // creating a new record
    for (int ix = 0 ; ix < maxBPs ; ++ix)
	bp[ix] = -1;
    bp[0] = line;
    strcpy(bpFile,path);
    next = nxt;
  }

//====================>>> BreakPoint::addBP <<<====================
  bool BreakPoint::addBP(char* baseName, long line)
  {
    // add a new breakpoint in given file.
    BreakPoint* bpp;

    if (bpFile[0] == 0)		// first time!
      {
	strcpy(bpFile, baseName);
	bp[0] = line;
	return true;
      }

    // see if in an exisiting record yet

    for (bpp = this ; bpp != 0 ; bpp = bpp->next)
      {
	if (strcmp(bpp->bpFile,baseName) == 0)	// found correct file
	  {
	    for (int ix = 0 ; ix < maxBPs ; ++ix)
	      {
		if (bpp->bp[ix] < 0)		// empty slot
		  {
		    bpp->bp[ix] = line;
		    return true;
		  }
	      }
	    return false;			// ran out of bps
	  }
      }
    // ok, file not being used yet
    next = new BreakPoint(baseName, line, next);
    return true;    
  }

//====================>>> BreakPoint::setCurPC <<<====================
  void BreakPoint::setCurPC(char* baseName, long line)
  {
    strcpy(pcFile,baseName);
    pcLine = line;
  }

//====================>>> BreakPoint::checkIfBP <<<====================
  bool BreakPoint::checkIfBP(char* baseName, long line)
  {
    if (bpFile[0] == 0)		// no bps at all
	return false;

    // see if in an exisiting bp record

    BreakPoint* bpp;
    for (bpp = this ; bpp != 0 ; bpp = bpp->next)
      {
	if (strcmp(bpp->bpFile,baseName) == 0)	// found correct file
	  {
	    for (int ix = 0 ; ix < maxBPs ; ++ix)
	      {
		if (bpp->bp[ix] < 0)		// empty slot, so fails...
		  {
		    return false;
		  }
		else if (bpp->bp[ix] == line)	// found it!
		    return true;
	      }
	    return false;			// ran out of bps
	  }
      }
    return false;
  }

//====================>>> BreakPoint::sendForAllBreakPoints <<<====================
  void BreakPoint::sendForAllBreakPoints(ItemVal id)
  {
    // set or clear depending on id

    if (id != m_SetBP && id != m_DeleteBP)
	return;			// ignore invalid parameter

    if (bpFile[0] == 0)		// no bps at all
	return;

    vdbCmdWindow* dbWin = ((videApp*)theApp)->getvdbCmdWin();

    if (dbWin == 0)	// no cmdwidow
	return;

    // scan all breakpoints, and send the command to the debugger
    BreakPoint* bpp;
    for (bpp = this ; bpp != 0 ; bpp = bpp->next)
      {
	for (int ix = 0 ; ix < maxBPs ; ++ix)
	  {
	    if (bpp->bp[ix] < 0)		// empty slot, so fails...
	      {
		break;				// all done
	      }
	    else				// a break line
	      {
		dbWin->breakCmd(bpp->bpFile,bpp->bp[ix],id);
	      }
	  }
      }
    return;
  }

//====================>>> BreakPoint::deleteAllBreakPoints <<<====================
  void BreakPoint::deleteAllBreakPoints()
  {
    //  clear all internal breakpoints

    if (bpFile[0] == 0)		// no bps at all
	return;


    // scan all breakpoints, and delete
    BreakPoint* bpp;
    for (bpp = this ; bpp != 0 ; bpp = bpp->next)
      {
	for (int ix = 0 ; ix < maxBPs ; ++ix)
	  {
	    bpp->bp[ix] = -1;
	  }
      }
    return;
  }

//====================>>> BreakPoint::deleteBP <<<====================
  bool BreakPoint::deleteBP(char* baseName, long line)
  {
    // delete a breakpoint. We greatly simplify this by never
    // actually deleting any breakpoint records if all bps
    // are deleted -- we will just keep it around under the assumption
    // that there are likely to be new bps in the same file, so why
    // bother. This also means that we don't need to worry about
    // losing the first record with its next pointer...

    if (bpFile[0] == 0)		// no bps at all
	return false;

    // see if in an exisiting bp record

    BreakPoint* bpp;
    for (bpp = this ; bpp != 0 ; bpp = bpp->next)
      {
	if (strcmp(bpp->bpFile,baseName) == 0)	// found correct file
	  {
	    for (int ix = 0 ; ix < maxBPs ; ++ix)
	      {
		if (bpp->bp[ix] < 0)		// empty slot, so fails...
		  {
		    return false;		// not found
		  }
		else if (bpp->bp[ix] == line)	// found it!
		  {
		    for (int ij = ix+1 ; ij < maxBPs ; ++ij)
			bpp->bp[ij-1] = bpp->bp[ij];  // shift down
		    bpp->bp[maxBPs-1] = -1;	// free last one
		    return true;
		  }
	      }
	    return false;			// ran out of bps
	  }
      }
    return false;				// not found
  }

// ===================>>> BreakPoint::adjustLines <<<======================
  void BreakPoint::adjustLines(char* name, long line_1, int by)
  {
    /* adjust starting line number int note locaetions */

    // scan all breakpoints, and adjust lines
    BreakPoint* bpp;
    for (bpp = this ; bpp != 0 ; bpp = bpp->next)
      {
	if (strcmp(bpp->bpFile, name) == 0)	// found correct file
	  {
	    for (int ix = 0 ; ix < maxBPs ; ++ix)
	      {
		if (bpp->bp[ix] < 0)		// empty slot, so fails...
		  {
		    break;				// all done
		  }
		else				// a break line
		  {
		    if (bpp->bp[ix] > line_1)
			bpp->bp[ix] += (long) by;
		  }
	      }
	    return;		// done in this file
	  }
      }
    return;
  }

//====================>>> myTimer::TimerTick <<<====================
  void myTimer::TimerTick()
  {
    theApp->SendWindowCommandAll(M_Save,1,C_Button);
    // vBeep();
  }

//=========================>>> videApp::videApp <<<==========================
  videApp::videApp(char* name, int sdi, int h, int w) : vApp(name, sdi, h, w)
  {
    // Constructor

    curCmdWin = 0;
    vdbCmdWin = 0;
    _videCmdWin = 0;
    _videMkFile[0] = 0;
    _videHelpPath[0] = 0;
    _videJavaPath[0] = 0;
    _videBCCPath[0] = 0;
    strcpy(_videBrowser,"netscape");
    strcpy(_videHelpViewer,"netscape");
    strcpy(_videCtags,"*");
    _autoCtags = 0;

#ifdef V_VersionX
    strcpy(_videDebugger,"ddd");
    strcpy(_videShell,"xterm");
#else
    strcpy(_videDebugger,"gdb");
    OSVERSIONINFO osver;
    osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    ::GetVersionEx(&osver);
    if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT)
	strcpy(_videShell,"cmd.exe");	// dynamic change to cmd.exe....
    else
	strcpy(_videShell,"command.com");	// dynamic change to cmd.exe....
#endif

    _autoSave = 5;			// every 5 minutes
    _backup = 0;
    _autoin = 0;
    _tabSpacing = 8;
    _wrapLim = 65;
    _colorScheme = 0;
    _braceIndent = 2;
    _winRows = 25;
    _winCols = 80;

    _saveTimer = new myTimer();
    bpList = new BreakPoint();		// first empty element

#ifdef DEVEL
    _emulation = See;
#else
     _emulation = Generic;
#endif
  }

//=========================>>> videApp::videApp <<<==========================
  videApp::~videApp()
  {
    // Destructor

//  If we really do the following, things die....
//  if (vdbCmdWin)		
//	delete vdbCmdWin;
//  if (_videCmdWin)
//	delete _videCmdWin;

    // but it is ok to stop the timer...
//    _saveTimer->TimerStop();
//    delete _saveTimer;
  }

//=====================>>> videApp::AppCommand <<<==============================
  void videApp::AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType)
  {
    // Commands not processed by the window will be passed here

    vApp::AppCommand(win, id, val, cType);
  }

//=====================>>> videApp::showHelp <<<==============================
  void videApp::showHelp(int id)
  {
    // This is called from videcmdw.cpp, and there needs to be an entry
    // for each of these values there, too.
    switch (id)
      {
	case videHelp:
	  {
	    if (findHelp("videdoc.htm","vrefman/videdoc/",_videHelpPath,0))
		break;
	    else if (findHelp("videdoc.htm","videdoc/",_videHelpPath,0))
		break;
	    else if (findHelp("videdoc.htm","",_videHelpPath,0))
		break;
	    else
		(void)findHelp("vnohelp.htm","",_videHelpPath);
	    break;
	  }

	case gnuHelp:
	  {
	    if (findHelp("vhelp.htm","",_videHelpPath,0))
		break;
	    else
		(void)findHelp("vnohelp.htm","",_videHelpPath);
	    break;
	  }

	case win32Help:
	  {
	    if (!findHelp("win32.hlp","win32/",_videHelpPath,0) )
	      {
	    	if (findHelp("win32.htm","win32/",_videHelpPath,0))
		    break;
		else
		    (void)findHelp("vnohelp.htm","",_videHelpPath);
		break;
	      }
	    break;
	  }

	case vguiHelp:
	  {
	    if (findHelp("v.htm","vrefman/",_videHelpPath,0))
		break;
	    else if (findHelp("vrefman.htm","vrefman/",_videHelpPath,0))
		break;
	    else
		(void)findHelp("vnohelp.htm","",_videHelpPath);
	    break;
	  }

	case javaHelp:
	  {
	    if (findHelp("index.html","",_videJavaPath,0))
		break;
	    else
		(void)findHelp("vnohelp.htm","",_videHelpPath);
	    break;
	  }

	case javaAPIHelp:
	  {
	    if (findHelp("index.html","api/",_videJavaPath,0))
		break;
	    else
		(void)findHelp("vnohelp.htm","",_videHelpPath);
	    break;
	  }

	case htmlHelp:
	  {
	    if (findHelp("index.html","html/",_videHelpPath,0))
		break;
	    else
		(void)findHelp("vnohelp.htm","",_videHelpPath);
	    break;
	  }

	case htmlCSSHelp:
	  {
	    if (findHelp("index.html","html/css/",_videHelpPath,0))
		break;
	    else
		(void)findHelp("vnohelp.htm","",_videHelpPath);
	    break;
	  }
      }
  }

//======================>>> videApp::findHelp <<<===========================
  int videApp::findHelp(const char* name, const char* dir, const char* path,
	 int failNotice, int browser)
  {
    char hName[maxFileNameSize+2];

    strcpy(hName, path);
    int len = strlen(hName);
    if (len > 0 && hName[len-1] != '/' && hName[len-1] != '\\')
	strcat(hName,"/");
    strcat(hName,dir);
    strcat(hName,name);

    ifstream infile(hName);
    if (!infile)
      {
	infile.close();
	if (!failNotice)
	    return 0;

	vNoticeDialog note(this,"Can't find help");

#ifdef V_VersionX
	char msg[] = 
"Sorry, VIDE can't find the help file you requested. Please\n\
use Options:VIDE command to set the path to the correct\n\
home path for the help files. For example, \"/usr/local/vhelp\" for\n\
V and C++ help, or \"/usr/local/jdk1.2/docs\" for Java help.";
#else
	char msg[] = 
"Sorry, VIDE can't find the help file you requested. Please\n\
use Options:VIDE command to set the path to the correct\n\
home path for the help files. For example, \"C:/egcs/vhelp\" for\n\
V and C++ help, or \"C:/jdk1.2/docs\" for Java help.";
#endif

	note.Notice(msg);
        return 0;
      }
    infile.close();
#ifdef V_VersionWindows
    ::ShellExecute(winHwnd(),"open",hName,0,0,SW_SHOWDEFAULT);
#else
    vOS os;
    char cmd[maxFileNameSize+20];
    if (browser)
	strcpy(cmd,_videBrowser);
    else
	strcpy(cmd,_videHelpViewer);
    strcat(cmd," ");
    strcat(cmd,hName);
    os.vRunProcess(cmd,0,0,0);
#endif
    return 1;
  }

//======================>>> videApp::getCurCmdWindow <<<===========================
  videCmdWindow* videApp::getCurCmdWin()
  {
    return curCmdWin;
  }

//======================>>> videApp::setCurCmdWindow <<<===========================
  void videApp::setCurCmdWin(videCmdWindow* curWin)
  {
    if (curWin == _videCmdWin || _videCmdWin == 0)
	return;
    curCmdWin = curWin;
  }

//======================>>> videApp::CloseAppWin <<<===========================
  int videApp::CloseAppWin(vWindow* win)
  {
    // This will be called BEFORE a window has been unregistered or
    // closed.  Default behavior: unregister and close the window.

    videCmdWindow* cw = (videCmdWindow*)win;

    if (vdbCmdWin == (vdbCmdWindow*)win)	// the debugger?
      {
	vdbCmdWin->dbSendCmd("quit\n",false);
	int rv = vApp::CloseAppWin((vWindow*)vdbCmdWin);
	vdbCmdWin = 0;			// done and destroyed
        return 1;
      }
    else if (cw->CheckClose())
        return vApp::CloseAppWin(win);
    else
        return 0;
  }

//=====================>>> videApp::debugCommand <<<==============================
  void videApp::debugCommand(videCmdWindow* win, ItemVal id)
  {
    // Debug Commands handled here...

    // we will keep track of breakpoints with or without a running
    // debugger. Pass to local routines first.

    long curlin = (win->GetTextEd())->GetCurLine();

    if (vdbCmdWin)			// debugger open!
      {
	if (id == m_StopDebug)
	  {
	    // kill the debugger
	    vdbCmdWin->dbSendCmd("quit\n",false);
	    int rv = vApp::CloseAppWin((vWindow*)vdbCmdWin);
	    vdbCmdWin = 0;			// done and destroyed
	    return;
	  }

	if (GetMsgWindow()->checkIfAnyChanged())
	  {
	    vNoticeDialog note(this,"Files changed!");
	    note.Notice(
"Your source files have changed. You should\n\
close the debugger and remake your files.\n\
Repeat this command to continue debugging.");

	    SendWindowCommandAll(M_Save,1,C_Button);
	    return;
	  }
      }


    // we handle debugger commands here
    switch (id)
      {
	case m_SetBP:
	    bpList->addBP(win->GetFileName(), curlin);
	    (win->GetTextEd())->Verify();	// do this here!
	    if (vdbCmdWin)			// debugger open!
	      {
		    vdbCmdWin->breakCmd(win->GetFileName(),curlin,id);
	      }
	    return;

	case m_DeleteBP:
	    bpList->deleteBP(win->GetFileName(), curlin);
	    (win->GetTextEd())->Verify();	// do this here!
	    if (vdbCmdWin)			// debugger open!
	      {
		vdbCmdWin->breakCmd(win->GetFileName(),curlin,id);
	      }
	    return;

	case m_DisableBP:
//	    if (vdbCmdWin)			// debugger open!
//	      {
//		vdbCmdWin->breakCmd(win->GetFileName(),curlin,id);
//	      }
	    return;

	case m_ShowBreakpoints:
	// for now, use debuggers, evenutally open our db editor
	    if (vdbCmdWin)			// debugger open!
	      {
		vdbCmdWin->breakCmd(win->GetFileName(),curlin,id);
	      }
	    return;

	case m_DeleteBreakpoints:
	    if (vdbCmdWin)			// debugger open!
	      {
		if (vdbCmdWin->getDebugModel() == dbJDB)	// java!
		    getBreakPoints()->sendForAllBreakPoints(m_DeleteBP);
		else		// gdb can do it all in one command
		    vdbCmdWin->breakCmd(win->GetFileName(),curlin,id);
	      }
	    getBreakPoints()->deleteAllBreakPoints();
	    (win->GetTextEd())->Verify();	// do this here!
	    
	    return;

	default:
	    break;
      }

    if (id == m_Debug || !vdbCmdWin)		// No debugger open!
      {
	vBeep();
    	return;
      }

    vdbCmdWin->debugCmd(win, id);
  }

//============================>>> videApp::Exit <<<===========================
  void videApp::Exit(void)
  {
    // This is called to close all windows.

    vApp::Exit();		// Default behavior
  }

//=========================>>> videApp::KeyIn <<<==============================
  void videApp::KeyIn(vWindow* win, vKey key, unsigned int shift)
  {
    // Key strokes not processed by the window will be passed here

    vApp::KeyIn(win, key, shift);
  }

//=====================>>> videApp::NewAppWin <<<==========================
  vWindow* videApp::NewAppWin(vWindow* win, VCONST char* name,
    int w, int h, vAppWinInfo* winInfo)
  {
    vAppWinInfo* awinfo = winInfo;
    VCONST char *appname = name;

    if (w < 0)
	w = getDefaultCols();
    if (h < 0)
	h = getDefaultRows();

    if (!*name)
      {
	 appname = "VIDE";		// Default name
      }
	
    // Create the first window using provided CmdWindow if passed

    videCmdWindow* cw = (videCmdWindow*) win;	// Pointer to instance of passed window

    if (cw != 0 && _videCmdWin == 0)
        _videCmdWin = (videCmdWindow*) cw;		// remember this first window

    if (!cw)
      {
	cw = new videCmdWindow(appname, w, h);
      }

    if (!awinfo)
	awinfo = new vAppWinInfo(appname);

    if (!vApp::NewAppWin((vWindow*) cw, appname, w, h, awinfo))
	return 0;
    return (vWindow*) cw;
  }

//=====================>>> videApp::runDebug <<<=============================
  void videApp::runDebug(videCmdWindow* win, const char* runName, 
	const char* debugger)
  {
#ifdef V_VersionX
//    if (strcmp(_videDebugger,"ddd") == 0)
//      {
	vOS os;

	char dbgName[maxFileNameSize+2];
	char runLine[maxFileNameSize+12];
	if (runName && *runName)
	    strcpy(dbgName,runName);
	else
	  {
	    vRunDialog note(this);
	    if (note.GetDebugName("",dbgName) == M_Cancel)
	      return;
	  }
	strcpy(runLine,_videDebugger);
	strcat(runLine," ");
	strcat(runLine,dbgName);
	os.vRunProcess(runLine,0,0,0);
	return;
//      }
 
#endif
#ifdef V_VersionWindows
    if (!vdbCmdWin)	// don't open again if already open...
      {
	char dbgName[maxFileNameSize+2];

	if (_videBCCPath[0] != 0 || !debugger || !*debugger)	// Borland...
	  {
	    vNoticeDialog nodb(this,"No Debugger");
	    nodb.Notice("Sorry, no debugger available for this environment (Borland).");
	    return;
	  }
	if (runName && *runName)
	    strcpy(dbgName,runName);
	else
	  {
	    vRunDialog note(this);
	    if (note.GetDebugName("",dbgName) == M_Cancel)
	      return;
	  }

	fixSlash(dbgName);

	// put up the window
#ifdef V_VersionX
	vdbCmdWin = new vdbCmdWindow("Debug info", 0, 0);
	(void) NewAppWin((vWindow*) vdbCmdWin,"Debug info", 0, 0, 0);
#else
	vdbCmdWin = new vdbCmdWindow("Debug info", 80, 8);
	(void) NewAppWin((vWindow*) vdbCmdWin,"Debug info", 80, 8, 0);
#endif
	CheckEvents();	// make sure window up

	// and start the debugger...
	vdbCmdWin->OpenDebugger(debugger,dbgName);
      }
    return;
#endif
  }

//============================>>> videApp::SetMkFile <<<===========================
  void videApp::SetMkFile(char* mkf)
  {
    // update our global variable CWD and update any open windows
    vOS vos;
    bool hadpath = false;
    char temp[maxFileNameSize + 2];

    if (strlen(mkf) < maxFileNameSize)
      {
	for (char* cp = mkf ; *cp ; ++cp)
	    if (*cp == '/' || *cp == '\\')
	      {
		hadpath = true;
		break;
	      }
	if (hadpath)
	    strcpy(_videMkFile, mkf);
	else
	  {
	    vos.vGetCWD(temp,maxFileNameSize);
	    strcpy(_videMkFile, temp); strcat(_videMkFile,"/");
	    strcat(_videMkFile,mkf);
	  }
      }
    else
        strcpy(_videMkFile,"Makefile");

    if (hadpath)
      {
	changeDir(_videMkFile);
      }
    theApp->SendWindowCommandAll(m_updateMkFile, 0, C_Button);
  }

//============================>>> videApp::readPrefs <<<===========================
  int videApp::readPrefs()
  {
    // read settings from Vide preferences file

    int fontSet = 0;
    long val;

    vPrefs vp;
    char buff[maxFileNameSize+2];

    if (!vp.openRead("Vide","[VIDE Preferences]"))
      {
	vNoticeDialog note(this,"VIDE needs options set");
	note.Notice(
"This seems to be the first time you've run VIDE.\n\
Please use the items on the Options menu to\n\
set the help path, editor emulation, font, etc.\n\
Then exit VIDE and start it again.");
	return 0;
      }

    (void)vp.getString("HelpPath",_videHelpPath);
    (void)vp.getString("JavaPath",_videJavaPath);
    (void)vp.getString("BCCPath",_videBCCPath);
    if (vp.getLong("AutoSave",val))
	_autoSave = val;
    else 
	_autoSave = 5;

    if (vp.getLong("MakeBackup",val))
	_backup = val;
    else 
	_backup = 0;

    if (vp.getLong("AutoIndent",val))
	_autoin = val;
    else 
	_autoin = 0;

    if (vp.getLong("Emulation",val))
	_emulation = (Emulation)val;
    else
	_emulation = Generic;
	
    if (vp.getLong("TabSpacing",val))
      {
	_tabSpacing = val;
      }

    if (vp.getLong("TextWrap",val))
      {
	_wrapLim = val;
      }

    if (vp.getLong("BraceIndent",val))
      {
	_braceIndent = val;
      }

    if (vp.getLong("DefaultRows",val))
      {
	_winRows = val;
      }

    if (vp.getLong("DefaultCols",val))
      {
	_winCols = val;
      }

    if (vp.getLong("ColorScheme",val))
      {
	_colorScheme = val;
      }

    if (vp.getString("CurrentDir",buff))
      {
	vOS os;
	os.vChDir(buff);	// start in last directory
      }

    if (vp.getString("Font",buff) && *buff)
      {
        DefaultFont.SetNativeFontString(buff);
        fontSet = 1;
      }

    (void)vp.getString("VideShell",_videShell);
    (void)vp.getString("Debugger",_videDebugger);
    (void)vp.getString("ctagsDefault",_videCtags);
    if (vp.getLong("ctagsAuto",val))
      {
	_autoCtags = val;
      }

#ifdef V_VersionX
    (void)vp.getString("Browser",_videBrowser);
    (void)vp.getString("HelpViewer",_videHelpViewer);
#endif
    vp.closeRead();

    setAutoSave(_autoSave);	// start the timer....

    // delete any leftover temp files...
    vOS vos;
    vos.vDeleteFile("makecmds.vtm");
    vos.vDeleteFile("makeerrs.vtm");

    return fontSet;
  }

//============================>>> videApp::setAutoSave <<<===========================
  void videApp::setAutoSave(int val)
  {
    _autoSave = val;
    if (_saveTimer)
      {
	_saveTimer->TimerStop();	// stop first
	if (_autoSave > 0)
	    _saveTimer->TimerSet((long)_autoSave * 60000L);
      }
  }

//============================>>> videApp::savePrefs <<<===========================
  void videApp::savePrefs()
  {
    // save settings to Vide preferences file
    char buff[maxFileNameSize+2];

    vOS os;
    vPrefs vp;

    if (!vp.openSave("Vide","[VIDE Preferences]"))
	return;
    vp.addString("HelpPath",_videHelpPath);
    vp.addString("JavaPath",_videJavaPath);
    vp.addString("BCCPath",_videBCCPath);
    vp.addLong("AutoSave",(long)_autoSave);
    vp.addLong("MakeBackup",(long)_backup);
    vp.addLong("AutoIndent",(long)_autoin);
    vp.addLong("Emulation",(long)_emulation);
    vp.addLong("TabSpacing",
		(long)(_videCmdWin->GetTextEd())->GetGlobalState().tabspc);
    vp.addLong("TextWrap",
		(long)(_videCmdWin->GetTextEd())->GetGlobalState().wraplm);
    vp.addLong("BraceIndent",
		(long)(_videCmdWin->GetTextEd())->GetGlobalState().braceIndent);
    vp.addLong("DefaultRows",(long)_winRows);
    vp.addLong("DefaultCols",(long)_winCols);
    vp.addLong("ColorScheme",(long)_colorScheme);

    strcpy(buff,".");

    os.vGetCWD(buff,maxFileNameSize);
    vp.addString("CurrentDir",buff);

    DefaultFont.GetNativeFontString(buff);
    vp.addString("Font",buff);

    vp.addString("VideShell",_videShell);
    vp.addString("Debugger",_videDebugger);
    vp.addString("ctagsDefault",_videCtags);
    vp.addLong("ctagsAuto",_autoCtags);
#ifdef V_VersionX
    vp.addString("Browser",_videBrowser);
    vp.addString("HelpViewer",_videHelpViewer);
#endif

    vp.closeSave();

  }

//============================>>> videApp::DropFile <<<===========================
  void videApp::DropFile(const char* fn)
  {
    theApp->CheckEvents();	// make sure window up
    if (strstr(fn,".vpj") != 0)
      {
	(((videApp*)theApp)->GetMsgWindow())->OpenProject((char*)fn);
      }
    else
      {
#ifdef V_VersionX
	// can't get LessTif version at least to work...
	vBeep();
#else
	videCmdWindow* cmdw = (videCmdWindow*)
	theApp->NewAppWin(0, "VIDE", -1, -1);
	if (!cmdw)
	    return;
	(void)cmdw->OpenFile((char*)fn);
	cmdw->RaiseWindow();
	(cmdw->GetTextEd())->Verify();
	cmdw->ChangeInsMode(1);
#endif
      }

  }

//============================>>> videApp::setEditLocals <<<===========================
  void videApp::setEditLocals()
  {
    globalState gs = (_videCmdWin->GetTextEd())->GetGlobalState();
    gs.tabspc = _tabSpacing;
    gs.wraplm = _wrapLim;
    gs.braceIndent = _braceIndent;
    (_videCmdWin->GetTextEd())->SetGlobalState(gs);	// and set!
  }

//###########################################################################

  static videApp vide_App(VIDE_VERS);	// The instance of the app

//============================>>> AppMain <<<==============================
  int AppMain(int argc, char** argv)
  {
    int fontSet = ((videApp*)theApp)->readPrefs();

    if (!fontSet)
      {
#ifdef V_VersionX
	(((videApp*)theApp)->DefaultFont).SetFontValues(vfFixed,12,vfNormal,vfBold);
#else
	(((videApp*)theApp)->DefaultFont).SetFontValues(vfFixed,10,vfNormal,vfBold);
#endif
      }

    // Use AppMain to create the main window
#ifdef V_VersionX
    // X version doesn't work same as Windows version
    videCmdWindow* cw = new videCmdWindow("", 0, 0);
    (void) theApp->NewAppWin((vWindow*) cw, "", 0, 0, 0);
#else
    videCmdWindow* cw = new videCmdWindow("", 70, 10);
    (void) theApp->NewAppWin((vWindow*) cw, "", 70, 10, 0);
#endif

    (((videApp*)theApp)->GetMsgWindow())->SetTitle("No Makefile, Project, or .java file Specified");
    (((videApp*)theApp)->GetMsgWindow())->AddLine("------------ V IDE ------------");
    (((videApp*)theApp)->GetMsgWindow())->SetRdOnly(1);

    theApp->CheckEvents();	// make sure window up

    ((videApp*)theApp)->setEditLocals();	// safe to do now that window is up

    if (argc > 1)
      {
        for (int ix = 1 ; ix < argc ; ++ix)
	  {
	    if (argv[ix][0] == '-' || argv[ix][0] == '/')	// switch
	      {
		switch (argv[ix][1])
		  {
		    case 'e':
		    case 'E':
		      {
			switch (argv[ix][2])
			  {
			    case 's':
			    case 'S':
			      {
				((videApp*)theApp)->SetEmulation(See);
				break;
			      }
			    default:
				break;
			  }
			break;
		      }	// case 'e'

		    default:
			break;
		  }
	      }
	    else	// a file name: project or source
	      {
		theApp->CheckEvents();	// make sure window up
		if (strstr(argv[ix],".vpj") != 0)
		  {
		    (((videApp*)theApp)->GetMsgWindow())->OpenProject(argv[ix]);
		  }
		else
		  {
#ifdef V_VersionX
		// can't get LessTif version at least to work...
		    vBeep();
#else
		    videCmdWindow* cmdw = (videCmdWindow*)
		        theApp->NewAppWin(0, "VIDE", -1, -1);
		    if (!cmdw)
			break;
#ifdef OLD_WAY
		    if (!cmdw->OpenFile(argv[ix]))
		      {
			delete cmdw;
			break;
		      }
		    else
		      {
			cmdw->RaiseWindow();
		      }
#else
		    (void)cmdw->OpenFile(argv[ix]);
		    cmdw->RaiseWindow();
#endif
		    cmdw->vedCanvas->Verify();
		    cmdw->ChangeInsMode(1);
#endif
		  }
	      }
	  }
      }
    else
      {

      }

    return 0;
  }
