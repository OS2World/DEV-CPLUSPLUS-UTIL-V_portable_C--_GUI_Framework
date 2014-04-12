//=======================================================================
//	vseeci.cpp:	Source for vseeci class
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

#include "vseeci.h"
#include <v/vcmdwin.h>
#include <v/vfilesel.h>
#include <fstream.h>
#include <v/vmodald.h>
#include <v/vdialog.h>

    enum { Cmd, Ins, Find, Record, SetReg };

    static int filterIndex = 0;
    static char* filter[] =
      {
        "*",
        "*.c;*.cpp;*.cxx;*.h",
        "*.txt;*.htm;*.html",
	"*.java",
        0
      };

//    char* vTextEdCmdInterp globalBuff[26] = {0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0};


// ==================>>> vSeeCI::vSeeCI <<<=================
  vSeeCI::vSeeCI(vTextEditor* textEd, vCmdWindow* cw)
        : vBaseTECmdInterp(textEd, cw)
  {

    lex_def = 1;
    param_count = 1;
    cmdmode = Cmd;
    countWait = 0;		// start in cmdmode
    slastl = savlen = nxtsav = 0;	// Save Buff stuff
    _SaveBuff  = new char*[SBAllocLines];	// build save in 500 line incrs.
    for (long lx = 0 ; lx < SBAllocLines ; ++lx)
        _SaveBuff[lx] = 0;

    _maxSBLines = SBAllocLines - 1;
  }

// ==================>>> vSeeCI::~vSeeCI <<<================
  vSeeCI::~vSeeCI()
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
  void vSeeCI::InitCmdInterp()
  {
    globalState es;
    es = te()->GetGlobalState();
    es.delOnInsert = 0;		// don't delete on insert
    es.findAtBeginning = 0;	// don't leave find at beginning
    te()->SetGlobalState(es);
  }

// =====================>>> vSeeCI::ProcessKey <<<====================
  int vSeeCI::ProcessKey(vKey key, unsigned int shift)
  {

    if (vk_IsModifier(key))             // ignore modifiers
	return -1;

    return edit(key,shift);
  }

