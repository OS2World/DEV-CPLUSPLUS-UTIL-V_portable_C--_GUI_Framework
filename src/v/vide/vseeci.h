//=======================================================================
//	vedci.h - header
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

#ifndef VSEECI_H
#define VSEECI_H

#include "vbaseci.h"

const int SBAllocLines = 500;	// Save Buffer line allocations


    class vSeeCI : public vBaseTECmdInterp  	// easy class to override
      {

        public:
          vSeeCI(vTextEditor* textEd, vCmdWindow* myCmdWin);
          ~vSeeCI();

          virtual int ProcessKey(vKey key, unsigned int shift);
          virtual void InitCmdInterp();
	  virtual void MouseJump(int row, int col);

        protected:
          int edit(vKey chr, unsigned int shift);
          int inset(int val, int *set);
          int RemoveLast(void);
          int save(long cnt, int app);
          int getsav();
	  int yankfile(long cnt);

          int reallocSaveBuff();	// for adding more save lines

	  virtual void CmdInterpHelp();

        private:


	   // Save buffers
           BUFFPTR* _SaveBuff;		// save buffer
	   

           int	cmdmode,		// 0: insert, 1: cmd
           	countWait,
           	lex_def;
		
	   long	param_count,
                slastl,			// Save Buff stuff
                savlen,			// number of lines for rmvlast
                nxtsav,			// next place to save line
                _maxSBLines;
           vKey oldlex;			// previous command
      };
#endif
