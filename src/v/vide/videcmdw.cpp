//=======================================================================
//	videCmdw.cpp:	Source for videCmdwindow class
//  Copyright (C) 1995-1999  Bruce E. Wampler
//	date: 05 Mar 2000 03:22:18 PM
//
//  This program is part of the V C++ GUI Framework example programs.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  (see COPYING) along with this program; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=======================================================================

/* @@ TO DO:
 * grep
 * right click stuff
 * emacs
 * templates
 *
 */
#include <v/vnotice.h>	// for vNoticeDialog
#include <v/vkeys.h>	// to map keys
#include <v/vfilesel.h>
#include <v/vfontsel.h>
#include <v/vutil.h>
#include <v/vynreply.h>
#include <v/vreply.h>
#include <v/vicon.h>

#include <sys/stat.h>
#ifdef V_VersionX
#include <stdlib.h>
#endif

#include "videapp.h"	// main header for some global defs

#include "videcmdw.h"	// our header

#include "vrundlg.h"
#include "vseeci.h"	// command interp for See
#include "vstdci.h"	// command interp for Standard editor
#include "vvici.h"	// command interp for Vi
#include "abtvdlg.h"
#include "prefmdlg.h"
#include "cnewmdlg.h"	// new c project wizard

#include "vcmdcmds.h"	// the menu bar, tool bar, status bar definitions

    static int filterIndex = 0;
    // The MS-Windows filter seems to hold only 5 entries for the
    // filter, but it doesn't die with more, it only shows the
    // first 5, so the last 3 filters won't show on MS-Windows.
    // I don't know if that is V or MS-Windows (BEW: 3/5/99)
    static char* filter[] =
      {
#ifdef V_VersionX
        "*.c;*.C;*.cpp;*.cxx;*.h;*.hxx;*.java;*.htm;*.html;*.txt",
        "*.c;*.C;*.cpp;*.cxx;*.h;*.hxx",
	"*.java;*.htm;*.html",
        "*.txt;*.htm;*.html,*.tex",
        "*",
        "*.c;*.C;*.cpp",
	"*.h;*.hxx",
	"*.java",
	"*.pl",
	"*.for;*.f;*.FOR;*.F",
#else
        "*.c;*.cpp;*.cxx;*.h;*.hxx;*.java;*.htm;*.html;*.txt",
        "*.c;*.cpp;*.cxx;*.h;*.hxx",
	"*.java;*.pl;*.for;*.f",
        "*.txt;*.htm;*.html;*.tex",
        "*",
#endif
        0
      };

    static char makebName[maxFileNameSize+2] = "make";
    char videCmdWindow::prjName[maxFileNameSize+2] = "";
    ProjectType videCmdWindow::prjType = NoProject;
    int videCmdWindow::cmdWinCount = 0;
    makefileMaker* videCmdWindow::mm;
    videJava* videCmdWindow::ideJava;


//====================>>> videCmdWindow::getSens <<<====================
  int videCmdWindow::getSens(ItemVal id)
  {
  
    // This is flakey... Somehow or another, V got designed with
    // no way to find out the current sensitivity of tool bar items.
    // Instead you get the CHECKED state of the corresponding
    // MENU item. So we will count on proper use of SetSens to
    // be sure the static ToolBar has the correct sensitivity.
    // Sigh... it is so hard to get everything right. (bew - 4/23/99)

    int ix = vGetcmdIdIndex(id, &ToolBar[0]);
    if (ix >= 0)
	return ToolBar[ix].Sensitive;
    return 0;
  }

//====================>>> videCmdWindow::SetSens <<<====================
  void videCmdWindow::SetSens(ItemVal id, int val)
  {
  
    SetValueAll(id,val,Sensitive);
    int ix = vGetcmdIdIndex(id, &ToolBar[0]);
    if (ix >= 0)
      {
	ToolBar[ix].Sensitive = val;
	// kludgy, but need to also set these sensitive in the debug window
	// if it is open.
	if (((videApp*)theApp)->getvdbCmdWin() != 0)
	    (((videApp*)theApp)->getvdbCmdWin())->SetToolBar(id, ix,val);

      }
  }

//====================>>> videCmdWindow::videCmdWindow <<<====================
  videCmdWindow::videCmdWindow(VCONST char* name, int width, int height) :
    vCmdWindow(name, width,  height)
  {
    // local inits

    fileName[0] = 0;
  
    // The Menu Bar
    videMenu = new vMenuPane(StandardMenu);
    AddPane(videMenu);

    // The Command Pane
    videCmdPane = new vCommandPane(ToolBar);
    AddPane(videCmdPane);

    // The Canvas
    vedCanvas = new vedTextEditor(this);
    AddPane(vedCanvas);

    if (((videApp*)theApp)->GetEmulation() == See)
      {
        vedCI = (vTextEdCmdInterp*) new vSeeCI(vedCanvas, this); // override interp
    					// ed canvas will do delete at end
        vedCanvas->ChangeCmdInterp(vedCI);
      }
    else if (((videApp*)theApp)->GetEmulation() == Vi)
      {
        vedCI = (vTextEdCmdInterp*) new vViCI(vedCanvas, this); // override interp
    					// ed canvas will do delete at end
        vedCanvas->ChangeCmdInterp(vedCI);
      }
    else if (((videApp*)theApp)->GetEmulation() == Generic)
      {
        vedCI = (vTextEdCmdInterp*) new vStandardCI(vedCanvas, this); // override interp
    					// ed canvas will do delete at end
        vedCanvas->ChangeCmdInterp(vedCI);
      }
    else
        vedCI = 0;


    // The Status Bar
    videStatus = new vStatusPane(StatBar);
    AddPane(videStatus);

    // Associated dialogs

    // Other stuff

    ideMake = new videMake();
    if (++cmdWinCount == 1)	// first time instance of videCmdWindow?
      {
	mm = new makefileMaker();
	ideJava = new videJava(this);
      }

    // Show Window

    ShowWindow();

    vedFont = ((videApp*)theApp)->DefaultFont;

    vedCanvas->SetFont(vedFont);

    if (width <= 0)
	width = ((videApp*)theApp)->getDefaultCols();
    if (height <= 0)
	height = ((videApp*)theApp)->getDefaultRows();
    if ( ( ((videApp*)theApp)->GetMsgWindow()) == 0)
      {
	height = 10;
	width += 10;
      }
    vedCanvas->SetTextRowsCols(height,width);


    (vedCanvas->GetCmdInterp())->InitCmdInterp();

    vedCanvas->setColorScheme();

    // setup make command
#ifdef V_VersionWindows
    if (*((videApp*)theApp)->getBCCPath())
      {
	strcpy(makebName,((videApp*)theApp)->getBCCPath());
	strcat(makebName,"\\bin\\make");
      }
    else
	strcpy(makebName,"make");
#endif

  }

//====================>>> videCmdWindow::~videCmdWindow <<<====================
  videCmdWindow::~videCmdWindow()
  {
    // Now put a delete for each new in the constructor.


    delete videMenu;
    delete vedCanvas;
    delete videCmdPane;
    delete videStatus;
    delete ideMake;
    if (--cmdWinCount == 0)
      {
	delete mm;
	delete ideJava;
      }
  }

