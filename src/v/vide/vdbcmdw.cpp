//=======================================================================
//	vdbcmdw.cpp:	Source for vdbCmdWindow class
//	date: 05 Mar 2000 04:12:02 PM
//      This is the MS-WINDOWS interface to gdb
//=======================================================================

#include <v/vnotice.h>	// for vNoticeDialog
#include <v/vkeys.h>	// to map keys
#include <v/vicon.h>
#include <v/vos.h>

#include "videapp.h"	// our header
#include "vdbcmdw.h"	// our header

#ifdef V_VersionX
#include <unistd.h>
#include <fcntl.h>
#endif

    // The following definitions mirror those of the edit/console
    // cmdwin (videCmdWindow), but are all no-ops except the edit
    // menu.

//@V@:BeginIDs
    enum {
	// remaining debug commands defined in videapp.h
	m_NA = 4100,
        m_x1, m_x2, m_x3, m_x4, m_x5, m_x6, m_x7, m_x8,
        m_x9, m_x10, m_x11, m_x12, m_x13, m_x14, m_x15,
	m_x16, m_x17,

	cmdAuxTimer,			// when timer ticks

	dbgLast
      };
//@V@:EndIDs

#include "vcmdcmds.h"

    static int filterIndex = 0;
    // The MS-Windows filter seems to hold only 5 entries for the
    // filter, but it doesn't die with more, it only shows the
    // first 5, so the last 3 filters won't show on MS-Windows.
    // I don't know if that is V or MS-Windows (BEW: 3/5/99)
    static char* filter[] =
      {
        "*.c;*.cpp;*.cxx;*.C;*.h;*.H;*.java;*.htm;*.html;*.txt",
        "*.c;*.cpp;*.cxx;*.h;*.hxx",
	"*.htm;*.html;*.java;*.js",
        "*.txt;*.htm;*.html",
        "*",
        "*.c;*.cpp;*.C",
	"*.h",
	"*.java",
        0
      };

// ###########################################################################

//====================>>> myAuxTimer::TimerTick <<<====================
  void dbAuxTimer::TimerTick()
  {
    cmdw->WindowCommand(cmdAuxTimer, cmdAuxTimer, C_Button); // update clock
  }

//====================>>> vdbCmdWindow::vdbCmdWindow <<<====================
  vdbCmdWindow::vdbCmdWindow(VCONST char* name, int width, int height) :
    vCmdWindow(name, width, height)
  {
    UserDebug1(Constructor,"vdbCmdWindow::vdbCmdWindow(%s) Constructor\n",name)

    // The Menu Bar
    vdbMenu = new vMenuPane(StandardMenu);
    AddPane(vdbMenu);

    // The Command Pane
    vdbCmdPane = new vCommandPane(ToolBar);
    AddPane(vdbCmdPane);

    // The Canvas
//    vdbCanvas = new vdbTextEditor(this);
    vdbCanvas = new vTextEditor(this);
    AddPane(vdbCanvas);

    // The Status Bar
    vdbStatus = new vStatusPane(StatBar);
    AddPane(vdbStatus);

    // timer to get db output

    auxTimer = new dbAuxTimer(this);	// create timer
    auxTimer->TimerSet(300);		// update 3 times a second

    // Show Window

    ShowWindow();

    vdbFont = ((videApp*)theApp)->DefaultFont;
    vdbCanvas->SetFont(vdbFont);
#ifdef V_VersionWindows
    vdbCanvas->SetTextRowsCols(height,width);
#endif

    vdbCanvas->ShowVScroll(1);	// Show Vert Scroll

    *dbCmd = 0;
    dbCmdPtr = dbCmd;		// build command in dbCmd
#ifdef V_VersionWindows
    dbProcessHandle = 0;
    dbProcessId = 0;
#endif

    dbOutLine[0] = 0;
    dbOutNext = 0;

    gdbDlg = 0;
    jdbDlg = 0;
  }

//====================>>> vdbCmdWindow::~vdbCmdWindow <<<====================
  vdbCmdWindow::~vdbCmdWindow()
  {
    UserDebug(Destructor,"vdbCmdWindow::~vdbCmdWindow() destructor\n")

    // Now put a delete for each new in the constructor.

    if (gdbDlg != 0)
      {
	gdbDlg->CloseDialog();
	delete gdbDlg;
      }

    if (jdbDlg != 0)
      {
	jdbDlg->CloseDialog();
	delete jdbDlg;
      }

    auxTimer->TimerStop();	// kill the timer first
    delete auxTimer;

    delete vdbMenu;
    delete vdbCanvas;
    delete vdbCmdPane;
    delete vdbStatus;

#ifdef V_VersionX
//    fclose(_inputfp);
//    fclose(_outputfp);
//    fclose(_errorfp);
    close(to_parent_error[pREAD]);
    close(to_parent[pREAD]);
    close(to_child[pWRITE]);
#endif

  }

