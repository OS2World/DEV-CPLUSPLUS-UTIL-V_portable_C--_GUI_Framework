//=======================================================================
// vttogdlg.cxx - Source file for toggleDialog class
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

#include "vttogdlg.h"
#include <v/v_defs.h>
#include <v/vapp.h>
#include <v/vnotice.h>
#include <v/vutil.h>

    static CommandObject DefaultCmds[] =
      {
	// A label, then 3 toggle buttons to select toggle frames
	{C_Label,490,0,"Tab Frame Demo",NoList,CA_None,isSens,NoFrame,0,0},

	// This borderless frame makes the toggle buttons tightly spaced
	{C_Frame,400,0,"",NoList,CA_NoSpace | CA_NoBorder,isSens,NoFrame,0,490},
	{C_ToggleButton,401,1,"Tab 1",NoList, CA_None, isSens, 400, 0, 0},
	{C_ToggleButton,402,0,"Tab 2",NoList, CA_None, isSens, 400, 401, 0},
	{C_ToggleButton,403,0,"Tab 3",NoList, CA_None, isSens, 400, 402, 0},

	// A Master frame to give uniform border to toggle frames
	{C_Frame,409,0, "", NoList,CA_None,isSens,400,0,401},

	// Toggle Frame 1 - default off
	{C_ToggleFrame, 410,1,"",NoList, CA_NoBorder,isSens,409,0,0},
	{C_Button, 411, 0, "Button A   ",NoList, CA_None, isSens, 410,0,0},
	{C_Button, 412, 0, "Button B(1)",NoList, CA_None, isSens, 410,0,411},

	// Toggle Frame 2 - default on
	{C_ToggleFrame, 420,0,"",NoList, CA_NoBorder | CA_Hidden,isSens,409,0,0},
	{C_Button, 421, 0, "Button A(2)",NoList, CA_Hidden, isSens, 420,0,0},
	{C_Button, 422, 0, "Button B(2)",NoList, CA_Hidden, isSens, 420,421,0},

	// Toggle Frame 3 - default off
	{C_ToggleFrame, 430,0,"",NoList, CA_NoBorder | CA_Hidden,isSens,409,0,0},
	{C_Button, 431, 0, "Button A(3)",NoList, CA_Hidden, isSens, 430,0,0},
	{C_Button, 432, 0, "Button B(3)",NoList, CA_Hidden, isSens, 430,431,431},

        {C_Button, 500, 500," ModalModal Test ",NoList,CA_None,isSens,NoFrame,0,400},
        {C_Button, M_Cancel, M_Cancel," Cancel ",NoList,CA_None,isSens,NoFrame,0,500},
        {C_Button, M_OK, M_OK, " OK ", NoList, CA_DefaultButton,
                isSens, NoFrame, M_Cancel, 500},

        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };


