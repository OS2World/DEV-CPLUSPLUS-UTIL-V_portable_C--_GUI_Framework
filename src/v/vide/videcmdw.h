//=======================================================================
//	videCmdw.h:	header for videCmdwindow class
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

#ifndef videCMDW_H
#define videCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vutil.h>	// For V Utilities
#include <v/vcmdpane.h> // command pane
#include <v/vstatusp.h>	// For the status pane
#include <v/vfont.h>	// for fonts

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "videmake.h"	// our make class
#include "videjava.h"	// our java support

#include "videcnv.h"	// vedCanvasPane
#include "cprojdlg.h"
#include "mmaker.h"
#include "vdbcmdw.h"

    enum {m_updateMkFile = 500 };
    enum ProjectType
      {
	NoProject = 100,
	CProject,
	JavaProject
      };

    class videCmdWindow;
    class vTextEdCmdInterp;

//function to convert Dos to ux filenames
extern void Dos2UxFName(char *dosFileName, char *uxFileName, bool fix);

    class videCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public
	videCmdWindow(VCONST char*, int, int);
	virtual ~videCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);

	virtual void UpdateView(vWindow* sender, int hint, void* pHint);
	void setCurPC(long lineNum);
	bool checkIfAnyChanged();

	void ChangeLoc(long line, int col);
	void ChangeInsMode(int IsInsMode, char* msg = 0);
	void StatusMessage(char *msg);
	void ErrorMsg(char *str);
	int OpenFile(char* name, int RdOnly = 0, int notify = 1);
	void OpenProject(char* name);

	int CheckClose(int ask = 1);
        int SetViewWindow(char *name);
        int AddLine(char* line);
        vedTextEditor* GetTextEd() { return vedCanvas; }
        void SetRdOnly(int ro);
        void GotoErrorLine();
        void lookupCtag(char* sym);
        char* GetFileName() { return fileName; }
	void SetSyntaxType(videCmdWindow* cmdw, char* name, int RdOnly);
	int getSens(ItemVal id);
	void SetSens(ItemVal id, int val);
        videCmdWindow* FindCmdWin(char* fname);

      protected:	//--------------------------------------- protected
        
  
      private:		//--------------------------------------- private

	// Standard elements
	vMenuPane* videMenu;		// For the menu bar
	vedTextEditor* vedCanvas;	// For the canvas
	vCommandPane* videCmdPane;	// For the command pane
	vStatusPane* videStatus;	// For the status bar
        vFont vedFont;			// for the font

	// Dialogs associated with CmdWindow

        // Other stuff
        videMake* ideMake;
        vTextEdCmdInterp* vedCI;	// command interp

	// these have to be static so that there is only
	// one copy of each - they contain the info about the
	// project - either C++ (makefileMaker) or Java (videJava)

	static int cmdWinCount;

	static makefileMaker* mm;
	static videJava* ideJava;

	
	static char prjName[maxFileNameSize+2];
	static ProjectType prjType;

	char fileName[maxFileNameSize+2];
	char fileBaseName[maxFileNameSize+2];


      };
#endif
