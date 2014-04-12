//===============================================================
// mmaker.cpp	- makefileMaker Class for VIDE
//
// Copyright (C) 1995-2000  Bruce E. Wampler
// date: 01 Mar 2000 04:40:21 PM
//
// This file is part of the V C++ GUI Framework, and is covered
// under the terms of the GNU Library General Public License,
// Version 2. This library has NO WARRANTY. See the source file
// vapp.cxx for more complete information about license terms.
//===============================================================

#include <v/vnotice.h>
#include <v/vos.h>
#include "mmaker.h"
#include "videapp.h"

//==================>>>   makefileMaker::fieldLen    <<<============================

    const int makefileMaker::fieldLen = 256;
    static const char SecProjOpts[] = "//% Section 1 - PROJECT OPTIONS";
    static const char SecProjOptsEnd[] = "//%end-proj-opts";
    static const char SecMakefile[] = "//% Section 2 - MAKEFILE";
    static const char SecOptions[] = "//% Section 3 - OPTIONS";
    static const char SecOptionsEnd[] = "//%end-options";
    static const char SecHomeV[] = "//% Section 4 - HOMEV";
    static const char SecTarget[] = "//% Section 5  - TARGET FILE";
    static const char SecObjFiles[] = "//% Section 6  - SOURCE FILES";
    static const char SecObjFilesEnd[] = "//%end-srcfiles";
    static const char SecCompiler[] = "//% Section 7  - COMPILER NAME";
    static const char SecIncDirs[] = "//% Section 8  - INCLUDE DIRECTORIES";
    static const char SecIncDirsEnd[] = "//%end-include-dirs";
    static const char SecLibDirs[] = "//% Section 9 - LIBRARY DIRECTORIES";
    static const char SecLibDirsEnd[] = "//%end-library-dirs";
    static const char SecDefs[] = "//% Section 10  - DEFINITIONS";
    static const char SecDefsPoolEnd[] = "//%end-defs-pool";
    static const char SecDefsEnd[] = "//%end-defs";
    static const char SecCFlags[] = "//% Section 11  - C FLAGS";
    static const char SecLibFlags[] = "//% Section 12  - LIBRARY FLAGS";
    static const char SecSrcDir[] = "//% Section 13  - SRC DIRECTORY";
    static const char SecObjDir[] = "//% Section 14  - OBJ DIRECTORY";
    static const char SecBinDir[] = "//% Section 15 - BIN DIRECTORY";
    static const char SecUserTargets[] = "//% Section 16 - USER TARGETS";
    static const char SecUserTargetsEnd[] = "//%end-user-targets";


//==================>>> makefileMaker::makefileMaker <<<=========================
  makefileMaker::makefileMaker()
  {
    makeName = new char[fieldLen];
    homeV =   new char[fieldLen];
    exeName = new char[fieldLen];
    cc =      new char[fieldLen];
    cFlags =  new char[fieldLen];
    Libs =    new char[fieldLen];
    libOptsBCC =    new char[fieldLen];
    ObjDir =  new char[fieldLen];
    BinDir =  new char[fieldLen];
    SrcDir =  new char[fieldLen];
    ctags = new char[fieldLen];

    setDefaults();
    cmdw = ((videApp*)theApp)->GetMsgWindow();

  }

//======================>>> makefileMaker::~makefileMaker <<<============================
  makefileMaker::~makefileMaker()
  {
    delete [] makeName;
    delete [] homeV;
    delete [] exeName;
    delete [] cc;
    delete [] cFlags;
    delete [] Libs;
    delete [] libOptsBCC;
    delete [] BinDir;
    delete [] SrcDir;
    delete [] ObjDir;
    delete [] ctags;
  }
 
//======================>>> makefileMaker::srcN2ObjN <<<=========================
  char* makefileMaker::srcN2ObjN(char* n, char *ext)
  {
    static char objn[fieldLen];
    char *curExt;
    strcpy(objn, n);
    int len = strlen(objn);
    int ix;

    // replace .cpp with .o
    for (ix = len - 1 ; ix >= 0 ; --ix)
      {
	if (objn[ix] == '.')
	  {
	    curExt = &objn[ix+1];
	    break;
	  }
      }

    objn[ix] = 0;

#ifdef V_VersionWindows
    if (stricmp(curExt,"rc") == 0)
      {
	if (stricmp(cc,"BCC32") == 0)	// borland uses .res
	    strcat(objn,".res");
	else
	    strcat(objn,ext);		// gcc uses .o
      }
    else
#endif
	strcat(objn,ext);
    return stripDir(objn);
  }

