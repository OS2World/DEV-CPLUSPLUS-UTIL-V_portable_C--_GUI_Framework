//=======================================================================
// V IDE Java Class - all the stuff interface with Java
//  Copyright (C) 1995-1999  Bruce E. Wampler
//
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
//=======================================================================

#include <v/vfilesel.h>
#include <v/vnotice.h>

#include "videapp.h"

#include "videcmdw.h"

#include "videjava.h"
//#include <direct.h>

//=====================>>> Constants <<<===========================

  static const char SecCompiler[] = "//% Section 1 - COMPILER";
  static const char SecCompilerOpts[] = "//% Section 2 - COMPILER OPTIONS";
  static const char SecCompilerOptsEndPool[] = "//%compiler-opts-pool-end";
  static const char SecCompilerOptsEnd[] = "//%compiler-opts-end";
  static const char SecInterp[] = "//% Section 3 - INTERPRETER";
  static const char SecInterpOpts[] = "//% Section 4 - INTERPRETER OPTIONS";
  static const char SecInterpOptsEndPool[] = "//%interp-opts-pool-end";
  static const char SecInterpOptsEnd[] = "//%interp-opts-end";
  static const char SecDebugger[] = "//% Section 5 - DEBUGGER";
  static const char SecDebuggerOpts[] = "//% Section 6 - DEBUGGER OPTIONS";
  static const char SecDebuggerOptsEndPool[] = "//%debug-opts-pool-end";
  static const char SecDebuggerOptsEnd[] = "//%debug-opts-end";
  static const char SecClassPath[] = "//% Section 7 - CLASSPATH";
  static const char SecAppletHTML[] = "//% Section 8 - APPLET HTML SORUCE";
  static const char SecMainFile[] = "//% Section 9 - TOP LEVEL JAVA FILE";
  static const char SecSrcDir[] = "//% Section 10 - SOURCE DIRECTORY";
  static const char SecSrcFiles[] = "//% Section 11 - SOURCE FILES";
  static const char SecSrcFilesEnd[] = "//%src-files-end";

  static const int fieldLen = 256;

//=====================>>> videJava::videJava <<<===========================
  videJava::videJava(videCmdWindow* parent)
  {
    myParent = parent;
    cmdw = ((videApp*)theApp)->GetMsgWindow();

    compilerName = new char[fieldLen];
    interpName = new char[fieldLen];
    debugName = new char[fieldLen];
    classPath = new char[fieldLen];
    appletHTML = new char[fieldLen];
    mainFile = new char[fieldLen];
    srcDir = new char[fieldLen];
    setDefaults();
  }

//=====================>>> videJava::~videJava <<<===========================
  videJava::~videJava()
  {
    delete [] compilerName;
    delete [] interpName;
    delete [] debugName;
    delete [] classPath;
    delete [] appletHTML;
    delete [] mainFile;
    delete [] srcDir;

  }

//======================>>> videJava::eraseAll <<============================
  void videJava::eraseAll()
  {
    strcpy(compilerName,"");
    strcpy(interpName,"");
    strcpy(debugName,"");
    strcpy(classPath,"");
    strcpy(appletHTML,"");
    strcpy(mainFile,"");
    strcpy(srcDir,"");

    compOptsPool.erase();
    compOpts.erase();
    interpOptsPool.erase();
    interpOpts.erase();
    debugOptsPool.erase();
    debugOpts.erase();
    srcFiles.erase();
  }

//======================>>> videJava::setDefaults <<============================
  void videJava::setDefaults(char* mainName, ItemVal id)
  {
    vOS vos;

    //erase previous definitions
    eraseAll();

    // init each one

    strcpy(compilerName,"javac");
    strcpy(interpName,"java");
    strcpy(debugName,"jdb");

//    if (vos.vGetEnvVal("CLASSPATH",classPath,fieldLen) < 1)
	strcpy(classPath,"<none>");

    strcpy(appletHTML,"<none>");	// every item needs a value

    strcpy(mainFile,mainName);

    strcpy(srcDir,".");

    compOptsPool.insert(-1,"-d . (Compilation output dir)");
    compOptsPool.insert(-1,"-g (Full Debugging Info)");
    compOptsPool.insert(-1,"-g:none (No Debugging Info)");
    compOptsPool.insert(-1,"-O (Optimize compiled code)");
    compOptsPool.insert(-1,"-nowarn (No warnings)");
    compOptsPool.insert(-1,"-verbose (more messages)");
    compOptsPool.insert(-1,"-deprecation (Deprecated API)");

    interpOptsPool.insert(-1,"-verbose (more output)");

    switch (id)
      {
	case m_NewJavaApplet:
	    strcpy(appletHTML,mainFile);
	    strcat(appletHTML,".html");
	    strcpy(interpName,"appletviewer");
	    strcpy(debugName,"appletviewer -debug");
	    break;

	case m_NewJavaWin:
	    strcpy(interpName,"javaw");
	    break;

	case m_NewJavaConsole:
	    break;

      }
  }

