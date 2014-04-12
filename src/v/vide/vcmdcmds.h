//=======================================================================
//	videCmdw.cpp:	Source for videCmdwindow class
//  Copyright (C) 1995-1999  Bruce E. Wampler
//	date: 01 Mar 2000 01:43:34 PM
//
//  This program is part of the V C++ GUI Framework example programs.
//=======================================================================

// ###########################################################################
// Keep the menu bar, command bar, and status bar definitions
// here in a single file so that it will be easy to keep them all
// the same for the debug window
//
//	Start defines for the main window with 100

#ifndef VCMDCMDS_H
#define VCMDCMDS_H

    enum {
        lblMkFile = 4000,
	lblLineCol,			// Status Bar
	lblIns,
	lblSBlank,
	lblEMsg,
        lblStatTitle,
	frmTool,
	blkLast			// Last non-debug item
      };

//@V@:BeginPulldownMenu FileMenu
    static vMenu FileMenu[] =
      {
	{"&New", M_New, isSens, notChk, noKeyLbl, noKey, noSub},
	{"&Open...", M_Open, isSens, notChk, noKeyLbl, noKey, noSub},
        {"&View...", m_View, isSens, notChk, noKeyLbl, noKey, noSub},
	{"&Save", M_Save, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Save &as...", M_SaveAs, isSens, notChk, noKeyLbl, noKey, noSub},
	{"&Close", M_CloseFile, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Save All", m_saveAll, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Save / Close", M_SaveClose, isSens, notChk, noKeyLbl, noKey, noSub},
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"Send to Browser", m_sendToBrowser, isSens, notChk, noKeyLbl, noKey, noSub},
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
	{"E&xit", M_Exit, isSens, notChk, noKeyLbl, noKey, noSub},
	{NULL}
      };