// =============================>>> vSeeCI::edit   <<<==========================
  int vSeeCI::edit(vKey chr, unsigned int shift)
  {
    // Hard wired See command interp.

    int s_succ, retval;

    static int ins_set[] =		/* allowable commands for insert */
      {
	'i', 'o', 'X'-'@', 'T'-'@', 'g', 'Y'-'@', 'y', 0
      };

    static int jump_set[] =	/* commands to not reset jump memory */
      {
	'j', '?', 'n', 0
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
	    case vk_BackSpace:		/* delete last character */
		retval = te()->charDelete(-cmdCount);
		break;

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
		te()->ChangeInsMode(te()->GetEdState().ins_mode,"Command   ");
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
        if (chr == vk_Escape)
          {
            cmdmode = Cmd;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode,"Command   ");
	    retval = te()->Find(newFindPat,0,(cmdCount >= 0),0);
	    cmdCount = 1;
	    return retval;
          }
        else 
	    echoInput(chr, newFindPat, MAX_LINE);
        return 1;
      }
    else if (cmdmode == Record)		// in Record mode
      {
        if (chr == 'Q'-'@')
          {
            cmdmode = Cmd;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode,"Command   ");
	    // copy newQReg into static QReg array
	    return setQReg(curReg);	// set the register
          }
        else 
	    echoQInput(chr,shift);
        return 1;
      }
    else if (cmdmode == SetReg)		// Set Register name
      {
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

// ----- command mode now

    te()->ChangeInsMode(te()->GetEdState().ins_mode,"Command   ");

    if (lexval >= '0' && lexval <= '9' || lexval == '-')
      {
	if (countWait == 0)
	  {
	    countWait = 1;
	    if (lexval == '-')
	      {
		cmdCount = 1;	// - implies 1
		countWait = -1;
	      }
	    else
		cmdCount = lexval - '0';
	    return 1;
	  }
	else if (lexval == '-')
	  {
	    countWait = -1;
	    return 1;
	  }
	else if (lexval >= '0' && lexval <= '9')      // it is a count
	  {
	    if (countWait < 0 && cmdCount == 1)
		cmdCount = 0;			// user entering value
	    cmdCount = (cmdCount * 10) + (lexval - '0');
	    return 1;
	  }
      }

    if (countWait != 0)
      {
	cmdCount *= countWait;        // negative?
	countWait = 0;                      // done building count
      }

    s_succ = 1;

    if (te()->GetCurLine() < 1)	// make sure legal command for empty buffer
      {
	if (!inset(lexval,ins_set))
	  {
	    te()->StatusMessage("Can't, buffer empty. Insert 1st ");
	    return 1;
	  }
      }

    if (!inset(lexval,jump_set))
	te()->SetJLine(te()->GetCurLine()); // sets previous line for Jump

    switch (lexval)
      {
        case 'C'-'@':           // ^C : Copy ; Shift-^C: fold case
          {
            te()->EditCopy();
            break;
          }

	case 'D'-'@': 			/* down in column */
	case vk_KP_Down:
	case vk_Down:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else
		retval = te()->lineDown(cmdCount);
	    break;
	  }


	case 'F'-'@':           // ^F: find/replace
	  {
	    retval = te()->EditCommand(edReplace, cmdCount);
	    break;
	  }

	case vk_BackSpace:		/* delete last character */
	    // don't delete highlight
	    retval = te()->charDelete(-cmdCount,true,false);
	    break;

	case vk_Tab:
	  {
	    retval = te()->wordRight(cmdCount);
	    break;
	  }

	case 'K'-'@':           // ^K: Kill line
	  {
	    te()->lineDelete(cmdCount);
	    break;
	  }

	case 'L'-'@':			/* goto line */
	  {
	    retval = te()->lineGoto(cmdCount);
	    break;
	  }

	case 'N'-'@':			/* return to noted location */
	  {
	    retval = te()->EditCommand(edNoteGoto,cmdCount);
	    break;
	  }

	case 'P'-'@':			/* page up */
	  {
	    retval = te()->lineDown((long)(-cmdCount * te()->GetRows()));
	    break;
	  }

	case vk_Page_Up:
	case vk_KP_Page_Up:
	  {
	    if (shift & VKM_Ctrl)
		te()->scrollDown(-cmdCount * te()->GetRows());
	    else
		retval = te()->lineDown((long)(-cmdCount * te()->GetRows()));
	    break;
	  }

        case 'Q'-'@':			// ^Q - save/close this file
          {
            cmdWin()->WindowCommand(M_SaveClose,1,(CmdType)0);
            break;
          }

	case 'U'-'@': 			/* up in column */
	case vk_Up:
	case vk_KP_Up:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else
		retval = te()->lineDown(-cmdCount);
	    break;
	  }

        case 'V'-'@':           // ^V: Paste
          {
            te()->EditPaste();
            break;
          }

        case 'X'-'@':           // ^X : Cut
          {
            te()->EditCut();
            break;
          }

	case 'G'-'@':		/* unkill last line killed */
        case 'Z'-'@':           // ^Z : Undo
          {
            te()->undoEdit();
            break;
          }

	case '"':			/* kill rest of line */
	  {
	    retval = te()->lineDeleteToEnd();
	    break;
	  }

	case '#':			/* ex. rpt k n times */
//@	    ED(target) = T_EXEC_RPT;	/* execute the repeat loop */
//@@	    param_count = cmdCount;	/* remember the count */
	    break;

	case '&':			/* execute repeat buffer again */
//&&	    if (cmdCount != 1)
//&&		ED(echof)=FALSE;	/* turn off echo */
//&&	    rptcnt[rptuse] = cmdCount > 0 ? cmdCount : (-cmdCount);
	    break;

	case '\'':			/* kill previous part of line */
	  {
	    retval = te()->lineDeleteFront();
	    break;
	  }

	case '*':			/* insert the find pattern */
//@@	    succ = ins_pat();
	    break;

	case '+':			/* insert counter value */
//@@	    ins_counter();
	    break;

	case ',':			/* move to beginning of line */
	  {
	    if (cmdCount > 1)
		retval = te()->lineDown(cmdCount-1);
	    else if (cmdCount < 0)
		retval = te()->lineDown(cmdCount);
	    te()->lineBeginning();
	    break;
	  }

	case '.':			/* move to end of the line */
	  {
	    if (cmdCount > 1)
		retval = te()->lineDown(cmdCount-1);
	    else if (cmdCount < 0)
		retval = te()->lineDown(cmdCount);
	    te()->lineEnd();
	    break;
	  }

	case '/':			/* delete last thing manipulated */
	    retval = RemoveLast();
	    break;

	case ';':			/* find again */
	  {
	    retval= te()->FindNext(0,(cmdCount >= 0),0);
	    break;
	  }

	case '=':			/* remove last and enter insert mode */
	  {
	    if ((retval = RemoveLast()))
	      {
		cmdmode = Ins;
		te()->ChangeInsMode(te()->GetEdState().ins_mode);
	      }
	    break;
	  }

	case 'E'-'@':			// execute q-reg
	case '@':			// execute q-reg macro
	  {
	    retval = execQReg(curReg,cmdCount);
	    break;
	  }

	case '[':			/* backwards jump over word */
	  {
	    retval = te()->wordRight(-cmdCount);
	    break;
	  }

	case ']':			/* balance match */
	  {
	    retval = te()->BalMatch(cmdCount);
	    break;
	  }

	case '^':			/* reverse find */