//====================>>> vedCmdWindow::ChangeLoc <<<====================
  void vdbCmdWindow::ChangeLoc(long line, int col)
  {
#ifdef USETHISCODE
    char buff[20];
    char colbuff[10];
    char outbuff[20];
    int ll;

    LongToStr(line,buff);

    IntToStr(col,colbuff);

    // center the line/col string

    int totalLen = strlen(buff) + strlen(colbuff) + 1;

    int add = (9 - totalLen) / 2;

    for (ll = 0 ; ll < add ; ++ll)
	outbuff[ll] = ' ';
    outbuff[ll] = 0;

    strcat(outbuff,buff); strcat(outbuff,"/");
    strcat(outbuff,colbuff);

    // pad with trailing blanks so X doesn't shift things

    for (ll = strlen(outbuff) ; ll < 9 ; ++ll)
	outbuff[ll] = ' ';
    outbuff[ll] = 0;
    SetString(lblLineCol,outbuff);
#endif

  }

//====================>>> videCmdWindow::SetSens <<<====================
  void vdbCmdWindow::SetToolBar(ItemVal id, int indx, int val)
  {
  
    SetValueAll(id,val,Sensitive);
    ToolBar[indx].Sensitive = val;
  }

//====================>>> vdbCmdWindow::StatusMessage <<<====================
  void vdbCmdWindow::StatusMessage(char *msg)
  {
    char lineout[42];
    int ix;

    // copy up to 40 chars
    for (ix = 0 ; ix < 40 && msg[ix] ; ++ix)
	lineout[ix] = msg[ix];

    // pad with trailing blanks
    for ( ; ix < 40 ; ++ix)
	lineout[ix] = ' ';

    lineout[ix] = 0;

    SetString(lblEMsg,lineout);
  }

//====================>>> vdbCmdWindow::KeyIn <<<====================
  void vdbCmdWindow::KeyIn(vKey keysym, unsigned int shift)
  {
    
    if (keysym ==  vk_Return)		// send a command...
      {
	*dbCmdPtr++ = '\n';
	*dbCmdPtr = 0;
	dbCmdPtr = dbCmd;
	vdbCanvas->bufferBottom();
	vdbCanvas->charInsert('\n');
	dbSendCmd(dbCmd, 0);
      }
    else if (keysym == vk_BackSpace || keysym == vk_Delete)
      {
	if (dbCmdPtr != dbCmd)
	  {
	    --dbCmdPtr;
	    vdbCanvas->bufferBottom();
	    vdbCanvas->charDelete(-1);
	  }
	
      }
 // else if (vdbCanvas->EditKeyIn(keysym, shift) < 0)
    else if (keysym >= ' ' && keysym <= 127)
      {
        *dbCmdPtr++ = (char) keysym;	// add to command buffer
	vdbCanvas->bufferBottom();
        vdbCanvas->charInsert((char) keysym);
      }
    else
        vdbCanvas->EditKeyIn(keysym, shift);


  }