//====================>>> videCmdWindow::ChangeLoc <<<====================
  void videCmdWindow::ChangeLoc(long line, int col)
  {
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

  }

//=========================>>> videCmdWindow::UpdateView <<<===========================
  void videCmdWindow::UpdateView(vWindow* sender, int hint, void* pHint)
  {
    switch (hint)
      {
	case ChangeFont:	// user changed prefs - so reset
	  {
	    int oldh = vedCanvas->GetRows();
	    int oldw = vedCanvas->GetCols();
	    vedFont = ((videApp*)theApp)->DefaultFont;
	    vedCanvas->SetFont(vedFont);
	    vedCanvas->SetTextRowsCols(oldh,oldw);
	    break;
	  }

	case ChangePrefs:
	  {
	    vedCanvas->setColorScheme();
	    vedCanvas->Verify();
	    break;
	  }

	case ChangePC:		// debugger changed current PC line
	    // only verify if this is the file with the breakpoint.
	    if (strcmp( ((videApp*)theApp)->getBreakPoints()->getPCFileName(),
			fileName) == 0)
	      {
		((videApp*)theApp)->getBreakPoints()->setCurPC("",-1);
		vedCanvas->Verify();
	      }
	    break;

	default:
	    break;
      }
  }

//=========================>>> videCmdWindow::setCurPC <<<===========================
  void videCmdWindow::setCurPC(long lineNum)
  {
     // used to set current line after a breakpoint is hit
     //@@@@ Add to status line, too!
    ((videApp*)theApp)->getBreakPoints()->setCurPC(fileName,lineNum);
    
  }

//=========================>>> videCmdWindow::FindCmdWin <<<===========================
  videCmdWindow* videCmdWindow::FindCmdWin(char* fname)
  {
    // Find a command window by file name
   videCmdWindow* clist;
  
    // try to match full name first
    for (clist = (videCmdWindow*)GetCmdWinList() ;
         clist != 0 ; clist = (videCmdWindow*)clist->GetNextCmdWin())
      {
        char* fp = clist->GetFileName();
        if (strcmp(fname, fp) == 0)
            return clist;
      }

    // now just match file, not path
    for (clist = (videCmdWindow*)GetCmdWinList() ;
         clist != 0 ; clist = (videCmdWindow*)clist->GetNextCmdWin())
      {
	char* gf = clist->GetFileName();
        char* cp = strstr(gf, fname);	// substring?
        if (cp != 0)
          {
	    // make sure really matches
            if (strcmp(cp,fname) == 0 &&	// substring match
		(gf == cp || *(cp-1) == '/' || *(cp-1) == '\\'))
                return clist;
          }
      }
    return 0;
  }

//=========================>>> videCmdWindow::checkIfAnyChanged <<<===========================
  bool videCmdWindow::checkIfAnyChanged()
  {
    // return true if any edit window has changed
    videCmdWindow* clist;
  
    // scan all windows
    for (clist = (videCmdWindow*)GetCmdWinList() ;
         clist != 0 ; clist = (videCmdWindow*)clist->GetNextCmdWin())
      {
	if (((videApp*)theApp)->GetMsgWindow() == clist) // skip msg win
	    continue;
	if ((void*)((videApp*)theApp)->getvdbCmdWin() == (void*)clist) // skip dbg
	    continue;
	if ((clist->vedCanvas)->Changed())    // changes have been made
	    return true;
      }

    return false;
  }

//====================>>> videCmdWindow::lookupCtag <<<====================
  void videCmdWindow::lookupCtag(char* symbol)
  {
    char buff[256];
    char* sym;
    char* ct;
    videCmdWindow* cmdw = ((videApp*)theApp)->GetMsgWindow();
  
    if (!symbol || !*symbol || *symbol == ' ')
	return;
    // prepare window for addlines
    cmdw->RaiseWindow();
    cmdw->SetRdOnly(0);
    (cmdw->GetTextEd())->SetFileType(gccError);

    strcpy(buff,"> Symbol: "); strcat(buff,symbol);
    cmdw->AddLine(buff);

    ifstream inFile("tags");

    if (!inFile)
      {
	cmdw->AddLine("  ! No tags file found. Try Build->Generate ctags first!");
	return; 		// file not there
      }

    int done = 0; int found = 0;
    while (inFile.getline(buff,255) && !done)
      {
	for (sym = symbol, ct = buff ; *sym ; ++sym, ++ct)
	  {
	    // this code should optimize by quitting when have
	    // gone too far in the file, but it seems to make little
	    // or no difference...
//	    if (*ct > *sym)
//	      {
//		done = 1;
//		break;
//	      }
	    if (*sym != *ct)
		break;
	  }
	if (!*sym && *ct == '\t')	// Match!
	  {
	    found = 1;
	    // format the line a little nicely
	    //    we will make it look like a gcc error line "  ! etc"
	    //    !  filename:linenumber: whatever
	    char* cp;
	    char fmt[255] = "  ! ";
	    char* fp = &fmt[4];		// after the "  ! "

	    for (cp = buff ; *cp != '\t' ; ++cp)	// skip symbol name
	      ;
	    ++cp;			// skip the tab
	     
	    while (*cp != '\t')		// copy the file name
		*fp++ = *cp++;
	    ++cp;			// skip the tab

	    *fp++ = ':';		// : separates file, line number
	    while (*cp != ';')		// copy the line number
		*fp++ = *cp++;
	    ++cp;			// skip the ;
	    ++cp;			// skip the "
	    *fp++ = ':';		// trailing :
	    
	    *fp = 0;			// terminate string

	    if (*cp == '\t')		// more info avaiable
	      {
		*fp++ = *cp++;		// copy tab to formatted line
		*fp = 0;			// terminate string

		// next char is a code for a C++ or java type

	    	if (vedCanvas->GetFileType() == CPP)
		  {
		    switch (*cp)
		      {
			case 'c':
			    strcat(fmt,"class");
			    break;
			case 'd':
			    strcat(fmt,"defines");
			    break;
			case 'e':
			    strcat(fmt,"enum");
			    break;
			case 'f':
			    strcat(fmt,"func");
			    break;
			case 'g':
			    strcat(fmt,"enum name");
			    break;
			case 'm':
			    strcat(fmt,"member");
			    break;
			case 'n':
			    strcat(fmt,"namespace");
			    break;
			case 'p':
			    strcat(fmt,"prototype");
			    break;
			case 's':
			    strcat(fmt,"struct");
			    break;
			case 't':
			    strcat(fmt,"typedef");
			    break;
			case 'u':
			    strcat(fmt,"union");
			    break;
			case 'v':
			    strcat(fmt,"var");
			    break;
			case 'x':
			    strcat(fmt,"extern");
			    break;
			case 'A':
			    strcat(fmt,"access");
			    break;
			case 'C':
			    strcat(fmt,"class");
			    break;
			default:
			    strcat(fmt,"other");
			    break;
		      }
		  }
		else if (vedCanvas->GetFileType() == Java)
		  {
		    switch (*cp)
		      {
			case 'c':
			    strcat(fmt,"class");
			    break;
			case 'f':
			    strcat(fmt,"field");
			    break;
			case 'i':
			    strcat(fmt,"interface");
			    break;
			case 'm':
			    strcat(fmt,"method");
			    break;
			case 'p':
			    strcat(fmt,"package");
			    break;
			default:
			    strcat(fmt,"other");
			    break;
		      }
		  }
		else
		  {
		    *fp++ = *cp;	// copy the char
		    *fp = 0;		// terminate
		  }
		// now reset fp to point to end of fmt
		cp++;			// skip the letter
		fp = &fmt[strlen(fmt)];
	      }

	    // and copy the rest now
	    while (*cp && *cp != '\r' && *cp != '\n')	// add the rest
		*fp++ = *cp++;
	    *fp++ = 0;			// terminate
	   
		
	    cmdw->AddLine(fmt);
	  }
      }

    inFile.close();

    if (!found)
	cmdw->AddLine("  ! Symbol not found in ctags file.");
    cmdw->SetRdOnly(1);

  }

