//	gdbdlg.h:	Header for jdbDialog class
//=======================================================================

#ifndef jdbDialog_H
#define jdbDialog_H
#include <v/vdialog.h>

    class jdbDialog : public vDialog
      {
      public:		//---------------------------------------- public
	jdbDialog(vBaseWindow* bw, char* title = "jdb");
	virtual ~jdbDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	void AddDefaultCmds();		// to add the defined commands

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

      };
#endif
