//===============================================================
// vPrefs.h - The vPrefs class - Windows
//
// Copyright (C) 1995,1996  Bruce E. Wampler
//
// This file is part of the V C++ GUI Framework, and is covered
// under the terms of the GNU Library General Public License,
// Version 2. This library has NO WARRANTY. See the source file
// vapp.cxx for more complete information about license terms.
//===============================================================

#ifndef VPREFS_H
#define VPREFS_H

#include <fstream.h>

    class vPrefs
      {

      public:		//---------------------------------------- public
	vPrefs();
	~vPrefs();

	int openSave(const char* name, const char* title = "[prefs]");
	void closeSave();
	void addString(const char* entry, char* str);
	void addLong(const char* entry, long val);

	int openRead(const char* name, const char* title="[prefs]");
	void closeRead();
	int getString(const char* entry, char* str);
	int getLong(const char* entry, long& val);


      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private
	ifstream inFile;
	ofstream outFile;

  };

#endif