//====================>>> videCmdWindow::GotoErrorLine <<<====================
  void videCmdWindow::GotoErrorLine()
  {
    // Pick up the error from the selected line in the message window
    // and open that file, going to the error line

    // First, get pointer to the error message window
    videCmdWindow* cmdw = ((videApp*)theApp)->GetMsgWindow();

    char errLine[100];

    long curLine = (cmdw->vedCanvas)->GetCurLine();
    (void)(cmdw->vedCanvas)->getLine(errLine, 99, curLine);


    char fileBase[100];
    char cLineNum[20];
    int ix;				// get base file name

    if (errLine[2] != '!')
      {
notValid:
        vNoticeDialog note(this);       // for user notification
        note.Notice("Selected line is not a g++/bcc/java error message.");
        return;
      }

    if ((cmdw->GetTextEd())->GetFileType() == bccError)
      {
	// error line in format "  ! Error E#### file.c ##: stuff
	// so need to get the # and the file name based on the :
  	char *cp;
	for ( cp = errLine ; *cp && *cp != ':' ; ++cp)
	  ;
	// cp should now point to the :
        if (*cp != ':')
	    goto notValid;
	*cp = 0;	// terminate the string to use strcpy
	while (*cp != ' ' && cp > errLine)
	    --cp;
	// Cp points to blank before line number
	strcpy(cLineNum,(cp+1));
	*cp = 0;	// now copy file name
	while (*cp != ' ' && cp > errLine)
	    --cp;
	strcpy(fileBase,(cp+1));

      }
    else
      {
	// error line in format "  ! filename.cpp:###:msg"

	for (ix = 4 ; ix < 100 && errLine[ix] != ':' ; ++ix)
	    fileBase[ix-4] = errLine[ix];
	fileBase[ix-4] = 0;

	char* np = &cLineNum[0];		// get the line number
	for ( ++ix ; ix < 100 && errLine[ix] != ':' ; ++ix)
	    *np++ = errLine[ix];
	*np = 0;
      }


    long lineNum = StrToLong(cLineNum);


    // start by seeing if file is in base directory
    char fullPath[maxFileNameSize];

    strcpy(fullPath,((videApp*)theApp)->GetMkFile());
    ix = strlen(fullPath);
    while (ix > 0)			// just that path
      {
	if (fullPath[ix] == '\\' || fullPath[ix] == '/')
	  {
	    fullPath[ix+1] = 0;		// keep the last /
	    break;
	  }
	--ix;
      }

    char* namePtr = fullPath;
    strcat(fullPath,fileBase);
    videCmdWindow* openw = FindCmdWin(fullPath);
    if (openw == 0)		// try just base name
      {
        openw = FindCmdWin(fileBase);
        namePtr = fileBase;	// just the base name
      }

    if (openw != 0)		// EASY, already open
      {
        (openw->GetTextEd())->EditCommand(edLineGoto, lineNum);
        (openw->GetTextEd())->EditCommand(edCenterScreen, 0);
        openw->RaiseWindow();
        return;
      }

    // Not already open, so need to open a new window.

    openw = new videCmdWindow("VIDE", -1, -1);
    (void) theApp->NewAppWin((vWindow*) cmdw, "", -1, -1, 0);

    if (!openw)
        return;	       // should never happen...
    if (!openw->OpenFile(fullPath,0,0))
      {
      	// hmmm - the file doesn't exist on Makefile path
        // try something else
        vFileSelect fsel(this);     // make an instance
        strcpy(fullPath,fileBase);	// base only
        if (!fsel.FileSelect("Open file",fullPath,maxFileNameSize,
                     filter,filterIndex))
          {
//            openw->CloseWin();
	    theApp->CloseAppWin(openw);
	    return;
	  }
	if (!openw->OpenFile(fullPath,0,0))
	  {
//              openw->CloseWin();
	    theApp->CloseAppWin(openw);
	    return;
	  }
        (openw->GetTextEd())->EditCommand(edLineGoto, lineNum);
        (openw->GetTextEd())->EditCommand(edCenterScreen, 0);
      }
    else
      {
        (openw->GetTextEd())->EditCommand(edLineGoto, lineNum);
        (openw->GetTextEd())->EditCommand(edCenterScreen, 0);
        openw->RaiseWindow();
      }
    
  }

//====================>>> videCmdWindow::ChangeInsMode <<<====================
  void videCmdWindow::ChangeInsMode(int IsInsMode, char* msg)
  {
    if (msg && *msg)
        SetString(lblIns,msg);
    else if ((vedCanvas->GetEdState()).readOnly)
        SetString(lblIns,"   View   ");
    else if (IsInsMode)
	SetString(lblIns,"  Insert  ");
    else
	SetString(lblIns," Overtype ");
  }

//====================>>> videCmdWindow::SetRdOnly <<<====================
  void videCmdWindow::SetRdOnly(int ro)
  {
    vedCanvas->SetRdOnly(ro);
    vedCanvas->Verify();

    ChangeInsMode(1);
   }

//====================>>> videCmdWindow::StatusMessage <<<====================
  void videCmdWindow::StatusMessage(char *msg)
  {
    char lineout[64];
    int ix;

    // copy up to 64 chars
    for (ix = 0 ; ix < 63 && msg[ix] ; ++ix)
	lineout[ix] = msg[ix];

#ifdef V_VersionX
    // pad with trailing blanks
    for ( ; ix < 63 ; ++ix)
	lineout[ix] = ' ';
#endif

    lineout[ix] = 0;

    SetString(lblEMsg,lineout);
  }

//====================>>> videCmdWindow::ErrorMsg <<<====================
  void videCmdWindow::ErrorMsg(char *str)
  {
      StatusMessage(str);
  }

//====================>>> videCmdWindow::KeyIn <<<====================
  void videCmdWindow::KeyIn(vKey keysym, unsigned int shift)
  {
    StatusMessage(" ");
    if (!vedCanvas->EditKeyIn(keysym, shift))
	vCmdWindow::KeyIn(keysym, shift);
  }

