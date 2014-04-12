//===============================================================
// vPrefs Class for working with .ini or .rc files
//
// Copyright (C) 1995,1996, 1997, 1998  Bruce E. Wampler
//
// This file is part of the V C++ GUI Framework, and is covered
// under the terms of the GNU Library General Public License,
// Version 2. This library has NO WARRANTY. See the source file
// vapp.cxx for more complete information about license terms.
//===============================================================

#include <v/v_defs.h>

#include "vprefs.h"
//#include <v/vprefs.h>
#include <v/vutil.h>

#ifdef V_VersionX
#include <v/vos.h>
#endif

//===========================>>> vPrefs::vPrefs <<<=======================
  vPrefs::vPrefs()
  {
  }

//===========================>>> vPrefs::~vPrefs <<<=======================
  vPrefs::~vPrefs()
  {

  }

//===========================>>> vPrefs::openSave <<<=======================
  int vPrefs::openSave(const char* name, const char* title)
  {
    // called with a base name of the file and a title line...
    char winDir[maxFileNameSize+20];
    
#ifdef V_VersionWindows
    if (!::GetWindowsDirectory(winDir, maxFileNameSize))
	return 0;
    strcat(winDir,"\\");
    strcat(winDir,name);
    strcat(winDir,".ini");
#endif
 
#ifdef V_VersionX
    vOS os;

    if (!os.vGetEnvVal("HOME",winDir,maxFileNameSize))
      {
	strcpy(winDir,"./");
	strcat(winDir,name);
	strcat(winDir,"rc");
      }
    else
      {
	strcat(winDir,"/.");
	strcat(winDir,name);
	strcat(winDir,"rc");
      }
#endif

    outFile.open(winDir);
    if (!outFile)
	return 0;
    outFile << title << endl;
    return 1;
    
  }

//===========================>>> vPrefs::closeSave <<<=======================
  void vPrefs::closeSave()
  {
    outFile.close();
  }

//===========================>>> vPrefs::openRead <<<=======================
  int vPrefs::openRead(const char* name, const char* title)
  {
    char winDir[maxFileNameSize+20];
    
#ifdef V_VersionWindows
    if (!::GetWindowsDirectory(winDir, maxFileNameSize))
	return 0;
    strcat(winDir,"\\");
    strcat(winDir,name);
    strcat(winDir,".ini");
#endif
#ifdef V_VersionX
    vOS os;

    if (!os.vGetEnvVal("HOME",winDir,maxFileNameSize))
      {
	strcpy(winDir,"./");
	strcat(winDir,name);
	strcat(winDir,"rc");
      }
    else
      {
	strcat(winDir,"/.");
	strcat(winDir,name);
	strcat(winDir,"rc");
      }
#endif
 
    inFile.open(winDir);
    if (!inFile)
	return 0;
    return 1;
    
  }

//===========================>>> vPrefs::closeRead <<<=======================
  void vPrefs::closeRead()
  {
    inFile.close();
  }

//===========================>>> vPrefs::getString <<<=======================
  int vPrefs::getString(const char* entry, char* str)
  {
    char buff[256];
    char name[64];
    char* np;
    char* bp; 

    inFile.seekg(0,ios::beg);
    if (!inFile)
	return 0;
    for ( ; ; )
      {
	inFile.getline(buff,255);
	if (!inFile)
	{
	    inFile.clear();
	    break;
	}
        if (*buff == '[')	// skip section lines
            continue;

 	np = name;
	for (bp = buff ; *bp && *bp != '=' ; )
	    *np++ = *bp++;
	*np = 0;
	if (*bp != '=')
	    return 0;
	bp++;
	if (strcmp(name,entry) != 0)
	    continue;
	strcpy(str,bp);
	return 1;
      }

    return 0;
  }

//===========================>>> vPrefs::getLong <<<=======================
  int vPrefs::getLong(const char* entry, long& val)
  {
    char buff[256];
    char name[64];
    char* np;
    char* bp; 


    inFile.seekg(0,ios::beg);
    if (!inFile)
	return 0;
    for ( ; ; )
      {
	inFile.getline(buff,255);
	if (!inFile)
	{
	    inFile.clear();
	    break;
	}
        if (*buff == '[')	// skip section lines
            continue;

 	np = name;
	for (bp = buff ; *bp && *bp != '=' ; )
	    *np++ = *bp++;
	*np = 0;
	if (*bp != '=')
	    return 0;
	bp++;
	if (strcmp(name,entry) != 0)
	    continue;
	strcpy(name,bp);
	val = StrToLong(name);
	return 1;
      }

    return 0;
  }

//===========================>>> vPrefs::addString <<<=======================
  void vPrefs::addString(const char* entry, char* str)
  {
    outFile << entry << "=" << str << endl;
  }

//===========================>>> vPrefs::addLong <<<=======================
  void vPrefs::addLong(const char* entry, long val)
  {
    outFile << entry << "=" << val << endl;
  }
