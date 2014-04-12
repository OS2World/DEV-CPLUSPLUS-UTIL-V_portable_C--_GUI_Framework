//=======================================================================
//	vstdci.cpp:	Source for vstandardci class
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

#include "vstdci.h"
#include <v/vcmdwin.h>
#include <v/vfilesel.h>
#include <fstream.h>
#include <v/vmodald.h>
#include <v/vdialog.h>

// ==================>>> vStandardCI::vStandardCI <<<=================
  vStandardCI::vStandardCI(vTextEditor* textEd, vCmdWindow* cw)
        : vBaseTECmdInterp(textEd, cw)
  {
    // following done in the base class
    // metaWait = 0;
    // countWait = 0;			// not waiting on meta cmd
    // metaChar = 'A'-'@';		// ^A is default meta char
    // cmdCount = 1;
    findWait = 0;
    recording = 0;			// not recording
    settingQ = 0;			// not setting Q
  }

// ==================>>> vStandardCI::~vStandardCI <<<================
  vStandardCI::~vStandardCI()
  {
  }

// =====================>>> vStandardCI::InitCmdInterp <<<====================
  void vStandardCI::InitCmdInterp()
  {
  }


// ----------------------------- Default CI --------------------------------

// ==================>>> vStandardCI::CmdInterpHelp <<<====================
  void vStandardCI::CmdInterpHelp()
  {
    static char* helplist [] =
      {
	"- Standard V Editor -",
	"Key  --  Command Description [synonymn]",
	"Esc  --  Prefix to enter count n",
	"  ",
	"  ***  Movement Commands  ***",
	"nLeft  --  Move left [^L]",
	"nCtl-Left  --  Move left a word",
	"nUp  --  Move up [^U]",
	"n^AUp  --  Move up to beg of line",
	"nRight  --  Move right [^R]",
	"nCtl-Right  --  Move right a word",
	"nDown  --  Move down [^D]",
	"n^ADown  --  Move down to beg of line",
	"Home  --  Goto beg of line [^A,]",
	"Ctrl-Home  --  Goto beg of file",
	"End  --  Goto end of line [^A.]",
	"Ctrl-End  --  Goto end of file",
	"nPgUp  --  Move a screenful up",
	"nCtrl-PgUp  --  Scroll a screenful up",
	"nPgDn  --  Move a screenful down",
	"nCtrl-PgDn  --  Scroll a screenful down",
	"n^G  --  Goto line n",
	"n^A^N  --  Note (mark) location n (1-25)",
	"n^An  --  Goto noted location n",
	"m^Am  --  Center Cursor in screen",
	"^Av  --  Repaint screen",
	" ",
	"  ***  Searching commands ***",
	"^[  --  Balance match",
	"^F  --  Find pattern (non-dialog)",
	"Shift-^F --  Find next [F3]",
	"^A^F  --  Find pattern (dialog)",
	"^Af  --  Find/Replace",
	" ",
	"  ***  Insertion Commands ***",
	"n^AIns  --  Insert char with value of n",
	"Ins  --  Toggle insert/overtype",
	"n^Ao  --  Open a new blank line",
	" ",
	"  ***  Kill/change commands ***",
	"nBkspace  --  Delete previous char",
	"^ABkspace  --  Delete to line begin [^A']",
	"nDel  --  Delete highlight or next char",
	"^ADel  --  Delete to line end [^A\"]",
	"nShft-Del  --  Delete line [^K]",
	"nShft-^C  --  Fold case",
	"n^B --  Beautify code/text",
	"^V  --  Paste from clipboard",
	"^C  --  Copy highlight to clipboard",
	"^X  --  Cut highlight to clipboard",
	"^Z  -- Undo",
	" ",
	"  ***  Q-Register commands  ***",
	"^Aq<a-z>  --  set q-reg/buff for next op",
	"[^Aq<a-z>]^Q  --  record to q-register until ^Q",
	"[^Aq<a-z>]n^E  --  execute q-register N times",
	" ",
	"  ***  File Commands ***",
	"^N -- Open new file",
	"^O -- Open file",
	"^S -- Save file",
	
	 0
      };

    static CommandObject HelpD[] =
      {
        {C_Button,M_Cancel,0,"Close",NoList,CA_None,isSens,NoFrame,0,0},
	{C_Label,8000,0,"List of Editor commands",NoList,CA_None,isSens,NoFrame,M_Cancel,0},
	{C_List,8001,0,"Help",(void*)helplist,CA_Size,
		isSens,	NoFrame,0,M_Cancel,32},
	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
        
      };

    // I'd have liked a modeless dialog, but then I'd have to track
    // and delete the object. This way it gets dynamically created
    // each  time.


    ItemVal temp;

    vModalDialog Help(te()->GetPaneParent(),"Editor Help");
    Help.AddDialogCmds(HelpD);
 
    (void)Help.ShowModalDialog("Generic Editor Help",temp);

  }