//====================>>> videCmdWindow::GotoDebugLine <<<====================
  videCmdWindow* vdbCmdWindow::GotoDebugLine(char* info)
  {
    // This is passed the gdb or jdb breakpoint line info.
    // Open that file, go to the line
    // The gdb info line is in the format:
    //     fullfilename:##:misc other stuff
    // The fullfile name CAN include a drive, and thus a :,
    // so must account for that...

    // The jdb infoline is in the format:
    //    Breakpoint hit: SimpleExample.main (SimpleExample:137)
    // This doesn't have drive info, so we must hope we are in
    // the same directory! Eventually, we might have to see if
    // we can use classpath...

    // First, get pointer to the error message window
    // so we can use the helper methods in videCmdWindow class

    videCmdWindow* cmdw = ((videApp*)theApp)->GetMsgWindow();

    // char errLine[100];

    // line number will be somewhere on line in format filename.c:##
    // error line in format "  ! filename.cpp:###:msg"

    char fullFileName[maxFileNameSize+2];
    char filePath[maxFileNameSize+2];
    char fileBase[maxFileNameSize+2];

    char cLineNum[20];

#ifdef V_VersionWindows
    // Windows version would prefer to have all \ for file names
    if (debugModel == dbGDB)
      {
	for (char* cp = info ; *cp ; ++cp)
	  {
	    if (*cp == '/')
		*cp = '\\';
	  }
      }
#endif

    if (debugModel == dbGDB)
      {
	// pick up the full file name
	fullFileName[0] = info[0];
	fullFileName[1] = info[1];	// These skip possible : in name
	int ifn;		// index to file name
	for (ifn = 2 ;
	    ifn < maxFileNameSize && info[ifn] != ':' && info[ifn] ;
	++ifn)
	fullFileName[ifn] = info[ifn];
	fullFileName[ifn] = 0;

	// fullFilename now has full file path

	// ifn points to the : before the line number

	char* np = &cLineNum[0];		// get the line number
	for ( ++ifn ; info[ifn] >= '0' && info[ifn] <= '9' ; ++ifn)
	    *np++ = info[ifn];
	*np = 0;

      }
    else if (debugModel == dbJDB)
      {
	//    Breakpoint hit: SimpleExample.main (SimpleExample:137)
	vOS vos;
	vos.vGetCWD(fullFileName,maxFileNameSize);
	strcat(fullFileName,"/");
	fileBase[0] = 0;
	// Check for java. breaks - hit on single steps, don't want
	// to see these usually, so just ignore.

	if (strstr(info,"hit: java.") != 0 ||
		strstr(info,"hit: sun.") != 0)
	  {
	    dbSendCmd("next\n");
	    return 0;
	  }

	for (int ix = 0 ; ix < dbOutMax ; ++ix)
	  {
	    if (info[ix] == '(')
	      {
		++ix;		// skip the (
		int ij;
		for (ij = 0 ; info[ix] && info[ix] != ':' ; )
		    fileBase[ij++] = info[ix++];
		fileBase[ij] = 0;
		strcat(fileBase,".java");	// add this!
		if (!info[ix])
		    break;
		ix++;		// skip the ':', and get the line num
		for (ij = 0 ; info[ix] && info[ix] != ')' ; )
		    cLineNum[ij++] = info[ix++];
		cLineNum[ij] = 0;
		break;
	      }
	  }
	strcat(fullFileName,fileBase);
      }
    else
	return 0;

    long lineNum = StrToLong(cLineNum);

    // get parts
    splitFileName(fullFileName,filePath,fileBase,0);

    
    // start by seeing if file is in the supplied full name
 

    videCmdWindow* openw = cmdw->FindCmdWin(fullFileName);

    if (openw == 0)		// nope, then try the base name only
      {
        openw = cmdw->FindCmdWin(fileBase);
      }

    if (openw != 0)		// EASY, already open
      {
        (openw->GetTextEd())->EditCommand(edLineGoto, lineNum);
        openw->setCurPC(lineNum);	// set PC to current line
        (openw->GetTextEd())->EditCommand(edCenterScreen, 0);
	RaiseWindow();		// first, raise myself
	openw->RaiseWindow();	// then, the target
        return openw;
      }

    // Not already open, so need to open a new window.

    openw = (videCmdWindow*)
                    theApp->NewAppWin(0,"V Text Editor", 90, 18);
    if (!openw)
        return 0;	       // should never happen...

    if (!openw->OpenFile(fullFileName,0,0))
      {
      	// hmmm - the file doesn't exist on Makefile path
        // try something else
        vFileSelect fsel(this);     // make an instance
        if (!fsel.FileSelect("Debug: Open Source File",fileBase,maxFileNameSize,
             filter,filterIndex))
          {
//            openw->CloseWin();
            theApp->CloseAppWin(openw);
            return 0;
          }
        if (!openw->OpenFile(fileBase,0,0))
	  {
//              openw->CloseWin();
            theApp->CloseAppWin(openw);
	    return 0;
          }
        (openw->GetTextEd())->EditCommand(edLineGoto, lineNum);
        openw->setCurPC(lineNum);	// set PC to current line
        (openw->GetTextEd())->EditCommand(edCenterScreen, 0);
      }
    else
      {
        (openw->GetTextEd())->EditCommand(edLineGoto, lineNum);
        openw->setCurPC(lineNum);	// set PC to current line
        (openw->GetTextEd())->EditCommand(edCenterScreen, 0);
      }
    RaiseWindow();		// first, raise myself
    openw->RaiseWindow();	// then, the target
    return openw;
  }

