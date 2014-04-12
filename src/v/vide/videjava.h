//=======================================================================
// V IDE Java Class - all the stuff needed to interface with Java
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
//  videMake header

#ifndef videjava_h
#define videjava_h

#include <fstream.h>

#include <v/vslist.h>

#include <v/vos.h>
#include "jprojdlg.h"

    class videCmdWindow;
    class videJava
      {
      friend class JavaProjectDialog;	// so it can set everything
      public:
	videJava(videCmdWindow* parent);
	~videJava();

	void CompileJava(videCmdWindow* parent);
	void RunJava(videCmdWindow* parent,const char* javaprog);

	bool isJavaProject(const char* pname);
	void closeJProject (const char *pname, bool reset = true);
	void editJProject (const char *pname, bool createIt = false, ItemVal id = 0);
	void loadJProject (const char *pname);
	bool addFile(char *n);
	char* getJavaName() {return mainFile;}
	char* getAppletName() {return  appletHTML;}
	char* getDebugName();

      protected:
        void ShowErrors(videCmdWindow* cmdw, char* errName);

      private:

	videCmdWindow* myParent;
	bool getSectionEntry(ifstream &prjf, const char* head, char* value);
	bool getSectionList(ifstream &prjf, const char* head, 
		const char* term, vSList& sl);
	char* stripDir(char* n);

	void setDefaults(char* mainName = "<none>", ItemVal id = 0);
        void eraseAll();

        videCmdWindow* cmdw;

	// Project variables...
	char* compilerName;
	char* interpName;
	char* debugName;
	char* classPath;
	char* appletHTML;
	char* mainFile;
	char* srcDir;

	vSList compOptsPool;
	vSList compOpts;
	vSList interpOptsPool;
	vSList interpOpts;
	vSList debugOptsPool;
	vSList debugOpts;
	vSList srcFiles;


      };

#endif