//======================>>> videJava::stripDir <<<============================
  char* videJava::stripDir(char* n)
  {
    for (int ix = strlen(n) - 1 ; ix >= 0 ; --ix)
      {
	if (n[ix] == '/' || n[ix] == '\\')
	  {
	    return &n[ix+1];
	  }
      }
    return n;
 }

//======================>>> videJava::addFile <<<============================
  bool videJava::addFile(char *n)
  {
    //adds a new source file to the list

    if (!*n)
	return false;

    // Filter name here... (add a behavior switch eventually)

    char* addName = stripDir(n);	// strip leading dir part

    for (int ix = 0 ; srcFiles.list[ix] != 0 ; ++ix)
	if (strcmp(srcFiles.list[ix], addName) == 0)
	    return false; //don't add a obj file twice

    srcFiles.insert(-1,addName);
    return true;
  }

//=====================>>> videJava::CompileJava <<<===========================
  void videJava::CompileJava(videCmdWindow* parent)
  {
    char cmd[maxFileNameSize];
    char mkpath[maxFileNameSize];
    char makeFile[maxFileNameSize];
    vOS vos;

    static char makeCmds[] = "makecmds.vtm" ;
    static char makeErrs[] = "makeerrs.vtm" ;

    strcpy(mkpath,((videApp*)theApp)->GetMkFile());
    cmdw = ((videApp*)theApp)->GetMsgWindow();

    // We will either be running off a Java file specified only as
    // a Makefile, or via the attributes in the project file.

    if (!*mkpath)
      {
        vNoticeDialog note((vWindow*)cmdw);
        note.Notice("No Java File specified. Use Select Makefile or Open Project first.");
        return;
      }

    if (strstr(mkpath,".java") == 0)	// no .java
	strcat(mkpath,".java");

    cmdw->RaiseWindow();
    cmdw->SetRdOnly(0);
    (cmdw->GetTextEd())->SetFileType(gccError);

    // Now, change directory
    strcpy(cmd,mkpath);
    int ix = strlen(cmd);
    while (ix > 0)
      {
        if (cmd[ix] == '\\' || cmd[ix] == '/')
          {
            cmd[ix] = 0;
            break;
          }
        --ix;
      }

    if (ix > 0)
      {
	strcpy(makeFile, &cmd[ix+1]);	// copy the makefile name
	vos.vChDir(cmd);			// change dir
      }
    else
    	strcpy(makeFile,cmd);

//@@ java compiler + switches...

    strcpy(cmd, compilerName);		// make or make clean passed from caller

    if (*classPath && strcmp(classPath,"<none>") != 0)
      {
	strcat(cmd," -classpath ");
	strcat(cmd, classPath);
      }

    for (int ixa = 0 ; compOpts.list[ixa] != 0 ; ++ixa)	// add switches
      {
	strcat(cmd," ");
	strcat(cmd,compOpts.list[ixa]);
      }

    // Finally, the file...
    strcat(cmd, " ");
    strcat(cmd, makeFile);		// the name of the make file.

    vos.vDeleteFile(makeCmds);
    vos.vDeleteFile(makeErrs);

    cmdw->AddLine("Running Java compiler, please wait...");
    cmdw->AddLine(cmd);
    theApp->CheckEvents();

    if (vos.vRunProcess(cmd,makeCmds,makeErrs,1,1) != 0)
      {
        ShowErrors(cmdw,makeErrs);
        ShowErrors(cmdw,makeCmds);
	cmdw->AddLine("--------");
	cmdw->AddLine("Compile failed. Right click on the error line (filename:###:msg) to open file.");
	cmdw->SetRdOnly(1);
	cmdw->RaiseWindow();
	theApp->CheckEvents();
	return;
      }

    ShowErrors(cmdw,makeErrs);
    ShowErrors(cmdw,makeCmds);
    cmdw->AddLine("----Compile Done----");
    cmdw->SetRdOnly(1);
    cmdw->RaiseWindow();
    theApp->CheckEvents();
    return;
  }