//====================>>> vdbCmdWindow::doBreakCmd <<<====================
  void vdbCmdWindow::doBreakCmd(videCmdWindow* win, char* bcmd)
  {
    long curlin = (win->GetTextEd())->GetCurLine();
    char *fileName = win->GetFileName();
    char fileBase[maxFileNameSize+2];
    char cmd[maxFileNameSize+2];
    char lNum[20];

    splitFileName(fileName,0,fileBase,0);
    strcpy(cmd,bcmd);

    if (debugModel == dbJDB)	// need to strip the .java
      {
	int ix = strlen(fileBase);
	if (ix > 5 && fileBase[ix-5] == '.')
	    fileBase[ix-5] = 0;
      }

    strcat(cmd,fileBase);
    strcat(cmd,":");
    LongToStr(curlin, lNum);
    strcat(cmd,lNum);
    strcat(cmd,"\n");
    dbSendCmd(cmd);
  }


//====================>>> vdbCmdWindow::breakCmd <<<====================
  void vdbCmdWindow::breakCmd(char* fname, long line, ItemVal id)
  {
    char fileBase[maxFileNameSize+2];
    char cmd[maxFileNameSize+2];
    char lNum[20];

    // get file base name
    splitFileName(fname,0,fileBase,0);
    if (debugModel == dbJDB)	// need to strip the .java
      {
	int ix = strlen(fileBase);
	if (ix > 5 && fileBase[ix-5] == '.')
	    fileBase[ix-5] = 0;
      }


    // build the first part of the command
    if (debugModel == dbGDB)
      {
	switch (id)
	  {
	    case m_SetBP:
	      {
		strcpy(cmd, "break ");
		break;
	      }

	    case m_DeleteBP:
	      {
		strcpy(cmd, "clear ");
		break;
	      }

	    case m_ShowBreakpoints:
	      {
		dbSendCmd("info breakpoints\n");
		return;
	      }

	    case m_DeleteBreakpoints:
	      {
		dbSendCmd("delete breakpoints\n");
		return;
	      }

	    default:
		return;
          }
      }
    else			// JDB commands....
      {
	switch (id)
	  {
	    case m_SetBP:
	      {
		strcpy(cmd, "stop at ");
		break;
	      }

	    case m_DeleteBP:
	      {
		strcpy(cmd, "clear ");
		break;
	      }

	    case m_ShowBreakpoints:
	      {
		dbSendCmd("clear\n");
		return;
	      }

	    default:
		return;
          }
      }

    // have the command, now add a file name and line number
    strcat(cmd,fileBase);
    strcat(cmd,":");
    LongToStr(line, lNum);
    strcat(cmd,lNum);
    strcat(cmd,"\n");
    dbSendCmd(cmd);

  }

//====================>>> vdbCmdWindow::debugCmd <<<====================
  void vdbCmdWindow::debugCmd(videCmdWindow* win, ItemVal id)
  {
    if (debugModel == dbGDB)
      {
	switch (id)
	  {
	    case m_StepOver:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("next\n");
		break;
	      }

	    case m_StepInto:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("step\n");
		break;
	      }

	    case m_CallStack:
	      {
		dbSendCmd("bt\n");
		break;
	      }

	    case m_Inspect:
	    case m_InspectInd:
	      {
		char varName[82];
		int res = (win->GetTextEd())->getSelection(varName, 80);
		if (!res || strlen(varName) < 1)
		  {
		    vNoticeDialog note(this);
		    note.Notice("Highlight variable to inspect first!");
		    break;
		  }
		char cmd[90];
		if (id == m_Inspect)
		    strcpy(cmd,"print ");
		else
		    strcpy(cmd,"print* ");
		strcat(cmd,varName);
		strcat(cmd,"\n");
		dbSendCmd(cmd);
		break;
	      }

	    case m_Up:
	      {
		dbSendCmd("up\n");
		break;
	      }

	    case m_Down:
	      {
		dbSendCmd("down\n");
		break;
	      }

	    case m_Args:
	      {
		dbSendCmd("info args\n");
		break;
	      }

	    case m_Locals:
	      {
		dbSendCmd("info locals\n");
		break;
	      }

	    case m_Continue:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("continue\n");
		break;
	      }

	    case m_Kill:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("kill\n");
		break;
	      }

	    case m_DBRun:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("run\n");
		break;
	      }

	    case m_SetArgs:
	      {
		break;
	      }

	    case m_RunTo:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		doBreakCmd(win,"until ");
		break;
	      }

	    case m_DBHelp:
	      {
		dbSendCmd("help\n");
		break;
	      }

	    default:
	      {
		break;
	      }
	  }
      }
    else			// JDB commands....
      {
	switch (id)
	  {
	    case m_StepOver:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("next\n");
		break;
	      }

	    case m_StepInto:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("step\n");
		break;
	      }

	    case m_CallStack:
	      {
		dbSendCmd("where\n");
		break;
	      }

	    case m_Inspect:
	      {
		char varName[82];
		int res = (win->GetTextEd())->getSelection(varName, 80);
		if (!res || strlen(varName) < 1)
		  {
		    vNoticeDialog note(this);
		    note.Notice("Highlight variable to inspect first!");
		    break;
		  }
		char cmd[90];
		strcpy(cmd,"print ");
		strcat(cmd,varName);
		strcat(cmd,"\n");
		dbSendCmd(cmd);
		break;
	      }

	    case m_Continue:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("cont\n");
		break;
	      }

	    case m_Kill:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("suspend\n");
		break;
	      }

	    case m_DBRun:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("run\n");
		break;
	      }

	    case m_SetArgs:
	      {
		vBeep();
		break;
	      }

	    case m_DBHelp:
	      {
		dbSendCmd("help\n");
		break;
	      }

	    case m_jdbLocals:
	      {
		dbSendCmd("locals\n");
		break;
	      }

	    case m_jdbClasses:
	      {
		dbSendCmd("classes\n");
		break;
	      }

	    case m_jdbStepUp:
	      {
		theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp
		dbSendCmd("step up\n");
		break;
	      }

	    case m_jdbMemory:
	      {
		dbSendCmd("memory\n");
		break;
	      }

	    default:
	      {
		break;
	      }

	  }
      }

  }