//======================>>> makefileMaker::eraseAll <<============================
  void makefileMaker::eraseAll()
  {
    projOpts.erase();
    incDirs.erase();
    libDirs.erase();

    objFiles.erase();
    defsPool.erase();
    curDefs.erase();
    curOpts.erase();
    rules.erase();
    userTargets.erase();
    strcpy(makeName,"");
    strcpy(homeV,"");
    strcpy(exeName,"");
    strcpy(ctags,"");
    strcpy(cc,"");
    strcpy(cFlags,"");
    strcpy(Libs, "");
    strcpy(ObjDir,"");
    strcpy(BinDir,"");
    strcpy(SrcDir,"");
  }

//======================>>> makefileMaker::setDefaults <<============================
  void makefileMaker::setDefaults()
  {
    //erase previous definitions
    eraseAll();

    // init each one
    strcpy(makeName,"Makefile.v");
    vOS vos;
#ifdef V_VersionX
    if (vos.vGetEnvVal("HOMEV",homeV,fieldLen) < 1)
	strcpy(homeV,"$(HOME)/v");
    strcpy(exeName,"");
    strcpy(ctags,"*");
    strcpy(Libs, "-lV");
#else
    if (vos.vGetEnvVal("HOMEV",homeV,fieldLen) < 1)
	strcpy(homeV,"C:/v");
    strcpy(exeName,"");
    strcpy(ctags,"*");
    strcpy(Libs, "-lV -lcomctl32 -mwindows");
#endif
    strcpy(cc,"g++");
    strcpy(cFlags,"-O");
    strcpy(ObjDir,".");
    strcpy(BinDir,".");
    strcpy(SrcDir,".");
//    libDirs.insert(-1,"$(HOMEV)/lib");	// handled by wizard now
//    incDirs.insert(-1,"$(HOMEV)/include");
    changed = false;		// not changed yet
  }

//======================>>> makefileMaker::stripDir <<<============================
  char * makefileMaker::stripDir(char* n)
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

//======================>>> makefileMaker::addFile <<<============================
  bool makefileMaker::addFile(char *n)
  {
    //adds a new source file to the list

    if (!*n)
	return false;

    // Filter name here... (add a behavior switch eventually)

    char* addName = stripDir(n);	// strip leading dir part

    for (int ix = 0 ; objFiles.list[ix] != 0 ; ++ix)
	if (strcmp(objFiles.list[ix], addName) == 0)
	    return false; //don't add a obj file twice

    objFiles.insert(-1,addName);
    return true;
  }

//======================>>> makefileMaker::findRule <<<============================
  void makefileMaker::findRule(char* name)
  {
    //finds the rules to build object file pointed by name

    vOS vos;		// To execute g++

    char command[500]; // command line
    char hvbuff[256];

    cmdw = ((videApp*)theApp)->GetMsgWindow();

    strcpy(command,cc);
    for (int ix = 0 ; incDirs.list[ix] != 0 ; ++ix)
      {
	char *incdir = incDirs.list[ix];
	char *hvp = strstr(incdir,"$(HOMEV)/");

	if (hvp != 0)
	  {
	    // Have $(HOMEV) in the path, so we must replace the
	    // $(HOMEV) with the real homeV path.
	    strcpy(hvbuff,homeV);
	    strcat(hvbuff, hvp+strlen("$(HOMEV)") );
	    incdir = hvbuff;
	  }

	strcat(command," -I");
	strcat(command,incdir);
	if (strlen(command) > 480)
	    break;
      }
    strcat(command," -MM "); 
    if (strcmp(SrcDir,".") != 0)
      {
	strcat(command, SrcDir);
	strcat(command, "/");
      }
    strcat(command, name);

    static char makedep[] = "makedep.vtm";
    static char makeerr[] = "makeerr.vtm";
    char currlin[256];

    vos.vDeleteFile(makedep);	// be sure these are gone...
    vos.vDeleteFile(makeerr);

    if (vos.vRunProcess(command, makedep, makeerr, 1, 1) != 0)
      {
CANTMAKE:
	cmdw->AddLine(command);
	vos.vDeleteFile(makedep);
        ifstream em(makeerr);
        if (em)
          {
            while(em.getline(currlin,256,'\n'))
              cmdw->AddLine(currlin);
	    cmdw->AddLine("Build of Makefile failed - usually in wrong directory or missing files...");
	    cmdw->AddLine("  Open a file in the source directory, then try this again.");
	    cmdw->AddLine("--------------");
            em.close();
          }
        vos.vDeleteFile(makeerr);
	return;
      }

    ifstream deps(makedep);
    if (!deps)
	goto CANTMAKE;


    while ( deps.getline(currlin, 256, '\n'))
      {
        rules.insert(-1,currlin);
      }

    deps.close();
    vos.vDeleteFile(makedep);
    vos.vDeleteFile(makeerr);
  }