//==============>>> videCmdWindow::CheckClose <<<================
  int videCmdWindow::CheckClose(int ask)
  {
    // Checks to see if the user wants to save changes, if changes have been
    // done, before it closes the application.
    // Return 1 if ok to close now, 0 to abort

    if (this == ((videApp*)theApp)->GetMsgWindow())
      {
         ((videApp*)theApp)->savePrefs();

         // we are the main message window...
         if (NumCmdWindows() > 1)
             return 0;
      }

    if (vedCanvas->Changed())    // changes have been made
      {
	if (ask)
	  {
	    vYNReplyDialog ynr(this);
	    int ans = ynr.AskYN("Save new or changed file?");
	    if (ans == 0)
	      {
		return 1;           // don't want to save
	      }
	    if (ans == -1)          // cancel
		return 0;
	  }

        // ok, want to save changed file
        if (*fileName)                    // have a name
          {
            if (!vedCanvas->SaveFile(fileName))        // Save in fileName
              {
                vNoticeDialog note(this);       // for user notification
                note.Notice("Unable to save file");
                return 0;
              }
          }
        else                            // need to request name
          {
            vFileSelect fsel(this);     // V file select dialog

            if (!fsel.FileSelectSave("Save file as",
                fileName,99,filter,filterIndex) || !*fileName)
            return 0;

            if (!vedCanvas->SaveFile(fileName))        // Save in fileName
              {
                vNoticeDialog note(this);       // for user notification
                note.Notice("Unable to save file");
                return 0;
              }
          }
      }
    return 1;
  }

