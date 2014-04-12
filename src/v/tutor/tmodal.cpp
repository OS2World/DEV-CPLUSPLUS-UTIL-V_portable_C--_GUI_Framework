//========================================================================
//  tmodal.cpp - Source file for tutorial tModalDialog class
//
//  Copyright 1995,1996 Bruce E. Wampler, All Rights Reserved
//========================================================================
//

#include "tmodal.h"             // our header file
#include <v/vnotice.h>

const ItemVal mmLbl1 = 300;
const ItemVal mmBtn1 = 301;
const ItemVal mmBtn2 = 302;

    static CommandObject DefaultCmds[] =
      {
        {C_Label, mmLbl1, 0,"X",NoList,CA_MainMsg,isSens,NoFrame, 0, 0},
        
        {C_Button,mmBtn1,mmBtn1," Test 1 ",NoList,CA_None,
                isSens,NoFrame,0,mmLbl1},
        {C_Button,mmBtn2,mmBtn2," Test 2 ",NoList,CA_None,
                isSens,NoFrame, mmBtn1,mmLbl1},

        {C_Button,M_Cancel,M_Cancel," Cancel ",NoList,CA_None,
                isSens,NoFrame, 0,mmBtn1},
        {C_Button,M_OK,M_OK,"   OK   ",NoList,CA_DefaultButton,
                isSens,NoFrame,M_Cancel,mmBtn1},

        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };


//======================>>> tModalDialog::tModalDialog <<<================
  tModalDialog::tModalDialog(vBaseWindow* bw) :
    vModalDialog(bw)
  {
    UserDebug(Constructor,"tModalDialog::tModalDialog()\n")
    AddDialogCmds(DefaultCmds);         // add the predefined commands
  }

//=================>>> tModalDialog::~tModalDialog <<<====================
  tModalDialog::~tModalDialog()
  {
    UserDebug(Destructor,"tModalDialog::~tModalDialog() destructor\n")
  }

//===================>>> tModalDialog::DialogCommand <<<==================
  void tModalDialog::DialogCommand(ItemVal id,ItemVal retval,CmdType ctype)
  {
    // After the user has selected a command from the dialog,
    // this routine is called with the id and retval.

    vNoticeDialog note(this);

    UserDebug1(CmdEvents,"tModalDialog::DialogCommand(id:%d)\n",id)

    switch (id)         // We will do some things depending on value
      {
        case mmBtn1:            // Button 1
            note.Notice(" Test 1 ");
            break;

        case mmBtn2:            // Button 2
            note.Notice(" Test 2 ");
            break;
      }

    // let default behavior handle Cancel and OK
    vModalDialog::DialogCommand(id,retval,ctype);
  }
