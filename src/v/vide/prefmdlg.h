//	prefmdlg.h:	Header for prefModalDialog class
//=======================================================================

#ifndef prefMDLG_H
#define prefMDLG_H

#include <v/vmodald.h>
#include "videcmdw.h"

    class prefModalDialog : public vModalDialog
      {
      public:		//---------------------------------------- public
	prefModalDialog(vBaseWindow* bw, char* title = "Editor Preferences");
	virtual ~prefModalDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	virtual void DialogDisplayed();
	virtual int setPrefs(char* msg);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private
	videCmdWindow* cmdWin;

      };

//########################################################################
//########################################################################

    class videModalDialog : public vModalDialog
      {
      public:		//---------------------------------------- public
	videModalDialog(vBaseWindow* bw, char* title = "Vide Preferences");
	virtual ~videModalDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	virtual void DialogDisplayed();
	virtual int setPrefs(char* msg);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private
	videCmdWindow* cmdWin;

      };
#endif

