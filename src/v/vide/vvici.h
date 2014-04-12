//=======================================================================
//	vvici.h - header
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

#ifndef VVICI_H
#define VVICI_H

#include "vbaseci.h"

    class vViCI : public vBaseTECmdInterp  	// easy class to override
      {

        public:
          vViCI(vTextEditor* textEd, vCmdWindow* myCmdWin);
          ~vViCI();

          virtual int ProcessKey(vKey key, unsigned int shift);
          virtual void InitCmdInterp();
	  virtual void MouseJump(int row, int col);

        protected:
	  int prefixCount;
          int edit(vKey chr, unsigned int shift);
          int inset(int val, int *set);
          int RemoveLast(void);
          int save(long cnt, int app);
          int getsav();
	  int yankfile(long cnt);

          int reallocSaveBuff();	// for adding more save lines

	  virtual void CmdInterpHelp();

        private:
           int	cmdmode,		// 0: insert, 1: cmd
           	countWait,
           	lex_def,
		findDirDown;
    	   long CmdCount,
           	param_count,
                slastl,			// Save Buff stuff
                savlen,			// number of lines for rmvlast
                nxtsav,			// next place to save line
                _maxSBLines;
           vKey oldlex;			// previous command
           BUFFPTR* _SaveBuff;		// save buffer

      };
#endif