//====================>>> vdbCmdWindow::WindowCommand <<<====================
  void vdbCmdWindow::WindowCommand(ItemVal id, ItemVal val, CmdType cType)
  {
    // Default: route menu and toolbar commands here


    UserDebug1(CmdEvents,"vdbCmdWindow:WindowCommand(%d)\n",id)

    // First, check for all debug commands so we can route them
    // through one place.
    switch (id)
      {
	case cmdAuxTimer:	// when timer ticks
	  {
	    dbShowOut();	// show any output
	    break;
	  }

	default:		// route unhandled commands
	  {
	    if (((videApp*)theApp)->getCurCmdWin() != 0)
		(((videApp*)theApp)->getCurCmdWin())->WindowCommand(id, val, cType);
	    else  if (((videApp*)theApp)->GetMsgWindow() != 0)
		(((videApp*)theApp)->GetMsgWindow())->WindowCommand(id, val, cType);
	    else
		vCmdWindow::WindowCommand(id, val, cType);
	    break;
	  }
      }
  }

//====================>>> vdbCmdWindow::OpenDebugger <<<====================
  bool vdbCmdWindow::OpenDebugger(const char* debugger, const char* exe)
  {
#ifdef V_VersionWindows
    char cmd[maxFileNameSize+20];

    if (strcmp(debugger,"gdb") == 0)
      {
	debugModel = dbGDB;
	strcpy(cmd, debugger);
	strcat(cmd," -f");		// emacs output format from gdb
      }
    else if (strcmp(debugger,"jdb") == 0 || strstr(debugger,"appletviewer") != 0)
      {
	strcpy(cmd, debugger);
	debugModel = dbJDB;
      }
    else			// not one we know about
      {
	debugModel = dbNone;
	return false;
      }

    dbOutLine[0] = 0;
    dbOutNext = 0;

    if (exe && *exe)
      {
	strcat(cmd," ");
	strcat(cmd,exe);
      }
    
    SECURITY_ATTRIBUTES saAttr; 
    BOOL fSuccess; 

    /* Set the bInheritHandle flag so pipe handles are inherited. */ 

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
 
    /* 
     * The steps for redirecting child's STDOUT: 
     *     1.  Save current STDOUT, to be restored later. 
     *     2.  Create anonymous pipe to be STDOUT for child. 
     *     3.  Set STDOUT of parent to be write handle of pipe, so 
     *         it is inherited by child.
     */ 
 
    /* Save the handle to the current STDOUT. */ 

    hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);
 
    /* Create a pipe for the child's STDOUT. */ 
 
    if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
        return 0;	// ErrorExit("Stdout pipe creation failed\n");
 
    /* Set a write handle to the pipe to be STDOUT. */ 

    if (! SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr))
        return 0;	// ErrorExit("Redirecting STDOUT failed"); 
 
    /* 
     * The steps for redirecting child's STDIN: 
     *     1.  Save current STDIN, to be restored later. 
     *     2.  Create anonymous pipe to be STDIN for child. 
     *     3.  Set STDIN of parent to be read handle of pipe, so 
     *         it is inherited by child. 
     *     4.  Create a noninheritable duplicate of write handle, 
     *         and close the inheritable write handle. 
     */ 
 
    /* Save the handle to the current STDIN. */ 

    hSaveStdin = GetStdHandle(STD_INPUT_HANDLE);
 
    /* Create a pipe for the child's STDIN. */ 
 
    if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) 
        return 0;	// ErrorExit("Stdin pipe creation failed\n");

    /* Set a read handle to the pipe to be STDIN. */ 
 
    if (! SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd)) 
        return 0;	// ErrorExit("Redirecting Stdin failed"); 
 
    /* Duplicate the write handle to the pipe so it is not inherited. */
 
    fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr, 
        GetCurrentProcess(), &hChildStdinWrDup, 0, 
        TRUE,       /* is inherited */ 
        DUPLICATE_SAME_ACCESS); 

    if (! fSuccess) 
        return 0;	// ErrorExit("DuplicateHandle failed"); 
 
    CloseHandle(hChildStdinWr); 
 
    /* Now create the child process. */


    static PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo; 

    /* Set up members of STARTUPINFO structure. */ 

    ZeroMemory(&siStartInfo, sizeof(siStartInfo));
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.lpReserved = NULL; 
    siStartInfo.lpReserved2 = NULL; 
    siStartInfo.cbReserved2 = 0; 
    siStartInfo.lpDesktop = NULL;
    siStartInfo.wShowWindow = SW_MINIMIZE;
