//	cnewmdlg.h:	Header for CNewProjMDlg class
//=======================================================================

#ifndef CNewProjMDLG_H
#define CNewProjMDLG_H

#include <v/vmodald.h>

#include "mmaker.h"

class vApp;
class videCmdWindow;

    class CNewProjMDlg : public vModalDialog
      {
      public:		//---------------------------------------- public
	CNewProjMDlg(makefileMaker* m,
	    vBaseWindow* bw, char* title = "New C/C++ Project");
	virtual ~CNewProjMDlg();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	virtual int CNewProjWizard(char* msg);
#ifdef V_VersionWindows
	virtual void DialogDisplayed();
#endif
      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private
#ifdef V_VersionWindows
	void setBorlandValues();
#endif
 	makefileMaker* mm;
        videCmdWindow* cmdw;

      };
#endif

