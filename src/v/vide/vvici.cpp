//=======================================================================
//	vvici.cpp:	Source for vvici class
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

#include "vvici.h"
#include <v/vcmdwin.h>
#include <v/vfilesel.h>
#include <fstream.h>
#include <v/vmodald.h>

    const static int SBAllocLines = 500;	// Save Buffer line allocations
    enum { Cmd, Ins, Find, CtrlW, Bang, Dquote, Squote,
      Less, Greater, Equal, At, LeftSq, RightSq,
      LeftTick, Record, Ch_F, Ch_T, Ch_c, Ch_d, Ch_f,
      Ch_g, Ch_m, Ch_q, Ch_r, Ch_t, Ch_y, Ch_z, Ch_Z
     };

    static int filterIndex = 0;
    static char* filter[] =
      {
        "*",
        "*.c;*.cpp;*.cxx;*.h",
        "*.txt;*.htm;*.html",
	"*.java",
        0
      };

// ==================>>> vSeeCI::vViCI <<<=================
  vViCI::vViCI(vTextEditor* textEd, vCmdWindow* cw)
        : vBaseTECmdInterp(textEd, cw)
  {

    findDirDown = 1;
    lex_def = 1;
    param_count = 1;
    cmdmode = Cmd;
    prefixCount = 1;
    countWait = 0;		// start in cmdmode
    slastl = savlen = nxtsav = 0;	// Save Buff stuff
    _SaveBuff  = new char*[SBAllocLines];	// build save in 500 line incrs.
    for (long lx = 0 ; lx < SBAllocLines ; ++lx)
        _SaveBuff[lx] = 0;

    _maxSBLines = SBAllocLines - 1;
  }

// ==================>>> vViCI::~vViCI <<<================
  vViCI::~vViCI()
  {
    if (_SaveBuff)
      {
	for (long lx = 0 ; lx < _maxSBLines ;++lx)	// delete contents
	  {
	    if (_SaveBuff[lx])
		delete [] _SaveBuff[lx];
	  }
	delete [] _SaveBuff;	// free SaveBuff	// delete line array
      }
  }

// =====================>>> vTextEdCmdInterp::InitCmdInterp <<<====================
  void vViCI::InitCmdInterp()
  {
    globalState es;
    es = te()->GetGlobalState();
    es.delOnInsert = 0;		// don't delete on insert
    es.findAtBeginning = 1;	// leave find at beginning
    te()->SetGlobalState(es);
  }

// =====================>>> vViCI::ProcessKey <<<====================
  int vViCI::ProcessKey(vKey key, unsigned int shift)
  {

    if (vk_IsModifier(key))             // ignore modifiers
	return -1;

    return edit(key,shift);
  }

