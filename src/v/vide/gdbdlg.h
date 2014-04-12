//	gdbdlg.h:	Header for gdbDialog class
//=======================================================================

#ifndef gdbDIALOG_H
#define gdbDIALOG_H
#include <v/vdialog.h>

    class gdbDialog : public vDialog
      {
      public:		//---------------------------------------- public
	gdbDialog(vBaseWindow* bw, char* title = "gdb Commands");
	virtual ~gdbDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	void AddDefaultCmds();		// to add the defined commands

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

      };
#endif