// =====================>>> vStandardCI::ProcessKey <<<====================
  int vStandardCI::ProcessKey(vKey key, unsigned int shift)
  {
// Commands supported by default keyboard mapping:
//
//      Most commands can be preceeded by a count, which is entered
//      with Esc followed by a value followed by the command. Negative
//      counts can be used for most commands as well. An n in front
//      of a command indicates it can use a count.
//
//      Some commands have synonyms compatible with See and
//      touch typing shown in []'s (e.g., ^A, for Home).
//
//      Commands that change text interact with highlighted text
//      by deleting it, but without copying it to the clipboard.
//      ^X and ^C interact with the clipboard.
//
//

    if (vk_IsModifier(key))             // ignore modifiers
        return -1;			// like alt

    int retval = 1;

    if (countWait != 0)
      {
        if (key == '-')
          {
            countWait = -1;
            return 1;
          }
        else if (key >= '0' && key <= '9')	// it is a count
          {
            cmdCount = (cmdCount * 10) + (key - '0');
            return 1;
          }
        else
          {
            cmdCount *= countWait;	// negative?
            countWait = 0;		// done building count
          }
      }

    if (metaWait)	// Meta command
      {
        metaWait = 0;
        switch (key)
          {
            case vk_KP_Insert:
            case vk_Insert:
              {
                retval = te()->charInsert(cmdCount);
                break;
              }

            case '\'':
            case vk_BackSpace:
              {
                retval = te()->lineDeleteFront();
                break;
              }

            case '"':
            case vk_Delete:
            case vk_KP_Delete:
              {
                retval = te()->lineDeleteToEnd();
                break;
              }

            case ',':                   // line beginning
              {
                te()->lineBeginning();
                break;
              }

            case '.':                   // line end
              {
                te()->lineEnd();
                break;
              }

	    case 'F'-'@':
	      {
		retval = te()->EditCommand(edFind, cmdCount);
		break;
	      }

	    case 'F':
	    case 'f':
	      {
		retval = te()->EditCommand(edReplace,cmdCount);
		break;
	      }

	    case 'M':
	    case 'm':
	      {
		te()->CenterScreen();
		break;
	      }


	    case 'N'-'@':           // ^N: note location
	      {
		retval = te()->EditCommand(edNoteLocation, cmdCount);
		break;
	      }

	    case 'N':		// go to noted location
	    case 'n':
	      {
		retval = te()->EditCommand(edNoteGoto,cmdCount);
		break;
	      }
	
            case 'O'-'@':	// O: Open line
            case 'O':           // O: Open line
	      {
		retval = te()->lineOpen(cmdCount);
		break;
	      }

	    case 'Q':		// ^AQ - set Q reg
	    case 'Q'-'@':
	      {
		settingQ = 1;
		break;
	      }

            case 'v':
            case 'V':
            case 'V'-'@':
              {
                te()->Verify();
                break;
              }

	    case 'U'-'@':
	    case vk_Up:
	    case vk_KP_Up:
	      {
		retval = te()->lineDownBeg(-cmdCount);
		break;
	      }

	    case 'D'-'@':
	    case vk_KP_Down:
	    case vk_Down:
	      {
		retval = te()->lineDownBeg(cmdCount);
		break;
	      }

            default:
              {
                retval = -1;
                break;
              }
          }
        cmdCount = 1;
        return retval;
      }
    else if (findWait)
      {
        if (key == vk_Escape)
          {
            findWait = 0;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode,"Insert    ");
	    retval = te()->Find(newFindPat,0,(cmdCount >= 0),0);
	    cmdCount = 1;
	    return retval;
          }
        else 
	    echoInput(key, newFindPat, MAX_LINE);
        return 1;
      }
    else if (recording)
      {
       if (key == 'Q'-'@')
          {
	    recording = 0;
	    te()->ChangeInsMode(te()->GetEdState().ins_mode,"Insert   ");
	    // copy newQReg into static QReg array
	    return setQReg(curReg);	// set the register
          }
        else 
	    echoQInput(key,shift);
        return 1;
      }
    else if (settingQ)
      {
	settingQ = 0;		// done
	if (!(key >= 'a' && key <= 'z'))	// validate value
	  {
	    te()->ErrorMsg("Valid range is <a-z>!");
	    return 0;
	  }
	else
	    curReg = key;		// set value
	return 1;
      }
    else if (key == (vKey) metaChar)
      {
        metaWait = 1;
        return 1;
      }

    switch (key)		// Primary commands
      {
        case 'B'-'@':		// ^B: Beautify code
          {
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
          }

        case 'C'-'@':           // ^C : Copy ; Shift-^C: fold case
          {
            if (shift & VKM_Shift)      // Shift-^F
              {
                retval = te()->charFoldCase(cmdCount);
              }
            else
                te()->EditCopy();
            break;
          }

	case 'E'-'@':		// ^E - execute
	  {
	    retval = execQReg(curReg,cmdCount);
	    break;
	  }

	case vk_F3:
	  {
	    retval= te()->FindNext(0,(cmdCount >= 0),0);
	    break;
	  }

        case 'F'-'@':           // ^F: find, Shift-^F: find next
          {
	    if (shift & VKM_Shift)      // Shift-^F
	      {
		retval= te()->FindNext(0,(cmdCount >= 0),0);
	      }
	    else		// support keyboard find
	      {
		// retval = te()->EditCommand(edFind, cmdCount);
		findWait = 1;	// now waiting for find
		te()->ChangeInsMode(te()->GetEdState().ins_mode,"Find Pat  ");
		newFindPat[0] = 0;	// no pattern yet
		return 1;
	      }
            break;
          }

        case 'G'-'@':           // ^G: Goto line
          {
            retval = te()->lineGoto(cmdCount);
            break;
          }

        case 'K'-'@':           // ^K: Kill line
          {
            te()->lineDelete(cmdCount);
            break;
          }


        case 'N'-'@':           // ^N - New
	  {
	    vWindow* wp = te()->GetPaneParent();
	    wp->WindowCommand(M_New,1,C_Button);
	    break;
	  }

        case 'O'-'@':           // ^O- Open file
	  {
	    vWindow* wp = te()->GetPaneParent();
	    wp->WindowCommand(M_Open,1,C_Button);
	    break;
	  }

	case 'Q'-'@':		// ^Q - record
	  {
	    recording = 1;	// recording now
	    te()->ChangeInsMode(te()->GetEdState().ins_mode,"Recording ");
	    newQReg[0] = 0;		// no q register yet
            cmdCount = 1;
	    return 1;
	  }

        case 'S'-'@':           // ^S - Save
	  {
	    vWindow* wp = te()->GetPaneParent();
	    wp->WindowCommand(M_Save,1,C_Button);
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

        case 'Z'-'@':           // ^Z : Undo
          {
            te()->undoEdit();
            break;
          }

        case 'L'-'@':
        case vk_Left:
        case vk_KP_Left:
          {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,key,shift);
            else if (shift & VKM_Ctrl)
                retval = te()->wordRight(-cmdCount);
            else
                retval = te()->charRight(-cmdCount,1);
            break;
          }

        case 'U'-'@':
        case vk_Up:
        case vk_KP_Up:
          {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,key,shift);
	    else
		retval = te()->lineDown(-cmdCount);
	    break;
          }

        case 'R'-'@':
        case vk_Right:
        case vk_KP_Right:
          {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,key,shift);
	    else if (shift & VKM_Ctrl)
                retval = te()->wordRight(cmdCount);
            else
                retval = te()->charRight(cmdCount,1);
            break;
          }

        case 'D'-'@':
        case vk_KP_Down:
        case vk_Down:
          {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,key,shift);
	    else 
		retval = te()->lineDown(cmdCount);
            break;
          }

	case ']'-'@':                   // ^] - bal match
	  {
	    retval = te()->BalMatch(cmdCount);
	    break;
	  }

        case vk_BackSpace:
          {
            retval = te()->charDelete(-cmdCount);
            break;
          }

        case vk_Tab:
          {
	    retval = te()->defaultKeyIn('\t',shift);
            break;
          }

        case vk_Linefeed:
          {
            break;
          }

        case vk_Return:
        case vk_KP_Enter:
          {
            retval = te()->defaultKeyIn('\n',shift);
            break;
          }

        case vk_Escape:
          {
            countWait = 1;
            cmdCount = 0;
            return 1;
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

        case vk_Home:
        case vk_KP_Home:
          {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,key,shift);
            else if (shift & VKM_Ctrl)
                retval = te()->lineGoto(1);
            else
                te()->lineBeginning();
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

        case vk_End:
        case vk_KP_End:
          {
	    if (shift & VKM_Shift)
		retval = doHighlight(cmdCount,key,shift);
            else if (shift & VKM_Ctrl)
                te()->bufferBottom();
            else
                te()->lineEnd();
            break;
          }

        case vk_KP_Insert:
        case vk_Insert:
          {
            te()->SetInsMode(!(te()->GetEdState().ins_mode));
            te()->ChangeInsMode(te()->GetEdState().ins_mode);
            break;
          }

        default:
          {
            retval = te()->defaultKeyIn(key,shift);
            break;
          }
      }
    cmdCount = 1;
    return retval;
  }

//=======================================================================
