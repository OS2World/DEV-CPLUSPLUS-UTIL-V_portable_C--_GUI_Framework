//========================================================================
//
//  tdialog.h - Header file for tutorial tDialog class
//
//  Copyright 1995,1996 Bruce E. Wampler, All Rights Reserved
//========================================================================

#ifndef TDIALOG_H
#define TDIALOG_H

#include <v/vdialog.h>  // we derive from vDialog

    class tDialog : public vDialog
      {
      public:           //---------------------------------------- public
        tDialog(vBaseWindow*);
        virtual ~tDialog();             // Destructor
        virtual void DialogCommand(ItemVal id, ItemVal retval,
                CmdType ctype);

      protected:        //------------------------------------- protected

      private:          //--------------------------------------- private
        int _toggleId;
      };
#endif
