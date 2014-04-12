//	jprojdlg.h:	Header for JavaProjectDialog class
//=======================================================================

#ifndef JPROJDLG_H
#define JPROJDLG_H

#include <v/vmodald.h>
#include <v/vnotice.h>

#include <v/vfilesel.h>
#include <v/vreply.h>
#include <string.h>

class vApp;
class videCmdWindow;
class videJava;

class JavaProjectDialog : public vModalDialog
{
 public:		//---------------------------------------- public

  JavaProjectDialog(videJava* vjv, vBaseWindow* bw, char* title = "Java Project Editor");
  virtual ~JavaProjectDialog();		// Destructor
  virtual int editProject();
  enum {Cancel=0, BuildMakeF, NoBuild, Edit};

 protected:	      //--------------------------------------- protected

  virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
  virtual void DialogDisplayed();

 private:		//--------------------------------------- private

  bool edit;
  int ta; //target file added
  videCmdWindow* cmdw;
  videJava* vj;

};

#endif
