//=======================================================================
//	vbaseci.h - header
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
#ifndef VBASETECMDINTERP
#define VBASETECMDINTERP

#include <v/vkeys.h>
#include <v/vtexted.h>

     class vBaseTECmdInterp : public vTextEdCmdInterp	// easy class to override
      {

        public:
          vBaseTECmdInterp(vTextEditor* textEd, vCmdWindow* cw);
          virtual ~vBaseTECmdInterp();


          virtual int ProcessKey(vKey key, unsigned int shift);
          // virtual vTextEditor* te() VCONST { return _textEd; }
          // virtual vCmdWindow* cmdWin() VCONST { return _myCmdWindow; }
          virtual void InitCmdInterp();
          virtual void CmdInterpHelp();
	  virtual void MouseJump(int row, int col);

	  // new to vBaseTECmdInterp
	  virtual void echoInput(vKey chr, char* buff, int maxb);
	  virtual void echoQInput(vKey chr, unsigned int shift);
	  virtual int doHighlight(long cmdCount, vKey key, unsigned int shift);

	protected:

	  // long cmdCount;	// how many times to repeat command

	  // int
	  //  	countWait,  // if building a count
	  //	metaChar,   // the meta char for commands
	  //	metaWait;   // if waiting on a meta command

	    int lenQ(vKey* qreg);
	    int setQReg(int reg);
	    vKey* getQReg(int reg);
	    unsigned short* getQRegShift(int reg);
	    int execQReg(int reg, long count);

	    int curReg;		// Q Reg or globalBuff

	    // Q Registers
	    vKey newQReg[MAX_LINE+2];
	    unsigned int newQRegShift[MAX_LINE+2];
	    static vKey* QReg[28];	// a-z + 1
	    static unsigned short* QRegShift[28];

	    char newFindPat[MAX_LINE+2];	// for keyboard find input

        private:
	    vTextEditor* _textEd;
	    vCmdWindow* _myCmdWindow;
      };
#endif