//@@	    ED(target) = T_FIND;
//@@	    (void) build_find(0,1,(int)(-cmdCount)); /* set up find build first */
	    break;

	case '_':			/* kill word */
//@@	    if (!(succ = wordr(cmdCount)))
//@@		break;
//@@	    retval = rmvlst();
	    break;

	case '?':
	    CmdInterpHelp();
	    break;

	case '\\':		// \ = set current (Q or buffer)
          {
            cmdmode = SetReg;
            return 1;
          }

	case 'a':			/* append to save buffer */
	    retval = save(cmdCount,1);
	    break;

	case 'b':			/* goto top of page */
	  {
	    retval = te()->lineGoto(1);
	    break;
	  }

	case vk_Home:
	case vk_KP_Home:
	  {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,chr,shift);
	    else if (shift & VKM_Ctrl)
		retval = te()->lineGoto(1);
	    else
	      {
		if (cmdCount > 1)
		    retval = te()->lineDown(cmdCount-1);
		else if (cmdCount < 0)
		    retval = te()->lineDown(cmdCount);
		te()->lineBeginning();
	      }
	    break;
	  }

	case 'c':			/* change characters */
	    if ((retval = te()->charDelete(cmdCount)) )
	      {
		cmdmode = Ins;
		te()->ChangeInsMode(te()->GetEdState().ins_mode);
	      }

	    break;

	case 'D':
	case 'd': 			/* down line */
	    retval = te()->lineDownBeg(cmdCount);
	    break;

	case 'e':			/* goto to bottom of page */
	    te()->bufferBottom();
	    break;

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
		    retval = te()->lineDown(cmdCount-1);
		else if (cmdCount < 0)
		    retval = te()->lineDown(cmdCount);
		te()->lineEnd();
	      }
	    break;
	  }

	case 'f':		// F: set find pattern only - unechoed
          {
            cmdmode = Find;
            te()->ChangeInsMode(te()->GetEdState().ins_mode,"Find Pat  ");
            newFindPat[0] = 0;	// no pattern yet
            return 1;
          }

	case 'F':		// f: find via dialog
	  {
	    // @@ add code to handle Finds from repeat buffer
            te()->SetFindPat("");	// new pattern each time!
	    retval = te()->EditCommand(edFind, cmdCount);
	    break;
	  }

	case 'g':			/* get move buffer */
	    retval = getsav();
	    break;

	case 'I':
	    if (cmdCount >= 1)
		retval = te()->charInsert(cmdCount);
	    break;

	case 'i': 			/* insert */
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;

	case 'j':			/* jump back to last location */
	  {
	    long itmp = te()->GetCurLine();
	    retval = te()->lineGoto(te()->JLine());
	    te()->SetJLine(itmp);
	    break;
	  }

	case 'k': 			/* delete next character */
	    // this will always delete the next char, and will
	    // ignore the mark range. Delete key will kill mark range
	    retval = te()->charDelete(cmdCount,true,false);
	    break;

	case vk_Delete:
	case vk_KP_Delete:
	  {
	    if (shift & VKM_Shift || shift & VKM_Ctrl)
		te()->lineDelete(cmdCount);
	    else
		retval = te()->charDelete(cmdCount);
	    break;
	  }

	case 'l': 			/* left */
	    retval = te()->charRight(-cmdCount,1);
	    break;

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

	case 'm':
	    te()->CenterScreen();
	    break;

	case 'n':			/* save current location */
            retval = te()->EditCommand(edNoteLocation, cmdCount);
	    break;

	case 'o':			/* open new line */
	    retval = te()->lineOpen(cmdCount);
	    cmdmode = Ins;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode);
	    break;

	case 'O'-'@':			/* overtype */
	case vk_KP_Insert:
	case vk_Insert:
	  {
	    te()->SetInsMode(!(te()->GetEdState().ins_mode));
	    break;
	  }

	case 'p':			/* page down screen */
	    retval = te()->lineDown(
		    (long) te()->minl((cmdCount * te()->GetRows()),
		    (long)(te()->GetLines() - te()->GetCurLine() + 1)));
	    break;

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

	case 'q': 			// record q macro #
	  {
	    param_count = cmdCount;	/* remember the count */
            cmdmode = Record;
            te()->ChangeInsMode(te()->GetEdState().ins_mode,"Recording ");
            newQReg[0] = 0;		// no q register yet
            oldlex = lexval;
            cmdCount = 1;
	    return 1;
	  }

	case 'r': 			/* right */
	    retval = te()->charRight(cmdCount,1);
	    break;

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

	case 's':			/* save lines in move buffer */
	    retval = save(cmdCount,0);
	    break;

	case 't':			/* tidy up screen */
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
		    retval = 0;
		    break;

		default:
		    retval = 0;
		    break;
	      }
	    break;

	case 'U':
	case 'u': 			/* up line */
	    retval = te()->lineDownBeg(-cmdCount);
	    break;

	case 'v':			/* verify */
	    te()->Verify();
	    break;


	case 'Y'-'@':			/* append external file to save buffer */
	case 'y':			/* write save buffer to file */
	    retval = yankfile(cmdCount);
	    break;

	case '~':			/* '~': change case */
	    retval = te()->charFoldCase(cmdCount);
	    break;

	default:
	    retval = 0;
	    break;
      }  			/* end of switch */

    cmdCount = 1;
    if (! retval)		// handle repeat loops
	return 0;
    else
	oldlex = lexval;

    return 1;
  }

