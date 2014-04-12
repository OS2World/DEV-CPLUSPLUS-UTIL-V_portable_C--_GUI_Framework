
//=======================================================================
// vtdialog.cxx - Source file for testDialog class
//  Copyright (C) 1995,1996, 1997, 1998  Bruce E. Wampler
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

#include "vtdialog.h"
#include <v/v_defs.h>
#include <v/vnotice.h>
#include <v/vcolor.h>
#include <v/vlabelc.h>
#include <v/vutil.h>

    static char change_me[] = "Change Me A (הו)";    // a label to change
#define ChangeMeID 444

    static vColor btncolor(255, 0 , 0);         // start at red

    static int prange[2] = {-20,200};

    CommandObject DefaultCmds[] =
      {

        {C_Frame, 10, 0, "", NoList, CA_None, isSens, NoFrame, 0, 0},
        {C_Label, 111, 0, "Frame - üיגה", NoList, CA_None, isSens, 10, 0, 0},
        {C_CheckBox, 112, 0, "Test A", NoList, CA_None, isSens, 10, 0, 111, 0,
		"Test A CheckBox"},
        {C_CheckBox, 113, 0, "Test B", NoList, CA_None, isSens, 10, 112, 111, 0,
		"Test B Checkbox"},
        {C_CheckBox, 114, 1, "Test C", NoList, CA_None, isSens, 10, 0, 112, 0,
		"Test C Checkbox"},

        {C_Frame,115, 0, "", NoList,CA_NoBorder,isSens, 10, 113, 0},
        {C_Label, 116, 0 , "Column A",NoList,CA_None,isSens,115,0,0},
        {C_Button, 117, 0, "Uneven",NoList, CA_None, isSens, 115, 0,116, 0,
		"Top Button"},
        {C_Button, 118, 0, "Vertical",NoList, CA_None, isSens, 115, 0,117, 0,
		"Middle Button"},
        {C_Button, 119, 0, "Buttons",NoList, CA_None, isSens, 115, 0,118, 0,
		"Bottom Button"},

        {C_Frame,120, 0, "", NoList,CA_NoBorder,isSens, 10, 115, 0},
        {C_Label, 121, 0 , "Column B",NoList,CA_None,isSens,120,0,0},
        {C_Button, 122, 0, "Another",NoList, CA_None, isSens, 120, 0,121,0,
		"Another button"},
        {C_Button, 123, 0, "Vertical",NoList, CA_None, isSens, 120, 0,122,0,
		"To test"},
        {C_Button, 124, 0, "Set",NoList, CA_None, isSens, 120, 0,123, 0,
		"Test code is boring"},

        {C_Frame, 11, 0, "", NoList,CA_None,isSens,NoFrame, 10, 0},
        {C_Label, 20, 0, "VFrame", NoList,CA_None, isSens,11, 0, 0},
        {C_RadioButton, 21, 1, "KOB",  NoList,CA_None,isSens, 11, 0, 20, 0,
		"TV station KOB"},
        {C_RadioButton, 22, 0, "KOAT", NoList, CA_None, isSens, 11, 0, 21, 0,
		"TV station KOAT"},
        {C_RadioButton, 23, 0, "KRQE", NoList, CA_None, isSens, 11, 0, 22, 0,
		"TV station KRQE"},

        {C_Frame, 12, 0, "", NoList,CA_NoSpace,isSens,NoFrame, 11, 0,0,
		"Does Frame Help Work?"},
        {C_Label, 40, 0 , "NoSpace",NoList,CA_None,isSens,12,0,0},
        {C_Button, 41, 41, "Button 1", NoList, CA_None, isSens, 12, 0, 40, 0,
		"Another Button"},
        {C_Button, 42, 42, "Button 2", NoList, CA_None, isSens, 12, 0, 41},
        {C_Button, 43, 43, "Button 3", NoList, CA_None, isSens, 12, 0, 42},
        {C_Button, 44, 44, "Button 4", NoList, CA_None, isSens, 12, 41, 40},
        {C_Button, 45, 45, "üצכ etc.", NoList, CA_None, isSens, 12, 41, 44, 0,
		"Test extended characters"},

        {C_Frame, 13, 0, "",   NoList,CA_None,isSens,NoFrame, 0,10},
        {C_Label, 30, 0 , "HFrame",NoList,CA_None,isSens,13,0,0},
        {C_CheckBox, 31, 1, "H Check 1", NoList, CA_None, isSens, 13, 30, 0},
        {C_CheckBox, 32, 0, "H Check 2", NoList, CA_None, isSens, 13, 31, 0},

        {C_CheckBox, 3, 1, "Check 1",    NoList,CA_None,isSens,NoFrame, 0, 13},
        {C_CheckBox, 301, 0, "Check 2 ", NoList,CA_None,isSens,NoFrame, 3, 13},

        {C_Text, 4, 0, "", (void *)"This is an example\nof a two line text. (üה)",
                CA_None,isSens,NoFrame, 301, 13},

        {C_Frame, 81, 0, "",   NoList,CA_None,isSens,NoFrame, 4,10},
        {C_ProgressBar, 76, 50, "", (void*)&prange[0],
            CA_Vertical | CA_NoLabel | CA_Small, isSens,81, 0, 0},

        {C_Frame, 86, 0, "",   NoList,CA_None,isSens,81, 76, 0},
        {C_Label, 87, 0, "Slider", NoList,CA_None,isSens,86,0,0},
        {C_Slider, 88, 50, "", (void*)&prange[0],CA_Horizontal ,isSens,86, 0, 87},
        {C_Text, 89, 50, "", (void*)" 50",CA_NoBorder,isSens,86, 88, 87},

   
        {C_Frame, 70, 0, "",   NoList,CA_None,isSens,81, 76,86},
        {C_Label, 71, 0, "Progress Bar", NoList,CA_None,isSens,70,0,0},
        {C_ProgressBar, 72, 50, "", (void*)&prange[0],
             CA_Horizontal | CA_Percent ,isSens,70, 0, 71},

        {C_Slider, 85, 50, "", NoList,CA_Vertical ,isSens,0, 81, 10},

        {C_RadioButton, 60, 1, "ABC", NoList,CA_None,isSens,NoFrame, 0, 3},
        {C_RadioButton, 61, 0, "DEF", NoList,CA_None,isSens,NoFrame, 60, 3},
        {C_RadioButton, 62, 0, "GHI", NoList,CA_None,isSens,NoFrame, 61, 3},

        {C_ColorButton, 7, 7, "  ", (void*)&btncolor, CA_None, isSens, NoFrame,
           0, 60, 16},
        {C_ColorLabel, 8, 8, "Color", (void*)&btncolor, CA_None, isSens, NoFrame,
           7, 60},
        {C_Button, ChangeMeID, 0, change_me, NoList, CA_None, isSens, NoFrame, 8, 60},
        {C_Blank, 18,18,"   ",NoList,CA_None,notSens,NoFrame,ChangeMeID,60},

        {C_Button, 200, 200,"Hide",NoList,CA_None,isSens,NoFrame,0,8},
        {C_Button, 201, 201,"UnHide",NoList,CA_Hidden,isSens,NoFrame,0,8},

        {C_Blank, 400,400,"   ",NoList,CA_None,notSens,NoFrame,201,8},

        {C_Button, M_Cancel, M_Cancel," Cancel ",NoList,CA_None,isSens,NoFrame,400,8},
        {C_Button, M_OK, M_OK, " OK ", NoList, CA_DefaultButton,
                isSens, NoFrame, M_Cancel, 8},
        {C_Button,880, 0, " Fill Chars",NoList,CA_None,isSens,NoFrame,M_OK,8},
        {C_BoxedLabel, 888,0,"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
                NoList,CA_None,isSens,NoFrame,0,M_OK},
        {C_BoxedLabel, 889,0,"d#########d#########d#########d#########d########d",
                NoList,CA_None,isSens,NoFrame,0,888},

        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static char let = ' ';

//=========================>>> testDialog::testDialog <<<====================
  testDialog::testDialog(vBaseWindow* bw) :
    vDialog(bw,0,"V Test Dialog (הו)")
  {
    UserDebug(Constructor,"testDialog::testDialog()\n")

    _myBase = bw;

    AddDialogCmds(DefaultCmds);         // add the predefined commands
  }

//========================>>> testDialog::~testDialog <<<====================
  testDialog::~testDialog()
  {
    UserDebug(Destructor,"testDialog::~testDialog() destructor\n")
  }

//====================>>> testDialog::DialogCommand <<<=======================
  void testDialog::DialogCommand(ItemVal id, ItemVal retval, CmdType ctype)
  {
    // After the user has selected a command from the dialog,
    // this routine is called with the value.  This code would be generated
    // by Vigr.


    UserDebug2(CmdEvents,"testDialog::DialogCommand(id:%d, val:%d)\n",id, retval)

    switch (id)         // We will do some things depending on value
      {
        case 21:	// kob
        case 22:	// koat
        case 23:	// krqe
        case 112:	// test a
        case 113:	// test b
        case 114:	// test c
        case 60:	// abc
        case 61:	// def
        case 62:	// ghi
          {
   //	    vNoticeDialog note(_myBase);
   //         if (retval == 1)
    //        	note.Notice("radio button ON");
   //         else
   //             note.Notice("Radio Button OFF");
            break;
          }

        case 880:
          {
            char str[42];
            for (int ix = 0 ; ix < 20 ; ++ix)
                str[ix] = let;
            str[20] = 0;
            str[0] = 'c';
            str[19] = 'c';
            let++;
            SetString(888, str);
            break;
          }

        case ChangeMeID:                // Change my own label
          {
            // We will change the label on this button
            change_me[10]++;            // change the "A"
            SetString(ChangeMeID, change_me);
            SetDialogTitle(change_me);
	    SetValue(62,1,Value);	// force a dyamic change to rb for test
	    SetValue(112,1,Value);	// force a dyamic change to rb for test

	    SetString(4,"Not much to change\nAnd only once!");
            break;
          }

        case 7:
          {
            // Test remapping of color to see how fast we use up colors
            btncolor.ResetColor(btncolor.r()+127, btncolor.g()+63, btncolor.b()+31);
#define SET1
#ifdef SET1
            SetValue(7,0,ChangeColor);          // change all at once
            SetValue(8,0,ChangeColor);
#else
            SetValue(7,(ItemVal)btncolor.r(),Red);
            SetValue(7,(ItemVal)btncolor.g(),Green);
            SetValue(7,(ItemVal)btncolor.b(),Blue);
#endif
            break;
          }

        case 88:                // Slider
          {
            char buff[20];
            IntToStr(retval,buff);
            SetString(89,buff);
            SetValue(72,retval,Value);
            SetValue(76,retval,Value);
            break;
          }

        case 200:                       // Hide some things
          {
            SetValue(200,1,Hidden);     // Hide this one
            SetValue(201,0,Hidden);     // Unhide the UnHide

            // Hide stuff
            SetValue(10,1,Hidden);      // Hide the frame
            SetValue(3,1,Hidden);       // A checkbox
            SetValue(4,1,Hidden);       // The text box
            SetValue(76,1,Hidden);      // The progress bar
            SetValue(72,1,Hidden);      // The progress bar
            SetValue(8,1,Hidden);       // The color button
            SetValue(60,1,Hidden);      // A radio button
            SetValue(87,1,Hidden);      // Slider Label
            SetValue(88,1,Hidden);      // Slider
            break;
          }

        case 201:
          {
            SetValue(201,1,Hidden);     // Hide the UnHide
            SetValue(200,0,Hidden);     // Unhide the Hide

            // Unhide stuff
            SetValue(10,0,Hidden);      // Hide the frame
            SetValue(3,0,Hidden);       // A checkbox
            SetValue(4,0,Hidden);       // The text box
            SetValue(76,0,Hidden);      // The progress bar
            SetValue(72,0,Hidden);      // The progress bar
            SetValue(8,0,Hidden);       // The color button
            SetValue(60,0,Hidden);      // A radio button
            SetValue(87,0,Hidden);      // Slider Label
            SetValue(88,0,Hidden);      // Slider
            break;
          }

      }
    // All commands should also route through the parent handler
    vDialog::DialogCommand(id,retval,ctype);
  }
