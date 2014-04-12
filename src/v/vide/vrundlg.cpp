//==============================================================
// vrundlg.cpp - vRunDialog class functions - 
//
// Copyright (C) 1998 Bruce E. Wampler
//
// This file is part of the V C++ GUI Framework, and is covered
// under the terms of the GNU Library General Public License,
// Version 2. This library has NO WARRANTY. See the source file
// vapp.cxx for more complete information about license terms.
//===============================================================

#include "videapp.h"
#include "vrundlg.h"           // our header
#include <stdlib.h>
#include <v/vos.h>
#include <v/vicon.h>           // for icon
#include <v/vfilesel.h>

// Define static data of the class

#define prompt_width 32
#define prompt_height 32
static unsigned char prompt_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x04, 0x00, 0x00, 0x20,
   0xe4, 0xff, 0xff, 0x27, 0x14, 0x00, 0x00, 0x28, 0x14, 0xc0, 0x01, 0x28,
   0x14, 0xe0, 0x03, 0x28, 0x14, 0x30, 0x06, 0x28, 0x14, 0x30, 0x06, 0x28,
   0x14, 0x00, 0x03, 0x28, 0x14, 0x80, 0x01, 0x28, 0x14, 0xc0, 0x00, 0x28,
   0x14, 0xc0, 0x00, 0x28, 0x14, 0xc0, 0x00, 0x28, 0x14, 0xc0, 0x00, 0x28,
   0x14, 0x00, 0x00, 0x28, 0x14, 0xc0, 0x00, 0x28, 0x14, 0x00, 0x00, 0x28,
   0xe4, 0xff, 0xff, 0x27, 0x04, 0x00, 0x00, 0x20, 0xf8, 0xff, 0xff, 0x1f,
   0xfe, 0xff, 0xff, 0x7f, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40,
   0x02, 0x00, 0xe0, 0x47, 0x02, 0x00, 0x00, 0x40, 0xfa, 0xff, 0xff, 0x5f,
   0xae, 0xaa, 0xaa, 0x6a, 0x56, 0x55, 0x55, 0x55, 0xaa, 0xaa, 0xaa, 0x6a,
   0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00};

    static vIcon prompt(&prompt_bits[0], prompt_height, prompt_width);

    static CommandObject ReplyDialog[] =
      {
	// Modified: TEH Jan98
	// Put icon + text in a frame so button is below frame.
	// With many-line text displays, the button will always be below text.
	// Also remove "Blanks" around buttons; not consistant with V look!
	// Put OK button to left, and Cancel to right for proper style.
	{C_Frame, 30, 0, "",
		NoList, CA_NoSpace | CA_NoBorder, isSens, NoFrame, 0, 0},
	{C_Icon,  10, 0, "Run?", (void*)&prompt, CA_None, isSens, 30, 0, 0},
	{C_Label, 1, 1, "Enter name of program to run  -  ", NoList,
		CA_None ,isSens, 30, 10,0},
	{C_TextIn, 2, 2, "", NoList,
		CA_Large, isSens, 30, 0, 10},
        {C_Button, 3, 3, "Browse",
		NoList, CA_None, isSens, NoFrame, 2, 10,0,"Use Dialog to select file"},
	{C_Button, M_OK, M_OK, " Run ",
		NoList, CA_DefaultButton, isSens, NoFrame, 0, 30},
	{C_Button, 4, 4, " Run in Console",
		NoList, CA_None, isSens, NoFrame, M_OK, 30},
	{C_Button, M_Cancel, M_Cancel, " Cancel ",
		NoList, CA_None, isSens, NoFrame, 4, 30},
	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static CommandObject DebugDialog[] =
      {
	{C_Frame, 30, 0, "",
		NoList, CA_NoSpace | CA_NoBorder, isSens, NoFrame, 0, 0},
	{C_Icon,  10, 0, "Debug?", (void*)&prompt, CA_None, isSens, 30, 0, 0},
	{C_Label, 1, 1, "Enter name of program to debug  -  ", NoList,
		CA_None ,isSens, 30, 10,0},
	{C_TextIn, 2, 2, "", NoList,
		CA_Large, isSens, 30, 0, 10},
        {C_Button, 3, 3, "Browse",
		NoList, CA_None, isSens, NoFrame, 2, 10,0,"Use Dialog to select file"},
	{C_Button, M_OK, M_OK, " Run ",
		NoList, CA_DefaultButton, isSens, NoFrame, 0, 30},
	{C_Button, M_Cancel, M_Cancel, " Cancel ",
		NoList, CA_None, isSens, NoFrame, M_OK, 30},
	{C_EndOfList,0,0,0,0,CA_None,0,0,0}
      };

    static int filterIndex = 0;
    static char* filter[] =
      {
        "*",
        0
      };
    static char runName[maxFileNameSize+2] = "";

//======================>>> vRunDialog::Run <<<=======================
  int vRunDialog::Run(char *initialName)
  {
    //	Show a message, wait for a reply
    //	no important return

    int ans;
    char reply[maxFileNameSize+2];

    if (!added)
      {
	AddDialogCmds(ReplyDialog);		// Set up standard dialog
	added = 1;
      }

    if (!*runName && initialName && *initialName)
	strcpy(runName,initialName);

    (void) ShowModalDialog("", ans);	// show and wait

    reply[0] = 0;

    if (ans != M_Cancel)
      {
	vOS vos;
	(void) GetTextIn(2, reply, maxFileNameSize);

        if (ans == 4)
	  {
	    char buff[maxFileNameSize+2];
#ifdef V_VersionWindows
	    // I don't really understand this, but I think it has
	    // something to do with the fact that VIDE is usually
	    // started from an icon, and thus doesn't have a console
	    // attached. This seems to make a big differencd on NT,
	    // but no difference on Win9x. So the shell has to be
	    // started differently on NT and 9x.
	    OSVERSIONINFO osver;
	    osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	    ::GetVersionEx(&osver);
	    if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT)
	      {
		strcpy(buff," /K ");
		strcat(buff,reply);
		::ShellExecute(theApp->winHwnd(),"open",
		    ((videApp*)theApp)->getVideShell(),buff,0,SW_SHOWDEFAULT);
	      }
	    else 
	      {
		strcpy(buff,((videApp*)theApp)->getVideShell());
		strcat(buff," /K ");
		strcat(buff,reply);
		vos.vRunProcess(buff, 0, 0, 0);
	      }
#else
	    strcpy(buff,((videApp*)theApp)->getVideShell());
	    strcat(buff," -e \"");
	    strcat(buff,reply);
	    strcat(buff,"\"");
	    vos.vRunProcess(buff, 0, 0, 0);
#endif
	  }
	else
	  {
	    vos.vRunProcess(reply, 0, 0, 0);
	  }
      }

    return ans;
  }

