//========================================================================
//  tdialog.cpp - Source file for tutorial tDialog class
//
//  Copyright 1995,1996 Bruce E. Wampler, All Rights Reserved
//========================================================================

// #include the headers we need
#include <v/vnotice.h>
#include "tdialog.h"

// The structure of a derived dialog class is very similar to the
// structure of a command window class. First we define IDs for the
// various command objects used in the dialog. Then we declare the
// static array that defines the dialog.

const ItemVal mdLbl1 = 200;

const ItemVal mdFrm1 = 201;  const ItemVal mdLbl2 = 202;
const ItemVal mdCB1 = 203;   const ItemVal mdCB2 = 204;
const ItemVal mdCB3 = 205;

const ItemVal mdFrmV1 = 206; const ItemVal mdLbl3 = 207;
const ItemVal mdRB1 = 208;   const ItemVal mdRB2 = 209;

const ItemVal mdFrmV2 = 210; const ItemVal mdLbl4 = 211;
const ItemVal mdBtn1 = 212;  const ItemVal mdBtn2 = 213;

const ItemVal mdBtnChange = 214;

    static char change_me[] = "Change Me A";    // a label to change

// This defines the dialog

    static DialogCmd DefaultCmds[] =
      {
        {C_Label, mdLbl1, 0,"X",NoList,CA_MainMsg,isSens,NoFrame, 0, 0},

        {C_Frame,mdFrmV2,0,"",NoList,CA_None,isSens,NoFrame,0,mdLbl1},
        {C_Label,mdLbl4,0,"Buttons",NoList,CA_None,isSens,mdFrmV2,0,0},
        {C_Button,mdBtn1,mdBtn1,"Button 1",NoList,CA_None,
                isSens,mdFrmV2,0,mdLbl4},
        {C_Button,mdBtn2,mdBtn2,"Button 2",NoList,CA_None,
                isSens,mdFrmV2,0,mdBtn1},

        {C_Frame,mdFrm1,0,"",NoList,CA_None,isSens,NoFrame,mdFrmV2,mdLbl1},
        {C_Label,mdLbl2,0,"CheckBox",NoList,CA_None,isSens,mdFrm1,0,0},
        {C_CheckBox,mdCB1,0,"Test A",NoList,CA_None,
                isSens,mdFrm1,0,mdLbl2},
        {C_CheckBox,mdCB2,0,"Test B",NoList,CA_None,
                isSens,mdFrm1,mdCB1,mdLbl2},
        {C_CheckBox,mdCB3,1,"Test C",NoList,CA_None,isSens,mdFrm1,0,mdCB1},

        {C_Frame,mdFrmV1,0,"",NoList,CA_None,isSens,NoFrame,mdFrm1,mdLbl1},
        {C_Label,mdLbl3,0,"Radios",NoList,CA_None,isSens,mdFrmV1,0,0},
        {C_RadioButton,mdRB1,1,"KOB",NoList,CA_None,
                isSens,mdFrmV1,0,mdLbl3},
        {C_RadioButton,mdRB2,0,"KOAT",NoList,CA_None,
                isSens,mdFrmV1,0,mdRB1},

        {C_Button,mdBtnChange,0,change_me,NoList,CA_None,
                isSens,NoFrame,0,mdFrmV1},
        {C_Button,M_Cancel,M_Cancel," Cancel ",NoList,CA_None,
            isSens,NoFrame,mdBtnChange,mdFrmV1},
        {C_Button,M_OK,M_OK," OK ",NoList,CA_DefaultButton,
            isSens,NoFrame,M_Cancel,mdFrmV1},

        {C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };


//==========================>>> tDialog::tDialog <<<======================
  tDialog::tDialog(vBaseWindow* bw) :
    vDialog(bw)
  {
    // The constructor for a derived dialog calls the superclass
    // constructor, and then adds the command objects to the dialog
    // by calling AddDialogCmds.

    UserDebug(Constructor,"tDialog::tDialog()\n")
    AddDialogCmds(DefaultCmds);         // add the command objects
  }

//=========================>>> tDialog::~tDialog <<<======================
  tDialog::~tDialog()
  {
    // Destructor often doesn't need to do anything

    UserDebug(Destructor,"tDialog::~tDialog() destructor\n")
  }

//====================>>> tDialog::DialogCommand <<<======================
  void tDialog::DialogCommand(ItemVal id, ItemVal retval, CmdType ctype)
  {
    // After the user has selected a command from the dialog,
    // this routine is called with the value.

    vNoticeDialog note(this);   // an instance we can use

    UserDebug1(CmdEvents,"tDialog::DialogCommand(id:%d)\n",id)

    switch (id)         // We will do some things depending on value
      {
        case mdCB1:             // CheckBox
            note.Notice("Test A");
            break;

        case mdCB2:             // CheckBox
            note.Notice("Test B");
            break;

        case mdCB3:             // CheckBox
            note.Notice("Test C");
            break;

        case mdRB1:             // Radio Button
            note.Notice("KOB");
            break;

        case mdRB2:             // Radio Button
            note.Notice("KOAT");
            break;

        case mdBtn1:            // Button
            note.Notice("Button 1");
            break;

        case mdBtn2:            // Button
            note.Notice("Button 2");
            break;

        case mdBtnChange:       // Example: change my own label
            // We will change the label on this button
            change_me[10]++;            // change the "A"
            SetString(mdBtnChange, change_me);
            break;
      }
    // All commands should also route through the parent handler
    // which has useful default behaviors for Cancel and OK
    vDialog::DialogCommand(id,retval,ctype);
  }
