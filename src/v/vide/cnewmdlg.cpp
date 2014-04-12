//=======================================================================
//    cnewmdlg.cpp:	Source for CNewProjMDlg class
//    This is part of VIDE. See videapp.h for copyrights.
//    date: 04 Mar 2000 11:54:18
//
//  Notes:
//
//	The Windows version has pretty good support for MinGW, Cygwin, and Borland
//
//	The X version is limited. It very specifically supports only
//	    V GUI on Linux X11R6 platforms at the moment.
//
//	The X version lacks support for:
//		OpenGL
//		building libraries
//		gtk
//		Non Linux platforms
//
//	Hey, its FREE! You want more, feel free to help!
//	
//=======================================================================

#include "cnewmdlg.h"
#include "videapp.h"
#include <v/vnotice.h>
#include <v/vreply.h>

//@V@:BeginIDs
    enum {
	lblMsg = 1500,
	frmM,
	frmC,
	frmD,
	lblC,
	lblTgt, txiTgt,
	lblT,
	lblDb,
	rdbC,
	rdbCPP,
	rdbCon,
	rdbGUI,
	rdbLib,
	rdbDLL,
	rdbMing,
	rdbCyg,
	rdbBorland,
	rdbOther,
	rdbRel,
	rdbDbg,
	blk1,
	blk2,
	blk3,
	txiRes,
	chkV,
	chkVdll,
	chkVMotif,
	chkVAthena,
	chkGL,
	chkRes,
	chkNoCyg,
	lastone
  };
//@V@:EndIds