//    siStartInfo.wShowWindow = SW_SHOWNORMAL;
    siStartInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    siStartInfo.hStdOutput = hChildStdoutWr;
    siStartInfo.hStdError = hChildStdoutWr;
    siStartInfo.hStdInput = hChildStdinRd;

    /* Create the child process. */
    OSVERSIONINFO osver;
    osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    int createResult;

    vdbCanvas->addLine(cmd); vdbCanvas->addLine(" - be patient...\n");
    vdbCanvas->bufferBottom();
    ::GetVersionEx(&osver);
    if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT)
      {
	createResult = CreateProcess(NULL,
		cmd,
		NULL,          // process security attributes
		NULL,          // primary thread security attributes
		TRUE,          // handles are inherited
		CREATE_NEW_PROCESS_GROUP,   /* creation flags                     */
//	/* CREATE_NEW_CONSOLE |*/ CREATE_NEW_PROCESS_GROUP,
		NULL,          /* use parent's environment           */
		NULL,          /* use parent's current directory     */ 

		&siStartInfo,  /* STARTUPINFO pointer                */ 
		&piProcInfo);  /* receives PROCESS_INFORMATION       */
      }
    else		// Win9x version
      {
	createResult = CreateProcess(NULL,
		cmd,
		NULL,          /* process security attributes        */ 
		NULL,          /* primary thread security attributes */ 
		TRUE,          /* handles are inherited              */
		0,             /* creation flags                     */
// maybe:	CREATE_NEW_CONSOLE,
		NULL,          /* use parent's environment           */
		NULL,          /* use parent's current directory     */ 

		&siStartInfo,  /* STARTUPINFO pointer                */ 
		&piProcInfo);  /* receives PROCESS_INFORMATION       */
      }

    if (!createResult)
      {
	static DWORD errorCode = ::GetLastError();
	if (errorCode != 0)
	  {
	    vdbCanvas->addLine("CreateProcess Fails....");
	    vdbCanvas->addLine(cmd);
	    vdbCanvas->bufferBottom();
	  }

	return 0;	// create fails
      }


    dbProcessId = piProcInfo.dwProcessId;	// save these for termination
    dbProcessHandle = piProcInfo.hProcess;

    /* After process creation, restore the saved STDIN and STDOUT. */

    if (! SetStdHandle(STD_INPUT_HANDLE, hSaveStdin))
        return 0;	// ErrorExit("Re-redirecting Stdin failed\n");

    if (! SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))
        return 0;	// ErrorExit("Re-redirecting Stdout failed\n");
#endif

//  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X