//-------------------- list dialog
    char* comboList[] =
      {
        "Bruce",
        "Katrina",
        "Risa",
        "Van",
        "Chaco and Sushi",
        0
      };

    char* testList[] =
      {
        "First 0", "Item 1", "Item 2", "Item 3", "Item 4", "Item 5",
        "Item 6",  "Item 7", "Item 8", "Item 9", "Item 10", "Item 11",
        "Item 12", "Item 13", "Item 14", "Item 15", "Item 16",
        "Item 17", "Item 18", "Item 19", "Item 20", "Item 21",
        "Item 22", "Item 23", "Item 24", "Item 25", "Item 26",
        "Item 27", "Item 28", "Item 29", "Item 30", "Item 31",
        "Item 32", "Item 33", "Item 34", "Item 35", "Item 36",
        "Item 37", "Item 38", "Item 39", "Item 40", "Item 41 (last)",
        0
      };

    char* testList2[] =
      {
        "First A",
        "Item B",
        "Item C",
        "Item D",
        "Item E",
        "Item F",
        "Item G",
        0
      };

    char* wideList[] =
      {
	"This Tests",
	"CA_ListWidth",
	"A", "B", "C", "D", "E", "F", "G", 0
      };

    static char* valList[] = {"Value A", "Value B", "A Long Value", "Last Value", 0};

    static int minMaxStep[3] = {-10, 110, 2};

    CommandObject ListList[] =
      {
        {C_Label, 210, 210, "List Test",
                 NoList, CA_MainMsg, isSens, 0,0},

//V:1.14	{C_TextEdit,222,0,"Initial data.",NoList,
//V:1.14                 CA_None,isSens,NoFrame,0,210},

        {C_List, 200, 0, "A List", (void*)testList,
                 CA_Size,isSens,NoFrame,0,210,32},
        {C_ComboBox,201,2,"ComboBox",(void*)comboList,
                 CA_Simple,isSens,NoFrame,200,210},

        {C_Spinner, 50,0,"Spinner", (void*)valList,CA_Text,isSens,NoFrame,
	    200,201},

        {C_Spinner, 51,32, "Spinner", NoList,CA_None,isSens,NoFrame, 200,50},
        {C_Spinner, 52,-2, "Spinner", (void*)minMaxStep,CA_None,isSens,NoFrame, 
		51,50},

        {C_List, 220, 80, "Wide List", (void*)wideList,
                 CA_Size|CA_ListWidth,isSens,NoFrame,200,52,4,"List Test"},

	{C_TextIn, 230,0,"TextIn",NoList,
		CA_TextInNotify,isSens,NoFrame,200,220},

        {C_Button, M_OK, M_OK, " OK ", NoList,
                CA_DefaultButton, isSens, NoFrame, 0, 200},
	{C_Button, 53,0,"Change Combo",NoList,CA_None,isSens,NoFrame,M_OK,200},
	{C_Button, 54,0,"Change List",NoList,CA_None,isSens,NoFrame,53,200},
        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

static int whichList = 0;

//=========================>>> toggleDialog::toggleDialog <<<====================
  toggleDialog::toggleDialog(vBaseWindow* bw) :
    vModalDialog(bw,"V Toggle Dialog")
  {
    UserDebug(Constructor,"toggleDialog::toggleDialog()\n")

    AddDialogCmds(DefaultCmds);         // add the predefined commands
  }

//========================>>> toggleDialog::~toggleDialog <<<====================
  toggleDialog::~toggleDialog()
  {
    UserDebug(Destructor,"toggleDialog::~toggleDialog() destructor\n")
  }

//========================>>> toggleDialog::ShowModalDialog <<<====================
 ItemVal toggleDialog::ShowModalDialog(VCONST char* msg, ItemVal& rv)
 {
  
   return vModalDialog::ShowModalDialog(msg,rv);
 }

//====================>>> toggleDialog::DialogCommand <<<=======================
  void toggleDialog::DialogDisplayed()
  {
    SetString(411,"Button A(1)");
  }

//====================>>> toggleDialog::DialogCommand <<<=======================
  void toggleDialog::DialogCommand(ItemVal id, ItemVal retval, CmdType ctype)
  {
    // After the user has selected a command from the dialog,
    // this routine is called with the value.  This code would be generated
    // by Vigr.


    UserDebug2(CmdEvents,"toggleDialog::DialogCommand(id:%d, val:%d)\n",id, retval)

    switch (id)         // We will do some things depending on value
      {
	case 4:
	  {
	    SetString(2,"This is new");
	    break;
	  }

	case 401:	// Always assume toggle to ON
	  {
	    SetValue(id,1,Value);	// turn on toggle button
	    SetValue(402,0,Value);
	    SetValue(403,0,Value);	// others off

	    SetValue(420,0,Value);	// Toggle others off
	    SetValue(430,0,Value);	// Toggle others off
	    SetValue(410,1,Value);	// and ours on
	    break;
	  }

	case 402:	// Toggle 2
	  {
	    SetValue(id,1,Value);	// turn on toggle button
	    SetValue(401,0,Value);
	    SetValue(403,0,Value);	// others off

	    SetValue(410,0,Value);	// Toggle others off
	    SetValue(430,0,Value);	// Toggle others off
	    SetValue(420,1,Value);	// and ours on
	    break;
	  }

	case 403:	// toggle 3
	  {
	    SetValue(id,1,Value);	// turn on toggle button
	    SetValue(402,0,Value);
	    SetValue(401,0,Value);	// others off

	    SetValue(420,0,Value);	// Toggle others off
	    SetValue(410,0,Value);	// Toggle others off
	    SetValue(430,1,Value);	// and ours on
	    break;
	  }

        case 411: case 412: case 421: case 422: case 431: case 432:
          {
            vNoticeDialog note(this);
            note.Notice("Button pressed.");
            break;
          }

	case 500:	// modal modal test
	  {
	    vNoticeDialog note(this);
	    note.Notice("This is a modal from a modal");
	    break;
	  }

      }
    // All commands should also route through the parent handler
    vDialog::DialogCommand(id,retval,ctype);
  }

//=========================>>> listDialog::listDialog <<<====================
  listDialog::listDialog(vBaseWindow* bw) :
    vDialog(bw,0,"V List Dialog")
  {
    UserDebug(Constructor,"listDialog::listDialog()\n")

    AddDialogCmds(ListList);         // add the predefined commands
  }

//========================>>> listDialog::~listDialog <<<====================
  listDialog::~listDialog()
  {
    UserDebug(Destructor,"listDialog::~listDialog() destructor\n")
  }

//====================>>> listDialog::DialogCommand <<<=======================
  void listDialog::DialogCommand(ItemVal id, ItemVal retval, CmdType ctype)
  {
    // After the user has selected a command from the dialog,
    // this routine is called with the value.  This code would be generated
    // by Vigr.


    UserDebug2(CmdEvents,"listDialog::DialogCommand(id:%d, val:%d)\n",id, retval)

    switch (id)         // We will do some things depending on value
      {
	case 53:
	  {
	    if (*comboList[0] == 'b')
		comboList[0] = "Bruce";
	    else
		comboList[0] = "bruce";
	    SetValue(201,0,ChangeList);

	    break;
	  }

	case 54:
	  {
	    if (whichList == 0)
	      {
		whichList = 1;
		ListList[1].itemList = (void*)testList2;
	      }
	    else
	      {
		whichList = 0;
		ListList[1].itemList = (void*)testList;
	      }
	    SetValue(200,3,ChangeListPtr);	// change the whole list!
	    break;
	  }

	case 230:			// test change notify
	    vBeep();
	    break;
      }
    // All commands should also route through the parent handler
    vDialog::DialogCommand(id,retval,ctype);
  }