//=====================>>> videJava::getDebugName <<<===========================
  char* videJava::getDebugName()
  {
    // need to tack on the debugger options here to make the
    // rest of the code work correctly. Happens here because
    // gdb and jdb share code later.

    static char dname[maxFileNameSize+80];

    strcpy(dname,debugName);	// the debugger name
    for (int ix = 0 ; debugOpts.list[ix] != 0 ; ++ix)
      {
	strcat(dname," ");
	strcat(dname,debugOpts.list[ix]);
      }
    return dname;
  }

//=====================>>> videJava::RunJava <<<===========================
  void videJava::RunJava(videCmdWindow* parent, const char* javaprog)
  {

    vOS vos;

    static char makeErrs[] = "javaerrs.vtm" ;

    char runit[maxFileNameSize+80];
    char path[maxFileNameSize+80];
    char prog[maxFileNameSize];
    bool needConsole = false;

    strcpy(runit,interpName);	// use interp provided

#ifdef V_VersionX
    if (strstr(runit,"javaw") != 0)	// GUI - assume no console
	strcpy(runit,"java");		// Unix only has java, no javaw
    else
	needConsole = true;
#endif

    cmdw = ((videApp*)theApp)->GetMsgWindow();

    path[0] = 0;		// no path by default


    //@@ Probably need to do something completely different
    //@@ for HTML applets and appletviewer applets - like
    //@@ launch the browser if HTML supplied, otherwise
    //@@ use the viewer.

    if (*appletHTML == 0 || strcmp(appletHTML,"<none>") == 0)
	strcpy(prog, javaprog);	// a Java app
    else
      {
	strcpy(runit, "appletviewer");	// use interp provided
	strcpy(prog, appletHTML);	// an applet .html file
      }

    for (int ix = strlen(javaprog)-1 ; ix > 0 ; --ix)
      {
	if (javaprog[ix] == '\\' || javaprog[ix] == '/')  // path...
	  {
	    strcpy(path,javaprog);
	    strcpy(prog, &path[ix+1]);		// this will be the prog
	    path[ix] = 0;			// and zap the prog
	    break;
	  }
      }
    char* jp = strstr(prog,".java");
    if (jp != 0)
	*jp = 0;			// strip the .java

    if (*classPath && strcmp(classPath,"<none>") != 0 &&
	strcmp(runit,"appletviewer") != 0)
      {
	strcat(runit," -cp ");
	strcat(runit, classPath);
      }

    for (int ixb = 0 ; interpOpts.list[ixb] != 0 ; ++ixb)	// add switches
      {
	strcat(runit," ");
	strcat(runit,interpOpts.list[ixb]);
      }

    strcat(runit," ");
    strcat(runit,prog);

    cmdw->AddLine("Starting Java interpreter...");
    cmdw->AddLine(runit);
    theApp->CheckEvents();

    if (needConsole)
      {
#ifdef V_VersionX
	char buff[maxFileNameSize+2];
	strcpy(buff,((videApp*)theApp)->getVideShell());
	strcat(buff," -e \"");
	strcat(buff,runit);
	strcat(buff,"\"");
	(void) vos.vRunProcess(buff, 0, 0, 0);
#else
	(void) vos.vRunProcess(runit,0,0,0,0);
#endif
      }
    else
	(void) vos.vRunProcess(runit,0,0,0,0);

    cmdw->AddLine("--------");
    theApp->CheckEvents();

  }

//=====================>>> videJava::ShowErrors <<<=========================
  void videJava::ShowErrors(videCmdWindow* cmdw, char* errName)
  {
    const int maxBuff = 510;
    char buff[maxBuff+2];
    char displayed[maxBuff+20];
    vOS vos;

    ifstream errFile(errName);

    if (!errFile)
        return;

    while (errFile.getline(buff,maxBuff))
      {
        strcpy(displayed,"  ! ");	// IMPORTANT!  videCmdWindow::GotoErrorLine
        strcat(displayed,buff);         //	REQUIRES this exact format!!!
        cmdw->AddLine(displayed);

      }
    theApp->CheckEvents();
    errFile.close();
    vos.vDeleteFile(errName);
  }

//=====================>>> videJava::isJavaProject <<<=========================
  bool videJava::isJavaProject(const char* pname)
  {
    ifstream prjf(pname);
    char skipStr[256] = "";
    if (!prjf)
	return false;
//    prjf.getline(skipStr,256, '\n');
    prjf.getline(skipStr,256);
    if (strstr(skipStr,"//Java") != 0)	// Java projects have //Java as first line
	return true;
    return false;
  }