#ifdef V_VersionX
    char cmd[maxFileNameSize+20];

    strcpy(cmd,"exec ");
    if (strcmp(debugger,"gdb") == 0)
      {
	debugModel = dbGDB;
	strcat(cmd, debugger);
	strcat(cmd," -f");		// emacs output format from gdb
      }
    else if (strcmp(debugger,"jdb") == 0 || strstr(debugger,"appletviewer") != 0)
      {
	strcat(cmd, debugger);
	debugModel = dbJDB;
      }
    else			// not one we know about
      {
	debugModel = dbNone;
	return false;
      }

    dbOutLine[0] = 0;
    dbOutNext = 0;

    if (exe && *exe)
      {
	strcat(cmd," ");
	strcat(cmd,exe);
      }
    

#ifdef X_WORKS
    // 1. setup parent side communications
    //    open pipes for stdin, stdout, and stderr
     if (pipe(to_child) < 0)
       {
	 return 0;
       }
    if (pipe(to_parent) < 0)
       {
	 return 0;
       }
    if (pipe(to_parent_error) < 0)
       {
	 return 0;
       }
    // 2. start child process

    if ((_pid = fork()) == 0)		// in child
      {
	// 2a. set up child communications
	//    close unused pipe ends
	close(to_child[pWRITE]);
	close(to_parent[pREAD]);
	close(to_parent_error[pREAD]);

	// assign stdin to to_child, stdout to to_parent, and
	// stderr to to_parent_error
	dup2(to_child[pREAD], fileno(stdin));
	close(to_child[pREAD]);

	dup2(to_parent[pWRITE], fileno(stdout));
	close(to_parent[pWRITE]);

	dup2(to_parent_error[pWRITE], fileno(stderr));
	close(to_parent_error[pWRITE]);

	// 2b. Now, execute child (we are in child fork...)
	execl("/bin/sh", "sh", "-c", cmd, (char *)0);

	// could not find child: send message to parent via stderr
	perror("/bin/sh");
	_exit(99);
      }

    if (_pid == -1)		// failed
      {
	_pid = 0;
	return 0;
      }

    // OK, fork worked, and we are the parent
    // 3. setup parent communication

    // I am the parent: close unused pipe ends
    close(to_child[pREAD]);
    close(to_parent[pWRITE]);
    close(to_parent_error[pWRITE]);

    // access remaining pipe ends via file descriptor I/O
    // to_parent_error[pREAD] is stderr from child
    // to_parant[pREAD] is stdout from child
    // to_child[pWRITE] is stdin to child.
    fcntl(to_parent[pREAD],F_SETFL,O_NONBLOCK);		// non block on input from child


    // access remaining pipe ends via stream I/O
    // using error, in and out...
//    _errorfp = fdopen(to_parent_error[pREAD], "r");
//    if (_errorfp == NULL)
//	return 0;

//    _inputfp = fdopen(to_parent[pREAD], "r");
//    if (_inputfp == NULL)
//	return 0;

//    _outputfp = fdopen(to_child[pWRITE], "w");
//    if (_outputfp == NULL)
//	return 0;

#endif
#endif

    switch (debugModel)
      {
	case dbGDB:
	  {
	    if (!gdbDlg)
		gdbDlg = new gdbDialog(this, "gdb");
	    gdbDlg->ShowDialog("gdb");
	    int l,t,w,h;
	    GetPosition(l,t,w,h);   // my position
	    gdbDlg->SetDialogPosition(l+w+2,t);   // put quick pick here
	    break;
	  }
	    
	case dbJDB:
	  {
	    if (!gdbDlg)
		jdbDlg = new jdbDialog(this, "Java jdb"); // @@@
	    jdbDlg->ShowDialog("jdb");
	    int l,t,w,h;
	    GetPosition(l,t,w,h);   // my position
	    jdbDlg->SetDialogPosition(l+w+2,t);   // put quick pick here
	    break;
	  }
	    
      }

    // now, send the bps we already have available

    ((videApp*)theApp)->getBreakPoints()->sendForAllBreakPoints(m_SetBP);
    return 1;

  }

//====================>>> vdbCmdWindow::dbSendCmd <<<====================
  void vdbCmdWindow::dbSendCmd(char* cmd, int echo)
  {
    if (echo)
      {
         vdbCanvas->bufferBottom();
         for (char* cp = cmd ; *cp && *cp != '\n' ; ++cp)
             vdbCanvas->charInsert(*cp);
         vdbCanvas->charInsert('\n');
      }
    else
	theApp->UpdateAllViews(this, ChangePC, 0);	// fix bp

#ifdef V_VersionWindows
    DWORD written;

    if (!WriteFile(hChildStdinWrDup, cmd, strlen(cmd), &written, 0))
        return;
    // output will be displayed by dbShowOut...
#endif
#ifdef V_VersionX
    return;			// @@@
//    (void) write(to_child[pWRITE],cmd,(size_t)strlen(cmd));
#endif

  }