//======================>>> makefileMaker::fixBCCLibs <<<============================
  char* makefileMaker::fixBCCLibs()
  {
    char *cp;
    char *to;

    to = libOptsBCC;

    for (cp = Libs ; *cp ; ++cp)
      {
	if (*cp == '-' && *(cp+1) == 'l')	// found a library
	  {
	    char libname[fieldLen];
	    char *lp;
	    cp += 2;		// skip -1
	    strcpy(libname,cp);
	    for ( ; *cp && *cp != ' ' ; ++cp)
		;
	    for (lp = libname ; *lp && *lp != ' ' ; ++lp)
		;
	    *lp = 0;		// this terminates it
	    strcat(libname,".lib");
	    libsBCC.insert(-1,libname);		// add to libs list
	  }
	*to++ = *cp;
      }
    *to = 0;
    return libOptsBCC;
  }

//======================>>> makefileMaker::fixTabs <<<============================
  void makefileMaker::fixTabs(char* fixed, char* orig)
  {
    while (*orig)
      {
	if (*orig == '\\' && *(orig+1) =='t')	// replace with real tab
	  {
	    *fixed++ = '\t';
	    orig++; orig++;
	  }
	else
	    *fixed++ = *orig++;
      }
    *fixed = 0;
  }

//====================>>> makefileMaker::saveMakefile <<<=========================
  void makefileMaker::saveMakefile()
  {

    bool isBorland = false;		// set up Borland stuff

    char resName[50];

    resName[0] = 0;

    libsBCC.erase();

    //builds the file named makefile using infos stored in project

    cmdw = ((videApp*)theApp)->GetMsgWindow();

    // Now, change directory, drive

    int ix = strlen(makeName);
    while (ix > 0)
      {
        if (makeName[ix] == '\\' || makeName[ix] == '/')
          {
            break;
          }
        --ix;
      }
    if (ix > 0)
      {
	char dirPath[maxFileNameSize];
	vOS vos;
	strcpy(dirPath, makeName);	// copy the makefile name
	makeName[ix] = 0;		// strip the name
	if (dirPath[1] == ':')		// a drive
	  {
	    int drive = dirPath[0]; // need to change drive, too.
	    if (drive > 'a')
		drive = drive - ' ';
	    vos.vChDrive(drive-'A');
	  }
	vos.vChDir(dirPath);			// change dir
      }

    ofstream mkf(makeName);		// where to write makefile
    //header
    char buff[256];
    char date[20];
    char time[40];

    vGetLocalTime(time);
    vGetLocalDate(date);

    isBorland = (stricmp(cc,"BCC32") == 0);

    if (isBorland)
	cmdw->AddLine("Building Borland Makefile...");
    else
	cmdw->AddLine("Building Makefile -- Running compiler to generate dependencies...");


    mkf << "#=======================================================================\n";
    mkf << "#@V@:Note: File automatically generated by VIDE: ";
    mkf << "(" << time << " " << date << ") (" << cc << ").\n";
    mkf << "#  This file regenerated each time you run VIDE, so save under a\n";
    mkf << "#    new name if you hand edit, or it will be overwritten.\n";
    mkf << "#=======================================================================\n\n";
    mkf << "# Standard defines:\n";

    if (isBorland)
      {
#ifdef V_VersionWindows
	mkf << ".AUTODEPEND\n\n"
	    << "# Borland C++ tools\n"
	    << "IMPLIB\t= $(BCCROOT)\\bin\\Implib\n"
	    << "ILINK32\t= $(BCCROOT)\\bin\\ILink32\n"
	    << "TLIB\t= $(BCCROOT)\\bin\\TLib\n"
	    << "BRC32\t= $(BCCROOT)\\bin\\Brc32\n"
	    << "TASM32\t= $(BCROOT)\\bin\\Tasm32\n"
	    << "CC  \t= $(BCCROOT)\\bin\\" << cc << "\n\n";
	if (*((videApp*)theApp)->getBCCPath())
	    mkf << "BCCROOT\t= " << ((videApp*)theApp)->getBCCPath() << "\n\n";
	else
	    mkf << "BCCROOT\t= C:\\borland\\bcc55\n\n";
#endif
      }
    else		// gnu stuff
      {
	mkf << "CC  \t=\t" << cc << "\n\n";
	mkf << "WRES\t=\twindres\n\n";
      }

    // HomeV
    mkf << "HOMEV\t=\t" << fixBackSlash(homeV,isBorland) << endl;
    
    //Dirs
    if (isBorland)
	mkf << "VPATH\t=\t$(HOMEV)\\include" << endl;
    else
	mkf << "VPATH\t=\t$(HOMEV)/include" << endl;

    mkf << "oDir\t=\t" << fixBackSlash(ObjDir,isBorland) << endl; //Obj Dir
    mkf << "Bin\t=\t" << fixBackSlash(BinDir,isBorland) << endl; //Bin Dir
    mkf << "Src\t=\t" << fixBackSlash(SrcDir, isBorland) << endl; //Src Dir

    mkf << "libDirs\t=";
 
    int nxtld = libDirs.size();

    if (isBorland && nxtld > 0)
	mkf << "\t-L";

    for (int ixx = 0 ; ixx < nxtld ; ++ixx)
      {
	if (isBorland)		// put out ; separated list
	  {
	    mkf << fixBackSlash(libDirs.list[ixx],isBorland);
	    if (ixx + 1 < nxtld)
		mkf << ";";
	  }
	else
	  {
	    mkf << "\t-L" << fixBackSlash(libDirs.list[ixx],isBorland);
	    if (ixx + 1 < nxtld)
		mkf << " \\";
	    mkf << endl;
	  }
      }
    if (isBorland)
	mkf << endl;
    mkf << endl;

    mkf << "incDirs\t="; 

    int nxtid = incDirs.size();

    if (isBorland && nxtid > 0)
	mkf << "\t-I";

    for (int ixa = 0 ; ixa < nxtid ; ++ixa)
      {
	if (isBorland)		// put out ; separated list
	  {
	    mkf << fixBackSlash(incDirs.list[ixa],isBorland);
	    if (ixa + 1 < nxtid)
		mkf << ";";
	  }
	else
	  {
	    mkf << "\t-I" << fixBackSlash(incDirs.list[ixa],isBorland);
	    if (ixa + 1 < nxtid)
		mkf << " \\";
	    mkf << endl;
	  }
      }
    if (isBorland)
	mkf << endl;
    mkf << endl;

    //Libs
    if (isBorland)
      {
	mkf << "LIBS\t=\t" << fixBCCLibs() << endl;
      }
    else
	mkf << "LIBS\t=\t" << Libs << endl;

    //C_FLAGS & paths to includes

    mkf << "C_FLAGS\t=\t" << fixBackSlash(cFlags,isBorland);

    //defines
    int nxtcd = curDefs.size();

    for (int ixb = 0 ; ixb < nxtcd ; ++ixb)
      {
	mkf << "\\\n\t" << curDefs.list[ixb];
      }
    mkf << endl << endl;

    //sources
    int nxtof = objFiles.size();

    mkf << "SRCS\t=";

    for (int ixc = 0 ; ixc < nxtof ; ++ixc)
      {
	if (isBorland)
	    mkf << "\\\n\t$(Src)\\" << fixBackSlash(objFiles.list[ixc], isBorland);
	else
	    mkf << "\\\n\t$(Src)/" << objFiles.list[ixc];
      }
    mkf << endl << endl;

    //objects

    mkf << "EXOBJS\t=";

    for (int ixd = 0 ; ixd < nxtof ; ++ixd)
      {
	char *objn;
	if (isBorland)
	  {
	    objn = fixBackSlash(srcN2ObjN(objFiles.list[ixd],".obj"),isBorland);
	    mkf << "\\" << endl << "\t$(oDir)\\" << objn;

	    if (strstr(objn,".res") != 0)
		strcpy(resName,srcN2ObjN(objFiles.list[ixd],".obj"));
	  }
	else
	  {
	    objn = srcN2ObjN(objFiles.list[ixd],".o");
	    mkf << "\\" << endl << "\t$(oDir)/" << objn;
	  }

      }
    mkf << endl << endl;

    mkf << "ALLOBJS\t=\t$(EXOBJS)" << endl;
    if (isBorland)
      {
	mkf << "BCCJUNK\t=\t*.il* *.csm *.tds\n";
	mkf << "ALLBIN\t=\t$(Bin)\\" << exeName << endl;
	mkf << "ALLTGT\t=\t$(Bin)\\" << exeName << endl << endl;
      }
    else
      {
	mkf << "ALLBIN\t=\t$(Bin)/" << exeName << endl;
	mkf << "ALLTGT\t=\t$(Bin)/" << exeName << endl << endl;
      }


    // Options

    mkf << "# User defines:" << endl;
    int nxtOpts = curOpts.size();

    for (int ixe = 0 ; ixe < nxtOpts ; ++ixe)
      {
	fixTabs(buff,curOpts.list[ixe]);
	mkf << buff << endl;
      }
    mkf << endl;

    mkf << "#@# Targets follow ---------------------------------" 
	<< endl << endl;
    //target file
    //all:
    mkf << "all:\t$(ALLTGT)" << endl << endl;
    //objs:
    mkf << "objs:\t$(ALLOBJS)" << endl << endl;

    //cleanobjs:
    mkf << "cleanobjs:\n\trm -f $(ALLOBJS)" << endl << endl;

    // cleanbin
    if (isBorland)
	mkf << "cleanbin:\n\trm -f $(ALLBIN) $(BCCJUNK)" << endl << endl;
    else
	mkf << "cleanbin:\n\trm -f $(ALLBIN)" << endl << endl;

    // clean
    mkf << "clean:\tcleanobjs cleanbin" << endl << endl;

    // cleanall
    mkf << "cleanall:\tcleanobjs cleanbin" << endl << endl;

    mkf << "#@# User Targets follow ---------------------------------" 
	<< endl << endl;

    for (int ixf = 0 ; userTargets.list[ixf] != 0 ; ++ixf)
      {
	fixTabs(buff,userTargets.list[ixf]);
	mkf << buff << endl;
      }
    mkf << endl;

    mkf << "#@# Dependency rules follow -----------------------------"
	<< endl << endl;
    //dependency rules

    // allow user to override the all: target with a #all comment
    if (!(userTargets.list[0] && strstr(userTargets.list[0],"#all")!=0))
      {
        bool isStaticLib = false;
        char ext[32];

	// check for a static library
	splitFileName(exeName,0,0,ext);	// get the extention
	if (stricmp(ext,".a") == 0
#ifdef V_VersionWindows
            || stricmp(ext,".lib") == 0 ||
	    stricmp(ext,".LIB") == 0
#endif
           )
	    isStaticLib = true;		// found a static lib
        
	if (isBorland)
	  {
#ifdef V_VersionWindows
	    mkf << "$(Bin)\\" << exeName << " : $(EXOBJS)" << endl ;
	    
	    if (isStaticLib)
	      {
		mkf << "  rm -f $<\n";
		mkf << "  $(TLIB) $< @&&|\n";
		for (int ixg = 0 ; objFiles.list[ixg] != 0 ; ixg++)
		  {
		    char* objn = srcN2ObjN(objFiles.list[ixg],".obj");
		    if (ixg != 0)
			mkf << " &\n";
		    mkf << " +$(oDir)\\" << srcN2ObjN(objFiles.list[ixg],".obj");
		  }
		mkf << " \n|\n\n";

	      }
	    else		// standard linker commands
	      {
		mkf << "  $(ILINK32) @&&|\n" << " $(LIBS) $(incDirs) $(libDirs) +\n";
		
		mkf << "$(BCC32STARTUP)";

		for (int ixg = 0 ; objFiles.list[ixg] != 0 ; ixg++)
		  {
		    char* objn = srcN2ObjN(objFiles.list[ixg],".obj");
		    if (strstr(objn,".res") == 0)
			mkf << " +\n$(oDir)\\" << srcN2ObjN(objFiles.list[ixg],".obj");
		  }

		mkf << "\n$<,$*\n";

		for (int ixa = 0 ; libsBCC.list[ixa] != 0 ; ixa++)
		  {
		    if (ixa != 0)
			mkf << " +\n";
		    mkf << libsBCC.list[ixa] ;
		  }

		if (*resName)		// had a .res file
		    mkf << "\n\n$(oDir)\\" << resName << "\n\n|\n";
		else
		    mkf << "\n\n\n\n|\n";
	      }
#endif
	  }
	else		// gnu compatible;
	  {
	    mkf << "$(Bin)/" << exeName << ": $(EXOBJS)" << endl;
	    if (isStaticLib)
	      {
		mkf << "\trm -f $(Bin)/" << exeName << endl;
		mkf << "\tar cr $(Bin)/" << exeName << " $(EXOBJS)\n";
		mkf << "\tranlib $(Bin)" << exeName << endl;
	      }
	    else
	      {
		mkf << "\t$(CC) -o $(Bin)/"
		<< exeName << " $(EXOBJS) $(incDirs) $(libDirs) $(LIBS)" << endl ;
	      }
	  }
      }

    if (isBorland)
      {
	// now count on autodependency for borland
	for (int ixg = 0 ; objFiles.list[ixg] != 0 ; ixg++)
	  {
	    mkf << "$(oDir)\\" << srcN2ObjN(objFiles.list[ixg],".obj");
	    mkf << " : $(Src)\\" << objFiles.list[ixg] << endl;

	    // check for .rc files
	    if (strstr(objFiles.list[ixg],".rc") != 0 
		|| strstr(objFiles.list[ixg],".RC") != 0)
	      {
		mkf 	<< "  $(BRC32) -r $(incDirs) -fo$@ $(Src)\\"
			<< objFiles.list[ixg] << "\n\n";

	      }
	    else		// it is a .rc file
	      {
		mkf 	<< "  $(CC) -c $(C_FLAGS) $(incDirs) -o$@ $(Src)\\"
			<< objFiles.list[ixg] << "\n\n";
	      }
	  }
      }
    else
      {
	rules.erase();	// fix by chorn - 03/08/99

	for (int ixg = 0 ; objFiles.list[ixg] != 0 ; ixg++)
	  {
#ifdef V_VersionWindows
	    if (strstr(objFiles.list[ixg],".rc") != 0 
		|| strstr(objFiles.list[ixg],".RC") != 0)
	      {
		mkf << "\n$(oDir)/" << srcN2ObjN(objFiles.list[ixg],".o");
		mkf << " : $(Src)/" << objFiles.list[ixg] << endl;
		mkf << "\t$(WRES) $(Src)/" << objFiles.list[ixg];
		mkf << " $(oDir)/" << srcN2ObjN(objFiles.list[ixg],".o");
		mkf << "\n";
	      }
	    else
	      {
	        findRule(objFiles.list[ixg]);
	      }
#else
	    findRule(objFiles.list[ixg]);
#endif
	  }

	int lim = rules.size();
	for (int ixh = 0 ; ixh < lim ; ++ixh)
	  {
	    mkf << endl << "$(oDir)/" << rules.list[ixh] << endl;
	    // check for continuation lines
	    while (*(rules.list[ixh]+strlen(rules.list[ixh])-1) == '\\')
	      {
		++ixh;
		if (rules.list[ixh])
		    mkf << rules.list[ixh] << endl;
	      }
	    mkf << "\t$(CC) $(C_FLAGS) $(incDirs) -c -o $@ $<" << endl;
	  }
      }

    mkf.close();

    
    strcpy(buff,makeName); 
    strcat(buff," ("); strcat(buff,time); strcat(buff," ");
    strcat(buff,date);
    strcat(buff,") - Makefile saved.");
    cmdw->AddLine(buff);

  }