//@V@:BeginPulldownMenu EditMenu
    static vMenu EditMenu[] =
      {
	{"Undo", M_UnDo, notSens, notChk, noKeyLbl, noKey, noSub},
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Cut  ", M_Cut, notSens, notChk, "^X", noKey, noSub},
	{"Copy ", M_Copy, notSens, notChk, "^C", noKey, noSub},
	{"Paste", M_Paste, notSens, notChk, "^V", noKey, noSub},
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Find...", edFind, isSens,notChk,noKeyLbl,noKey,noSub},
	{"Find Next", edFindNext, isSens,notChk,noKeyLbl,noKey,noSub},
	{"Replace...", edReplace, isSens,notChk,noKeyLbl,noKey,noSub},
	{"Find Matching Paren", edBalMatch, isSens,notChk,noKeyLbl,noKey,noSub},
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Set BP", m_SetBP, isSens,notChk,noKeyLbl,noKey,noSub},
	{"Delete BP", m_DeleteBP, isSens,notChk,noKeyLbl,noKey,noSub},
	{"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"Editor Help", edHelp, isSens, notChk, noKeyLbl, noKey, noSub},

	{NULL}
      };
//@V@:EndPulldownMenu

//@V@:BeginPulldownMenu OptMenu
    static vMenu OptMenu[] =
      {
	{"VIDE", m_VidePrefs,isSens,notChk,noKeyLbl,noKey,noSub},
	{"Editor", m_EditorPrefs,isSens,notChk,noKeyLbl,noKey,noSub},
	{"Font", M_Font,isSens,notChk,noKeyLbl,noKey,noSub},
	{NULL}
      };
//@V@:EndPulldownMenu

//@V@:BeginPulldownMenu MakeMenu
    static vMenu MakeMenu[] =
      {
        {"Make C++/Compile Java", m_MakeAll, notSens, notChk, noKeyLbl, noKey, noSub},
        {"Make clean", m_MakeCleanAll, notSens, notChk, noKeyLbl, noKey, noSub},
        {"Make cleanobj", m_MakeCleanObj, notSens, notChk, noKeyLbl, noKey, noSub},
        {"Make cleanbin", m_MakeCleanBin, notSens, notChk, noKeyLbl, noKey, noSub},
        {"Make <target>", m_MakeArgs, notSens, notChk, noKeyLbl, noKey, noSub},
//        {"Make Debug", m_MakeDebug, notSens, notChk, noKeyLbl, noKey, noSub},
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Generate ctags",m_GenCtags,isSens,notChk,noKeyLbl,noKey,noSub},
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"Stop Make", m_StopMake, notSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };
//@V@:EndPulldownMenu

//@V@:BeginPulldownMenu ToolsMenu
    static vMenu ToolsMenu[] =
      {
        {"Run program w/ args", m_RunCmdArgs, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Run project", m_RunProject, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Start Debugger", m_Debug, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Run OS Shell", m_RunShell, isSens, notChk, noKeyLbl, noKey, noSub},
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
        {"V App Gen", m_RunVgen, isSens, notChk, noKeyLbl, noKey, noSub},
        {"V Icon Editor", m_RunViconed, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };
//@V@:EndPulldownMenu

//@V@:BeginPulldownMenu NewJavaMenu
    static vMenu NewJavaMenu[] =
      {
	{"Applet", m_NewJavaApplet, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Windowed App", m_NewJavaWin, isSens, notChk, noKeyLbl, noKey, noSub},
	{"Console App", m_NewJavaConsole, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };
//@V@:EndPulldownMenu

//@V@:BeginPulldownMenu ProjectMenu
    static vMenu ProjectMenu[] =
      {
	{"&Open", m_ProjectOpen, isSens, notChk, noKeyLbl, noKey, noSub},
	{"&New C++ Project", m_ProjectNewC, isSens, notChk, noKeyLbl, noKey, noSub},
	{"New &Java Project", m_ProjectNewJava, isSens, notChk, noKeyLbl, noKey, NewJavaMenu},
	{"&Edit", m_ProjectEdit, notSens, notChk, noKeyLbl, noKey, noSub},
	{"&Close", m_ProjectClose, notSens, notChk, noKeyLbl, noKey, noSub},
	{"Save Project &as...", m_ProjectSaveAs, notSens, notChk, noKeyLbl, noKey, noSub},
	{"&Rebuild Makefile", m_MakeMake, notSens, notChk, noKeyLbl, noKey, noSub},
        {"-", M_Line, notSens, notChk, noKeyLbl, noKey, noSub},
 	{"Select Makefile or Java file", m_SelectMake, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };
//@V@:EndPulldownMenu

    static vMenu HelpMenu[] =
      {
        {"VIDE", videHelp, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Editor Command Set", edHelp, isSens, notChk, noKeyLbl, noKey, noSub},
#ifndef V_VersionX
        {"VIDE Help System", gnuHelp, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Win32 API", win32Help, isSens, notChk, noKeyLbl, noKey, noSub},
#endif
        {"V GUI", vguiHelp, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Java JDK", javaHelp, isSens, notChk, noKeyLbl, noKey, noSub},
        {"Java API", javaAPIHelp, isSens, notChk, noKeyLbl, noKey, noSub},
        {"HTML", htmlHelp, isSens, notChk, noKeyLbl, noKey, noSub},
        {"HTML - CSS", htmlCSSHelp, isSens, notChk, noKeyLbl, noKey, noSub},
        {"-", M_Line, isSens, notChk, noKeyLbl, noKey, noSub},
        {"About V IDE", m_AboutVide, isSens, notChk, noKeyLbl, noKey, noSub},
        {NULL}
      };

//@V@:BeginMenu StandardMenu
    static vMenu StandardMenu[] =
      {
	{"&File", M_File, isSens, notUsed, notUsed, noKey, &FileMenu[0]},
	{"&Edit", M_Edit, isSens, notUsed, notUsed, noKey, &EditMenu[0]},
        {"&Project",m_Project,isSens,notUsed,notUsed,noKey,&ProjectMenu[0]},
	{"&Build", m_Build, isSens, notUsed, notUsed, noKey, &MakeMenu[0]},
        {"&Tools", m_Tools, isSens, notUsed, notUsed, noKey, &ToolsMenu[0]},
        {"&Options", m_Options, isSens, notUsed, notUsed, noKey, &OptMenu[0]},
        {"&Help", m_Help, isSens, notUsed, notUsed, noKey, &HelpMenu[0]},
	{NULL}
      };
//@V@:EndMenu
//vbm8
#define new_depth 8
#define new_width 20
#define new_height 20
static unsigned char new_bits[] = {
3,
0,0,0,
255,255,255,
0,255,255,
192,192,192,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,
3,3,3,3,3,0,2,1,2,1,2,0,0,3,3,3,3,3,3,3,
3,3,3,3,3,0,1,2,1,2,1,0,2,0,3,3,3,3,3,3,
3,3,3,3,3,0,2,1,2,1,2,0,0,0,0,3,3,3,3,3,
3,3,3,3,3,0,1,2,1,2,1,2,1,2,0,3,3,3,3,3,
3,3,3,3,3,0,2,1,2,1,2,1,2,1,0,3,3,3,3,3,
3,3,3,3,3,0,1,2,1,2,1,2,1,2,0,3,3,3,3,3,
3,3,3,3,3,0,2,1,2,1,2,1,2,1,0,3,3,3,3,3,
3,3,3,3,3,0,1,2,1,2,1,2,1,2,0,3,3,3,3,3,
3,3,3,3,3,0,2,1,2,1,2,1,2,1,0,3,3,3,3,3,
3,3,3,3,3,0,1,2,1,2,1,2,1,2,0,3,3,3,3,3,
3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
};
    static vIcon newI(&new_bits[0],new_height,new_width,new_depth,Transparent);


//vbm8
#define open_depth 8
#define open_width 20
#define open_height 20
static unsigned char open_bits[] = {
4,
0,0,0,
255,255,255,
192,192,192,
255,255,0,
128,128,0,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,0,2,2,2,0,2,0,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,2,2,2,
2,2,2,0,0,0,2,2,2,2,2,2,2,2,0,0,0,2,2,2,
2,2,0,1,3,1,0,0,0,0,0,0,0,2,2,2,2,2,2,2,
2,2,0,3,1,3,1,3,1,3,1,3,0,2,2,2,2,2,2,2,
2,2,0,1,3,1,3,1,3,1,3,1,0,2,2,2,2,2,2,2,
2,2,0,3,1,3,1,0,0,0,0,0,0,0,0,0,0,0,2,2,
2,2,0,1,3,1,0,4,4,4,4,4,4,4,4,4,0,2,2,2,
2,2,0,3,1,0,4,4,4,4,4,4,4,4,4,0,2,2,2,2,
2,2,0,1,0,4,4,4,4,4,4,4,4,4,0,2,2,2,2,2,
2,2,0,0,4,4,4,4,4,4,4,4,4,0,2,2,2,2,2,2,
2,2,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};
    static vIcon openI(&open_bits[0],open_height,open_width,open_depth,Transparent);

//vbm8
#define save_depth 8
#define save_width 20
#define save_height 20
static unsigned char save_bits[] = {
3,
0,0,0,
255,255,255,
192,192,192,
128,128,0,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,
2,2,2,0,3,0,2,2,2,2,2,2,2,2,0,1,0,2,2,2,
2,2,2,0,3,0,2,2,2,2,2,2,2,2,0,0,0,2,2,2,
2,2,2,0,3,0,2,2,2,2,2,2,2,2,0,3,0,2,2,2,
2,2,2,0,3,0,2,2,2,2,2,2,2,2,0,3,0,2,2,2,
2,2,2,0,3,0,2,2,2,2,2,2,2,2,0,3,0,2,2,2,
2,2,2,0,3,0,2,2,2,2,2,2,2,2,0,3,0,2,2,2,
2,2,2,0,3,3,0,0,0,0,0,0,0,0,3,3,0,2,2,2,
2,2,2,0,3,3,3,3,3,3,3,3,3,3,3,3,0,2,2,2,
2,2,2,0,3,3,0,0,0,0,0,0,0,0,0,3,0,2,2,2,
2,2,2,0,3,3,0,0,0,0,0,0,2,2,0,3,0,2,2,2,
2,2,2,0,3,3,0,0,0,0,0,0,2,2,0,3,0,2,2,2,
2,2,2,0,3,3,0,0,0,0,0,0,2,2,0,3,0,2,2,2,
2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};

    static vIcon saveI(&save_bits[0],save_height,save_width,save_depth,Transparent);

//vbm8
#define openprj_depth 8
#define openprj_width 20
#define openprj_height 20
static unsigned char openprj_bits[] = {
6,
0,0,0,
255,255,255,
192,192,192,
128,128,128,
0,255,255,
0,0,128,
255,255,0,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,0,4,1,4,1,0,0,2,2,
2,2,2,2,2,2,2,0,2,2,2,0,1,5,5,4,0,0,0,2,
2,2,2,2,2,2,2,0,0,2,2,0,4,1,4,1,4,1,0,2,
2,2,2,2,3,0,0,0,0,0,2,0,1,5,5,5,5,4,0,2,
2,2,2,3,0,0,0,0,0,0,2,0,4,1,4,1,4,1,0,0,
2,2,2,0,0,3,2,0,0,2,2,0,1,5,5,5,5,4,0,1,
2,2,2,0,0,2,2,0,2,2,2,0,4,1,4,1,4,1,0,4,
2,2,2,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,1,
2,2,2,2,2,2,2,2,2,2,2,2,2,0,1,5,5,5,1,4,
2,2,0,0,0,0,0,2,2,2,2,2,2,0,4,1,4,1,4,1,
2,2,0,6,1,6,0,2,2,2,2,2,2,0,1,5,5,5,5,4,
2,0,0,0,0,0,0,0,0,0,0,0,2,0,4,1,4,1,4,1,
2,0,1,6,1,6,1,6,1,6,1,0,2,0,0,0,0,0,0,0,
2,0,6,1,6,1,6,1,6,1,6,0,2,2,2,2,2,2,2,2,
2,0,1,6,1,6,1,6,1,6,1,0,2,2,2,2,2,2,2,2,
2,0,6,1,6,1,6,1,6,1,6,0,2,2,2,2,2,2,2,2,
2,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};
    static vIcon openprjI(&openprj_bits[0],openprj_height,openprj_width,openprj_depth,Transparent);

//vbm8
#define find_depth 8
#define find_width 20
#define find_height 20
static unsigned char find_bits[] = {
6,
0,0,0,
255,255,255,
192,192,192,
128,128,128,
255,0,0,
255,255,0,
128,0,0,
2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,0,1,1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
0,2,1,1,1,0,4,2,2,2,2,2,2,2,2,2,2,2,2,2,
0,2,2,1,1,1,0,4,2,2,2,2,2,2,2,2,2,2,2,2,
2,0,2,2,1,1,1,0,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,0,2,2,1,1,1,0,0,2,2,2,2,2,2,2,2,2,2,
2,2,2,0,2,2,1,1,1,1,0,0,0,2,2,2,2,2,2,2,
2,2,2,2,0,2,2,1,1,4,4,4,4,0,2,2,2,2,2,2,
2,2,2,2,2,0,2,2,6,4,4,4,0,1,1,1,1,1,1,1,
2,2,2,2,2,0,3,6,6,6,4,0,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,0,6,6,3,0,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,0,6,3,0,5,1,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,0,6,0,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,0,5,1,5,1,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,5,1,5,1,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,5,1,5,1,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,5,1,5,1,5,1,5,1,5,1,5,1
};

    static vIcon findI(&find_bits[0],find_height,find_width,find_depth,Transparent);

//vbm8
#define findagn_depth 8
#define findagn_width 20
#define findagn_height 20
static unsigned char findagn_bits[] = {
6,
0,0,0,
255,255,255,
192,192,192,
128,128,128,
255,0,0,
255,255,0,
128,0,0,
2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,0,1,1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
0,2,1,1,1,0,4,2,2,2,2,2,2,2,2,2,2,2,2,2,
0,2,2,1,1,1,0,4,2,2,2,2,2,2,2,2,2,2,2,2,
2,0,2,2,1,1,1,0,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,0,2,2,1,1,1,0,0,2,2,2,2,2,2,2,2,2,2,
2,2,2,0,2,2,1,1,1,1,0,0,0,2,2,2,2,2,2,2,
2,2,2,2,0,2,2,1,1,4,4,4,4,0,2,2,2,2,2,2,
2,2,2,2,2,0,2,2,6,4,4,4,0,1,1,1,1,1,1,1,
2,2,2,2,2,0,3,6,6,6,4,0,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,0,6,6,3,0,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,0,6,3,0,5,1,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,0,6,0,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,0,5,1,5,1,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,5,1,5,1,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,5,0,0,1,5,0,0,1,5,0,0,1,
2,2,2,2,2,2,2,2,1,0,0,1,1,0,0,1,1,0,0,1,
2,2,2,2,2,2,2,2,5,1,5,1,5,1,5,1,5,1,5,1
};
    static vIcon findagnI(&findagn_bits[0],findagn_height,findagn_width,
	findagn_depth,Transparent);

//vbm8
#define replace_depth 8
#define replace_width 20
#define replace_height 20
static unsigned char replace_bits[] = {
6,
0,0,0,
255,255,255,
192,192,192,
128,128,128,
255,0,0,
255,255,0,
128,0,0,
2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,0,1,1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
0,2,1,1,1,0,4,2,2,2,2,2,2,2,2,2,2,2,2,2,
0,2,2,1,1,1,0,4,2,2,2,2,2,2,2,2,2,2,2,2,
2,0,2,2,1,1,1,0,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,0,2,2,1,1,1,0,0,2,2,2,2,2,2,2,2,2,2,
2,2,2,0,2,2,1,1,1,1,0,0,0,2,2,2,2,2,2,2,
2,2,2,2,0,2,2,1,1,4,4,4,4,0,2,2,2,2,2,2,
2,2,2,2,2,0,2,2,6,4,4,4,0,1,1,1,1,1,1,1,
2,2,2,2,2,0,3,6,6,6,4,0,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,0,6,6,3,0,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,0,6,3,0,5,1,5,1,5,1,5,1,5,1,
2,2,2,2,2,2,0,6,0,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,0,5,0,5,1,5,1,5,1,5,0,0,1,
2,2,2,2,2,2,2,2,0,1,0,1,1,0,1,1,1,0,1,0,
2,2,2,2,2,2,2,2,0,1,0,1,5,0,0,1,5,0,5,0,
2,2,2,2,2,2,2,2,0,1,0,1,0,0,0,0,1,0,0,1,
2,2,2,2,2,2,2,2,0,0,0,1,5,0,0,1,5,0,5,0,
2,2,2,2,2,2,2,2,0,1,0,1,1,0,1,1,1,0,1,0,
2,2,2,2,2,2,2,2,0,1,0,1,5,1,5,1,5,0,0,1
};
    static vIcon replaceI(&replace_bits[0],replace_height,replace_width,
	replace_depth,Transparent);

//vbm8
#define make_depth 8
#define make_width 20
#define make_height 20
static unsigned char make_bits[] = {
3,
0,0,0,
255,255,255,
255,255,0,
192,192,192,
3,3,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,0,2,1,2,1,0,3,3,3,3,3,3,3,3,3,3,3,3,3,
0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,3,3,
0,1,2,1,2,1,2,1,2,1,2,0,3,3,0,3,3,0,3,3,
0,2,1,2,1,2,1,2,1,2,1,0,3,0,3,3,3,0,3,3,
0,1,2,1,2,1,2,1,2,1,2,0,3,3,3,3,3,0,3,3,
0,2,1,2,1,2,1,2,1,2,1,0,3,3,3,3,0,0,3,3,
0,1,2,1,2,1,2,1,2,1,2,0,3,3,3,3,0,3,3,3,
0,2,1,2,1,2,1,2,1,2,1,0,3,3,3,3,0,3,3,3,
0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,0,3,3,0,3,3,3,0,3,3,3,0,3,3,0,3,3,3,0,
0,0,3,0,3,0,3,0,3,0,3,0,0,3,0,3,0,3,0,0,
3,0,3,0,3,0,3,0,3,0,3,3,0,3,0,3,0,3,3,0,
3,0,3,0,3,0,3,0,3,0,3,3,0,3,0,3,0,3,3,0,
3,0,3,0,3,0,3,0,3,0,3,3,0,3,0,3,0,3,3,0,
3,0,3,3,0,3,3,3,0,3,3,3,0,3,3,0,3,3,3,0,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
};
    static vIcon makeI(&make_bits[0],make_height,make_width,make_depth,Transparent);


//vbm8
#define vrun_depth 8
#define vrun_width 20
#define vrun_height 20
static unsigned char vrun_bits[] = {
2,
0,0,0,
192,192,192,
255,255,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,1,1,1,0,2,2,2,2,2,2,0,1,1,1,1,1,
1,1,1,1,1,0,0,2,2,2,2,2,2,0,1,1,1,1,1,1,
1,1,1,1,0,2,2,2,2,2,2,2,0,1,1,1,1,1,1,1,
1,1,1,0,0,0,0,0,0,2,2,2,2,0,0,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,0,2,2,2,2,2,0,1,1,1,1,
1,1,1,1,1,1,1,1,0,2,2,2,2,2,0,1,1,1,1,1,
1,1,1,1,1,1,1,0,2,2,2,2,2,0,1,1,1,1,1,1,
1,1,1,1,1,1,0,2,2,2,2,2,0,1,1,1,1,1,1,1,
1,1,1,1,1,1,0,0,0,0,0,2,2,0,0,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,2,2,0,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,0,2,2,0,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,2,2,0,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,0,2,2,0,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,0,2,0,0,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};
    static vIcon vrunI(&vrun_bits[0],vrun_height,vrun_width,vrun_depth,Transparent);

//vbm8
#define undo_depth 8
#define undo_width 20
#define undo_height 20
static unsigned char undo_bits[] = {
4,
0,0,0,
0,128,0,
192,192,192,
128,128,128,
0,255,0,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,0,4,0,0,0,0,0,0,0,2,2,2,2,2,2,2,
2,2,2,0,4,4,4,4,4,4,4,4,4,0,0,2,2,2,2,2,
2,2,0,4,4,4,4,4,4,4,4,4,4,4,1,0,2,2,2,2,
2,2,2,0,4,4,4,4,4,4,4,4,4,1,4,1,3,2,2,2,
2,2,2,2,0,4,0,0,0,0,0,0,0,4,1,1,0,2,2,2,
2,2,2,2,2,0,0,2,2,2,2,2,3,0,0,1,0,2,2,2,
2,2,2,2,2,2,0,2,2,2,2,2,2,2,3,0,0,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,3,0,0,1,0,2,2,2,
2,2,2,2,2,2,2,2,0,0,0,0,0,4,1,1,0,2,2,2,
2,2,2,2,2,2,2,2,0,4,4,4,4,1,4,1,3,2,2,2,
2,2,2,2,2,2,2,2,0,4,4,4,4,4,1,0,2,2,2,2,
2,2,2,2,2,2,2,2,0,4,4,4,4,0,0,2,2,2,2,2,
2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};
    static vIcon undoI(&undo_bits[0],undo_height,undo_width,undo_depth,Transparent);

//vbm8
#define debug_depth 8
#define debug_width 20
#define debug_height 20
static unsigned char debug_bits[] = {
3,
0,0,0,
191,191,191,
255,0,0,
255,255,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,
1,0,0,1,1,1,1,0,3,3,3,0,1,1,1,1,0,0,1,1,
1,1,1,0,1,1,0,2,0,3,0,2,0,1,1,0,1,1,1,1,
1,1,1,1,0,1,0,3,3,3,3,3,0,1,0,1,1,1,1,1,
1,1,1,1,1,0,3,3,3,0,3,3,3,0,1,1,1,1,1,1,
1,1,1,1,1,0,2,2,3,3,3,2,2,0,1,1,1,1,1,1,
1,1,0,0,0,0,2,2,2,0,2,2,2,0,0,0,0,1,1,1,
1,1,1,1,1,0,2,2,3,3,3,2,2,0,1,1,1,1,1,1,
1,1,1,1,1,0,3,3,3,0,3,3,3,0,1,1,1,1,1,1,
1,1,1,1,0,1,0,3,3,3,3,3,0,1,0,1,1,1,1,1,
1,1,1,0,1,1,0,2,2,0,2,2,0,1,1,0,1,1,1,1,
1,0,0,1,1,1,1,0,2,2,2,0,1,1,1,1,0,0,1,1,
1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

    static vIcon debugI(&debug_bits[0],debug_height,debug_width,debug_depth,Transparent);

//@V@:BeginCmdPane ToolBar
#define SQUISH
#ifdef SQUISH
//#define STYLE CA_Flat
#define STYLE CA_None
    static CommandObject ToolBar[] =
      {
	{C_Frame,frmTool,frmTool,"",NoList,CA_NoSpace|CA_NoBorder,isSens,NoFrame,0,0,0,""},
	{C_IconButton,M_New,0,"New",&newI,STYLE,isSens,frmTool,0,0,0,"New File"},
	{C_IconButton,M_Open,0,"Open",&openI,STYLE,isSens,frmTool,M_New,0,0,"Open File"},
	{C_IconButton,M_Save,0,"Save",&saveI,STYLE,isSens,frmTool,M_Open,0,0,"Save File"},
	{C_IconButton,m_ProjectOpen,0,"OpenProject",&openprjI,STYLE,isSens,frmTool,M_Save,0,0,"Open Project"},
	{C_Blank,990,0," ",NoList,CA_None,isSens,frmTool,m_ProjectOpen,0},
	{C_IconButton,M_UnDo,0,"Undo",&undoI,STYLE,notSens,frmTool,990,0,0,"Undo"},
	{C_IconButton,edFind,0,"Find",&findI,STYLE,isSens,frmTool,M_UnDo,0,0,"Find"},
	{C_IconButton,edFindNext,0,"Find Next",&findagnI,STYLE,isSens,frmTool,edFind,0,0,"Find Next"},
	{C_IconButton,edReplace,0,"Replace",&replaceI,STYLE,isSens,frmTool,edFindNext,0,0,"Replace"},
        {C_Blank,991,0," ",NoList,CA_None,isSens,frmTool,edReplace,0},
        {C_IconButton,m_MakeAll,0,"MAKE",&makeI,STYLE,notSens,frmTool,991,0,0,"make all C++ / Compile Java"},
	{C_IconButton,m_Debug,0,"Debug",&debugI,STYLE,isSens,frmTool,m_MakeAll,0,0,"Start debugger"},
	{C_IconButton,m_RunProject,0,"Run",&vrunI,STYLE,isSens,frmTool,m_Debug,0,0,"Run current program"},

  	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };
#else
#define STYLE CA_None
    static CommandObject ToolBar[] =
      {
	{C_IconButton,M_New,0,"New",&newI,CA_None,isSens,NoFrame,0,0,0,"New File"},
	{C_IconButton,M_Open,0,"Open",&openI,CA_None,isSens,NoFrame,0,0,0,"Open File"},
	{C_IconButton,M_Save,0,"Save",&saveI,CA_None,isSens,NoFrame,0,0,0,"Save File"},
	{C_IconButton,m_ProjectOpen,0,"OpenProject",&openprjI,CA_None,isSens,NoFrame,0,0,0,"Open Project"},
	{C_Blank,99,0," ",NoList,CA_None,isSens,NoFrame,0,0},
	{C_IconButton,M_UnDo,0,"Undo",&undoI,CA_None,notSens,NoFrame,0,0,0,"Undo"},
	{C_IconButton,edFind,0,"Find",&findI,CA_None,isSens,NoFrame,0,0,0,"Find"},
	{C_IconButton,edFindNext,0,"Find Next",&findagnI,CA_None,isSens,NoFrame,0,0,0,"Find Next"},
	{C_IconButton,edReplace,0,"Replace",&replaceI,CA_None,isSens,NoFrame,0,0,0,"Replace"},
        {C_Blank,99,0," ",NoList,CA_None,isSens,NoFrame,0,0},
        {C_IconButton,m_MakeAll,0,"MAKE",&makeI,CA_None,notSens,NoFrame,0,0,0,"make all C++ / Compile Java"},
	{C_IconButton,m_Debug,0,"Debug",&debugI,CA_None,isSens,NoFrame,0,0,0,"Start debugger"},
	{C_IconButton,m_RunProject,0,"Run",&vrunI,CA_None,isSens,NoFrame,0,0,0,"Run current program"},

  	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };
#endif
//@V@:EndCmdPane

//@V@:BeginStatPane StatBar
    static vStatus StatBar[] =
      {
        {" V IDE ", lblStatTitle, CA_NoBorder, isSens, 0},
	{"1/1          .", lblLineCol, CA_None, isSens, 0},
	{"Command........", lblIns, CA_None, isSens, 0},
	// handle messages this big
#ifdef V_VersionX
	{".                                                  .",
#else
	{"                                                    ",
#endif
		 lblEMsg, /* CA_NoBorder */ CA_NoBorder, isSens, 0},
	{0,0,0,0,0}
      };
//@V@:EndStatPane

// ###########################################################################
#endif
