//	abtvdlg.h:	Header for abtvideModalDialog class
//=======================================================================

#ifndef abtvideMDLG_H
#define abtvideMDLG_H

#include <v/vmodald.h>

    class abtvideModalDialog : public vModalDialog
      {
      public:		//---------------------------------------- public
	abtvideModalDialog(vBaseWindow* bw, char* title = "About V IDE");
	virtual ~abtvideModalDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	virtual int abtvideAction(char* msg);

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

      };
#endif