//======================>>> makefileMaker::saveProject <<<============================
  void makefileMaker::saveProject (char *nameIn)
  {

    char n[300];

    if (!changed)
	return;

    if (strlen(nameIn) > 296)
	strcpy(n,"BADFILE.tmp");
    else
	strcpy(n,nameIn);

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

    prjf << "//=======================================================================\n";
    prjf << "//@V@:Note: Project File generated by VIDE: ";
    prjf << "(" << buff << " " << date << ").\n";
    prjf << "//CAUTION! Hand edit only if you know what you are doing!\n";
    prjf << "//=======================================================================\n\n";

    // Project Options
    prjf << SecProjOpts << endl;

    projOpts.erase();		// erase all options
    if (*ctags != 0)
      {
	char buff[fieldLen];
	strcpy(buff,"ctags:");
	strcat(buff,ctags);
	projOpts.insert(-1,buff);
      }

    for (int nxtOpts = 0 ; projOpts.list[nxtOpts] != 0 ; ++nxtOpts)
	prjf << projOpts.list[nxtOpts] << endl;

    prjf << SecProjOptsEnd << endl << endl;

    // Makefile name
    prjf << SecMakefile << endl;
    prjf << makeName << endl << endl;

    // Options
    prjf << SecOptions << endl;

    for (int nxtOptsa = 0 ; curOpts.list[nxtOptsa] != 0 ; ++nxtOptsa)
	prjf << curOpts.list[nxtOptsa] << endl;

    prjf << SecOptionsEnd << endl << endl;

    // HOMEV
    prjf << SecHomeV << endl;
    prjf << homeV << endl << endl;

    // 1) name of exe target file
    prjf << SecTarget << endl;
    prjf << exeName << endl << endl;
 
    // 2) object files
    prjf << SecObjFiles << endl;
    for (int ixa = 0 ; objFiles.list[ixa] != 0 ; ++ixa)
      {
	prjf <<  objFiles.list[ixa] << endl;
      }
    prjf << SecObjFilesEnd << endl << endl;

    // 3) compiler name
    prjf << SecCompiler << endl;
    prjf << cc << endl << endl;

    // 4) include dirs
    prjf << SecIncDirs << endl;

    for (int nxtid = 0 ; incDirs.list[nxtid] != 0 ; ++nxtid)
	prjf << incDirs.list[nxtid] << endl;

    prjf << SecIncDirsEnd << endl << endl;

    // 5) lib dirs
    prjf << SecLibDirs << endl;
    for (int ld = 0 ; libDirs.list[ld] != 0 ; ++ld)
	prjf << libDirs.list[ld] << endl;
    prjf << SecLibDirsEnd << endl << endl;

    // 6) defs
    prjf << SecDefs << endl;
    for (int ixb = 0 ; defsPool.list[ixb] != 0 ; ++ixb)
	prjf << defsPool.list[ixb] << endl;
    prjf << SecDefsPoolEnd << endl;

    for (int ixc = 0 ; curDefs.list[ixc] != 0 ; ++ixc)
	prjf << curDefs.list[ixc] << endl;
    prjf << SecDefsEnd << endl << endl;

    // 7) cflag
    prjf << SecCFlags << endl;
    prjf << cFlags << endl << endl;

    // 8) libflag 
    prjf << SecLibFlags << endl;
    prjf << Libs << endl << endl;

    // src directory
    prjf << SecSrcDir << endl;
    prjf << SrcDir << endl << endl;

    // 9) object directory
    prjf << SecObjDir << endl;
    prjf << ObjDir << endl << endl;

    //10) bin directory
    prjf << SecBinDir << endl;
    prjf << BinDir << endl << endl;
    // User Targets
    prjf << endl << "//% User targets section. Following lines will be" << endl;
    prjf << "//% inserted into Makefile right after the generated cleanall target." << endl;
    prjf << "//% The Project File editor does not edit these lines - edit the .vpj" << endl;
    prjf << "//% directly. You should know what you are doing." << endl;

    prjf << SecUserTargets << endl;
    for (int lda = 0 ; userTargets.list[lda] != 0 ; ++lda)
	prjf << userTargets.list[lda] << endl;
    prjf << SecUserTargetsEnd << endl << endl;

    prjf.close();

    saveMakefile();		// and always save the makefile, too.
    changed = false;
  }

