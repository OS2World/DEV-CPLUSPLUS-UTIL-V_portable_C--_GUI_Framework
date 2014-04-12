//=======================================================================
//  mydialog.h - Header file for myDialog class
//  Copyright (C) 1995  Bruce E. Wampler
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

#ifndef MYDIALOG_H
#define MYDIALOG_H
#include <v/vdialog.h>

    class myDialog : public vDialog
      {
      public:		//---------------------------------------- public
	myDialog(vBaseWindow*, char* title);
	virtual ~myDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	void AddDefaultCmds();		// to add the defined commands

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	int _toggleId;

      };
#endif