//====================>>> videCmdWindow::WindowCommand <<<====================
  void videCmdWindow::WindowCommand(ItemVal id, ItemVal val, CmdType cType)
  {
    // Default: route menu and toolbar commands here

    StatusMessage(" ");

    // First, check for all debug commands so we can route them
    // through one place.
    if (id >= (ItemVal)m_Debug && id < (ItemVal)m_lastDebug)
      {
	if (id == (ItemVal)m_Debug)
	  {
	    const char *runName;
	    const char *debugName;

	    theApp->SendWindowCommandAll(M_Save,1,C_Button);

	    if (strstr(((videApp*)theApp)->GetMkFile(),".java") != 0)
 	      {
		runName = (const char*)((videApp*)theApp)->GetMkFile();
		debugName = (const char*)((videApp*)theApp)->getDebugger();
	      }
	    else if (prjType == JavaProject)
 	      {
		runName = (const char*) ideJava->getJavaName();
		debugName = (const char*) ideJava->getDebugName();
		if (strstr(debugName,"appletviewer")!=0)
		    runName = (const char*)ideJava->getAppletName();
	      }
	    else
	      {
		runName = (const char*)mm->exeName;
		debugName = (const char*)((videApp*)theApp)->getDebugger();
	      }

	    ((videApp*)theApp)->runDebug(this, runName,debugName);

	  }
	else
	    ((videApp*)theApp)->debugCommand(this, id);
	return;
      }

    switch (id)
      {
	case M_New:
	  {
	    videCmdWindow* ncmdw = new videCmdWindow("VIDE", -1, -1);
	    (void) theApp->NewAppWin((vWindow*) ncmdw, "", -1, -1, 0);
            if (ncmdw)		// If a new window, raise it
	      {
                ncmdw->RaiseWindow();
		ncmdw->WindowCommand(M_SaveAs, val, cType);
	      }
	    break;
	  }	//@V@:EndCase

	case M_SaveAs:
	  {

	    char sname[maxFileNameSize+2] = ""; // start out with null name
            vFileSelect fselsa(this);     // V file select dialog

	    strcpy(sname,fileName);
            if (!fselsa.FileSelectSave("Save file as",
                sname,99,filter,filterIndex) || !*sname)
	        break;

	    strcpy(fileName,sname);

            if (!vedCanvas->SaveFile(fileName))        // Save in fileName
              {
                vNoticeDialog note(this);       // for user notification
                note.Notice("Unable to save file");
              }
	    else
	      {
		SetTitle(fileName);
		SetSyntaxType(this, fileName,0);
	      }
	    break;
	  }	//@V@:EndCase

	//@V@:Case M_Open
        case m_View:
	case M_Open: 			// View/Open - always open a new window!
	  {
	    char name[maxFileNameSize+2] = ""; // start out with null name
	    vFileSelect fsel(this);     // make an instance

	    int oans = fsel.FileSelect("Open file",name,maxFileNameSize,
                     filter,filterIndex);

	    if (oans)
	      {
		videCmdWindow* openw = FindCmdWin(name);
		if (openw != 0)
		  {
		    vNoticeDialog note(this);       // for user notification
		    note.Notice("Sorry - you can only edit one copy of each file.");
		    return;		 // @@@ not twice
		  }
		// Now, see if it really exists - the FileSelect
		// allows you to have non-existant files...

		ifstream inFile(name);

		if (!inFile)
		  {
		    vNoticeDialog note(this);       // for user notification
		    note.Notice("Unable to open specified file.");
		    return;		// file not there
		  }
		inFile.close();

		videCmdWindow* cmdw = new videCmdWindow("VIDE", -1, -1);
		(void) theApp->NewAppWin((vWindow*) cmdw, "", -1, -1, 0);

		if (!cmdw)
		    break;
		if (!cmdw->OpenFile(name))
		  {
		    vNoticeDialog note(this);
		    note.Notice("Unable to open specified file.");
		    if (cmdw != this)		// A new window?
			delete cmdw;
		    break;
		  }
		else
		  {
		    if (cmdw != this)		// If a new window, raise it
			cmdw->RaiseWindow();
		  }

		SetSyntaxType(cmdw,name,0);

		edState curState = cmdw->vedCanvas->GetEdState();
		if (id == m_View)
		  {
		    curState.readOnly = 1;
		  }

		cmdw->vedCanvas->SetEdState(curState);
		cmdw->vedCanvas->Verify();
		cmdw->ChangeInsMode(1);
	      }

	    break;
	  }	//@V@:EndCase

	//@V@:Case M_Save
	case M_Save:
	  {
	    CheckClose(0);
	    break;
	  }	//@V@:EndCase

	//@V@:Case M_saveAll
	case m_saveAll:
	  {
	    theApp->SendWindowCommandAll(M_Save,1,C_Button);
	    break;
	  }	//@V@:EndCase

	//@V@:Case M_CloseFile
	case M_CloseFile:
	  {
	    theApp->CloseAppWin(this);
	    break;
	  }	//@V@:EndCase

	//@V@:Case m_SaveClose
	case M_SaveClose:
	  {
	    if (!CheckClose(0))		// force the save
		break;
	    theApp->CloseAppWin(this);
	    break;
	  }	//@V@:EndCase


	//@V@:Case M_Exit
	case M_Exit:
	  {
	    theApp->Exit();
	    break;
	  }	//@V@:EndCase

        //@V@:Case btnDoMake
	case m_MakeAll:
	  {
	    //@@ Need to distinguish Java vs. Make here

	    theApp->SendWindowCommandAll(m_saveAll,1,C_Button);

	    if (((videApp*)theApp)->getAutoCtags() )
		WindowCommand(m_GenCtags, val, cType);

	    if (strstr(((videApp*)theApp)->GetMkFile(),".java") != 0)
	      {
		ideJava->CompileJava(this);
	      }
	    else if (prjType == JavaProject)
	      {
		ideJava->CompileJava(this);
	      }
	    else		// must be a C project
	      {
		SetSens(m_StopMake, 1);
		if (ideMake->DoMake(this, makebName,""))
		  {
		    if (ideMake->CheckMake(makebName,""))
			ideMake->DoMake(this, makebName,"");
		  }
		SetSens(m_StopMake, 0);
	      }
	    break;
	  }	//@V@:EndCase

         //@V@:Case m_MakeArgs
	case m_MakeArgs:
	  {
            static char target[80] = "";
            vReplyDialog rp(this);
            if (rp.Reply("Enter make target",target,79) == M_Cancel)
                break;
	    char cmd[100];
	    SetSens(m_StopMake, 1);
	    theApp->SendWindowCommandAll(M_Save,1,C_Button);
            (void) ideMake->DoMake(this, cmd, target);
	    SetSens(m_StopMake, 0);
	    break;
	  }	//@V@:EndCase

        case m_MakeCleanObj:
	  {
	    SetSens(m_StopMake, 1);
	    theApp->SendWindowCommandAll(M_Save,1,C_Button);
            (void) ideMake->DoMake(this, makebName, "cleanobjs");
	    SetSens(m_StopMake, 0);
	    break;
	  }

        case m_MakeCleanAll:
	  {
	    SetSens(m_StopMake, 1);
	    theApp->SendWindowCommandAll(M_Save,1,C_Button);
            (void) ideMake->DoMake(this, makebName, "clean");
	    SetSens(m_StopMake, 0);
	    break;
	  }

        case m_MakeCleanBin:
	  {
	    SetSens(m_StopMake, 1);
	    theApp->SendWindowCommandAll(M_Save,1,C_Button);
            (void) ideMake->DoMake(this, makebName, "cleanbin");
	    SetSens(m_StopMake, 0);
	    break;
	  }

        case m_StopMake:
          {
            ideMake->StopMake();
	    break;
	  }	//@V@:EndCase

        case m_MakeDebug:
          {
            char cmd[100];
	    SetSens(m_StopMake, 1);
	    theApp->SendWindowCommandAll(M_Save,1,C_Button);
            if (ideMake->DoMake(this, makebName,"debug"))
	      {
	        if (ideMake->CheckMake(cmd,"debug"))
		    ideMake->DoMake(this, cmd,"debug");
	      }
	    SetSens(m_StopMake, 0);
	    break;
	  }	//@V@:EndCase

	case m_GenCtags:
	  {
	    // generate a new tags file by running ctags
	    char cmd[255];
            vOS vos;
	    strcpy(cmd,"ctags -n ");
	   
	    // use args from project if there is one, else the default

	    if (prjType == CProject /* || prjType == JavaProject */)
	      {
		if (*mm->ctags)
		    strcat(cmd,mm->ctags);
		else
		    strcat(cmd,((videApp*)theApp)->getCtags());
	      }
	    else
	      {
		strcat(cmd,((videApp*)theApp)->getCtags());
	      }

	    ((videApp*)theApp)->GetMsgWindow()->AddLine(cmd);
#ifdef V_VersionX
	    system(cmd);
#else
            vos.vRunProcess(cmd, 0, 0, 0, 1);
#endif
	    ((videApp*)theApp)->GetMsgWindow()->AddLine("------");
            break;
	  }

    //**************************************************
    // The Project Menu
    //**************************************************

	case m_ProjectClose:
	  {
	    changeDir(prjName);
	    if (prjType == CProject)
	      {
		if (prjName[0] != 0)
		  {
		    mm->saveProject(prjName);	// save previous
		  }
		mm->setDefaults();
	      }
	    else if (prjType == JavaProject)
	      {
		ideJava->closeJProject(prjName);
	      }

	    strcpy(prjName,"");
	    prjType = NoProject;

	    ((videApp*)theApp)->SetMkFile("");
    //	    ((videApp*)theApp)->debugCommand(this, (ItemVal) m_StopDebug);	// stop debugger

	    // Set menus/tool buttons ready to open project/makefile

	    SetSens(m_ProjectSaveAs, 0);
	    SetSens(m_MakeMake, 0);
	    SetSens(m_ProjectClose, 0);
	    SetSens(m_ProjectEdit, 0);
	    SetSens(m_MakeAll, 0);
	    SetSens(m_MakeCleanObj, 0);
	    SetSens(m_MakeCleanAll, 0);
	    SetSens(m_MakeCleanBin, 0);
	    SetSens(m_MakeArgs, 0);

	    SetSens(m_ProjectNewC, 1);
	    SetSens(m_SelectMake, 1);
	    SetSens(m_ProjectNewJava, 1);
	    SetSens(m_NewJavaApplet, 1);
	    SetSens(m_NewJavaWin, 1);
	    SetSens(m_NewJavaConsole, 1);
	    SetSens(m_ProjectOpen, 1);

	    break;
	  }

	case m_ProjectEdit:
	  {
	    if (prjName[0] == 0)	// can't edit a non-existent file
		break;

	    changeDir(prjName);

	    if (prjType == CProject)
	      {
		projectDialog PDlg(mm,this);
		if (!PDlg.editProject())	// edit an old one
		    break;
		if (mm->changed)
		  {
		    mm->saveProject(prjName);	// always save after edit
		  }
		((videApp*)theApp)->SetMkFile(mm->makeName);
	      }
	    else if (prjType == JavaProject)
	      {
		// The java version was designed later, so it is
		// more self contained...
		ideJava->editJProject(prjName, /*createIt*/ false);
		((videApp*)theApp)->SetMkFile(ideJava->getJavaName());
	      }

	    break;
	  }

	case m_EditorPrefs:
	  {
	    prefModalDialog pmd(this);
	    pmd.setPrefs("Set Editor Preferences");
	    vedCanvas->setColorScheme();
	    vedCanvas->Verify();
	    theApp->UpdateAllViews(this, ChangePrefs, 0);
	    break;
	  }

	case m_VidePrefs:
	  {
	    videModalDialog vmd(this);
	    vmd.setPrefs("Set VIDE Preferences");
	    break;
	  }

	case m_MakeMake:
	  {
	    if (prjType == CProject)
	      {
		mm->saveMakefile();	// regenerate Makefile
	      }
	    break;
	  }

	case m_ProjectSaveAs:
	  {
	    static char* projFilter[] = {
		"*.vpj", 
		"*.c;*.C;*.cpp;*.cxx;*.h;*.hxx",
		"*.java;*.htm;*.html",
		"*", 0};

	    int fi = 0;

	    changeDir(prjName);
            vFileSelect fselp(this);     // V file select dialog

            if (!fselp.FileSelectSave("Save Project as",
                prjName,99,projFilter,fi) || !*prjName)
	        break;

	    if (prjType == CProject)
	      {
		mm->saveProject(prjName);	// always save after edit
		((videApp*)theApp)->SetMkFile(mm->makeName);
	      }
	    else if (prjType == JavaProject)
	      {
		ideJava->closeJProject(prjName, /*reset*/ false);
	      }
	    break;
	  }

	case m_ProjectNewC:
	  {
	    char MSDOSpath[256]= "";
	    ItemVal oans;
	    int fi = 0;
	    // allowing other filters possibilites allows
	    // user to see files to know what will be in the project.
	    static char* makeFilter[] = {
		"*.vpj",
		"*.c;*.C;*.cpp;*.cxx;*.h;*.hxx",
		"*", 0};
	    vFileSelect fselc(this);

	    if (prjName[0] != 0)		// shouldn't happen, but...
	      {
		if (prjType == CProject)
		    mm->saveProject(prjName);	// save previous
		else if (prjType == JavaProject)
		    ideJava->closeJProject(prjName);
		prjType = NoProject;
	      }

	    //find where to save new project
	    oans = fselc.FileSelectSave("New C/C++ Project file",
	    MSDOSpath , 255, makeFilter,fi);
	    if (oans && *MSDOSpath) //filename picked
		Dos2UxFName(MSDOSpath, prjName, 0);
	    else
		break;

	    prjType = CProject;		// now is the time to set this

	    mm->setDefaults();		// make default values
	    
	    // first, open the wizard to set up just what kind
	    // of C++ project
	    CNewProjMDlg wiz(mm,this);
	    if (!wiz.CNewProjWizard("Create New C/C++ Project"))
		break;

	    projectDialog PDlg(mm,this);
	    if (!PDlg.editProject())	// and edit the default values
		break;

	    SetSens(m_MakeAll, 1);
	    SetSens(m_MakeCleanObj, 1);
	    SetSens(m_MakeCleanAll, 1);
	    SetSens(m_MakeCleanBin, 1);
	    SetSens(m_MakeArgs, 1);
	    SetSens(m_ProjectSaveAs, 1);
	    SetSens(m_ProjectClose, 1);
	    SetSens(m_ProjectEdit, 1);
	    SetSens(m_MakeMake, 1);

	    SetSens(m_SelectMake, 0);
	    SetSens(m_ProjectNewC, 0);
	    SetSens(m_ProjectOpen, 0);
	    SetSens(m_ProjectNewJava, 0);
	    SetSens(m_NewJavaApplet, 0);
	    SetSens(m_NewJavaWin, 0);
	    SetSens(m_NewJavaConsole, 0);

	    mm->saveProject(prjName);	// Save the file
            ((videApp*)theApp)->SetMkFile(mm->makeName);
	    break;
	  }

//	case m_ProjectNewJava:
	case m_NewJavaApplet:
	case m_NewJavaWin:
	case m_NewJavaConsole:
	  {
	    char MSDOSpath[256]= "";
	    ItemVal oans;
	    int fi = 0;
	    static char* makeFilter[] = {
		"*.vpj",
		"*.java;*.htm;*.html",
		"*",
		 0};
	    vFileSelect fselj(this);

	    if (prjName[0] != 0)
	      {
		if (prjType == CProject)
		    mm->saveProject(prjName);	// save previous
		else if (prjType == JavaProject)
		    ideJava->closeJProject(prjName);
		prjType = NoProject;
	      }

	    //find where to save new project
	    oans = fselj.FileSelectSave("New Java Project file",
	    MSDOSpath , 255, makeFilter,fi);
	    if (oans && *MSDOSpath) //filename picked
		Dos2UxFName(MSDOSpath, prjName, 0);
	    else
		break;
	    
	    prjType = JavaProject;	// remember this now

	    // edit / create project - the 1 means create

	    ideJava->editJProject(prjName,/*createIt*/true, id);
	    ((videApp*)theApp)->SetMkFile(ideJava->getJavaName());

	    SetSens(m_ProjectSaveAs, 1);
	    SetSens(m_ProjectClose, 1);
	    SetSens(m_ProjectEdit, 1);
	    SetSens(m_SelectMake, 0);
	    SetSens(m_ProjectNewC, 0);
	    SetSens(m_ProjectNewJava, 0);
	    SetSens(m_NewJavaApplet, 0);
	    SetSens(m_NewJavaWin, 0);
	    SetSens(m_NewJavaConsole, 0);
	    SetSens(m_ProjectOpen, 0);
	    SetSens(m_MakeMake, 0);

	    SetSens(m_MakeAll, 1);
	    SetSens(m_MakeCleanObj, 0);
	    SetSens(m_MakeCleanAll, 0);
	    SetSens(m_MakeCleanBin, 0);
	    SetSens(m_MakeArgs, 0);
	    ideJava->closeJProject(prjName, /*reset*/ false);	// save it

	    break;
	  }

	case m_ProjectOpen:
	  {
	    if (prjName[0] != 0)
	      {
		if (prjType == CProject)
		    mm->saveProject(prjName);	// save previous
		else if (prjType == JavaProject)
		    ideJava->closeJProject(prjName);
		prjType = NoProject;
	      }

	    //open prj file
	    char MSDOSpath[256]= "";
	    int fi = 0;
	    static char* makeFilter[] = {"*.vpj", "*", 0};
	    vFileSelect fselpr(this);

	    int ans = fselpr.FileSelect("Select a project file",
	        MSDOSpath ,255, makeFilter,fi);

	    if (!(ans && *MSDOSpath))	 //filename picked?
		break;

	    OpenProject(MSDOSpath);	// open it
	    if (((videApp*)theApp)->getAutoCtags() )
		WindowCommand(m_GenCtags, val, cType);
	    break;
	  }

        case m_RunVgen:
          {
            vOS vos;
            vos.vRunProcess("vgen", 0, 0, 0);
            break;
          }

        case m_RunViconed:
          {
            vOS vos;
            vos.vRunProcess("viconed", 0, 0, 0);
            break;
          }

        case m_RunShell:
          {
#ifdef V_VersionWindows
	    // I don't really understand this, but I think it has
	    // something to do with the fact that VIDE is usually
	    // started from an icon, and thus doesn't have a console
	    // attached. This seems to make a big differencd on NT,
	    // but no difference on Win9x. So the shell has to be
	    // started differently on NT and 9x.
	    OSVERSIONINFO osver;
	    osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	    ::GetVersionEx(&osver);
	    if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT)
	      {
		::ShellExecute(theApp->winHwnd(),"open",
		    ((videApp*)theApp)->getVideShell(),0,0,SW_SHOWDEFAULT);
	      }
	    else		// win 9x
	      {
		vOS vos;
		vos.vRunProcess(((videApp*)theApp)->getVideShell(), 0, 0, 0);
	      }
#else
            vOS vos;
            vos.vRunProcess(((videApp*)theApp)->getVideShell(), 0, 0, 0);
#endif
            break;
          }

        case m_RunProject:
          {
//@@ Save all for now, but evenutally need to add logic
//@@ to handle changed files without make...

	    theApp->SendWindowCommandAll(M_Save,1,C_Button);

	    if ( ((videApp*)theApp)->debuggerOpen())
	      {
		((videApp*)theApp)->debugCommand(this, m_DBRun);
		break;
	      }

	    if (strstr(((videApp*)theApp)->GetMkFile(),".java") != 0)
 	      {
		ideJava->RunJava(this,((videApp*)theApp)->GetMkFile());
	      }
	    else if (prjType == JavaProject)
 	      {
		ideJava->RunJava(this,ideJava->getJavaName());
	      }
	    else
	      {
		char name[maxFileNameSize+2] = ""; // start out with null name
		if (strcmp(mm->BinDir,".") != 0)
		  {
		    strcpy(name,mm->BinDir);
		    strcat(name,"/");
		  }
		strcat(name,mm->exeName);
		vRunDialog run(this);      // make an instance
		(void)run.Run(name);
	      }
            break;
          }

        case m_RunCmdArgs:
          {
	    char name[maxFileNameSize+2] = ""; // start out with null name
	    if (strcmp(mm->BinDir,".") != 0)
	      {
		strcpy(name,mm->BinDir);
		strcat(name,"/");
	      }
	    strcat(name,mm->exeName);
	    vRunDialog run(this);      // make an instance
	    (void)run.Run(name);

            break;
          }

	//@V@:Case M_Font
	case M_Font:
	  {
	    vFontSelect fd(this);

	    
	    (void) fd.FontSelect(vedFont,"Select Font",/*fixedOnly*/ 1);

	    ((videApp*)theApp)->DefaultFont = vedFont;

	    // first, change myself
	    int oldh = vedCanvas->GetRows();
	    int oldw = vedCanvas->GetCols();
	    vedCanvas->SetFont(vedFont);
	    vedCanvas->SetTextRowsCols(oldh,oldw);

	    // now, all the other views
	    theApp->UpdateAllViews(this, ChangeFont, 0);
	    break;
	  }	//@V@:EndCase


	//@V@:Case M_Cut
	case M_Cut:
	  {
	    vedCanvas->EditCommand(edCut,0);
	    break;
	  }	//@V@:EndCase

	//@V@:Case M_Copy
	case M_Copy:
	  {
	    vedCanvas->EditCommand(edCopy,0);
	    break;
	  }	//@V@:EndCase

	//@V@:Case M_Paste
	case M_Paste:
	  {
	    vedCanvas->EditCommand(edPaste,0);
	    break;
	  }	//@V@:EndCase

        case m_SelectMake:	// select makefile
          {
            static int filterIndex = 0;
            static char* filter[] =
              {
        	"Make*;make*;Make*.*;make*.*;*.java",
		"*.java",
        	"*",
        	0
              };
            char fn[maxFileNameSize+2] = "Makefile";

            vFileSelect fselmk(this);     // V file select dialog

            if (!fselmk.FileSelect("Select Makefile (C++) or main .java file (Java)",
                fn,maxFileNameSize,filter,filterIndex))
	        break;

	    // close any open project
	    WindowCommand((ItemVal)m_ProjectClose, (ItemVal)0, (CmdType)0 );
	    ((videApp*)theApp)->SetMkFile(fn);
	    if (strstr(fn,".java") != 0)	// don't allow make for java
	      {
		SetSens(m_MakeCleanObj, 0);
		SetSens(m_MakeCleanAll, 0);
		SetSens(m_MakeCleanBin, 0);
		SetSens(m_MakeArgs, 0);
	      }
	    else
	      {
		SetSens(m_MakeAll, 1);
		SetSens(m_MakeCleanObj, 1);
		SetSens(m_MakeCleanAll, 1);
		SetSens(m_MakeCleanBin, 1);
		SetSens(m_MakeArgs, 1);
	      }

            break;
          }

        case m_updateMkFile:	// fix up the label for cwd
          {
	    static char title[2*maxFileNameSize+20] = 
		"No Makefile, Project, or .java file Specified";

	    if (prjName[0] != 0)
	      {
		strcpy(title,"Project: ");
		strcat(title,prjName);
	      }

            char *mkname = ((videApp*)theApp)->GetMkFile();
            if (*mkname != 0)
	      {
		if (strstr(mkname,".java") != 0)
		  {
		    if (prjName[0] != 0)
			strcat(title," - Main Java file: ");
		    else
			strcpy(title,"Main Java file: ");
		    SetSens(m_MakeAll, 1);
		    SetSens(m_MakeCleanObj, 0);
		    SetSens(m_MakeCleanAll, 0);
		    SetSens(m_MakeCleanBin, 0);
		    SetSens(m_MakeArgs, 0);
		  }
		else
		  {
		    if (prjName[0] != 0)
			strcat(title," - Makefile: ");
		    else
			strcpy(title,"Makefile: ");
		    SetSens(m_MakeAll, 1);
		    SetSens(m_MakeCleanObj, 1);
		    SetSens(m_MakeCleanAll, 1);
		    SetSens(m_MakeCleanBin, 1);
		    SetSens(m_MakeArgs, 1);
		  }
		strcat(title,mkname);
	      }
	    (((videApp*)theApp)->GetMsgWindow())->
                    SetTitle(title);
            break;
          }

	//@V@:Case M_UnDo
	case M_UnDo:
	  {
	    vedCanvas->EditCommand(edUndo,0);
	    break;
	  }	//@V@:EndCase

	case m_AboutVide:
	  {
	    abtvideModalDialog abt(this,"About the V IDE...");
	    abt.abtvideAction("");
	    break;
	  }

	case m_sendToBrowser:
	  {
	    if (vedCanvas->GetFileType() == HTML)
	      {
		CheckClose(0);			// save the file
		if (!((videApp*)theApp)->findHelp(fileName,"","",0,1))
		    ErrorMsg("Couldn't browse file");
	      }
	    else
		ErrorMsg("HTML files only!");
	    break;
	  }

	case videHelp:
	case gnuHelp:
	case win32Help:
	case vguiHelp:
	case javaHelp:
	case javaAPIHelp:
	case htmlHelp:
        case htmlCSSHelp:
	  {
	    ((videApp*)theApp)->showHelp(id);
	    break;
	  }

	default:	// route unhandled commands through editor
	  {
	    if (vedCanvas->EditCommand(id, 1) < 0)
		vCmdWindow::WindowCommand(id, val, cType);
	    break;
	  }
      }
  }