// =====================>>> vTextEdCmdInterp::InitCmdInterp <<<====================
  void vSeeCI::CmdInterpHelp()
  {
    static char* helplist [] =
      {
	"See Editor Emulation",
	"Key  --  Command Description",
	"  ",
	"  ***  Movement Commands  ***",
	"nl  --  Move left [Left Arrow]",
	"nr  --  Move right [Right Arrow]",
	"nu  --  Move up to beg of line [Shft-Up]",
	"nd  --  Move down to beg of line",
	"n^U  --  Move up [Up Arrow]",
	"n^D  --  Move down [Down Arrow]",
	"n[  --  Move left a word",
	"nTab  --  Move right a word [Ctrl-Right]",
	"n^P  --  Move a screenful up [PgUp]",
	"np  --  Move a screenful down [PgDn]",
	"n,  --  Goto beg of n'th line [Home]",
	"n.  --  Goto end of n'th line [End]",
	" b  --  Goto beg of file [Ctrl-Home]",
	" e  --  Goto end of file [Ctrl-End]",
	" j  --  Jump back to prev location",
	"n^L  --  Goto line n",
	" m  --  Center Cursor in screen",
	"nn  --  Note (mark) location n (1-25)",
	"n^N  --  Goto noted location n",
	"nCtrl-PgUp  --  Scroll a screenful up",
	"nCtrl-PgDn  --  Scroll a screenful down",
	" ",
	"  ***  Searching commands ***",
	"n]  --  Balance match",
	"nf  --  Find pattern (non-dialog) (- for up)",
	" F  --  Find pattern (dialog)",
	"n;  --  Find next (- for up)",
	"^F  --  find/replace (dialog)",
	" ",
	"  ***  Insertion Commands ***",
	"nI  --  Insert char with value of n",
	" i  --  Enter insert mode",
	"Esc  --  Exit from Insert Mode",
	"^O  --  Toggle insert/overtype [Ins]",
	"no  --  Open a new blank line",
	" ",
	"  ***  Kill/change commands ***",
	"^C  --  Copy highlight to clipboard",
	"^V  --  Paste from clipboard",
	"^X  --  Cut highlight to clipboard",
	"'  --  Delete to line begin",
	" \"  --  Delete to line end",
	"n^K  --  Kill line",
	"n~  --  Toggle case",
	" /  --  Kill 'last thing'",
	" =  --  Change 'last thing'",
	"nt  --  Tidy n lines (text or code)",
	"nBkspace  --  Delete previous char",
	"nk  --  Delete next char",
	"nDel  --  Delete highlight or next char",
	"^Z  --  Undo [^G]",
	" ",
	"  ***  Save buffer commands ***",
	"ns  --  Save n lines in save buffer",
	"na  --  Append n lines to save buffer",
	" g --  Get contents of save buffer",
	" y  --  Yank file to save buffer",
	"-y --  Write save buffer to file",
	" ",
	"  ***  Q-Register commands  ***",
	"\\<a-z>  --  set q-reg/buff for next op",
	"[\\<a-z>]q  --  record to q-register until ^Q",
	"[\\<a-z>]n@  --  execute q-register N times [^E]",
	" ",
	"  ***  Misc. commands  ***",
	" v  --  Repaint screen",
	" ?  --  Help",
	"^Q  --  save file and close window",
	 0
      };

    static CommandObject HelpD[] =
      {
        {C_Button,M_OK,0,"Close",NoList,CA_None,isSens,NoFrame,0,0},
	{C_Label,8000,0,"List of See Editor commands",NoList,CA_None,isSens,NoFrame,M_OK,0},
	{C_List,8001,0,"Help",(void*)helplist,CA_Size,
		isSens,	NoFrame,0,M_OK,32},
	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
        
      };

    // I'd have liked a modeless dialog, but then I'd have to track
    // and delete the object. This way it gets dynamically created
    // each  time.


    vModalDialog Help(te()->GetPaneParent(),"See Help");
    Help.AddDialogCmds(HelpD);
 
    ItemVal DUMMY;
    (void) Help.ShowModalDialog("See Help",DUMMY);

  }