// =============================>>> vViCI::edit   <<<==========================
  int vViCI::edit(vKey chr, unsigned int shift)
  {
    // Hard wired See command interp.

    int s_succ, retval;

    static int ins_set[] =		/* allowable commands for insert */
      {
	'i', 'o', 'O', 'a', 0
      };

    static int jump_set[] =	/* commands to not reset jump memory */
      {
	'\'', 'm', '`', 0
      };

    vKey lexval = chr; 		// @@ For now, simple translation

    if (cmdmode == Ins)		// in insert mode
      {
	// This code can allow the keypad keys to still move
	// the cursor around while still staying in insert mode...
	// but that seems to be a bad idea - ugly
	retval = 1;
	switch (lexval)
	  {
// =========================================================================
//
// ************* Insert mode **************
//
// =========================================================================

// CTRL-@		insert previously inserted text and stop insert
	    case '@'-'@':
// CTRL-A		insert previously inserted text
	    case 'A'-'@':
// CTRL-C		quit insert mode, without checking for abbreviation.
	    case 'C'-'@':
// CTRL-D		delete one shiftwidth of indent in the current line
	    case 'D'-'@':
// CTRL-E		insert the character which is below the cursor
	    case 'E'-'@':
// CTRL-G		reserved for future expansion
	    case 'G'-'@':
// CTRL-K {char1} {char2}	enter digraph
	    case 'K'-'@':
// CTRL-L		when 'insertmode' set: Leave Insert mode (@@@ ? what is this?)
	    case 'L'-'@':
// CTRL-N		find next match for keyword in front of the cursor
	    case 'N'-'@':
// CTRL-O		execute a single command and return to insert mode
	    case 'O'-'@':
// CTRL-P		find previous match for keyword in front of the cursor
	    case 'P'-'@':
// CTRL-R <0-9a-z"%#:=>	insert the contents of a register
	    case 'R'-'@':
// CTRL-T		insert one shiftwidth of indent in current line
	    case 'T'-'@':
// CTRL-U		delete all entered characters in the current line
	    case 'U'-'@':
// CTRL-W		delete word before the cursor
	    case 'W'-'@':
// CTRL-X {mode}	enter CTRL-X sub mode
	    case 'X'-'@':
// CTRL-X CTRL-D	complete defined identifiers
// CTRL-X CTRL-E	scroll up
// CTRL-X CTRL-F	complete file names
// CTRL-X CTRL-I	complete identifiers
// CTRL-X CTRL-K	complete identifiers from dictionary
// CTRL-X CTRL-L	complete whole lines
// CTRL-X CTRL-Y	scroll down
// CTRL-X CTRL-]	complete tags

// CTRL-Y		insert the character which is above the cursor
	    case 'Y'-'@':
// CTRL-]		trigger abbreviation
	    case ']'-'@':
// CTRL-_		change language (Hebrew, Farsi)
	    case '_'-'@':
	      {
		te()->ErrorMsg("Command not implemented.");
	        return 0;
	      }

// CTRL-F		not used
// CTRL-B		not used
// CTRL-Q		not used - control flow
// CTRL-S		(used for terminal control flow)
// CTRL-Z		suspend - never...
// CTRL-\		not used
// CTRL-^		not used
	    case 'B'-'@':
	    case 'F'-'@':
	    case 'Q'-'@':
	    case 'S'-'@':
	    case 'Z'-'@':
	    case '\\'-'@':
	    case '^'-'@':
	      {
		te()->ErrorMsg("Invalid Command.");
	        return 0;
	      }

	// following are actually the same as See - nice and similar...

	    case vk_BackSpace:		/* delete last character */
	      {
		retval = te()->charDelete(-cmdCount);
		break;
	      }

	    case vk_Delete:
	    case vk_KP_Delete:
	      {
		if (shift & VKM_Shift || shift & VKM_Ctrl)
		    te()->lineDelete(cmdCount);
		else
		    retval = te()->charDelete(cmdCount);
		break;
	      }


	    case vk_KP_Insert:
	    case vk_Insert:
	      {
		te()->SetInsMode(!(te()->GetEdState().ins_mode));
		break;
	      }

	    case vk_Escape:
	      {
		cmdmode = Cmd;
		prefixCount = 1;
		te()->ChangeInsMode(te()->GetEdState().ins_mode,"  Command ");
		return 1;		// done!
	      }

            case 'V'-'@':           // ^V: Paste
	      {
		te()->EditPaste();
		break;
	      }

	    default:
		retval = te()->defaultKeyIn(lexval, shift);
		break;
	  }

	return retval;
      }
    else if (cmdmode == Find)		// in Find pattern mode
      {
        if (chr == vk_Escape || chr == '\r' || chr == '\n')
          {
            cmdmode = Cmd;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode,"  Command ");
	    if (newFindPat[0] == 0)
		retval= te()->FindNext(0,findDirDown,0);
	    else
		retval = te()->Find(newFindPat,0,findDirDown,0);
	    cmdCount = 1;
	    return retval;
          }
        else
          {
	    echoInput(chr, newFindPat, MAX_LINE);
          }
        return 1;
      }

    switch (cmdmode)		// handle multi char commands
      {
	// Cmd, Ins, Find
	case CtrlW:
	  {
	    cmdmode = Cmd;
	    prefixCount = 1;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

// !{motion}{filter}	filter Nmove text through the {filter} command
// !!{filter}	filter N lines through the {filter} command
	case Bang:
	  {
	    cmdmode = Cmd;
	    prefixCount = 1;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Dquote:
	  {
// "<a-z>	use buffer <a-zA-Z0-9.%#:-"> for next
// 			delete, yank or put (upper case to append)
	    prefixCount = 1;
	    cmdmode = Cmd;
	    if (!(lexval >= 'a' && lexval <= 'z'))	// validate value
	      {
		te()->ErrorMsg("Valid range is <a-z>!");
		return 0;
	      }
	    else
		curReg = lexval;		// set value
	    return 1;
	  }

	case LeftTick:
// `<a-zA-Z0-9>	cursor to the first CHAR on the line with mark <a-zA-Z0-9>
// `<	cursor to the start of the highlighted area
// `>	cursor to the end of the highlighted area
// `[	cursor to the start of last operated text or start of putted text
// `]	cursor to the end of last operated text or
// 	end of putted text
// ``	cursor to the position before latest jump
	case Squote:
	  {
// '<a-zA-Z0-9>	cursor to the first CHAR on the line with mark <a-zA-Z0-9>
// ''	cursor to first CHAR of line where cursor was before latest jump
// '<	cursor to first CHAR of line where highlighted area starts/started
// 	   in the c urrent buffer.
// '>	cursor to first CHAR of line where highlighted area ends/ended
// 	   in the current buffer.
// '[	cursor to first CHAR on line of start of last operated
// 	  text or start of putted text
// ']	cursor to first CHAR on line of end of last operated text 
// 	  or end of putted text
	    prefixCount = 1;
	    cmdmode = Cmd;
	    retval = 0;
	    if (lexval >= 'a' && lexval <= 'z')
		retval = te()->EditCommand(edNoteGoto,cmdCount);
	    else if (lexval == '\'' || lexval == '`')
	      {
		long itmp = te()->GetCurLine();
		retval = te()->lineGoto(te()->JLine());
		te()->SetJLine(itmp);
	      }
	    else
		te()->ErrorMsg("Command not implemented.");
	    return retval;
	  }

        case Less:
	  {
// <{motion}	shift Nmove lines one 'shiftwidth' leftwards
// <<	shift N lines one 'shiftwidth' leftwards
// ={motion}	filter Nmove lines through "indent"
// ==	filter N lines through "indent"
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Greater:
	  {
// >{motion}	shift Nmove lines one 'shiftwidth' rightwards
// >>	shift N lines one 'shiftwidth' rightwards
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Equal:
	  {
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case At:
	  {
// @<a-z>	execute the contents of named buffer <a-z> N times
// @@	repeat the previous @<a-z> N times
	    cmdmode = Cmd;
	    retval = 0;
	    if (lexval >= 'a' && lexval <= 'z')
	      {
		curReg = lexval;
		retval = execQReg(curReg,prefixCount);
	      }
	    else if (lexval == '@')
	      {
		retval = execQReg(curReg,prefixCount);
	      }
	    else
	      {
		te()->ErrorMsg("@ must be followed by <a-z> or @.");
		retval = 0;
	      }
	    prefixCount = 1;
	    return retval;
	  }

	case LeftSq:
	  {
// [{char}	square bracket command (see below)
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case RightSq:
	  {
// ]{char}	square bracket command (see below)
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Ch_F:
	  {
// F{char}	cursor to the Nth occurrence of {char} to the left
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Ch_T:
	  {
// T{char}	cursor till after Nth occurrence of {char} to the left
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Ch_c:
	case Ch_d:
			// cmdmode has whatever
	  {
// ["x]c{motion}	delete Nmove text [into buffer x] and start insert
// ["x]d{motion}	delete Nmove text [into buffer x]

	    if (cmdmode == Ch_d && lexval == 'd')
	      {
		// ["x]dd -- delete N lines [into buffer x]
//		te()->lineDelete(prefixCount);
		save(prefixCount,0);
		retval = RemoveLast();
	      }
	    else if (cmdmode == Ch_c && lexval == 'c')
	      {
		// ["x]cc -- delete N lines [into buffer x] and start insert
//		te()->lineDelete(prefixCount);
		save(prefixCount,0);
		(void) RemoveLast();
		retval = te()->lineOpen(1);
		cmdmode = Ins;
		te()->ChangeInsMode(te()->GetEdState().ins_mode);
	      }
	    else 
	      {
		switch (lexval)		// a motion command - just the most used...
		  {
		    case vk_BackSpace:
		    case 'h':
			retval = te()->charDelete(-prefixCount);
			break;

		    case '$':
			(void) te()->lineDeleteToEnd();
			break;

		    case '0':
			(void) te()->lineDeleteFront();
			break;

		    case '\n':
		    case 'N'-'@':
		    case 'j':
			// te()->lineDelete(prefixCount+1);
			save(prefixCount,0);
			(void)RemoveLast();
			if (cmdmode == Ch_c)
			  {
			    te()->lineDown(1);
			    retval = te()->lineOpen(1);
		 	  }
			break;

		    case 'P'-'@':
		    case 'k':
		      {
			// te()->lineDelete(-(prefixCount+1));
			save(prefixCount,0);
			(void)RemoveLast();
			if (cmdmode == Ch_c)
			    retval = te()->lineOpen(1);
			break;
		      }

		    case ' ':
		    case 'l':
			retval = te()->charDelete(prefixCount);
			break;

		    case 'B':
		    case 'b':
			te()->wordRight(-prefixCount);
			retval = RemoveLast();
			break;

		    case 'W':
		    case 'w':
			te()->wordRight(prefixCount);
			retval = RemoveLast();
			break;

		    default:
			te()->ErrorMsg("Command not implemented.");
			cmdmode = Cmd;
			return 0;
		  }

		if (cmdmode == Ch_c)		// enter insert for 'c'
		  {
		    cmdmode = Ins;
		    te()->ChangeInsMode(te()->GetEdState().ins_mode);
		    prefixCount = 1;
		    return retval;
		  }
	      }
	    prefixCount = 1;
	    cmdmode = Cmd;
	    return retval;
	  }

	case Ch_f:
	  {
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Ch_g:
	  {
	    prefixCount = 1;
	    cmdmode = Cmd;
	    retval = 0;
	    switch (lexval)
	      {
	 	case 'm':	// middle of screen
		    te()->CenterScreen();
		    retval = 1;
		    break;

		case 'b':	// beautify
		    switch (te()->GetFileType())
		      {
			case Text:
			case HTML:
			case TeX:
			    retval = te()->lineFill(cmdCount);
			    break;

			case CPP:
			case Java:
			    retval = te()->formatC(cmdCount);
			    break;

			case gccError:
			case javaError:
			    break;

			default:
			    break;
			  }
		    break;


		default:
		    te()->ErrorMsg("Command not implemented.");
		    retval = 0;
		    break;
	      }
	    return retval;
	  }

	case Ch_m:		// mark a location
	  {
// m<A-Za-z>	set mark <A-Za-z> at cursor position
	    prefixCount = 1;
	    cmdmode = Cmd;
	    retval = 0;
	    if (lexval >='a' && lexval <= 'z')
		retval = te()->EditCommand(edNoteLocation,lexval);
	    else
		te()->ErrorMsg("Location must be <a-z>.");
	    return retval;
	  }

	case Ch_q:
	  {
// q<0-9a-zA-Z">	record typed characters into named register
// 		  <0-9a-zA-Z"> (upper case to append)
// ^q	(while recording) stops recording
	    retval = 1;
	    if (lexval >= 'a' && lexval <= 'z')
	      {
		cmdmode = Record;
		curReg = lexval;
		cmdCount = 1;
		newQReg[0] = 0;		// make empty
		te()->ChangeInsMode(te()->GetEdState().ins_mode," Recording");
	      }
	    else
	      {
		cmdmode = Cmd;
		te()->ErrorMsg("q must be followed by <a-z>");
		retval = 0;
	      }
	    return retval;
	  }

	case Record:		// record mode
	  {
	    char tmp[2] = "x";
	    if (chr == 'Q'-'@')
	      {
		cmdmode = Cmd;
		te()->ChangeInsMode(te()->GetEdState().ins_mode,"  Command ");
		// copy newQReg into static QReg array
		return setQReg(curReg);	// set the register
	      }
	    else
	      {
		echoQInput(chr,shift);
		return 1;
	      }
	  }

	case Ch_r:
	  {
// r{char}	replace chars with {char} N times
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

// t{char}	cursor till before Nth occurrence of {char} to right
	case Ch_t:
	  {
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Ch_y:
	  {
// ["x]y{motion}	yank Nmove text [into buffer x]
// ["x]yy	yank N lines [into buffer x]
// ["x]y	yank N lines [into buffer x]
	    if (lexval == 'y')
	      {
		retval = save(prefixCount,0);
		(void) te()->lineGoto(te()->JLine());
	      }
	    else if (lexval == '<')	// read file
	      {
		yankfile(1);
	      }
	    else if (lexval == '>')	// write file
	      {
		yankfile(-1);
	      }
	    else
		te()->ErrorMsg("Command not implemented.");
	    prefixCount = 1;
	    cmdmode = Cmd;
	    return 1;
	  }

	case Ch_z:
	  {
	    prefixCount = 1;
	    cmdmode = Cmd;
	    te()->ErrorMsg("Command not implemented.");
	    return 1;
	  }

	case Ch_Z:
	  {
// ZZ	store current file if modified, and exit
// ZQ	exit current file always
	    prefixCount = 1;
	    cmdmode = Cmd;
	    if (lexval == 'Z')
	      {
		cmdWin()->WindowCommand(M_SaveClose,1,(CmdType)0);
		return 1;
	      }
	    else
	      {
		te()->ErrorMsg("Command not implemented.");
		return 0;
	      }
	  }

	default:
	  {
	    prefixCount = 1;
	    cmdmode = Cmd;
	    break;
	  }

      }

    te()->ChangeInsMode(te()->GetEdState().ins_mode,"  Command ");

    if (lexval >= '0' && lexval <= '9')	// '-' not used in Vi...
      {
	if (countWait == 0 && lexval == '0') // leading 0 special
	  {
	    countWait = 0;	// no op - made coding easier
	  }
	else if (lexval >= '0' && lexval <= '9')      // it is a count
	  {
	    if (countWait == 0)
	      {
		cmdCount = 0;			// user entering value
		countWait = 1;
	      }
	    cmdCount = (cmdCount * 10) + (lexval - '0');
	    return 1;
	  }
      }

    countWait = 0;                      // done building count

    s_succ = 1;

    if (te()->GetCurLine() < 1)	// make sure legal command for empty buffer
      {
	if (!inset(lexval,ins_set))
	  {
	    te()->StatusMessage("Can't, buffer empty. Insert 1st ");
	    return 1;
	  }
      }

// Fall through for normal commands...

    if (!inset(lexval,jump_set))
	te()->SetJLine(te()->GetCurLine()); // sets previous line for Jump

    switch (lexval)
      {
// =========================================================================
//
// ************* Normal (command) mode **************
//
// =========================================================================
// CTRL-@		not used
	case '@'-'@':
// CTRL-K		not used
	case 'K'-'@':
// CTRL-Q		(used for terminal control flow)
	case 'Q'-'@':
// CTRL-S		(used for terminal control flow)
	case 'S'-'@':
// CTRL-[ <Esc>	not used
	case vk_Escape:
// CTRL-\		not used
	case '\\'-'@':
// CTRL-_		not used
	case '_'-'@':
// \			not used
	case '\\':
	  {
	    te()->ErrorMsg("Invalid Command.");
	    return 0;
	  }

// CTRL-A		add N to number at/after cursor
	case 'A'-'@':
// CTRL-E		scroll N lines upwards (N lines Extra)
	case 'E'-'@':
// CTRL-G		display current file name and position
	case 'G'-'@':
// <Tab>		go to N newer entry in jump list
// CTRL-I		same as <Tab>
	case vk_Tab:
// CTRL-O		go to N older entry in jump list
	case 'O'-'@':
// CTRL-R		redo changes which were undone with 'u'
	case 'R'-'@':
// CTRL-T		Tidy
	case 'T'-'@':
// CTRL-Y		scroll N lines downwards
	case 'Y'-'@':
// CTRL-Z		suspend program (or start new shell)
	case 'Z'-'@':
// CTRL-]		:ta to ident under cursor
	case ']'-'@':
// CTRL-^		edit Nth alternate file (equivalent to ":e #N")
	case '^'-'@':
// #	search backward for Nth occ. of ident under cursor
	case '#':
// &	repeat last :s
	case '&':
// (	cursor N sentences backward
	case '(':
// )	cursor N sentences forward
	case ')':
// *	search forward for Nth occurrence of ident under cursor
	case '*':
// +	cursor to the first nonwhite CHAR N lines lower
	case '+':
// ,	repeat latest f, t, F or T in opposite direction N times
	case ',':
// -	cursor to the first CHAR N lines higher
	case '-':
// .	repeat last change with count replaced with N
	case '.':
// ;	repeat latest f, t, F or T N times
	case ';':
// E	cursor forward to the end of WORD N
// e	cursor forward to the end of word N
	case 'e':
	case 'E':
// H	cursor to line N from top of screen
// L	cursor to line N from bottom of screen
// M	cursor to middle line of screen
	case 'H':
	case 'L':
	case 'M':
// I	insert text before first CHAR on line N times
	case 'I':
// K	lookup Keyword under cursor with 'keywordprg'
	case 'K':
// V	start linewise Visual mode
	case 'V':
// ^	cursor to the first CHAR of the line
	case '^':
// _	cursor to the first CHAR N - 1 lines lower
	case '_':
// v	start characterwise Visual mode
	case 'v':
// {	cursor N paragraphs backward
	case '{':
// |	cursor to column N
	case '|':
// }	cursor N paragraphs forward
	case '}':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    cmdCount = 1;
	    return 0;
	  }

// Q	switch to "Ex" mode
	case 'Q':
// :	start entering an Ex command
	case ':':
	  {
	    te()->ErrorMsg("Ex mode not supported. Use menus (e.g. find/replace).");
	    cmdCount = 1;
	    return 0;
	  }

// ******** DOWN motions

// <NL>		same as "j"
// CTRL-J	same as "j"
// CTRL-N		same as "j"
// j	cursor N lines downward
	case '\n':
	case 'N'-'@':
	case 'j':
	  {
	    retval = te()->lineDown(cmdCount);
	    break;
	  }

// <Down>	same as "j"
	case vk_KP_Down:
	case vk_Down:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else
		retval = te()->lineDown(cmdCount);
	    break;
	  }

// CTRL-D		scroll Down N lines (default: half a screen)
	case 'D'-'@':
	  {
	    if (cmdCount == 1)
		retval = te()->lineDown(
	    	  (long) te()->minl((cmdCount * (te()->GetRows()/2)),
	    	  (long)(te()->GetLines() - te()->GetCurLine() + 1)));
	    else
		retval = te()->lineDown(cmdCount);
	    break;
	  }

// CTRL-F		scroll N screens Forward
	case 'F'-'@':
	  {
	    retval = te()->lineDown(
	    	    (long) te()->minl((cmdCount * te()->GetRows()),
	    	    (long)(te()->GetLines() - te()->GetCurLine() + 1)));
	    break;
	  }

// Page Down
	case vk_Page_Down:
	case vk_KP_Page_Down:
	  {
	    if (shift & VKM_Ctrl)
		te()->scrollDown(cmdCount * te()->GetRows());
	    else
		retval = te()->lineDown(
	    	    (long) te()->minl((cmdCount * te()->GetRows()),
	    	    (long)(te()->GetLines() - te()->GetCurLine() + 1)));
	    break;
	  }

// ********* UP motions

// CTRL-B		scroll N screens Backwards
	case 'B'-'@':
	  {
	    retval = te()->lineDown((long)(-cmdCount * te()->GetRows()));
	    break;
	  }

// CTRL-P	same as "k"
// <Up>		same as "k"
// k	cursor N lines upward
	case 'P'-'@':
	case 'k':
	  {
	    retval = te()->lineDown(-cmdCount);
	    break;
	  }

// CTRL-U		scroll N lines Upwards (default: half a screen)
	case 'U'-'@':
	  {
	    if (cmdCount == 1)
		retval = te()->lineDown(
	    	  (long) te()->minl((-cmdCount * (te()->GetRows()/2)),
	    	  (long)(te()->GetLines() - te()->GetCurLine() + 1)));
	    else
		retval = te()->lineDown(-cmdCount);
	    break;
	  }

	case vk_Up:
	case vk_KP_Up:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else
		retval = te()->lineDown(-cmdCount);
	    break;
	  }

// <PageUp>	same as CTRL-B
	case vk_Page_Up:
	case vk_KP_Page_Up:
	  {
	    if (shift & VKM_Ctrl)
		te()->scrollDown(-cmdCount * te()->GetRows());
	    else
		retval = te()->lineDown((long)(-cmdCount * te()->GetRows()));
	    break;
	  }


// ***** GLOBAL motions

	case vk_Home:
	case vk_KP_Home:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else if (shift & VKM_Ctrl)
		retval = te()->lineGoto(1);
	    else
		te()->lineBeginning();
	    break;
	  }

	case vk_End:
	case vk_KP_End:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else if (shift & VKM_Ctrl)
		te()->bufferBottom();
	    else
	      {
		if (cmdCount > 1)
		    retval = te()->lineDown(cmdCount);
		te()->lineEnd();
	      }
	    break;
	  }

// ["x]<Del>	same as "x"
// {count}<Del>	remove the last digit from {count}
// @@@@ not compatible - just does a char delete, can't fix count
	case vk_Delete:
	case vk_KP_Delete:
	  {
	    if (shift & VKM_Shift || shift & VKM_Ctrl)
		te()->lineDelete(cmdCount);
	    else
		retval = te()->charDelete(cmdCount);
	    break;
	  }

// ******* LEFT motions

// CTRL-H		same as "h"
// <Left>	same as "h"
// h	cursor N chars to the left

	case 'h':
	  {
	    retval = te()->charRight(-cmdCount,1);
	    break;
	  }

	case vk_Left:
	case vk_KP_Left:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else if (shift & VKM_Ctrl)
		retval = te()->wordRight(-cmdCount);
	    else
		retval = te()->charRight(-cmdCount,1);
	    break;
	  }


// ****** RIGHT motions

// <Space>		same as "l"
// <Right>		same as "l"
// l	cursor N chars to the right
	case ' ':
	case 'l':
	  {
	    retval = te()->charRight(cmdCount,1);
	    break;
	  }

	case vk_Right:
	case vk_KP_Right:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else if (shift & VKM_Ctrl)
		retval = te()->wordRight(cmdCount);
	    else
		retval = te()->charRight(cmdCount,1);
	    break;
	  }


// CTRL-C	Copy to clipboard
        case 'C'-'@':           // ^C : Copy
          {
            te()->EditCopy();
            break;
          }

// CTRL-L		redraw screen
	case 'L'-'@':
	  {
	    te()->Verify();
	    break;
	  }

// <CR>		cursor to the first CHAR N lines lower
	case '\r':
	  {
	    retval = te()->lineDownBeg(cmdCount);
	    break;
	  }


// CTRL-P	Paste from clipboard
        case 'V'-'@':           // ^V: Paste
          {
            te()->EditPaste();
            break;
          }

// CTRL-X	Cut to clipboard
        case 'X'-'@':           // ^X : Cut
          {
            te()->EditCut();
            break;
          }

// CTRL-W {char}	window commands, see |CTRL-W|
	case 'W'-'@':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    prefixCount = cmdCount;
	    // cmdmode = CtrlW;
	    break;
	  }

// !{motion}{filter}	filter Nmove text through the {filter} command
// !!{filter}	filter N lines through the {filter} command
	case '!':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    prefixCount = cmdCount;
	    // cmdmode = Bang;
	    break;
	  }

// "<a-zA-Z0-9.%#:-">	use buffer <a-zA-Z0-9.%#:-"> for next
// 			delete, yank or put (upper case to append)
// 			(<.%#:> only work with put)
	case '\"':
	  {
	    cmdmode = Dquote;
	    prefixCount = cmdCount;
	    break;
	  }

// $	cursor to the end of Nth next line
	case '$':
	  {
	    if (cmdCount > 1)
		retval = te()->lineDown(cmdCount);
	    te()->lineEnd();
	    break;
	  }

// %	find next (curly/square) bracket on this line and go to
// 	   its match, or go to matching comment bracket, or go to matching
// 	   preprocessor directive.
// @@@@ not compatibly implemented....
	case '%':			
	  {
// {count}%	go to N percentage in the file
	    if (cmdCount > 0)
		te()->ErrorMsg("Command not implemented.");
	    else
		retval = te()->BalMatch(cmdCount);
	    break;
	  }

	case '\'':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Squote;
	    prefixCount = cmdCount;
	    break;
	  }

// /{pattern}<CR>	search forward for the Nth occurrence of {pattern}
// /<CR>	search forward for {pattern} of last search
	case '/':		// F: set find pattern only - unechoed
          {
            cmdmode = Find;
	    findDirDown = 1;
            te()->ChangeInsMode(te()->GetEdState().ins_mode," Find Pat ");
            newFindPat[0] = 0;	// no pattern yet
            break;
          }

// ?<pattern><CR>	search backward for the Nth previous occurrence of <pattern>
// ?<CR>	search backward for {pattern} of last search
	case '?':
          {
            cmdmode = Find;
	    findDirDown = 0;
            te()->ChangeInsMode(te()->GetEdState().ins_mode," Find Pat ");
            newFindPat[0] = 0;	// no pattern yet
            break;
          }


// 0	cursor to the first char of the line
	case '0':
	  {
	    te()->lineBeginning();
	    break;
	  }

// <{motion}	shift Nmove lines one 'shiftwidth' leftwards
	case '<':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Less;
	    prefixCount = cmdCount;
	    break;
	  }

	case '>':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Greater;
	    prefixCount = cmdCount;
	    break;
	  }

// @<a-z> -- execute q-buffer N times
	case '@':
	  {
	    cmdmode = At;
	    prefixCount = cmdCount;
	    break;
	  }

// A	append text after the end of the line N times
	case 'A':
	  {
	    te()->lineEnd();
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;
	  }

// B	cursor N WORDS backward
// @@@@ not exactly same
	case 'B':
	  {
	    retval = te()->wordRight(-cmdCount);
	    break;
	  }

// ["x]C	change from cursor position to end of line, and N-1 more lines
// 	  [into	buffer x]; synonym for "c$"
	case 'C':
	  {
	    (void) te()->lineDeleteToEnd();
//	    retval = te()->lineDeleteFront();
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;
	  }

// ["x]D	delete chars under cursor until end of line and N-1 more
// 	  lines [into buffer x]; synonym for "d$"
	case 'D':
	  {
	    (void) te()->lineDeleteToEnd();
	    if (cmdCount > 1)
	      {
		(void) te()->lineDownBeg(1);
		(void) te()->lineDelete(cmdCount-1);
		(void) te()->lineDownBeg(-1);
		(void) te()->lineEnd();
	      }
	    break;
	  }

// F{char}	cursor to the Nth occurrence of {char} to the left
	case 'F':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Ch_F;
	    prefixCount = cmdCount;
	    break;
	  }