//======================>>> videJava::loadJProject <<<============================
  void videJava::loadJProject(const char *n)
  {
    //loads infos contained in project file n into member vars

    ifstream prjf(n);
    if (!prjf)
	return;

    eraseAll();
  

    //% Section 1 - COMPILER
    if (!getSectionEntry(prjf, SecCompiler, compilerName ))
	return;

    //% Section 2 - COMPILER OPTIONS
    if (!getSectionList(prjf, SecCompilerOpts, 
	    SecCompilerOptsEndPool, compOptsPool))
	return;
    if (!getSectionList(prjf, "", SecCompilerOptsEnd, compOpts))
	return;

    //% Section 3 - INTERPRETER
    if (!getSectionEntry(prjf, SecInterp, interpName ))
	return;

    //% Section 4 - INTERPRETER OPTIONS
    if (!getSectionList(prjf, SecInterpOpts, 
	    SecInterpOptsEndPool, interpOptsPool))
	return;
    if (!getSectionList(prjf, "", SecInterpOptsEnd, interpOpts))
	return;

    //% Section 5 - DEBUGGER
    if (!getSectionEntry(prjf, SecDebugger, debugName ))
	return;

    //% Section 6 - DEBUGGER OPTIONS
    if (!getSectionList(prjf, SecDebuggerOpts, 
	    SecDebuggerOptsEndPool, debugOptsPool))
	return;
    if (!getSectionList(prjf, "", SecDebuggerOptsEnd, debugOpts))
	return;

    //% Section 7 - CLASSPATH

    if (!getSectionEntry(prjf, SecClassPath, classPath ))
	return;

    //% Section 8 - APPLET HTML SORUCE";

    if (!getSectionEntry(prjf, SecAppletHTML, appletHTML ))
	return;

    //% Section 9 - TOP LEVEL JAVA FILE';
    if (!getSectionEntry(prjf, SecMainFile, mainFile ))
	return;

    //% Section 10 - SOURCE DIRECTORY
    if (!getSectionEntry(prjf, SecSrcDir, srcDir ))
	return;

    //% Section 11 - SOURCE FILES
    if (!getSectionList(prjf, SecSrcFiles, 
	    SecSrcFilesEnd, srcFiles))
	return;

  }

//====================>>> videJava::editJProject <<<============================
  void videJava::editJProject (const char *pname, bool createIt, ItemVal id)
  {
    JavaProjectDialog jp(this, myParent);
    
    if (createIt)
      {
	// strip project name down to main name
	char* stripped = stripDir((char*)pname);
	char nm[300];
	if (strlen(stripped) > 299)
	    strcpy(nm,"<none>");
	else
	  {
	    strcpy(nm,stripped);		// strip the .
	    for (int ix = strlen(nm) - 1 ; ix > 0 ; --ix)
	      if (nm[ix] == '.')
		{
		  nm[ix] = 0;
		  break;
		}
	  }

	setDefaults(nm,id);
      }
    (void) jp.editProject();

    closeJProject(pname,/*reset*/false);
 
  }