//===================>>> makefileMaker::getSectionEntry <<<===========================
  bool makefileMaker::getSectionEntry(ifstream &prjf, const char* head, char* value)
  {
    // read section head into value
    char skipStr[256];
    if (!prjf)
	return false;
    for (prjf.getline(skipStr, 256, '\n') ; prjf ;  prjf.getline(skipStr, 256, '\n'))
      {
	char* cp = skipStr;
	while (*cp)			// make cr/lf files ok on Linux systems
	  {
	    if (*cp == '\r' || *cp == '\n')
		*cp = 0;
	    else
		++cp;		    
	  }
	if (strcmp(skipStr,head) == 0)
	    break;
      }

    // next line will be the entry...

    prjf.getline(skipStr, 256, '\n') ;	// skip blank lines

    char* cp = skipStr;
    while (*cp)			// make cr/lf files ok on Linux systems
      {
	if (*cp == '\r' || *cp == '\n')
	    *cp = 0;
	else
	    ++cp;		    
      }

    strcpy(value,skipStr);			// copy to output

    if (!prjf)
	return false;
    else
	return true;

  }

//===================>>> makefileMaker::getSectionList <<<============================
  bool makefileMaker::getSectionList(ifstream &prjf, const char* head, const char* term, vSList& sl)
  {
    char skipStr[256];
    if (!prjf)
	return false;

    if (head && *head)	// allow no head - assumes at right location
      {
	for (prjf.getline(skipStr, 256, '\n') ; prjf ;  prjf.getline(skipStr, 256, '\n'))
	  {
	    char* cp = skipStr;
	    while (*cp)			// make cr/lf files ok on Linux systems
	      {
		if (*cp == '\r' || *cp == '\n')
		    *cp = 0;
		else
		    ++cp;		    
	      }
	    if (strcmp(skipStr,head) == 0)
		break;
	  }
      }

    // get list now
    for (prjf.getline(skipStr, 256, '\n') ; prjf ;  prjf.getline(skipStr, 256, '\n'))
      {
	char* cp = skipStr;
	while (*cp)			// make cr/lf files ok on Linux systems
	  {
	    if (*cp == '\r' || *cp == '\n')
		*cp = 0;
	    else
		++cp;		    
	  }
	if (strcmp(skipStr,term) == 0)
	    break;
	if (*skipStr != ' ')
	    sl.insert(-1,skipStr);		// add to end of list
      }

    if (!prjf)
	return false;
    else
	return true;

  }

