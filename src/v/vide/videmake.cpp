//=======================================================================
// V IDE Make Class - all the stuff needed to create
// makefiles, and run make.
//  Copyright (C) 1995-1999  Bruce E. Wampler
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

#include <v/vfilesel.h>
#include <v/vnotice.h>

#include "videapp.h"

#include "videcmdw.h"

#include "videmake.h"
//#include <direct.h>

    static int filterIndex = 0;
    static char* filter[] =
      {
        "Make*;make*;Make*.*;make*.*",
        "*",
        0
      };

//=====================>>> videMake::videMake <<<===========================
  videMake::videMake()
  {
    _stop = 0;
    cmdw = ((videApp*)theApp)->GetMsgWindow();
  }

//=====================>>> videMake::~videMake <<<===========================
  videMake::~videMake()
  {
  }

//=====================>>> videMake::DoMake <<<===========================
  int videMake::DoMake(videCmdWindow* parent, char* makecmd, char* what)
  {
    char cmd[maxFileNameSize];
    vOS vos;
    int isBorland = *((videApp*)theApp)->getBCCPath() != 0;

    static char makeCmds[] = "makecmds.vtm" ;
    static char makeErrs[] = "makeerrs.vtm" ;

    char* mkpath = ((videApp*)theApp)->GetMkFile();
    cmdw = ((videApp*)theApp)->GetMsgWindow();

    if (!*mkpath)
      {
        vNoticeDialog note((vWindow*)cmdw);
        note.Notice("No Makefile specified. Use Select Makefile or Open Project first.");
        return 0;
      }

    if (isBorland)
	(void)fixBackSlash(mkpath,isBorland);
    else
	fixSlash(mkpath);

    cmdw->RaiseWindow();
    cmdw->SetRdOnly(0);
    if (isBorland)
	(cmdw->GetTextEd())->SetFileType(bccError);
    else
	(cmdw->GetTextEd())->SetFileType(gccError);

    strcpy(cmd,makecmd);	// just using cmd as a temp
	strcat(cmd," -f ");
    strcat(cmd,mkpath);
    if (what && *what)
      {
	strcat(cmd," ");
	strcat(cmd,what);
      }
    cmdw->AddLine(cmd);
    if (isBorland)
	cmdw->AddLine("Please wait until the complete make is done...");

    theApp->CheckEvents();
    _stop = 0;			// not stopping

    // Now, change dir

    changeDir(mkpath);
    
    // build command
    strcpy(cmd,makecmd);

    if (isBorland)
	strcat(cmd, " -f");             // -f for file
    else
	strcat(cmd, " -n -f ");         // -n for dry run, -f for file
    strcat(cmd, mkpath);		// the name of the make file.

    if (what && *what)
      {
	strcat(cmd," ");
	strcat(cmd,what);
      }

    vos.vDeleteFile(makeCmds);
    vos.vDeleteFile(makeErrs);

    if (vos.vRunProcess(cmd,makeCmds,makeErrs,1,1) != 0)
      {
	ShowErrors(cmdw,makeCmds);
	ShowErrors(cmdw,makeErrs);
	if (isBorland)
	  {
	    cmdw->AddLine("Make failed, usually because of errors.");
	  }
	else
	  {
	    cmdw->AddLine("Make failed, usually because of missing files. You may");
	    cmdw->AddLine("need to run make from a command window to see the problem.");
	  }
	cmdw->AddLine("----------");
	cmdw->SetRdOnly(1);
	cmdw->RaiseWindow();
	theApp->CheckEvents();
	return 0;
      }

    theApp->CheckEvents();
    if (_stop)
      {
	cmdw->AddLine("Make Aborted.");
	cmdw->SetRdOnly(1);
	cmdw->RaiseWindow();
	theApp->CheckEvents();
	return 0;
      }

    ifstream inFile(makeCmds);

    if (!inFile)
	return 0;		// file not there

    if (!cmdw)
      {
        inFile.close();
        vos.vDeleteFile(makeCmds);
        return 0;
      }

    if (isBorland)
      {
	ShowErrors(cmdw,makeCmds);
	ShowErrors(cmdw,makeErrs);
	inFile.close();
	cmdw->AddLine("---- Make completed ----");
	cmdw->SetRdOnly(1);
	cmdw->RaiseWindow();
	theApp->CheckEvents();
	return 1;
      }

  // read and process makecmds.tmp
    const int maxBuff = 2000;
    char buff[maxBuff+2];
    char displayed[maxBuff+20];

    while (inFile.getline(buff,maxBuff))
      {
        strcpy(displayed,"> ");
        strcat(displayed,buff);
	if (!cmdw->AddLine(displayed))
	  {
	    theApp->CheckEvents();
	    vNoticeDialog note(theApp);
	    note.Notice("Make output too big -- only paritally read.");
	    break;
	  }
        char* cp;
        for (cp = buff ; *cp && *cp != '\r' && *cp != '\n' ; ++cp)
            ;
        *cp = 0;

        int retv = vos.vRunProcess(buff,0,makeErrs,1,1);
        
        ShowErrors(cmdw,makeErrs);

        theApp->CheckEvents();
        if (_stop)
          {
            inFile.close();
            vos.vDeleteFile(makeCmds);
            cmdw->AddLine("Make Aborted.");
            cmdw->SetRdOnly(1);
	    cmdw->RaiseWindow();
	    theApp->CheckEvents();
            return 0;
          }

        if (retv != 0)
          {
            inFile.close();
            vos.vDeleteFile(makeCmds);
            cmdw->AddLine("Make failed. Right click error line to open file.");
            cmdw->SetRdOnly(1);
	    cmdw->RaiseWindow();
	    theApp->CheckEvents();
            return 0;
          }
        }

    inFile.close();
    vos.vDeleteFile(makeCmds);
    cmdw->AddLine("--------");
    cmdw->SetRdOnly(1);
    cmdw->RaiseWindow();
    theApp->CheckEvents();
    return 1;
  }

