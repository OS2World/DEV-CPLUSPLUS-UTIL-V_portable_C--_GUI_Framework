//===============================================================
// V Shell App Generator
//
// Copyright (C) 1995,1996  Bruce E. Wampler
//
// This file is part of the V C++ GUI Framework, and is covered
// under the terms of the GNU Library General Public License,
// Version 2. This library has NO WARRANTY. See the source file
// v/srcx/vapp.cxx for more complete information about license terms.
//===============================================================

#ifndef VGCODE_H
#define VGCODE_H

#include "vgdefs.h"

  int genApp(vgOptions& op);
  int genCmdw(vgOptions& op);
  int genMDlg(vgOptions& op);
  int genDlg(vgOptions& op);
  int genCnv(vgOptions& op);
  int genTextCnv(vgOptions& op);
  int genOGLCnv(vgOptions& op);
  int genMake(vgOptions& op);
  int genProject(vgOptions& op);

#endif
