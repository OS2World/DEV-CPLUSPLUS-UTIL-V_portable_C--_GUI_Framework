#ifndef VIDELIB_H
#define VIDELIB_H
//=======================================================================
//	videlib.h:	Source for videlib
//  Copyright (C) 1995-2000  Bruce E. Wampler
//	date: 23 Feb 2000 11:16:42
//
//  This program is part of the V IDE

  void fixSlash(char* fn);
  char* fixBackSlash(char* fn, int doit = 1);
  void splitFileName(const char* fn, char* fdir, char* fbase, char* fext);
  void changeDir(const char* fullname);
  void Dos2UxFName(char *dosFileName, char *uxFileName, bool trimfile);
#endif