// G	cursor to line N, default last line
// @@@@@@ not quite compatible - can't tell 1 from nothing on count
//	so can't go to line 1
	case 'G':
	  {
	    if (cmdCount != 1)
		retval = te()->lineGoto(cmdCount);
	    else
		te()->bufferBottom();

	    break;
	  }

// J	Join N lines; default is 2
// @@@@ Not compatible - just joins 2 lines, no N
	case 'J':
	  {
	    (void) te()->lineEnd();
	    retval = te()->charDelete(1);
	    break;
	  }

// O	begin a new line above cursor and insert text, repeat N times
	case 'O':
	  {
	    retval = te()->lineOpen(cmdCount);
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;
	  }

// ["x]P	put the text [from buffer x] before cursor N times
	case 'P':
	  {
	    retval = getsav();
	    break;
	  }

// R	enter replace mode: overtype existing characters;
// 	  repeat the entered text N-1 times
// @@@@@ Not compatible - toggles overtype
	case 'R':
	  {
	    te()->SetInsMode(!(te()->GetEdState().ins_mode));
	    break;
	  }

// ["x]S	delete N lines [into buffer x] and start insert;
// 	  synonym for "^cc" or "0cc", depending on autoindent
	case 'S':
	  {
	    te()->lineDelete(cmdCount);
	    retval = te()->lineOpen(1);
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;
	  }

