//=======================================================================
// V IDE Make Class - all the stuff needed to create
// makefiles, and run make.
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
//  videMake header

#ifndef videMake_h
#define videMake_h

#include <v/vos.h>

    class videCmdWindow;
    class videMake
      {
      public:
	videMake();
	~videMake();
	int DoMake(videCmdWindow* parent, char* cmd, char* what);
	int CheckMake(char* makecmd, char* what);
        void StopMake() { _stop = 1; }

      protected:
        void ShowErrors(videCmdWindow* cmdw, char* errName);

      private:

        videCmdWindow* cmdw;
        int _stop;

      };

#endif
