//=======================================================================
//	videlib.cpp:	Source for videlib
//  Copyright (C) 1995-2000  Bruce E. Wampler
//	date: 23 Feb 2000 11:26:34
//
//  This program is part of the V IDE
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

#include "videlib.h"
#include <v/vos.h>
static const int maxFNSize = 262;

//============================>>> splitFileName <<<===========================
  void splitFileName(const char* fn, char* fdir, char* fbase, char* fext)
  {
    // splits a file into three parts - the directory, the base name
    // (which includes the name+ext), and the extension (with .)
    // assumes output char*'s are at least as long as fn in.
    // Can be called with outputs NULL to skip

    int len = strlen(fn);	// length
    int ix;

    if (fdir)			// want dir?
      {
	strcpy(fdir,fn);		// copy it all
	for (ix = len ; ix > 0 ; --ix)
	  {
#ifdef V_VersionWindows
	    if (fn[ix] == '/' || fn[ix] == '\\' || fn[ix] == ':')
#else
	    if (fn[ix] == '/' || fn[ix] == '\\')
#endif
	      {
		fdir[ix+1] = 0;
		break;
	      }
	  }
      }

    if (fbase)			// want base?
      {
	for (ix = len ; ix > 0 ; --ix)
	  {
#ifdef V_VersionWindows
	    if (fn[ix] == '/' || fn[ix] == '\\' || fn[ix] == ':')
#else
	    if (fn[ix] == '/' || fn[ix] == '\\')
#endif
	      {
		strcpy(fbase,&fn[ix+1]);
		break;
	      }
	  }
      }
    
    if (fext)			// want ext?
      {
	for (ix = len ; ix > 0 ; --ix)
	  {
	    if (fn[ix] == '.')
	      {
		strcpy(fext,&fn[ix]);
		break;
	      }
	  }
      }

  }

//============================>>> fixSlash <<<===========================
  void fixSlash(char* fullname)
  {
    // change \ to /
    while (*fullname)
      {
	if (*fullname == '\\')
	    *fullname = '/';
	++fullname;
      }
  }

//============================>>> fixBackSlash <<<===========================
  char* fixBackSlash(char* fullname, int doit)
  {
    // change / to \ 

    char *orig = fullname;

    if (!doit)			// depends on OS
	return orig;

    while (*fullname)
      {
	if (*fullname == '/')
	    *fullname = '\\';
	++fullname;
      }
    return orig;
  }

//============================>>> changeDir <<<===========================
  void changeDir(const char* fullname)
  {
    char temp[maxFNSize + 2];
    vOS vos;

    splitFileName(fullname , temp, 0, 0);	// get the path

    int len = strlen(temp);
    if (len < 1)
	return;

    if (temp[len-1] == '/' || temp[len-1] == '\\')
	temp[--len] = 0;

    if (temp[1] == ':')		// a drive
      {
	int drive = temp[0]; // need to change drive, too.
	if (drive > 'a')
	    drive = drive - ' ';
	vos.vChDrive(drive-'A');
      }
    vos.vChDir(temp);			// change dir
  }

//============================>>> Dos2UxFName <<<===========================
  void Dos2UxFName(char *dosFileName, char *uxFileName, bool trimfile)
  {
    //makes things working with gnu-win32/ming32
    //translates a MSDOS file name into a unix like file name

    char* np = uxFileName;
    char* p = dosFileName;

    for( ; *p ; p++, np++)
      {
	*np = (*p=='\\') ? '/' : *p;
      }
    *np = 0;

    if (trimfile)
      {
	int lim = strlen(uxFileName);
	for (int ix = lim ; ix >= 0 ; --ix)
	  {
	    if (uxFileName[ix] == '/' || uxFileName[ix] == '\\')
	      {
		uxFileName[ix] = 0;	// zap rest
		break;
	      }
	  }
      }
  }