// T{char}	cursor till after Nth occurrence of {char} to the left
	case 'T':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Ch_T;
	    prefixCount = cmdCount;
	    break;
	  }

// U	undo all latest changes on one line
// u	undo changes
// @@ not compatible, U and u same
	case 'U':
	case 'u':
	  {
            te()->undoEdit();
	    break;
	  }

// W	cursor N WORDS forward
	case 'W':
	  {
	    retval = te()->wordRight(cmdCount);
	    break;
	  }

// <BS>		map to X - more conventional
	case vk_BackSpace:
// ["x]X	delete N characters before the cursor [into buffer x]
	case 'X':
	  {
	    // don't delete highlight
	    retval = te()->charDelete(-cmdCount,true,false);
	    break;
	  }

// ["x]Y	yank N lines [into buffer x]; synonym for "yy"
// @@ not compatible - leaves cursor at end of Yank, not beginning
	case 'Y':
	  {
	    retval = save(cmdCount,0);
	    (void) te()->lineGoto(te()->JLine());
	    break;
	  }

// ZZ	store current file if modified, and exit
// ZQ	exit current file always
	case 'Z':
	  {
	    cmdmode = Ch_Z;
	    prefixCount = cmdCount;
	    break;
	  }

// [{char}	square bracket command (see below)
	case '[':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = LeftSq;
	    prefixCount = cmdCount;
	    break;
	  }