//@V@:BeginDialogCmd DefaultCmds
    static DialogCmd WCmds[] =
      {
	{C_Label, lblMsg, 0,"Project Type:",NoList,CA_None,isSens,NoFrame, 0, 0},
	{C_RadioButton,rdbCPP,1,"C++ ",NoList,CA_None,isSens,NoFrame,
		lblMsg,0,0,"Create a C++ language project"},
	{C_RadioButton,rdbC,0,"C",NoList,CA_None,isSens,NoFrame,
		rdbCPP,0,0,"Create a C language project"},

	{C_Label, lblTgt, 0, "Target name: ",NoList,CA_None,isSens,NoFrame,0,lblMsg},
	{C_TextIn, txiTgt,0,"",NoList,CA_None,isSens,NoFrame,lblTgt,lblMsg,0,
	    "Name of target, e.g., foo.exe, foo.a"},

	{C_Frame, frmM, 0, 0, NoList,CA_None,isSens,NoFrame,0,txiTgt},

	{C_Label, lblT,0,"Type of build",NoList,CA_None,isSens,frmM,0,0},
#ifdef V_VersionX
	{C_CheckBox,chkVMotif,0,"Use V GUI (Motif)",NoList,CA_None,isSens,frmM,
		0,lblT,0,"Build GUI App using Motif V GUI framework lib"},
	{C_CheckBox,chkVAthena,0,"Use V GUI (static)",NoList,CA_None,isSens,frmM,
		0,chkVMotif,0,"Build GUI App using static V GUI framework lib"},
#endif

#ifdef V_VersionWindows
	{C_RadioButton,rdbCon,0,"Console Application",NoList,CA_None,isSens,frmM,
		0,lblT,0,"Build a console application"},
	{C_RadioButton,rdbGUI,1,"GUI Application",NoList,CA_None,isSens,frmM,
		0,rdbCon,0,"Build a GUI application"},
	{C_Blank,blk1,0,"  ",NoList,CA_None,isSens,frmM,0,rdbGUI},

	{C_CheckBox,chkV,0,"Use V GUI (static)",NoList,CA_None,isSens,frmM,
		blk1,rdbGUI,0,"Build GUI App using static V GUI framework lib"},
	{C_CheckBox,chkVdll,0,"Use V GUI (DLL)",NoList,CA_None,isSens,frmM,
		blk1,chkV,0,"Build GUI App using DLL V GUI framework lib"},
	{C_CheckBox,chkGL,0,"Use OpenGL",NoList,CA_None,isSens,frmM,
		blk1,chkVdll,0,"Build GUI App using OpenGL"},

	{C_RadioButton,rdbLib,0,"Static Library",NoList,CA_None,isSens,frmM,
		0,chkGL,0,"Build a static library"},
#ifdef NEWDLL
	{C_RadioButton,rdbDLL,0,"Dynamimc Library (DLL)",NoList,CA_None,notSens,frmM,
		0,rdbLib,0,"Build a dynamic link library (DLL)"},
#endif
#endif
	
	{C_Frame,frmD,0,0,NoList,CA_None,isSens,NoFrame,frmM,txiTgt},
	{C_Label,lblDb,0,"Release/Debug",NoList,CA_None,isSens,frmD,0,0},
	{C_RadioButton,rdbRel,1,"Release Version",NoList,CA_None,isSens,
		frmD,0,lblDb,0,"Build release version - no debugging"},
	{C_RadioButton,rdbDbg,0,"Debug Version",NoList,CA_None,isSens,
		frmD,0,rdbRel,0,"Build debug version - include debugging info"},

#ifdef V_VersionWindows
	{C_Frame,frmC,0,0,NoList,CA_None,isSens,NoFrame,frmM,frmD},
	{C_Label,lblC,0,"Compiler",NoList,CA_None,isSens,frmC,0,0},
	{C_RadioButton,rdbMing,1,"Mingw32",NoList,CA_None,isSens,frmC,
		0,lblC,0,"Using mingw32 compiler"},
	{C_RadioButton,rdbCyg,0,"Cygnus",NoList,CA_None,isSens,frmC,
		0,rdbMing,0,"Using Cygnus environment"},
	{C_Blank,blk3,0,"  ",NoList,CA_None,isSens,frmC,0,rdbCyg},
	{C_CheckBox,chkNoCyg,0,"-mno-cygwin",NoList,CA_None,isSens,frmC,
		blk3,rdbCyg,0,"Don't use Cygwin DLL"},
	{C_RadioButton,rdbBorland,0,"Borland BCC32",NoList,CA_None,isSens,frmC,
		0,chkNoCyg,0,"Use Borland BCC32 5.5 Compiler"},

	{C_RadioButton,rdbOther,0,"Other compiler",NoList,CA_None,isSens,frmC,
		0,rdbBorland,0,"Using some other compiler (you are on your own!)"},

	{C_Button, M_OK, 0, "   OK   ", NoList, CA_DefaultButton,
	    isSens, NoFrame, 0, frmC},
	{C_Button, M_Cancel, 0, " Cancel ",NoList,CA_None,
	    isSens,NoFrame,M_OK, frmC},
#else
	{C_Button, M_OK, 0, "   OK   ", NoList, CA_DefaultButton,
	    isSens, NoFrame, 0, frmM},
	{C_Button, M_Cancel, 0, " Cancel ",NoList,CA_None,
	    isSens,NoFrame,M_OK, frmM},
#endif

	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
    };
//@V@:EndDialogCmd


//======================>>> CNewProjMDlg::CNewProjMDlg <<<==================
  CNewProjMDlg::CNewProjMDlg(makefileMaker* m,
   vBaseWindow* bw, char* title) : vModalDialog(bw, title)
  {
    mm = m;
    cmdw = (videCmdWindow*) bw;
    AddDialogCmds(WCmds);		// add the predefined commands
  }

//===================>>> CNewProjMDlg::~CNewProjMDlg <<<====================
  CNewProjMDlg::~CNewProjMDlg()
  {
    UserDebug(Destructor,"CNewProjMDlg::~CNewProjMDlg() destructor\n")
  }