//====================>>> videCmdWindow::SetSyntaxType <<<====================
  void videCmdWindow::SetSyntaxType(videCmdWindow* cmdw, char* name, int RdOnly)
  {
    // Set syntax highlighting based on file name
    char* CExts[] = {".cpp", ".CPP", ".C", ".c", ".cxx", ".CXX", ".cc", ".CC",
               ".h", ".H", ".hpp", ".HPP", ""};
    static char* HTMLExts[] = {".htm", ".HTM", ".html", ".HTML",
		""};

    static char* PerlExts[] = {".pl", ".PL", ""};
    static char* TexExts[] = {".tex", ""};
    static char* JavaExts[] = {".java", ""};
    static char* FortranExts[] = {".for",".FOR",".f",".F",".fpp",
		".FPP",".f77",".f90",""};

    edState curState = cmdw->vedCanvas->GetEdState();
    curState.ftype = Text;			// assume Text
    curState.readOnly = RdOnly;
    curState.autoIndent = 0;			// assum not autoindent

    for (int ix = 0 ; *CExts[ix] ; ++ix)	// check C extensions
      {
        if (strstr(name,CExts[ix]) != 0)	// found one!
          {                                     // @@@ .h matches .html!!!!
            curState.ftype = CPP;		// gets overridden next for loop
	    curState.autoIndent = ((videApp*)theApp)->getAutoIndent(); // change
            break;
          }
      }
    for (int ixa = 0 ; *HTMLExts[ixa] ; ++ixa)	// check HTML extensions
      {
        if (strstr(name,HTMLExts[ixa]) != 0)	// found one!
          {
            curState.ftype = HTML;
	    curState.autoIndent = 0;
            break;
          }
      }

    for (int ixb = 0 ; *JavaExts[ixb] ; ++ixb)	// check Java extensions
      {
        if (strstr(name,JavaExts[ixb]) != 0)	// found one!
          {
            curState.ftype = Java;
	    curState.autoIndent = ((videApp*)theApp)->getAutoIndent(); // change
            break;
          }
      }

    for (int ix = 0 ; *PerlExts[ix] ; ++ix)	// check Perl extensions
      {
        if (strstr(name,PerlExts[ix]) != 0)	// found one!
          {
            curState.ftype = Perl;
	    curState.autoIndent = ((videApp*)theApp)->getAutoIndent(); // change
            break;
          }
      }

    for (int ix = 0 ; *FortranExts[ix] ; ++ix)	// check Fortran extensions
      {
        if (strstr(name,FortranExts[ix]) != 0)	// found one!
          {
            curState.ftype = Fortran;
	    curState.autoIndent = ((videApp*)theApp)->getAutoIndent(); // change
            break;
          }
      }

    for (int ix = 0 ; *TexExts[ix] ; ++ix)	// check TeX extensions
      {
        if (strstr(name,TexExts[ix]) != 0)	// found one!
          {
            curState.ftype = TeX;
	    curState.autoIndent = 0; // change
            break;
          }
      }

    cmdw->vedCanvas->SetEdState(curState);
  }

