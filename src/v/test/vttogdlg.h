//=======================================================================
// vttogdlg.h - Header file for toggleDialog class
//  Copyright (C) 1995,1996  Bruce E. Wampler
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

#ifndef TOGGLEDIALOG_H
#define TOGGLEDIALOG_H
#include <v/vmodald.h>

    class toggleDialog : public vModalDialog
      {
      public:		//---------------------------------------- public
	toggleDialog(vBaseWindow*);
	virtual ~toggleDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected
	virtual ItemVal ShowModalDialog(VCONST char* msg, ItemVal& rv);
        virtual void DialogDisplayed();

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

      };

    class listDialog : public vDialog
      {
      public:		//---------------------------------------- public
	listDialog(vBaseWindow*);
	virtual ~listDialog();		// Destructor
	virtual void DialogCommand(ItemVal,ItemVal,CmdType); // action selected

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

      };

#endif