//=====================>>> videMake::ShowErrors <<<=========================
  void videMake::ShowErrors(videCmdWindow* cmdw, char* errName)
  {
    const int maxBuff = 1000;
    char buff[maxBuff+2];
    char displayed[maxBuff+20];
    vOS vos;

    ifstream errFile(errName);

    if (!errFile)
        return;

    while (errFile.getline(buff,maxBuff))
      {
        strcpy(displayed,"  ! ");	// IMPORTANT!  videCmdWindow::GotoErrorLine
        strcat(displayed,buff);         //	REQUIRES this exact format!!!
        cmdw->AddLine(displayed);
        theApp->CheckEvents();
        if (_stop)
          {
            cmdw->AddLine("Make Aborted.");
	    vos.vDeleteFile(errName);
            return;
          }

      }
    errFile.close();
    vos.vDeleteFile(errName);
  }

//=====================>>> videMake::CheckMake <<<===========================
  int videMake::CheckMake(char* makecmd,char* what)
  {
    // Check if make would do anything...
    char cmd[maxFileNameSize];
    vOS vos;

    static char makeCmds[] = "makecmds.vtm" ;
    static char makeErrs[] = "makeerrs.vtm" ;

    char* mkpath = ((videApp*)theApp)->GetMkFile();

    videCmdWindow* cmdw = ((videApp*)theApp)->GetMsgWindow();

    if (*((videApp*)theApp)->getBCCPath() != 0)
	return 0;

    if (!*mkpath)
      {
	return 0;
      }

    // Now, change directory
    strcpy(cmd,mkpath);

    int ix = strlen(cmd);
    while (ix > 0)
      {
        if (cmd[ix] == '\\' || cmd[ix] == '/')
          {
            cmd[ix] = 0;
            break;
          }
        --ix;
      }
    char makeFile[100];
    strcpy(makeFile, &cmd[ix+1]);	// copy the makefile name
    vos.vChDir(cmd);			// change dir

    strcpy(cmd, makecmd);		// make or make clean passed from caller
    strcat(cmd, " -n -f ");             // -n for dry run, -f for file
    strcat(cmd, makeFile);		// the name of the make file.

    if (what && *what)
      {
	strcat(cmd, " ");
	strcat(cmd, what);
      }
    if (vos.vRunProcess(cmd,makeCmds,makeErrs,1,1) != 0)
      {
	vos.vDeleteFile(makeErrs);
	vos.vDeleteFile(makeCmds);
     	return 0;
      }
    // OK, then perhaps "nothing to make" issued.
    ifstream inFile(makeCmds);			// open the makecmds file

    int retv = 1;
    char buff[256];
    if (inFile.getline(buff,155))
      {
	if (strstr(buff, "Nothing to be done") != 0)
	    retv = 0;
      }
    inFile.close();

    vos.vDeleteFile(makeErrs);
    vos.vDeleteFile(makeCmds);
    return retv;
  }