//====================>>> videCmdWindow::OpenProject <<<====================
  void videCmdWindow::OpenProject(char* name)
  {

    if (!name || !*name)
	return;

    strcpy(prjName,name);		// save name

    SetSens(m_ProjectSaveAs, 1);	// set sensitivity
    SetSens(m_ProjectClose, 1);
    SetSens(m_ProjectEdit, 1);
    SetSens(m_MakeAll, 1);

    SetSens(m_SelectMake, 0);
    SetSens(m_ProjectNewC, 0);
    SetSens(m_ProjectNewJava, 0);
    SetSens(m_NewJavaApplet, 0);
    SetSens(m_NewJavaWin, 0);
    SetSens(m_NewJavaConsole, 0);
    SetSens(m_ProjectOpen, 0);


    if (ideJava->isJavaProject(prjName))
      {
	ideJava->loadJProject(prjName);
	prjType = JavaProject;
	((videApp*)theApp)->SetMkFile(ideJava->getJavaName());
	SetSens(m_MakeCleanObj, 0);
	SetSens(m_MakeCleanAll, 0);
	SetSens(m_MakeCleanBin, 0);
	SetSens(m_MakeArgs, 0);
	SetSens(m_MakeMake, 0);
      }
    else		// C Project
      {
	struct stat fstproj, fstmake;

	prjType = CProject;
	mm->loadProject(prjName);
	if (stat(prjName,&fstproj) == 0)	// got a stat
	  {
	    if (stat(mm->makeName,&fstmake) == 0)	// got a stat for makefile
	      {
		// only need to make make if project is newer
		if (fstproj.st_mtime >= fstmake.st_mtime)
		    mm->saveMakefile();			// default - save the makefile
	      }
	    else
		mm->saveMakefile();			// default - save the makefile
	  }
	else
	    mm->saveMakefile();			// default - save the makefile
	((videApp*)theApp)->SetMkFile(mm->makeName);
	SetSens(m_MakeCleanObj, 1);
	SetSens(m_MakeCleanAll, 1);
	SetSens(m_MakeCleanBin, 1);
	SetSens(m_MakeArgs, 1);
	SetSens(m_MakeMake, 1);
      }
  }