// ]{char}	square bracket command (see below)
	case ']':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = RightSq;
	    prefixCount = cmdCount;
	    break;
	  }

// `<a-zA-Z0-9>	cursor to the mark <a-zA-Z0-9>
	case '`':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = LeftTick;
	    prefixCount = cmdCount;
	    break;
	  }

// a	append text after the cursor N times
	case 'a':
	  {
	    te()->charRight(1,1);
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;
	  }

// b	cursor N words backward
	case 'b':
	  {
	    retval = te()->wordRight(-cmdCount);
	    break;
	  }

// ["x]c{motion}	delete Nmove text [into buffer x] and start insert
// ["x]cc	delete N lines [into buffer x] and start insert
	case 'c':
	  {
	    cmdmode = Ch_c;
	    prefixCount = cmdCount;
	    break;
	  }

// ["x]d{motion}	delete Nmove text [into buffer x]
// ["x]dd	delete N lines [into buffer x]
	case 'd':
	  {
	    cmdmode = Ch_d;
	    prefixCount = cmdCount;
	    break;
	  }

// f{char}	cursor to Nth occurrence of {char} to the right
	case 'f':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Ch_f;
	    prefixCount = cmdCount;
	    break;
	  }

// g{char}		extended commands, see below
	case 'g':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Ch_g;
	    prefixCount = cmdCount;
	    break;
	  }

	case vk_KP_Insert:
	case vk_Insert:
// i	insert text before the cursor N times
// <Insert> same as "i"
	case 'i':
	  {
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;
	  }

// m<A-Za-z>	set mark <A-Za-z> at cursor position
	case 'm':
	  {
	    cmdmode = Ch_m;
	    prefixCount = cmdCount;
	    break;
	  }

// N	repeat the latest '/' or '?' N times in opposite direction
	case 'N':
	  {
	    retval= te()->FindNext(0,!findDirDown,0);
	    break;
	  }

// n	repeat the latest '/' or '?' N times
	case 'n':			/* find again */
	  {
	    retval= te()->FindNext(0,findDirDown,0);
	    break;
	  }

// o	begin a new line below the cursor and insert text, repeat N times
	case 'o':
	  {
	    (void) te()->lineDownBeg(1);
	    retval = te()->lineOpen(cmdCount);
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;
	  }

// ["x]p	put the text [from register x] after the cursor N times
	case 'p':
	  {
	    (void) te()->lineDownBeg(1);
	    retval = getsav();
	    break;
	  }

// q<a-z>	record typed characters into named register
// q	(while recording) stops recording
	case 'q':
	  {
	    cmdmode = Ch_q;
	    prefixCount = cmdCount;
	    break;
	  }

// r{char}	replace chars with {char} N times
	case 'r':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Ch_r;
	    prefixCount = cmdCount;
	    break;
	  }

// ["x]s	(substitute) delete N characters [into buffer x] and start insert
	case 's':
	  {
	    if ((retval = te()->charDelete(cmdCount)) )
	      {
		cmdmode = Ins;
		te()->ChangeInsMode(te()->GetEdState().ins_mode);
	      }
	    break;
	  }

// t{char}	cursor till before Nth occurrence of {char} to right
	case 't':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Ch_t;
	    prefixCount = cmdCount;
	    break;
	  }

// w	cursor N words forward
// @@@@ not quite compatible - different algorithm for word
	case 'w':
	  {
	    retval = te()->wordRight(cmdCount);
	    break;
	  }

// ["x]x	delete N characters under and after cursor [into buffer x]
	case 'x':
	  {
	    retval = te()->charDelete(cmdCount,true,false);
	    break;
	  }

// ["x]y{motion}	yank Nmove text [into buffer x]
// ["x]yy	yank N lines [into buffer x]
	case 'y':
	  {
	    cmdmode = Ch_y;
	    prefixCount = cmdCount;
	    break;
	  }

// z<CR>	   redraw, cursor line to top of window, cursor on first non-blank
// z{height}<CR>	redraw, make window {height} lines high
// z.	redraw, cursor line to center of window, cursor on first non-blank
// z-	redraw, cursor line at bottom of window, cursor on first non-blank
// zb	redraw, cursor line at bottom of window
// ze	when 'wrap' off scroll horizontally to position the cursor
// 	  at the end (right side) of the screen
// zh	when 'wrap' off scroll screen N characters to the right
// zl	when 'wrap' off scroll screen N characters to the left
// zs	when 'wrap' off scroll horizontally to position the cursor
// 	  at the start (left side) of the screen
// zt	redraw, cursor line at top of window
// zz	redraw, cursor line at center of window
// z<Left>	same as "zh"
// z<Right>	same as "zl"

	case 'z':
	  {
	    te()->ErrorMsg("Command not implemented.");
	    // cmdmode = Ch_z;
	    prefixCount = cmdCount;
	    break;
	  }


// ~	'tildeop' off: switch case of N characters under cursor 
// 	   and move the cursor N characters to the right
	case '~':
	  {
	    retval = te()->charFoldCase(cmdCount);
	    break;
	  }

// <LeftMouse>	move cursor to the mouse click position

	default:
	  {
	    te()->ErrorMsg("Invalid Command.");
	    retval = 0;
	    break;
	  }
      }  			/* end of switch */

    cmdCount = 1;
    if (! retval)		// handle repeat loops
	return 0;
    else
	oldlex = lexval;

    return 1;
  }