//======================>>> makefileMaker::loadProject <<<============================
  void makefileMaker::loadProject (char *n)
  {
    //loads infos contained in project file n into makefileMaker member vars

    ifstream prjf(n);
    if (!prjf)
	return;

    eraseAll();
  

    // Project Options
    if (!getSectionList(prjf, SecProjOpts, SecProjOptsEnd, projOpts))
	return;

    // extract options to useful fields
    int optlim = projOpts.size();
    for (int ixa = 0 ; ixa < optlim ; ++ixa)
      {
	if (strstr(projOpts.list[ixa],"ctags:")!= 0)
	  {
	    strcpy(ctags,&projOpts.list[ixa][6]);
	  }
      }
    projOpts.erase();		// now erase all options to avoid dups

    // Makefile
    if (!getSectionEntry(prjf, SecMakefile, makeName))
	return;

    //0) options
    if (!getSectionList(prjf, SecOptions, SecOptionsEnd, curOpts))
	return;

    // HOMEV
    if (!getSectionEntry(prjf, SecHomeV, homeV))
	return;

    //1) exe name
    if (!getSectionEntry(prjf, SecTarget, exeName))
	return;

    //2) objs
    if (!getSectionList(prjf, SecObjFiles, SecObjFilesEnd, objFiles))
	return;

    //3) compiler name
    if (!getSectionEntry(prjf, SecCompiler, cc))
	return;

    //4) include dirs
    if (!getSectionList(prjf, SecIncDirs, SecIncDirsEnd, incDirs))
	return;

    //5) lib dirs
    if (!getSectionList(prjf, SecLibDirs, SecLibDirsEnd, libDirs))
	return;

    //6) defs
    if (!getSectionList(prjf, SecDefs, SecDefsPoolEnd, defsPool))
	return;
    if (!getSectionList(prjf, "", SecDefsEnd, curDefs))
	return;

    //7) cflag
    if (!getSectionEntry(prjf, SecCFlags, cFlags))
	return;

    //8) libflag 
    if (!getSectionEntry(prjf, SecLibFlags, Libs))
	return;

    //  SRC directory
    if (!getSectionEntry(prjf, SecSrcDir, SrcDir))
	return;

    // 9) object directory
    if (!getSectionEntry(prjf, SecObjDir, ObjDir))
	return;

    //10) bin directory
    if (!getSectionEntry(prjf, SecBinDir, BinDir))
	return;

    // User targets.
    if (!getSectionList(prjf, SecUserTargets, SecUserTargetsEnd, userTargets))
	return;

  }