//====================>>> videCmdWindow::OpenFile <<<====================
  int videCmdWindow::OpenFile(char* name, int RdOnly, int notify)
  {
     if (!name || !*name)
	return 0;

    if (!vedCanvas->ReadFile(name,0))
      {
        if (!notify)
            return 0;
	vNoticeDialog note(this);       // for user notification
	note.Notice("Unable to open specified file");
	return 0;
      }

    SetTitle(name);
    strcpy(fileName,name);
    SetSyntaxType(this,name,RdOnly);
    vedCanvas->Verify();
    ChangeInsMode(1);
    return 1;
  }

//====================>>> videCmdWindow::SetViewWindow <<<====================
  int videCmdWindow::SetViewWindow(char *name)
  {
    SetTitle(name);
    strcpy(fileName,name);
    vedCanvas->resetBuff();
#ifdef V_VersionX
    vedFont.SetFontValues(vfFixed,12);	// use Courier rathern than default
#else
#ifdef DEVEL
    vedFont.SetFontValues(vfFixed,9,vfNormal,vfBold);	// use Courier rathern than default
#else
    vedFont.SetFontValues(vfFixed,10);	// use Courier rathern than default
#endif
#endif
    vedCanvas->SetFont(vedFont);

    vedCanvas->Verify();
    return 1;
  }

//====================>>> videCmdWindow::AddLine <<<====================
  int videCmdWindow::AddLine(char* line)
  {
    char linout[2048];

    if (strlen(line) > 2047)
	line[2047] = 0;
    if (vedCanvas->GetLines() > 1000)	// kill off top of buffer if too big
      {
	vedCanvas->lineGoto(1L);
	vedCanvas->lineDelete(500);
	vedCanvas->bufferBottom();
      }

    // The following code splits lines so they fit on the window

    int cols = vedCanvas->GetCols();
    if (strlen(line) > (unsigned int)cols )
      {
	strcpy(linout,line);
	linout [cols-1] = 0;
	if (!vedCanvas->addLine(linout))
	    return 0;
        for (int nxt = cols - 1 ; nxt > 0 ; )
	  {
	    if (line[0] == '>')
		strcpy(linout,"+ ");
	    else
		strcpy(linout,"*  ");
	    strcat(linout,&line[nxt]);
	    if (strlen(linout) > (unsigned int)cols)
	      {
		nxt = nxt + cols - 2;
		linout[cols] = 0;
	      }
	    else
		nxt = 0;
	    if (!vedCanvas->addLine(linout))
		return 0;
	  }
      }
    else
	if (!vedCanvas->addLine(line))
	    return 0;
    vedCanvas->displayBuff(99999,1);	// goto last line
    return 1;
  }