//====================>>> vdbCmdWindow::dbShowOut <<<====================
  bool vdbCmdWindow::dbShowOut(bool atBottom)
  {
    // This routine checks for output from the debugger
    // This is MS-Win32 specific...

#ifdef V_VersionWindows
    DWORD bytesRead;
    DWORD bytesAvail;
#endif
#ifdef V_VersionX
    ssize_t bytesRead;
	return false;	//@@@@
#endif

    char chrin[10] = "";

    bool first = true;
    static int colOut = 0;		// current output column

    for ( ; ; )		// read all debugger output available
      {
	// first, see if any output is available...
#ifdef V_VersionWindows
	if (!PeekNamedPipe(hChildStdoutRd,
	    chrin, (DWORD)1, &bytesRead, &bytesAvail, 0))
	  {
	    return false;
	  }
	if (bytesAvail == 0)	// no output avail, so return
	    return false;

	if (!ReadFile(hChildStdoutRd,	// now really read it
	    chrin, (DWORD)1, &bytesRead, 0) ) return false;

#endif
#ifdef V_VersionX
	bytesRead = read(to_parent[pREAD],chrin,(size_t)1);
#endif
	if (bytesRead == 0)		// just in case
	    return false;

	if (first)			// buffer bottom once...
	  {
	    first = false;
	    if (!atBottom)
		vdbCanvas->bufferBottom();		// add at bottom
	  }

	// add the character to the display
	if (*chrin == '\032')		// prefix from gdb for break
	  {
	    // should be 2 \032's in a row. wait for rest of line...
#ifdef V_VersionWindows
	    if (!ReadFile(hChildStdoutRd,	// now really read it
		chrin, (DWORD)1, &bytesRead, 0) ) return false;
#endif
#ifdef V_VersionX
	    bytesRead = read(to_parent[pREAD],chrin,(size_t)1);
#endif
	    if (bytesRead == 0)		// just in case
		return false;
	    if (*chrin != '\32')
		continue;
	    // ok - special formatted breakpoint line coming...
	    // read it all
	    char info[maxFileNameSize+2];
	    char* cp = info;
	    for ( ; ; )
	      {
		videCmdWindow* cw;
#ifdef V_VersionWindows
		if (!ReadFile(hChildStdoutRd,	// now really read it
		    chrin, (DWORD)1, &bytesRead, 0) ) return false;
#endif
#ifdef V_VersionX
		bytesRead = read(to_parent[pREAD],chrin,(size_t)1);
#endif
		if (bytesRead == 0)		// just in case
		    return false;
		if (*chrin == '\n')
		  {
		    *cp = 0;
                    for (char* lp = info ; *lp ; ++lp)
                        vdbCanvas->charInsert(*lp, /*doundo*/ false);
                    vdbCanvas->charInsert('\n', /*doundo*/ false);
		    cw = GotoDebugLine(info);
		    dbOutNext = 0;
                    return true;
		  }
		else
		    *cp++ = *chrin;

		theApp->CheckEvents();
	      }
	    dbOutNext = 0;
	    return true;
	  }
	else if (*chrin != '\r')
	  {
	    if (*chrin == '\n')
	      {
		dbOutLine[dbOutNext] = 0;
		dbOutNext = 0;
		colOut = 0;
		vdbCanvas->charInsert(*chrin, /*doundo*/ false);
		if (debugModel == dbJDB)	// need to check for breakpoints
		  {
		    if (strstr(dbOutLine,"Breakpoint hit:") == dbOutLine)
		      {
			videCmdWindow* cw;
			cw = GotoDebugLine(dbOutLine);
		      }
		  }
	      }
	    else 
	      {
		if (colOut++ >= vdbCanvas->GetCols())
		  {
		    colOut = 3;
		    vdbCanvas->charInsert('\n', /*doundo*/ false);
		    vdbCanvas->charInsert(' ', /*doundo*/ false);
		    vdbCanvas->charInsert('+', /*doundo*/ false);
		    vdbCanvas->charInsert(*chrin, /*doundo*/ false);
		    dbOutNext = 0;
		  }
		if (dbOutNext < dbOutMax)	// save out line to check for jdb breaks
		    dbOutLine[dbOutNext++] = *chrin;
		vdbCanvas->charInsert(*chrin, /*doundo*/ false);
	      }
	  }
      }
    return true;
  }
