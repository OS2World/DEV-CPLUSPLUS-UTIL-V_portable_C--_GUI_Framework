//=======================================================================
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
//
//	prefmdlg.cpp:	Source for prefModalDialog class
//=======================================================================

#include "videapp.h"	// for defs of emulation, etc.
#include "prefmdlg.h"

//@V@:BeginIDs
    enum {
	lblMainMsg = 1000,
	blnk0,
	blnk1,
	lblTS,
	spnTabs,
	lblAS,
	spnAuto,
	lblWR,
	spnWrap,
	lblROW, 
	spnRows,
	lblCOL, 
	spnCols,
	frmEmu,
	lblEmu,
	rdbStd,
	rdbSee,
	rdbVi,
	rdbEmacs,
	rdbOther,
	chkBackup,
	lblBI,
	spnBrace,
 	lblColor,
	cbxColor,
	chkAutoIn,

	// IDs for VIDE dialog
	lblVMainMsg,
	lblPN,
	txiHelpPath,
	lblJN,
	txiJavaPath,
	lblBCCN,
	txiBCCPath,
	lblBN,
	txiBrowser,
	lblSH,
	txiShell,
	lblHBN,
	txiHelpViewer,
	lblDB,
	txiDebugger,
	lblCtags,
	txiCtags,
	chkCtags,

	lastOne
    };
//@V@:EndIds

// Help path
// tab spacing
// emulation
// fixed scroll
// 
  static int minMaxStep[3] =  // specify range of
    {                         // 1 to 10
      1, 10, 1              // in steps of 1
    };

  static int autoMinMax[3] =  // specify range of
    {                         // 0 to 20
      0, 20, 1              // in steps of 1
    };

  static int wrapMinMax[3] =  // specify range of
    {                         // 0 to 20
      0, 80, 1              // in steps of 1
    };

  static int braceMinMax[3] =  // specify range of
    {                         // 0 to 2
      0, 2, 2              // in steps of 2
    };

  static int rowsMinMax[3] =  // specify range of
    {                         // 0 to 20
      0, 100, 1              // in steps of 1
    };

  static int colsMinMax[3] =  // specify range of
    {                         // 0 to 20
      0, 130, 1              // in steps of 1
    };

    static char* colorList[] =
      {
	"Standard",
	"Gray",
	"Dark Blue",
	"Midnight",
	"Parchment",
	"Pale Green",
	0
      };

