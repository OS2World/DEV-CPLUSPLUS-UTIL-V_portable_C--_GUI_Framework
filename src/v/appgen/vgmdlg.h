//=======================================================================
//@V@:Note: This file generated by vappgen V 1.00.
//	vgmdlg.h:	Header for vgModalDialog class
//=======================================================================

#ifndef vgMDLG_H
#define vgMDLG_H

#include <v/vmodald.h>
#include "vgdefs.h"

    class vgModalDialog : public vModalDialog
      {
      public:		//---------------------------------------- public
	vgModalDialog(vBaseWindow*, char* title = "Standard V App");
	virtual ~vgModalDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	int vgAction(char* msg, vgOptions& op);

       protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

     };
#endif
 