//====================>>> videJava::closeJProject <<<============================
  void videJava::closeJProject (const char *pname, bool reset)
  {

    char n[300];

    if (strlen(pname) > 296)
	strcpy(n,"BADFILE.tmp");
    else
	strcpy(n,pname);

    int len = strlen(n);
    int ix;

    for (ix = len - 1 ; ix >= 0 ; --ix)
      {
	if (n[ix] == '.' || n[ix] == '/' || n[ix] == '\\')
	    break;
      }

    if (n[ix] != '.')	// user didn't supply extension
	strcat(n,".vpj");

    ofstream prjf(n);

    //header
    char buff[40];
    char date[20];
    vGetLocalTime(buff);
    vGetLocalDate(date);

    prjf << "//Java ==================================================================\n";
    prjf << "//@V@:Note: Java Project File generated by VIDE: ";
    prjf << "(" << buff << " " << date << ").\n";
    prjf << "//CAUTION! Hand edit only if you know what you are doing!\n";
    prjf << "//=======================================================================\n\n";

    prjf << SecCompiler << endl;	//% Section 1 - COMPILER
    if (!*compilerName)
	prjf << "<none>" << endl << endl;
    prjf << compilerName << endl << endl;

    prjf << SecCompilerOpts << endl;	//% Section 2 - COMPILER OPTIONS
    for (ix = 0 ; compOptsPool.list[ix] != 0 ; ++ix)
	prjf << compOptsPool.list[ix] << endl;

    prjf << SecCompilerOptsEndPool << endl;	//%compiler-opts-pool-end
    for (ix = 0 ; compOpts.list[ix] != 0 ; ++ix)
	prjf << compOpts.list[ix] << endl;

    prjf << SecCompilerOptsEnd << endl << endl;	//%compiler-opts-end

    prjf << SecInterp << endl;		//% Section 3 - INTERPRETER
    if (!*interpName)
	prjf << "<none>" << endl << endl;
    prjf << interpName << endl << endl;

    prjf << SecInterpOpts << endl;	//% Section 4 - INTERPRETER OPTIONS
    for (ix = 0 ; interpOptsPool.list[ix] != 0 ; ++ix)
	prjf << interpOptsPool.list[ix] << endl;
    prjf << SecInterpOptsEndPool << endl;	//%interp-opts-pool-end

    for (ix = 0 ; interpOpts.list[ix] != 0 ; ++ix)
	prjf << interpOpts.list[ix] << endl;
    prjf << SecInterpOptsEnd << endl << endl;	//%interp-opts-end

    prjf << SecDebugger << endl;	//% Section 5 - DEBUGGER
    if (!*debugName)
	prjf << "<none>" << endl << endl;
    prjf << debugName << endl << endl;


    prjf << SecDebuggerOpts << endl;	//% Section 6 - DEBUGGER OPTIONS
    for (ix = 0 ; debugOptsPool.list[ix] != 0 ; ++ix)
	prjf << debugOptsPool.list[ix] << endl;
    prjf << SecDebuggerOptsEndPool << endl;	//%debug-opts-pool-end

    for (ix = 0 ; debugOpts.list[ix] != 0 ; ++ix)
	prjf << debugOpts.list[ix] << endl;
    prjf << SecDebuggerOptsEnd << endl << endl;	//%debug-opts-end

    prjf << SecClassPath << endl;	//% Section 7 - CLASSPATH
    if (!*classPath)
	prjf << "<none>" << endl << endl;
    prjf << classPath << endl << endl;

    prjf << SecAppletHTML << endl;	//% Section 8 - APPLET HTML SORUCE
    if (!*appletHTML)
	prjf << "<none>" << endl << endl;
    prjf << appletHTML << endl << endl;

    prjf << SecMainFile << endl;	//% Section 9 - TOP LEVEL JAVA FILE
    if (!*mainFile)
	prjf << "<none>" << endl << endl;
    prjf << mainFile << endl << endl;

    prjf << SecSrcDir << endl;		//% Section 10 - SOURCE DIRECTORY
    if (!*srcDir)
	prjf << "<none>" << endl << endl;
    prjf << srcDir << endl << endl;

    prjf << SecSrcFiles << endl;	//% Section 11 - SOURCE FILES
    for (ix = 0 ; srcFiles.list[ix] != 0 ; ++ix)
	prjf << srcFiles.list[ix] << endl;
    prjf << SecSrcFilesEnd << endl << endl;	//%src-files-end

    prjf.close();

    // now reset back to defaults
    if (reset)
	setDefaults();
  }

//===================>>> videJava::getSectionList <<<============================
  bool videJava::getSectionList(ifstream &prjf, const char* head, 
	const char* term, vSList& sl)
  {
    char skipStr[256];
    if (!prjf)
	return false;
    if (head && *head)	// allow no head - assumes at right location
      {
	for (prjf.getline(skipStr, 256) ;	// skip line
	 	prjf && strcmp(skipStr,head) != 0 ;
	 	prjf.getline(skipStr, 256))
	    ;
      }

    // get list now
    for (prjf.getline(skipStr, 256) ;	// skip blank lines
	 prjf && strcmp(skipStr, term) != 0 ;
	 prjf.getline(skipStr, 256))
      {
	if (*skipStr != ' ')
	    sl.insert(-1,skipStr);		// add to end of list
      }

    if (!prjf)
	return false;
    else
	return true;

  }

//===================>>> videJava::getSectionEntry <<<===========================
  bool videJava::getSectionEntry(ifstream &prjf, const char* head, char* value)
  {
    // read section head into value
    char skipStr[256];
    if (!prjf)
	return false;
    for (prjf.getline(skipStr, 256) ;	// skip line
	 prjf && strcmp(skipStr,head) != 0 ;
	 prjf.getline(skipStr, 256))
	;

    for (prjf.getline(skipStr, 256) ;	// skip blank lines
	 prjf && (*skipStr == 0 || *skipStr == ' ') ;
	 prjf.getline(skipStr, 256))
	;

    strcpy(value,skipStr);			// copy to output

    if (!prjf)
	return false;
    else
	return true;

  }