//@V@:BeginDialogCmd DefaultCmds
    static DialogCmd DefaultCmds[] =
      {
	{C_Label, lblMainMsg, 0,"X",NoList,CA_MainMsg,isSens,NoFrame, 0, 0},

	{C_Label, lblAS, 0, "Auto Save (minutes):",NoList,CA_None,isSens,NoFrame,
		0, lblMainMsg},
        {C_Spinner,spnAuto,0,"Auto", (void*)autoMinMax,CA_None,
	        isSens,NoFrame, lblAS,lblMainMsg,0,"Auto save files"},

	{C_Blank,blnk0,0,"  ",NoList,CA_None,isSens,NoFrame,
		spnAuto, lblMainMsg},
	{C_Label, lblTS, 0, "Tab spacing:",NoList,CA_None,isSens,NoFrame,
		blnk0, lblMainMsg},
        {C_Spinner,spnTabs,8,"Tabs", (void*)minMaxStep,CA_None,
	        isSens,NoFrame, lblTS,lblMainMsg,0,"Set how tabs are displayed"},

	{C_Label, lblBI, 0, "{} Indentation:",NoList,CA_None,isSens,NoFrame,
		0,spnAuto},
        {C_Spinner,spnBrace,2,"{}'s", (void*)braceMinMax,CA_None,
	        isSens,NoFrame, lblAS,spnAuto,0,"{} indentation for beautify C++/Java"},

	{C_Label, lblWR, 0, "Text wrap:",NoList,CA_None,isSens,NoFrame,
		blnk0, spnTabs},
        {C_Spinner,spnWrap,65,"Wrap", (void*)wrapMinMax,CA_None,
	        isSens,NoFrame, lblTS,spnTabs,0,"Set right wrap limit for text formatting"},


	{C_Label, lblROW, 0, "Edit window - rows: ",NoList,CA_None,isSens,NoFrame,
		0, spnWrap},
        {C_Spinner,spnRows,30,"rows", (void*)rowsMinMax,CA_None,
	        isSens,NoFrame, lblAS,spnWrap,0,"Set rows of text"},


	{C_Label, lblCOL, 0, "cols:",NoList,CA_None,isSens,NoFrame,
		blnk0, spnWrap},
        {C_Spinner,spnCols,30,"cols", (void*)colsMinMax,CA_None,
	        isSens,NoFrame, lblTS,spnWrap,0,"Set cols of text"},


	{C_Frame, frmEmu,0,"",NoList,CA_None,isSens,NoFrame,
		0,spnRows},
	{C_Label,lblEmu,0,"Editor Command Set Emulation", NoList,CA_None,isSens,
		frmEmu,0,0},
	{C_RadioButton,rdbStd,1,"Standard",NoList,CA_None,isSens,frmEmu,
		0,lblEmu,0,"Standard generic command set"},
	{C_RadioButton,rdbSee,0,"See",NoList,CA_None,isSens,frmEmu,
		rdbStd,lblEmu,0,"See moded editor command set"},
	{C_RadioButton,rdbVi,0,"Vi",NoList,CA_None,isSens,frmEmu,
		rdbSee,lblEmu,0,"Vi editor"},
	{C_RadioButton,rdbEmacs,0,"Emacs",NoList,CA_None,notSens,frmEmu,
		rdbVi,lblEmu,0,"Emacs editor"},
	{C_RadioButton,rdbOther,0,"Write your own!",NoList,CA_None,notSens,frmEmu,
		rdbEmacs,lblEmu,0,"You write it."},

	{C_CheckBox, chkBackup, 1,"Make Backup File",NoList,CA_None,isSens,
		NoFrame,0,frmEmu, 0,"Make a backup file when file is read."},

	{C_Label, lblColor, 0, "  Color scheme:",NoList,CA_None,isSens,NoFrame,
		chkBackup, frmEmu},
	{C_ComboBox, cbxColor,0,"Colors",(void*)colorList,CA_NoNotify,isSens,NoFrame,
		lblColor,frmEmu,0,"Editor color scheme"},

	{C_CheckBox, chkAutoIn, 0,"Autoindent Code",NoList,CA_None,isSens,
		NoFrame,0,chkBackup, 0,"Autoindent code files"},

	{C_Button, M_Cancel, 0, " Cancel ",NoList,CA_None,
	    isSens,NoFrame,0, chkAutoIn},
	{C_Button, M_OK, 0, " OK ", NoList, CA_DefaultButton,
	    isSens, NoFrame, M_Cancel, chkAutoIn},

	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
    };
//@V@:EndDialogCmd
    static DialogCmd VideDefaultCmds[] =
      {
	{C_Label, lblVMainMsg, 0,"X",NoList,CA_MainMsg,isSens,NoFrame, 0, 0},

	{C_Label, lblPN, 0, "Help file path:    ",NoList,CA_None,isSens,NoFrame,
		0, lblVMainMsg},
	{C_TextIn, txiHelpPath,0," ",NoList,CA_Large,isSens,NoFrame,
		lblPN,lblVMainMsg,0,"Path for V Help files"},

	{C_Label, lblJN, 0, "Java path:",NoList,CA_None,isSens,NoFrame,
		0, txiHelpPath},
	{C_TextIn, txiJavaPath,0," ",NoList,CA_Large,isSens,NoFrame,
		lblPN,txiHelpPath,0,"Path for Java Help files"},

	{C_Label, lblSH, 0, "OS shell:",NoList,CA_None,isSens,NoFrame,
		0, txiJavaPath},
	{C_TextIn, txiShell,0," ",NoList,CA_Large,isSens,NoFrame,
		lblPN,txiJavaPath,0,"Used to 'Run OS Shell'"},


	{C_Label, lblDB, 0, "Debugger:",NoList,CA_None,isSens,NoFrame,
		0, txiShell},
	{C_TextIn, txiDebugger,0," ",NoList,CA_Large,isSens,NoFrame,
		lblPN,txiShell,0,"Debugger to use"},

	{C_Label, lblCtags, 0, "ctags args:",NoList,CA_None,isSens,NoFrame,
		0, txiDebugger},
	{C_TextIn, txiCtags,0," ",NoList,CA_Large,isSens,NoFrame,
		lblPN,txiDebugger,0,"Default args for running ctags"},

	{C_CheckBox,chkCtags,0,"Automatically run ctags",
		NoList,CA_None,isSens,NoFrame,lblPN,txiCtags,0,
		"Autorun ctags when opening a project or making."},

#ifdef V_VersionX
	{C_Label, lblHBN, 0, "Help Viewer:",NoList,CA_None,isSens,NoFrame,
		0, chkCtags},
	{C_TextIn, txiHelpViewer,0," ",NoList,CA_Large,isSens,NoFrame,
		lblPN,chkCtags,0,"Use View HTML Help files"},

	{C_Label, lblBN, 0, "HTML Browser:",NoList,CA_None,isSens,NoFrame,
		0, txiHelpViewer},
	{C_TextIn, txiBrowser,0," ",NoList,CA_Large,isSens,NoFrame,
		lblPN,txiHelpViewer,0,"Use to view HTML files."},

	{C_Blank,blnk1,0," ",NoList,CA_None,isSens,NoFrame,
		0, txiBrowser},
#else
	{C_Label, lblBCCN, 0, "Borland root:",NoList,CA_None,isSens,NoFrame,
		0, chkCtags},
	{C_TextIn, txiBCCPath,0," ",NoList,CA_Large,isSens,NoFrame,
		lblPN,chkCtags,0,"BCCROOT Path - Set ONLY if you are using BCC 5.5"},

	{C_Blank,blnk1,0," ",NoList,CA_None,isSens,NoFrame,
		0, txiBCCPath},
#endif

    
	{C_Button, M_Cancel, 0, " Cancel ",NoList,CA_None,
	    isSens,NoFrame,0, blnk1},
	{C_Button, M_OK, 0, " OK ", NoList, CA_DefaultButton,
	    isSens, NoFrame, M_Cancel, blnk1},

	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
    };

