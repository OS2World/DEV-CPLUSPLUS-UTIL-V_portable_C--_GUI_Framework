//========================================================
//  tmodal.h - Header file for tModalDialog class
//
//  Copyright 1995,1996 Bruce E. Wampler, All Rights Reserved
//========================================================

#ifndef TMODAL_H
#define TMODAL_H

#include <v/vmodald.h>  // derived from vModalDialog

    class tModalDialog : public vModalDialog
      {
      public:           //---------------------------------------- public
        tModalDialog(vBaseWindow*);
        virtual ~tModalDialog();                // Destructor
        virtual void DialogCommand(ItemVal id, ItemVal retval,
                CmdType ctype);

      protected:        //--------------------------------------- protected

      private:          //--------------------------------------- private

      };
#endif
