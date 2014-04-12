//=======================================================================
//  vdbcnv.cpp:	Source for vdbTextEditor class
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


#include "vdbcnv.h"

#include "videapp.h"
#include "vdbcmdw.h"


//===================>>> vdbTextEditor::vdbTextEditor <<<====================
  vdbTextEditor::vdbTextEditor(vdbCmdWindow* parent) : 
      vTextEditor((vCmdWindow*)parent)
  {
  }

//===================>>> vdbTextEditor::~vdbTextEditor <<<====================
  vdbTextEditor::~vdbTextEditor()
  {
  }

//===================>>> vdbTextEditor::ChangeLoc <<<====================
  void vdbTextEditor::ChangeLoc(long line, int col)
  {
    ((vdbCmdWindow*)_parent)->ChangeLoc(line,col);
  }

//===================>>> vdbTextEditor::StatusMessage <<<====================
  void vdbTextEditor::StatusMessage(char *msg)
  {
    ((vdbCmdWindow*)_parent)->StatusMessage(msg);
  }

//===================>>> vdbTextEditor::ErrorMsg <<<====================
  void vdbTextEditor::ErrorMsg(char *str)
  {
  }

// #########################################################################
//
// DEFAULT vdbCI
//
// #########################################################################

// ==================>>> vdbCI::vdbCI <<<=================
  vdbCI::vdbCI(vTextEditor* textEd, vCmdWindow* cw)
  {
      _textEd = textEd;
      _myCmdWindow = cw;
      metaWait = 0;
      countWait = 0;                // not waiting on meta cmd
      metaChar = 'A'-'@';           // ^A is default meta char
      cmdCount = 1;

  }

// ==================>>> vdbCI::~vdbCI <<<================
  vdbCI::~vdbCI()
  {
  }

// =====================>>> vdbCI::InitCmdInterp <<<====================
  void vdbCI::InitCmdInterp()
  {
  }

// =====================>>> vdbCI::InitCmdInterp <<<====================
  void vdbCI::CmdInterpHelp()
  {
    static char* helplist [] =
      {
	"Key  --  Command Description",
	"  ",
	"Arrow keys  --  Standard function",
	
	 0
      };

    static CommandObject HelpD[] =

      {
        {C_Button,M_Cancel,0,"Done",NoList,CA_None,isSens,NoFrame,0,0},
	{C_Label,8000,0,"List of Editor commands",NoList,CA_None,isSens,NoFrame,M_Cancel,0},
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

// =====================>>> vdbCI::ProcessKey <<<====================
  int vdbCI::ProcessKey(vKey key, unsigned int shift)
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
//      ^Av             Repaint screen
//      ^C              Copy highlighted text to clipboard
//      ^F              Find pattern
//      Shift-^F        Find next
//      n^G             Goto line specified by count
//      nLeft           Move left [^L]
//      nCtrl-Left      Move left a word
//      nUp             Move up [^U]
//      nShift-Up       Move up to beginning of line
//      nRight          Move right [^R]
//      nCtrl-Right     Move right a word
//      nDown           Move down [^D]
//      nShift-Down     Move down to beginning of line
//      Esc             Prefix for entering counts
//      nPgUp           Move a screenful up
//      nCtrl-PgUp      Scroll a screenful up
//      nPgDn           Move a screenful down
//      nCtrl-PgDn      Scroll a screenful down
//      Ins             Toggle insert/overtype

    if (vk_IsModifier(key))             // ignore modifiers
        return -1;

    int retval = 1;

    if (countWait != 0)
      {
        if (key == '-')
          {
            countWait = -1;
            return 1;
          }
        else if (key >= '0' && key <= '9')      // it is a count
          {
            cmdCount = (cmdCount * 10) + (key - '0');
            return 1;
          }
        else
          {
            cmdCount *= countWait;        // negative?
            countWait = 0;                      // done building count
          }
      }

    if (metaWait)	// Meta command
      {
        metaWait = 0;
        switch (key)
          {
	    case 'M':
	    case 'm':
	      {
		te()->CenterScreen();
		break;
	      }

            case 'v':
            case 'V':
            case 'V'-'@':
              {
                te()->Verify();
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
    else if (key == (vKey) metaChar)
      {
        metaWait = 1;
        return 1;
      }

    switch (key)
      {
        case 'C'-'@':           // ^C : Copy ; Shift-^C: fold case
          {
	    te()->EditCopy();
            break;
          }

        case 'F'-'@':           // ^F: find, Shift-^F: find next
          {
            if (shift & VKM_Shift)      // Shift-^F
              {
                retval = te()->EditCommand(edFindNext, cmdCount);
              }
            else
              {
                retval = te()->EditCommand(edFind, cmdCount);
              }
            break;
          }

        case 'G'-'@':           // ^G: Goto line
          {
            retval = te()->lineGoto(cmdCount);
            break;
          }

        case 'L'-'@':
        case vk_Left:
        case vk_KP_Left:
          {
            if (shift & VKM_Ctrl || shift & VKM_Shift)
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
                retval = te()->lineDownBeg(-cmdCount);
            else
                retval = te()->lineDown(-cmdCount);
            break;
          }

        case 'R'-'@':
        case vk_Right:
        case vk_KP_Right:
          {
            if (shift & VKM_Ctrl ||shift & VKM_Shift)
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
                retval = te()->lineDownBeg(cmdCount);
            else
                retval = te()->lineDown(cmdCount);
            break;
          }

        case vk_Return:
        case vk_KP_Enter:
          {
            return -1;
          }

        case vk_Escape:
          {
            countWait = 1;
            cmdCount = 0;
            return 1;
          }


        case vk_Home:
        case vk_KP_Home:
          {
            if (shift & VKM_Shift || shift & VKM_Ctrl)
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
            if (shift & VKM_Shift || shift & VKM_Ctrl)
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
	    return -1;		// don't process the key
          }
      }
    cmdCount = 1;
    return retval;
  }
