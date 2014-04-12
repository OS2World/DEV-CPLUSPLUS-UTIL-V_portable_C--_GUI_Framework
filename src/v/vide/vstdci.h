#ifndef VSTDCMDINTERP_H
#define VSTDCMDINTERP_H

#include "vbaseci.h"

     class vStandardCI : public vBaseTECmdInterp
      {

        public:
          vStandardCI(vTextEditor* textEd, vCmdWindow* cw);
          virtual ~vStandardCI();


          virtual int ProcessKey(vKey key, unsigned int shift);
          virtual void InitCmdInterp();
          virtual void CmdInterpHelp();

	protected:

	    // long cmdCount;	// how many times to repeat command

	    // int
	    // 	countWait,  // if building a count
	    //	metaChar,   // the meta char for commands
	    //	metaWait;   // if waiting on a meta command

        private:
	    int findWait;	// for keyboard input finds
	    int recording;	// recording Q-macro
	    int settingQ;	// setting current Q register

      };

#endif
