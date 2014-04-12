//=======================================================================
//	cprjdlg.h:	Header for projectDialog class
//=======================================================================

#ifndef CPROJDLG_H
#define CPROJDLG_H

#include <v/vmodald.h>
#include <v/vnotice.h>
#include <v/vfilesel.h>
#include <v/vreply.h>

#include <string.h>

#include "mmaker.h"
class vApp;
class videCmdWindow;

class projectDialog : public vModalDialog
{
 public:		//---------------------------------------- public

  projectDialog(makefileMaker* m,
	vBaseWindow* bw, char* title = "C++ Project Editor");
  projectDialog(makefileMaker* m,
	vApp *aw, char* title= "C++ Project Editor");
  virtual ~projectDialog();		// Destructor
  virtual int editProject();
  enum {Cancel=0, BuildMakeF, NoBuild, Edit};

 protected:	      //--------------------------------------- protected

  virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
  virtual void DialogDisplayed();

 private:		//--------------------------------------- private

  bool edit;
  int ta; //target file added
  makefileMaker* mm;
  videCmdWindow* cmdw;

};

#endif