//====================>>> CNewProjMDlg::CNewProjWizard <<<====================
  int CNewProjMDlg::CNewProjWizard(char* msg)
  {
    ItemVal ans,rval;

    ans = ShowModalDialog(msg,rval);
    if (ans == M_Cancel)
	return 0;

    int isMing = GetValue(rdbMing);		// get if ming or cygwin
    int isCygwin = GetValue(rdbCyg);

#ifdef V_VersionWindows
    if (GetValue(rdbBorland))
      {
	setBorlandValues();
	return ans == M_OK;
      }
#endif

    if (GetValue(rdbCPP))			// c++ project
	strcpy(mm->cc,"g++");
    else
	strcpy(mm->cc,"gcc");

    strcpy(mm->Libs,"");			// no libs for simple console

    if (GetValue(rdbDbg))			// Debug version
      {
	strcpy(mm->cFlags,"-g");		// debug flag
      }
    else					// Releae version
      {
	strcpy(mm->cFlags,"-O");		// optimize flag
#ifdef V_VersionWindows
        strcpy(mm->Libs,"-s ");
#endif
      }

    GetTextIn(txiTgt,mm->exeName, 255);

#ifdef V_VersionX		// X X X X X X X

    if (GetValue(chkVAthena))			// stuff for V Athena
      {
	strcat(mm->Libs,"-lVx -lXaw -lXmu -lXt -lXext -lX11 ");
	mm->libDirs.insert(-1,"/usr/X11R6/lib");
	mm->incDirs.insert(-1,"/usr/X11R6/include");
	mm->curDefs.insert(-1,"-DAthena");
	mm->defsPool.insert(-1,"-DAthena");
      }

    if (GetValue(chkVMotif))			// stuff for V Motif
      {
	strcat(mm->Libs,"-lV -lXm -lXmu -lXt -lXext -lX11 ");
	mm->libDirs.insert(-1,"/usr/X11R6/lib");
	mm->incDirs.insert(-1,"/usr/X11R6/include");
      }

#endif				// X X X X X X X

#ifdef V_VersionWindows		// WINDOWS WINDOWS WINDOWS WINDOWS WINDOWS
    if (GetValue(rdbCyg))
      {
	if (GetValue(chkNoCyg))
	    strcat(mm->Libs,"-mno-cygwin ");
      }

    if (!GetValue(chkV) && !GetValue(chkVdll))
	strcpy(mm->homeV,"");

    if (GetValue(rdbCon))			// Console app
      {
	// nothing to do!
      }
    else if (GetValue(rdbGUI))			// GUI
      {
	strcat(mm->Libs,"-mwindows");
	if (GetValue(rdbCyg))			// cygwin?
	  {
	    strcat(mm->Libs," -e _mainCRTStartup");
	  }
	if (GetValue(chkV))			// stuff for V
	  {
	    strcat(mm->Libs," -lV -lcomctl32");
	    mm->libDirs.insert(-1,"$(HOMEV)/lib");
	    mm->incDirs.insert(-1,"$(HOMEV)/include");
	  }
	if (GetValue(chkVdll))			// stuff for V
	  {
	    strcat(mm->Libs," -lVDLL");
	    mm->libDirs.insert(-1,"$(HOMEV)/lib");
	    mm->incDirs.insert(-1,"$(HOMEV)/include");
	    mm->curDefs.insert(-1,"-DUSE_V_DLL");
	    mm->defsPool.insert(-1,"-DUSE_V_DLL");
	  }

	if (GetValue(chkGL))			// use OpenGL
	 {
	    if (GetValue(chkV))
		strcat(mm->Libs," -Vgl");
	    strcat(mm->Libs," -lglu32 -lopengl32");
	 }

      }
    else if (GetValue(rdbLib))		 	// Static Lib
      {
	// force mmaker to not generate the default all: target. We do it here.
      }
    else if (GetValue(rdbDLL))			// DLL
      {
      }
#endif		// WINDOWS WINDOWS WINDOWS

    return ans == M_OK;
  }

#ifdef V_VersionWindows
//====================>>> CNewProjMDlg::DialogDisplayed <<<====================
  void CNewProjMDlg::DialogDisplayed()
  {
    if (*((videApp*)theApp)->getBCCPath())
	SetValue(rdbBorland,1,Value);
  }