// =====================>>> vTextEdCmdInterp::InitCmdInterp <<<====================
  void vViCI::CmdInterpHelp()
  {
    static char* helplist [] =
      {
	"Vi Editor emulation",
	"   (* after cmd means emulation not exact)",
	"Key  --  Command Description",
	"  ",
	"  ***  Movement Commands  ***",
"h,<Left>  --  cursor N chars to the left",
"j,<Down>  --  cursor N lines downward",
"k,<Up>  --  cursor N lines up",
"l,<Right>,<Space>  --  cursor N chars to the right",
"m<a-z>  --  set mark <a-z> at cursor position",
"CTRL-D  --  scroll Down N lines (default: half a screen)",
"CTRL-U  --  scroll N lines Upwards (default: half a screen)",
"CTRL-B,<PageUp>  --  scroll N screens Backwards",
"CTRL-F,<PageDown>  --  scroll N screens Forward",
"<CR>  --  cursor to the first CHAR N lines lower",
"0  --  cursor to the first char of the line",
"$  --  cursor to the end of Nth next line",
"<Home>  --  line beginning",
"<CTRL-Home>  --  file beginning",
"<End>  --  line end",
"<CTRL-End>  --  file end",
"B*  --  cursor N WORDS backward ['word' not same]",
"b  --  cursor N words backward",
"`<a-z>  --  cursor to the first CHAR on the line with mark <a-z>"
"``  --  cursor to the position before latest jump",
"'<a-z>  --  cursor to the first CHAR on the line with mark <a-z>",
"''  --  cursor to first CHAR of line where cursor was before latest jump",
"<MouseClick>  --  move cursor to the mouse click position",
	" ",
	"  ***  Searching commands ***",
	" ",
"/{pattern}<CR>  --  search forward for {pattern}",
"/<CR>  --  search forward for {pattern} of last search",
"?{pattern}<CR>  --  search backward for {pattern}",
"?<CR>  --  search backward for {pattern} of last search",
"N  --  repeat the latest '/' or '?' in opposite direction",
"n  --  repeat the latest '/' or '?'",
"%  --  go to matching paren (){}[]",
	" ",
	"  ***  Insertion Commands ***",
	" ",
"A  --  append text after the end of the line",
"a  --  append text after the cursor",
"i,<Insert>  --  insert text before the cursor (until Esc)",
"O  --  begin a new line above cursor and insert text",
"o  --  begin a new line below the cursor and insert text",
"R*  --  toggle replace mode: overtype existing characters [just toggle]",
"CTRL-C  --  Copy to clipboard",
"CTRL-V  --  Paste from clipboard",
"CTRL-X  --  Cut to clipboard",
	" ",
	"  ***  Kill/change commands ***",
	" ",
"C  --  change from cursor position to end of line, and N-1 more",
"cc  --  delete N lines and start insert",
"c[bBhjklwW$0]  --  delete Nmotion and start insert",
"D  --  delete chars under cursor until end of line and N-1 more",
"dd  --  delete N lines",
"d[bBhjklwW$0]  --  delete Nmotion",
"J*  --  Join 2 lines [2 lines only]",
"S  --  delete N lines and start insert;",
"s  --  (substitute) delete N characters and start insert",
"u  --  undo changes",
"X, <BS>*  --  delete N characters before the cursor",
"x, <Del>  --  delete N characters under and after cursor",
"<Del>  --  delete highlight or N characters under and after cursor",
"~  --  switch case of N characters under cursor ",
	" ",
	"  ***  Yank buffer commands ***",
	" ",
"P  --  put the text on line before cursor",
"p  --  put the text on line after the cursor",
"Y*  --  yank N lines; synonym for 'yy' [cursor at end]",
"y<  --  read file into yank buffer",
"y>  --  write yank buffer to file",
	" ",
	"  ***  Misc. commands  ***",
"CTRL-L  --  redraw screen",
"ZZ  -- save file and close window",
"gb  --  beautify N lines of C/C++/Java or text",
"gm  --  center cursor in screen",
"\"<a-z>  --  set q-reg/buff for next op",
"[\"<a-z>]q  --  record to q-register until ^Q",
"[\"<a-z>]@  --  execute q-register N times",
	 0
      };

    static CommandObject HelpD[] =
      {
        {C_Button,M_Cancel,0,"Close",NoList,CA_None,isSens,NoFrame,0,0},
	{C_Label,8000,0,"List of Vi Editor commands",NoList,CA_None,isSens,NoFrame,M_Cancel,0},
	{C_List,8001,0,"Help",(void*)helplist,CA_Size,
		isSens,	NoFrame,0,M_Cancel,32},
	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
        
      };

    // I'd have liked a modeless dialog, but then I'd have to track
    // and delete the object. This way it gets dynamically created
    // each  time.

    int temp;

    vModalDialog Help(te()->GetPaneParent(),"Editor Help");
    Help.AddDialogCmds(HelpD);
 
    Help.ShowModalDialog("help",temp);

  }

/* =============================>>> vViCI::inset <<<============================= */
  void vViCI::MouseJump(int row, int col)
  {
    // Jumping elsewhere via a mouse click means we should go back to
    // command mode. Too many messy things happen if we don't
    cmdmode = Cmd;
    te()->ChangeInsMode(te()->GetEdState().ins_mode,"  Command ");
  }

/* =============================>>> vViCI::inset <<<============================= */
  int vViCI::inset(int val, int *set)
  {
     /* return true if val is in set set */

    while (*set)
	if (val == *set++)
	    return 1;
    return 0;
  }

// =============================>>> vViCI::RemoveLast <<<==========================
  int vViCI::RemoveLast(void)
  {  /* RemoveLast - delete the previous thing found or manipulated
      *	length of oldlen is set by insert, find, and save
      *	may also use savlen if set by save */

    if (te()->RemoveMarkRange())	/* was a mark range to remove */
	return 1;			/* all done */

//    if (!inset(oldlex,rmv_set))	// called only internally in Vi
//	return 0;

    te()->ClearMarkRange();		/* no range now */

    if (savlen > 0)
      {
        int oldech = te()->GetEdState().echof;
        if (savlen > 1)
            te()->SetEchoF(0);
	if (te()->GetCurLine() == te()->GetLines()-1 && slastl != 0)
	  {
	    --savlen;	/* reduce the count */
	    if (savlen > 0)
	      {
		te()->lineDelete(-savlen);	/* kill off previous lines */
              }
	    te()->lineDelete((long) 1);		/* kill the last line */
            if (oldech)
                te()->SetEchoF(oldech);
            te()->Verify();
	  }
	else
          {
	    te()->lineDelete(-savlen);		/* kill off savlen lines */
            if (oldech)
                te()->SetEchoF(oldech);
            if (savlen > 1)
            	te()->Verify();
          }

      }
    else if (te()->OldLen() != 0)
      {
	if (! te()->charDelete((long)-te()->OldLen()) )
	    return 0;
      }
    te()->SetOldLen(0);			/* don't allow multiple deletes! */
    savlen = (-1);
    return 1;
  }