//======================>>> prefModalDialog::prefModalDialog <<<==================
  prefModalDialog::prefModalDialog(vBaseWindow* bw, char* title) :
    vModalDialog(bw, title)
  {
    cmdWin = (videCmdWindow*)bw;

    AddDialogCmds(DefaultCmds);		// add the predefined commands
  }

//===================>>> prefModalDialog::~prefModalDialog <<<====================
  prefModalDialog::~prefModalDialog()
  {
    UserDebug(Destructor,"prefModalDialog::~prefModalDialog() destructor\n")
  }

//====================>>> prefModalDialog::setPrefs <<<====================
  int prefModalDialog::setPrefs(char* msg)
  {
    ItemVal ans,rval;

    // char buff[maxFileNameSize];

    ans = ShowModalDialog(msg,rval);
    if (ans == M_Cancel)
	return 0;

    // *** Code to process dialog values here

    globalState gs = (cmdWin->GetTextEd())->GetGlobalState();
    gs.tabspc = GetValue(spnTabs);	// set the tab field
    gs.wraplm = GetValue(spnWrap);	// set the wrap
    gs.braceIndent = GetValue(spnBrace);	// set the brace
    (cmdWin->GetTextEd())->SetGlobalState(gs);	// and set!

    ((videApp*)theApp)->setDefaultRows(GetValue(spnRows));
    ((videApp*)theApp)->setDefaultCols(GetValue(spnCols));

    ((videApp*)theApp)->setAutoSave(GetValue(spnAuto));
    ((videApp*)theApp)->setBackup(GetValue(chkBackup));
    ((videApp*)theApp)->setAutoIndent(GetValue(chkAutoIn));
    ((videApp*)theApp)->setColorScheme(GetValue(cbxColor));


    // Emulation
    if(GetValue(rdbStd))
	((videApp*)theApp)->SetEmulation(Generic);
    else if(GetValue(rdbSee))
	((videApp*)theApp)->SetEmulation(See);
    else if(GetValue(rdbVi))
	((videApp*)theApp)->SetEmulation(Vi);
    else if(GetValue(rdbEmacs))
	((videApp*)theApp)->SetEmulation(Emacs);
    else
	((videApp*)theApp)->SetEmulation(Generic);

    return ans == M_OK;
  }

//====================>>> prefModalDialog::DialogDisplayed <<<====================
  void prefModalDialog::DialogDisplayed()
  {
    // Set dialog values to current values

    switch ( ((videApp*)theApp)->GetEmulation() )
      {
	case Generic:
	  {
	    SetValue(rdbStd, 1, Value);
	    break;
	  }

        case See:
	  {
	    SetValue(rdbSee, 1, Value);
	    break;
	  }

        case Vi:
	  {
	    SetValue(rdbVi, 1, Value);
	    break;
	  }

	case Emacs:
	  {
	    SetValue(rdbEmacs, 1, Value);
	    break;
	  }

	default:
	  {
	    SetValue(rdbStd, 1, Value);
	    break;
	  }
      }

    SetValue(spnRows,((videApp*)theApp)->getDefaultRows(),Value);
    SetValue(spnCols,((videApp*)theApp)->getDefaultCols(),Value);
    SetValue(spnAuto,((videApp*)theApp)->getAutoSave(),Value);
    SetValue(chkBackup,((videApp*)theApp)->getBackup(),Value);
    SetValue(chkAutoIn,((videApp*)theApp)->getAutoIndent(),Value);
    SetValue(spnTabs,(cmdWin->GetTextEd())->GetGlobalState().tabspc, Value);
    SetValue(spnWrap,(cmdWin->GetTextEd())->GetGlobalState().wraplm, Value);
    SetValue(spnBrace,(cmdWin->GetTextEd())->GetGlobalState().braceIndent, Value);
    SetValue(cbxColor,((videApp*)theApp)->getColorScheme(),Value);

  }