// =========================>>> vSeeCI::MouseJump <<<============================
  void vSeeCI::MouseJump(int row, int col)
  {
    // Jumping elsewhere via a mouse click means we should go back to
    // command mode. Too many messy things happen if we don't
    cmdmode = Cmd;
    te()->ChangeInsMode(te()->GetEdState().ins_mode,"Command   ");
  }

// =============================>>> vSeeCI::inset <<<===========================
  int vSeeCI::inset(int val, int *set)
  {
     /* return true if val is in set set */

    while (*set)
	if (val == *set++)
	    return 1;
    return 0;
  }

// =============================>>> vSeeCI::RemoveLast <<<==========================
  int vSeeCI::RemoveLast(void)
  {  /* RemoveLast - delete the previous thing found or manipulated
      *	length of oldlen is set by insert, find, and save
      *	may also use savlen if set by save */

    static int rmv_set[] =
      {
	'f', 'F' - '@', '^', 's', 'g', ';', 'a', 'I'-'@', '[', '_', 0
      };

    if (te()->RemoveMarkRange())	/* was a mark range to remove */
	return 1;			/* all done */

    if (!inset(oldlex,rmv_set))
	return 0;

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

// =============================>>> vSeeCI::save   <<<=========================
  int vSeeCI::save(long cnt, int app)
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

    if ((oldlex != 's' && !app) || cnt == 0)
      { 			/* if new save, cnt == 0 and not appending */
        // Clear old save buffer - delete storage for lines
        for (long lx = 0 ; lx < nxtsav ; ++lx)  // copy old lines
	    if (_SaveBuff[lx])
	      {
                delete [] _SaveBuff[lx];	// free space
		_SaveBuff[lx] = 0;		// don't have it now!
	      }

        slastl = savlen = nxtsav = 0;	/* reset these guys */

	if (cnt == 0)
	  {
	    return 1;
	  }
      }

    if (oldlex != 'a' && app)	/* need to reset savlen for append */
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

//========================>>> vSeeCI::reallocSaveBuff <<<======================
  int vSeeCI::reallocSaveBuff()
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

//=============================>>> vSeeCI::getsav <<<==========================
  int vSeeCI::getsav()
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

// =============================>>> vSeeCI::yankfile   <<<=========================
  int vSeeCI::yankfile(long cnt)
  { 	// yank file to or from save bufer


    int old_ef = te()->GetEdState().echof;   	// remember state of echo flag

    char buff[MAX_LINE+2];
    vFileSelect fsel(cmdWin());

    if (cnt < 0)		// write save buffer
      {
	buff[0] = 0;
	int ians = fsel.FileSelectSave("Write save buffer to file",
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
	int ians = fsel.FileSelect("Yank file to save buffer",
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
                _SaveBuff[lx] = 0;	// free space
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

#ifdef XXYYXX
//=============================>>> vSeeCI::ins_pat  <<<=============================
#ifdef ANSI_C
  int ins_pat(void)
#else
  int ins_pat()
#endif
  {
    char *chrp;

    if (!*pat_buff)
	return 0;

    for (chrp = pat_buff ; *chrp ; )	/* simply insert pattern buffer */
      {
	if (!ins_chr((int)*chrp++))	/* make sure it works */
	    return 0;
      }

    return 1;
  }
#endif