//====================>>> CNewProjMDlg::setBorlandValues <<<====================
  void CNewProjMDlg::setBorlandValues()
  {
    // set up values used for Borland BCC 5.5

    strcpy(mm->cc,"BCC32");

    strcpy(mm->cFlags,"");		// c only flag

    strcpy(mm->Libs,"");			// no libs for simple console

    mm->libDirs.insert(-1,"$(BCCROOT)\\lib");
    mm->libDirs.insert(-1,"$(BCCROOT)\\lib\\psdk");
    mm->incDirs.insert(-1,"$(BCCROOT)\\include");

    if (GetValue(rdbDbg))			// Debug version
      {
	strcat(mm->cFlags," -v");		// debug flag
	strcpy(mm->Libs,"-v ");			// linker, too
      }
    else					// Releae version
      {
	strcat(mm->cFlags," -O1 -v-");		// optimize size flag
	strcpy(mm->Libs,"-v- ");		// linker, too
      }

    if (!GetValue(chkV) && !GetValue(chkVdll))
	strcpy(mm->homeV,"");

    GetTextIn(txiTgt,mm->exeName, 255);

    mm->defsPool.insert(-1,"-D_RTLDLL");
    mm->userTargets.insert(-1,"# BCC32 RT lib: cw32.lib static");
    mm->userTargets.insert(-1,"BCC32RTLIB = cw32");
    mm->userTargets.insert(-1,"# BCC32 Startup: c0x32.obj-console, c0w32.obj-winapi");
    if (GetValue(rdbCon))			// Console app
      {
	mm->userTargets.insert(-1,"BCC32STARTUP = c0x32.obj");
	strcat(mm->Libs,"-Tpe -ap -c -limport32 -l$(BCC32RTLIB)");	// win .exe, console, case
      }
    else if (GetValue(rdbGUI))			// GUI
      {
	mm->userTargets.insert(-1,"BCC32STARTUP = c0w32.obj");
	strcat(mm->Libs,"-Tpe -aa -c -limport32 -l$(BCC32RTLIB)");	// win .exe, console, case

	if (GetValue(chkV))			// stuff for V
	  {
	    strcat(mm->Libs," -lvguibcc -lcomctl32");
	    mm->libDirs.insert(-1,"$(HOMEV)/lib");
	    mm->incDirs.insert(-1,"$(HOMEV)/include");
	  }
	if (GetValue(chkVdll))			// stuff for V
	  {
	    strcat(mm->Libs," -lvguidbcc -lcomctl32");
	    mm->libDirs.insert(-1,"$(HOMEV)/lib");
	    mm->incDirs.insert(-1,"$(HOMEV)/include");
	    mm->curDefs.insert(-1,"-DUSE_V_DLL");
	    mm->defsPool.insert(-1,"-DUSE_V_DLL");
	  }

	if (GetValue(chkGL))			// use OpenGL
	 {
	    if (GetValue(chkV))
		strcat(mm->Libs," -lVgl");
	    strcat(mm->Libs," -lglu32 -lopengl32");
	 }
      }
    else if (GetValue(rdbLib))		 	// Static Lib
      {
      }
    else if (GetValue(rdbDLL))			// DLL
      {
      }

  }
#endif

//====================>>> CNewProjMDlg::DialogCommand <<<====================
  void CNewProjMDlg::DialogCommand(ItemVal id, ItemVal retval, CmdType ctype)
  {
    UserDebug2(CmdEvents,"CNewProjMDlg::DialogCommand(id:%d, val:%d)\n",id, retval)

    switch (id)		// We will do some things depending on value
      {
#ifdef V_VersionWindows
	case chkV:
	    SetValue(chkVdll,0,Value);
	    break;
	case chkVdll:
	    SetValue(chkV,0,Value);
	    break;
#else
	case chkVMotif:
	    SetValue(chkVAthena,0,Value);
	    break;
	case chkVAthena:
	    SetValue(chkVMotif,0,Value);
	    break;
#endif
      }

    vModalDialog::DialogCommand(id,retval,ctype);
  }
