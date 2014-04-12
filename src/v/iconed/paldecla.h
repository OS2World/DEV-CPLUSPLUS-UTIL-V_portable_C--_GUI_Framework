//========================= paldeclare.h ================================
// This is the header file used to .ResetColor the colors in the color 
// palette after initial startup.
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//=======================================================================

#ifndef PALHEAD_H
#define PALHEAD_H

#include  <v/vcolor.h>

   _Palette[0].ResetColor( 0, 0, 0);
   _Palette[1].ResetColor( 255, 255, 255);
   _Palette[2].ResetColor( 191, 191, 191);
   _Palette[3].ResetColor( 127, 127, 127);

   _Palette[4].ResetColor( 255, 0, 0);
   _Palette[5].ResetColor( 0, 255, 0);
   _Palette[6].ResetColor( 0, 0, 255);

   _Palette[8].ResetColor( 0, 255, 255);
   _Palette[7].ResetColor( 255, 255, 0);
   _Palette[9].ResetColor( 255, 0, 255);

   _Palette[10].ResetColor( 0, 0, 127);
   _Palette[11].ResetColor( 0, 127, 0);
   _Palette[12].ResetColor( 127, 0, 0);
   _Palette[13].ResetColor( 0, 0, 191);
   _Palette[14].ResetColor( 0, 191, 0);
   _Palette[15].ResetColor( 191, 0, 0);

   _Palette[16].ResetColor( 0, 127, 127);
   _Palette[17].ResetColor( 0, 127, 191);
   _Palette[18].ResetColor( 0, 127, 255);
   _Palette[19].ResetColor( 0, 191, 127);
   _Palette[20].ResetColor( 0, 191, 191);
   _Palette[21].ResetColor( 0, 191, 255);
   _Palette[22].ResetColor( 0, 255, 127);
   _Palette[23].ResetColor( 0, 255, 191);

   _Palette[24].ResetColor( 127, 0, 127);
   _Palette[25].ResetColor( 127, 0, 191);
   _Palette[26].ResetColor( 127, 0, 255);
   _Palette[27].ResetColor( 127, 127, 0);
   _Palette[28].ResetColor( 127, 127, 191);
   _Palette[29].ResetColor( 127, 127, 255);
   _Palette[30].ResetColor( 127, 191, 0);
   _Palette[31].ResetColor( 127, 191, 127);
   _Palette[32].ResetColor( 127, 191, 191);
   _Palette[33].ResetColor( 127, 191, 255);
   _Palette[34].ResetColor( 127, 255, 0);
   _Palette[35].ResetColor( 127, 255, 127);
   _Palette[36].ResetColor( 127, 255, 191);
   _Palette[37].ResetColor( 127, 255, 255);

   _Palette[38].ResetColor( 191, 0, 127);
   _Palette[39].ResetColor( 191, 0, 191);
   _Palette[40].ResetColor( 191, 0, 255);
   _Palette[41].ResetColor( 191, 127, 0 );
   _Palette[42].ResetColor( 191, 127, 127);
   _Palette[43].ResetColor( 191, 127, 191);
   _Palette[44].ResetColor( 191, 127, 255);
   _Palette[45].ResetColor( 191, 191, 0);
   _Palette[46].ResetColor( 191, 191, 127);
   _Palette[47].ResetColor( 191, 191, 255);
   _Palette[48].ResetColor( 191, 255, 0);
   _Palette[49].ResetColor( 191, 255, 127);
   _Palette[50].ResetColor( 191, 255, 191);
   _Palette[51].ResetColor( 191, 255, 255);

   _Palette[52].ResetColor( 255, 0, 127);
   _Palette[53].ResetColor( 255, 0, 191);
   _Palette[54].ResetColor( 255, 127, 0);
   _Palette[55].ResetColor( 255, 127, 127);
   _Palette[56].ResetColor( 255, 127, 191);
   _Palette[57].ResetColor( 255, 127, 255);
   _Palette[58].ResetColor( 255, 191, 0);
   _Palette[59].ResetColor( 255, 191, 127);
   _Palette[60].ResetColor( 255, 191, 191);
   _Palette[61].ResetColor( 255, 191, 255);
   _Palette[62].ResetColor( 255, 255, 127);
   _Palette[63].ResetColor( 255, 255, 191);


   for (int ipx = 64 ; ipx < 256 ; ++ipx)
	_Palette[ipx].Set(0,0,0);

#endif