//======================>>> vRunDialog::GetDebugName <<<=======================
  int vRunDialog::GetDebugName(char *initialName, char* nameOut)
  {
    //	Show a message, wait for a reply
    //	no important return

    int ans;
    char reply[maxFileNameSize+2];

    if (!added)
      {
	AddDialogCmds(DebugDialog);		// Set up standard dialog
	added = 1;
      }

    if (!*runName && initialName && *initialName)
	strcpy(runName,initialName);

    (void) ShowModalDialog("", ans);	// show and wait

    reply[0] = 0;

    if (ans != M_Cancel)
      {
	(void) GetTextIn(2, reply, maxFileNameSize);
	strcpy(nameOut,reply);
      }

    return ans;
  }

//====================>>> vRunDialog::DialogDisplayed <<<=======================
  void vRunDialog::DialogDisplayed()
  {
      SetString(2,runName);
  }

//====================>>> vRunDialog::DialogCommand <<<=======================
  void vRunDialog::DialogCommand(ItemVal id, ItemVal val, CmdType ctype)
  {

    switch (id)
      {
        case 3:		// Browse
          {
            vFileSelect fsel(this);     // make an instance

            if (!fsel.FileSelect("Run Program",runName,maxFileNameSize,
                     filter,filterIndex))
              {
               	return;
              }
            SetString(2,runName);
            return;
          }
      }
    vModalDialog::DialogCommand(id,val,ctype);
    if (id == 4)
	CloseDialog();
  }
// --------------------------------------------------------------------- 