//====================>>> prefModalDialog::DialogCommand <<<====================
  void prefModalDialog::DialogCommand(ItemVal id, ItemVal retval, CmdType ctype)
  {
    UserDebug2(CmdEvents,"prefModalDialog::DialogCommand(id:%d, val:%d)\n",id, retval)

    switch (id)		// We will do some things depending on value
      {
      }

    vModalDialog::DialogCommand(id,retval,ctype);
  }

//##################################################################################
//##################################################################################

//======================>>> videModalDialog::videModalDialog <<<==================
  videModalDialog::videModalDialog(vBaseWindow* bw, char* title) :
    vModalDialog(bw, title)
  {
    cmdWin = (videCmdWindow*)bw;

    AddDialogCmds(VideDefaultCmds);		// add the predefined commands
  }

//===================>>> videModalDialog::~videModalDialog <<<====================
  videModalDialog::~videModalDialog()
  {
    UserDebug(Destructor,"videModalDialog::~videModalDialog() destructor\n")
  }

//====================>>> videModalDialog::setPrefs <<<====================
  int videModalDialog::setPrefs(char* msg)
  {
    ItemVal ans,rval;

    char buff[maxFileNameSize];

    ans = ShowModalDialog(msg,rval);
    if (ans == M_Cancel)
	return 0;

    // *** Code to process dialog values here

    GetTextIn(txiHelpPath,buff,maxFileNameSize);
    ((videApp*)theApp)->setHelpPath(buff);
    GetTextIn(txiJavaPath,buff,maxFileNameSize);
    ((videApp*)theApp)->setJavaPath(buff);
    GetTextIn(txiShell,buff,maxFileNameSize);
    ((videApp*)theApp)->setVideShell(buff);
    GetTextIn(txiDebugger,buff,maxFileNameSize);
    ((videApp*)theApp)->setDebugger(buff);
    GetTextIn(txiCtags,buff,maxFileNameSize);
    ((videApp*)theApp)->setCtags(buff);
    ((videApp*)theApp)->setAutoCtags(GetValue(chkCtags));
    
#ifdef V_VersionX
    GetTextIn(txiBrowser,buff,maxFileNameSize);
    ((videApp*)theApp)->setBrowser(buff);
    GetTextIn(txiHelpViewer,buff,maxFileNameSize);
    ((videApp*)theApp)->setHelpViewer(buff);

#else
    GetTextIn(txiBCCPath,buff,maxFileNameSize);
    ((videApp*)theApp)->setBCCPath(buff);
#endif

    return ans == M_OK;
  }

//====================>>> videModalDialog::DialogDisplayed <<<====================
  void videModalDialog::DialogDisplayed()
  {
    // Set dialog values to current values

    SetString(txiHelpPath,((videApp*)theApp)->getHelpPath());
    SetString(txiJavaPath,((videApp*)theApp)->getJavaPath());
    SetString(txiDebugger,((videApp*)theApp)->getDebugger());
    SetString(txiCtags,((videApp*)theApp)->getCtags());
    SetValue(chkCtags, ((videApp*)theApp)->getAutoCtags(), Value);
    SetString(txiShell,((videApp*)theApp)->getVideShell());
#ifdef V_VersionX
    SetString(txiBrowser,((videApp*)theApp)->getBrowser());
    SetString(txiHelpViewer,((videApp*)theApp)->getHelpViewer());
#else
    SetString(txiBCCPath,((videApp*)theApp)->getBCCPath());
#endif
  }

//====================>>> videModalDialog::DialogCommand <<<====================
  void videModalDialog::DialogCommand(ItemVal id, ItemVal retval, CmdType ctype)
  {
    UserDebug2(CmdEvents,"videModalDialog::DialogCommand(id:%d, val:%d)\n",id, retval)

    switch (id)		// We will do some things depending on value
      {
      }

    vModalDialog::DialogCommand(id,retval,ctype);
  }


