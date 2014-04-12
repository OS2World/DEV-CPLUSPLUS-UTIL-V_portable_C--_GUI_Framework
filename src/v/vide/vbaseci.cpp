// #########################################################################
//
// DEFAULT vBaseTECmdInterp
//
// #########################################################################

#include "vbaseci.h"

    // initially, all blank. We will allocate space on an as needed
    // basis. We will let the allocated space hang around until
    // we exit the editor.
    vKey* vBaseTECmdInterp::QReg[28] = {0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
    unsigned short* vBaseTECmdInterp::QRegShift[28];

// ==================>>> vBaseTECmdInterp::vBaseTECmdInterp <<<=================
  vBaseTECmdInterp::vBaseTECmdInterp(vTextEditor* textEd, vCmdWindow* cw)
        : vTextEdCmdInterp(textEd, cw)
  {
      curReg = 'a';
  }

// ==================>>> vBaseTECmdInterp::~vBaseTECmdInterp <<<================
  vBaseTECmdInterp::~vBaseTECmdInterp()
  {
  }

// =====================>>> vBaseTECmdInterp::InitCmdInterp <<<====================
  void vBaseTECmdInterp::InitCmdInterp()
  {
  }

// --------------- Global Helper routines for most CI's --------------------

// =====================>>> vBaseTECmdInterp::setQReg <<<====================
  int vBaseTECmdInterp::setQReg(int reg)
  {
    // validate reg range, then copy newQReg to appropriate entry

    if (reg < 'a' || reg > 'z')
	return 0;		// use  default [0]

    int regI = reg - 'a';	// make correct Index range [1-26]
    if (QReg[regI] == 0)	// not allocated yet
      {
	QReg[regI] = new vKey[lenQ(newQReg)+1];
	QRegShift[regI] = new unsigned short[lenQ(newQReg)+1];
      }
    else if (lenQ(QReg[regI]) < lenQ(newQReg))	// reallocate space
      {
	delete [] QReg[regI];
	delete [] QRegShift[regI];
	QReg[regI] = new vKey[lenQ(newQReg)+1];
	QRegShift[regI] = new unsigned short[lenQ(newQReg)+1];
      }
    if (!QReg[regI])		// did we get the space?
	return 0;
    int ix;
    for (ix = 0 ; newQReg[ix] ; ++ix)
      {
	QReg[regI][ix] = newQReg[ix];
	QRegShift[regI][ix] = newQRegShift[ix];
      }
    QReg[regI][ix] = 0;
    QRegShift[regI][ix] = 0;
    return 1;
  }

// =====================>>> vBaseTECmdInterp::getQReg <<<====================
  vKey* vBaseTECmdInterp::getQReg(int reg)
  {
    if (reg < 'a' || reg > 'z')
	return 0;		// use  default [0]
    int regI = reg - 'a';	// make correct Index range [1-26]
    return QReg[regI];
  }

// =====================>>> vBaseTECmdInterp::getQRegShift <<<====================
  unsigned short* vBaseTECmdInterp::getQRegShift(int reg)
  {
    if (reg < 'a' || reg > 'z')
	return 0;		// use  default [0]
    int regI = reg - 'a';	// make correct Index range [1-26]
    return QRegShift[regI];
  }

// =====================>>> vBaseTECmdInterp::lenQ <<<====================
  int vBaseTECmdInterp::lenQ(vKey* qreg)
  {
    int len;
    for (len = 0 ; qreg[len] != 0 ; ++len)
	;
    return len;
  }

// =====================>>> vBaseTECmdInterp::execQReg <<<====================
  int vBaseTECmdInterp::execQReg(int reg, long count)
  {
    // send all commands in Q register reg to the editor count times

    char execbeg[] = "Q[x] - Executing";
    char execdone[] = "Q[x] - Done";
    char execfail[] = "Q[x] - Did not complete";
    char execempty[] = "Q[x] - Empty - no commands";

    execbeg[2] = curReg;
    execdone[2] = curReg;
    execfail[2] = curReg;
    execempty[2] = curReg;

    int retval = 1;

    vKey* rp = getQReg(reg);
    unsigned short* sp = getQRegShift(reg);

    if (!rp || ! *rp)
      {
	te()->StatusMessage(execempty);
	return 0;
      }

    te()->StatusMessage(execbeg);
    int oldech = te()->GetEdState().echof;
    if (count > 1)
	te()->SetEchoF(0);

    for (long ix = 0 ; ix < count && retval ; ++ix)
      {
	vKey* cp;
	unsigned short* sh;
	for (cp = rp, sh = sp ; *cp ; ++cp, ++sh)
	  {
	    retval = ProcessKey(*cp, *sh);	// recursively pass to the CI
	    if (retval <= 0)
	      {
		retval = 0;
		break;
	      }
	  }
      }   

    te()->SetEchoF(oldech);	// restore echo state
    te()->Verify();
    if (!retval)
	te()->StatusMessage(execfail);
    else
	te()->StatusMessage(execdone);
    return retval;		// done...
  }

// ==================>>> vBaseTECmdInterp::echoInput <<<==================
  void vBaseTECmdInterp::echoInput(vKey chr, char* buff, int maxb)
  {
    // echoes input to status line, with editing and ctrl char mapping

    char eb[256];		// to really echo
    char tmp[2] = "x";

    if (chr == vk_BackSpace)
      {
	int pl = strlen(buff);

	if (pl > 0)
	    buff[pl-1] = 0;	// wipe last char
      }
    else
      {
	tmp[0] = (char)chr;	// add to pattern
	if (strlen(buff) < (unsigned int)maxb)
	  {
	    strcat(buff, tmp);
	  }
      }
    int ix = 0;
    for (char* cp = buff ; *cp && ix < 254 ; cp++)
      {
	if (*cp == vk_Escape)
	  {
	    eb[ix++] = '$';
	  }
	else if (*cp < ' ')		// a ctrl char
	  {
	    eb[ix++] = '^';
	    eb[ix++] = *cp + '@';
	  }
	else
	    eb[ix++] = *cp;
      }
    eb[ix] = 0;
    te()->StatusMessage(eb);
    return;
  }

// ==================>>> vBaseTECmdInterp::echoQInput <<<==================
  void vBaseTECmdInterp::echoQInput(vKey chr, unsigned int shift)
  {
    // echoes input to status line, with editing and ctrl char mapping

    char eb[256];		// to really echo

    if (chr == vk_BackSpace)
      {
	int pl = lenQ(newQReg);

	if (pl > 0)
	    newQReg[pl-1] = 0;	// wipe last char
      }
    else
      {
	int len = lenQ(newQReg);
	if (len < MAX_LINE)
	  {
	    newQReg[len] = chr;
	    newQReg[len+1] = 0;
	    newQRegShift[len] = (unsigned short)shift;
	  }
      }
    int ix = 0;
    for (vKey* cp = newQReg ; *cp && ix < 250 ; cp++)
      {
	if (*cp == vk_Escape)
	  {
	    eb[ix++] = '$';
	  }
	else if (*cp > 0xff)		// a keypad key!
	  {
	    eb[ix++] = '{';
	    eb[ix++] = 'f';
	    eb[ix++] = 'n';
	    eb[ix++] = '}';
	  }
	else if (*cp < ' ')		// a ctrl char
	  {
	    eb[ix++] = '^';
	    eb[ix++] = *cp + '@';
	  }
	else
	    eb[ix++] = *cp;
      }
    eb[ix] = 0;
    te()->StatusMessage(eb);
    return;
  }

// =====================>>> vBaseTECmdInterp::doHighlight <<<====================
  int vBaseTECmdInterp::doHighlight(long cmdCount, 
	vKey key, unsigned int shift)
  {
    // handle the keyboard highlighting here....
    // valid keys are vk_Home, vk_KP_Home, vk_End, vk_KP_End,
    // and the 8 arrow keys (normal, KP)

    int retval = 1;
    switch (key)
      {
	case vk_Home:
	case vk_KP_Home:
	  {
	    if (shift & VKM_Ctrl)	// whole thing
	      {
		te()->addToRange(-99999,-1000);
	      }
	    else
		te()->addToRange(0,-1000);
	    break;
	  }

	case vk_End:
	case vk_KP_End:
	  {
	    if (shift & VKM_Ctrl)	// whole thing
	      {
		te()->addToRange(99999,1000);
	      }
	    else
		te()->addToRange(0,1000);
	    break;
	  }

	case vk_Up:
	case vk_KP_Up:
	  {
	    te()->addToRange(-cmdCount,0);
	    break;
	  }

	case vk_KP_Down:
	case vk_Down:
	  {
	    te()->addToRange(cmdCount,0);
	    break;
	  }

	case vk_Left:
	case vk_KP_Left:
	  {
	    te()->addToRange(0,-1);
	    break;
	  }

	case vk_Right:
	case vk_KP_Right:
	  {
	    te()->addToRange(0,1);
	    break;
	  }

	default:
	    break;
      }
    return retval;
  }

// ----------------------------- BaseCI same as Default --------------------------------

// =====================>>> vBaseTECmdInterp::MouseJump <<<====================
  void vBaseTECmdInterp::MouseJump(int row, int col)
  {
    vTextEdCmdInterp::MouseJump(row, col);
  }

// =====================>>> vBaseTECmdInterp::CmdInterpHelp <<<====================
  void vBaseTECmdInterp::CmdInterpHelp()
  {
    vTextEdCmdInterp::CmdInterpHelp();
  }

// =====================>>> vBaseTECmdInterp::ProcessKey <<<====================
  int vBaseTECmdInterp::ProcessKey(vKey key, unsigned int shift)
  {
    return vTextEdCmdInterp::ProcessKey(key, shift);
  }

