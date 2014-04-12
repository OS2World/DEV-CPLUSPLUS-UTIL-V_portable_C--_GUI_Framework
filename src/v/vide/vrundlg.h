//===============================================================
// vrundlg.h - run modal dialog
//
// Copyright (C) 1995,1996  Bruce E. Wampler
//
// This file is part of the V C++ GUI Framework, and is covered
// under the terms of the GNU Library General Public License,
// Version 2. This library has NO WARRANTY. See the source file
// vapp.cxx for more complete information about license terms.
//===============================================================

#ifndef VRUNDLG_H
#define VRUNDLG_H
#include <v/vmodald.h>

    class vApp;
    class vRunDialog : protected vModalDialog
      {
      public:		//---------------------------------------- public
	vRunDialog(vBaseWindow* bw, char* title = "Run Program") : vModalDialog(bw, title)
	  {added = 0;}
	vRunDialog(vApp *aw, char* title = "Run Program") : vModalDialog(aw, title)
	  {added = 0;}
	~vRunDialog() { }

	int Run(char *initialName = "");
	int GetDebugName(char *initialName, char* nameOut);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	virtual void DialogDisplayed();
	virtual void DialogCommand(ItemVal id, ItemVal val, CmdType ctype);
	int added;
      };
#endif