// =============================>>> vViCI::save   <<<=========================
  int vViCI::save(long cnt, int app)
  { /* save - save cnt lines in save buffer */

    long l, lend;

    int old_ef = te()->GetEdState().echof;   	// remember state of echo flag

    if (cnt < 0)
	return 0;

    if (te()->GetCurLine() == te()->GetLines()-1 && slastl != 0)
      {
	te()->ErrorMsg("Can't save last line twice! ");
	return 0;
      }

    te()->SetOldLen(0);			/* use savlin instead */

    if (!app || cnt == 0)
      { 		/* if new save, cnt == 0 and not appending */
        // Clear old save buffer - delete storage for lines
        for (long lx = 0 ; lx < nxtsav ; ++lx)  // copy old lines
	    if (_SaveBuff[lx])
	      {
                delete [] _SaveBuff[lx];	// free space
                _SaveBuff[lx] = 0;	// free space
	      }

        slastl = savlen = nxtsav = 0;	/* reset these guys */

	if (cnt == 0)
	  {
	    return 1;
	  }
      }

    if (app)		/* need to reset savlen for append */
	savlen = 0;

    if (cnt > 10)
	te()->SetEchoF(0);

    lend = (te()->GetCurLine()+cnt-1 < te()->GetLines()-1)
	    ? te()->GetCurLine()+cnt-1
	    : te()->GetLines()-1 ;

    for (l = te()->GetCurLine() ; l <= lend ; ++l)
      {
	if (nxtsav >= _maxSBLines)         // check for max lines
	  {
	    if (!reallocSaveBuff())
	      {
		if (cnt > 10)
		  {
		    te()->SetEchoF(old_ef);
		    te()->Verify();
		  }
		te()->ErrorMsg("Not enough memory for save ");
		return 0;
	      }
          }

        char line[MAX_LINE + 2];
        te()->getLine(line, MAX_LINE, l);	// fetch the line
        char* cp = new char[strlen(line) + 1];
        strcpy(cp, line);
        _SaveBuff[nxtsav++] = cp;	// copy the line to buffer

	++savlen;		// savlen for RemoveLast
	if (te()->GetCurLine() == te()->GetLines()-1)	/* don't save last line twice! */
	  {
	    slastl = 1;
	    break;
	  }
        (void) te()->lineDownBeg(1);
      }

    if (cnt > 10)
      {
	te()->SetEchoF(old_ef);
	te()->Verify();
      }
    return 1;
  }

//========================>>> vViCI::reallocSaveBuff <<<======================
  int vViCI::reallocSaveBuff()
  {
    BUFFPTR* oldLines = _SaveBuff;

    _SaveBuff  = new char*[_maxSBLines + SBAllocLines];
    if (!_SaveBuff)
      {
	_SaveBuff = oldLines;              // failed to get more lines
	return 0;
      }

    long lx;
    for (lx = 0 ; lx <= _maxSBLines ; ++lx)  // copy old lines
	_SaveBuff[lx] = oldLines[lx];

    _maxSBLines = _maxSBLines + SBAllocLines - 1;       // safety factor

    for (; lx <= _maxSBLines ; ++lx)  // null new lines
	_SaveBuff[lx] = 0;


    delete [] oldLines;                 // free the old lines
    return 1;
  }

//=============================>>> vViCI::getsav <<<==========================
  int vViCI::getsav()
  { /* ## getsav - get text from save buffer */

    long lx;

    if (nxtsav <= 0)		/* nothing to get */
      {
	return 1;
      }

    int old_ef = te()->GetEdState().echof;
    te()->SetEchoF(0);

    te()->ClearMarkRange();		/* no range now */

    // If someday I want the insert to be allowed in the middle
    // of a line, then the easiest way is to insert the first line
    // of the save buffer char by char, then bulk insert the rest
    // after the next line. For now, just insert in front of current
    // line.

    te()->lineBeginning();			// force to beginning

    long curLine = te()->GetCurLine();		// find current line

    for (lx = nxtsav - 1 ; lx >= 0 ; --lx)
       te()->insertLine(_SaveBuff[lx], curLine);	// insert the lines backwards

    te()->lineDownBeg(nxtsav);			// go back down to where we were

    te()->IncChanges();				/* note changes */

    te()->SetOldLen(0);				// fix up this stuff
    savlen = nxtsav;				// update for removelast
    te()->SetEchoF(1);
    te()->Verify();
    return 1;
  }

// =============================>>> vViCI::yankfile   <<<=========================
  int vViCI::yankfile(long cnt)
  { 	// yank file to or from save bufer


    int old_ef = te()->GetEdState().echof;   	// remember state of echo flag

    char buff[MAX_LINE+2];
    vFileSelect fsel(cmdWin());

    if (cnt < 0)		// write save buffer
      {
	buff[0] = 0;
	int ians = fsel.FileSelectSave("Write yank buffer to file",
		buff, MAX_LINE, filter, filterIndex);
	if (!ians)
	    return 0;

	ofstream fout(buff);		// open the file to yank
 	if (!fout)
	    return 0;

	for (long lx = 0; lx < nxtsav ; ++lx)
	    fout << _SaveBuff[lx] << endl;
	fout.close();
	return 1;
      }
    else
      {
	buff[0] = 0;
	int ians = fsel.FileSelect("Read file to yank buffer",
		buff, MAX_LINE, filter, filterIndex);
	if (!ians)
	    return 0;

	ifstream fin(buff);		// open the file to yank
 	if (!fin)
	    return 0;

        // Clear old save buffer - delete storage for lines
        for (long lx = 0 ; lx < nxtsav ; ++lx)  // copy old lines
	    if (_SaveBuff[lx])
	      {
                delete [] _SaveBuff[lx];	// free space
                _SaveBuff[lx] = 0;	// empty now
	      }

        slastl = savlen = nxtsav = 0;	/* reset these guys */

	while (fin.getline(buff,511))
	  {
	    if (nxtsav >= _maxSBLines)         // check for max lines
	      {
		if (!reallocSaveBuff())
		  {
		    te()->ErrorMsg("File too big -- only partially read ");
		    fin.close();
		    return 0;
		  }
	      }
	    // have a line to add
	    char* cp = new char[strlen(buff) + 1];
	    strcpy(cp, buff);
	    _SaveBuff[nxtsav++] = cp;	// copy the line to buffer

	  }
	fin.close();
      }
    return 1;
  }

