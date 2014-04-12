#===============================================================
#  V test application Makefile - Version 6/25/1996
#
#  Copyright (C) 1995  Bruce E. Wampler
#
#  This file is part of the V C++ GUI Framework.
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#===============================================================

CONFIG=../Config.mk
include $(CONFIG)

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

ifeq ($(TOOLKIT),Athena)
PROG	=	$(Bin)/vtestlibx
else
ifeq ($(TOOLKIT),gnuwin32)
PROG	=	$(Bin)/vtestlib.exe
else
PROG	=	$(Bin)/vtestlib
endif
endif

EXOBJS=\
	$(oDir)/testapp.o \
	$(oDir)/vtcanvas.o \
	$(oDir)/vtcmdwin.o \
	$(oDir)/vtcw2.o \
	$(oDir)/vttogdlg.o \
	$(oDir)/vtdialog.o

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

.PHONY: default all objs clean cleanobj cleanall

default: all

all: $(PROG)

relink:
	-rm -f $(PROG)
	make

clean:
	-rm -f $(CLEANEXTS)
	-rm -f $(EXOBJS)

cleanall: cleanobj
	-rm -f $(PROG)

cleanexe:
	-rm -f $(PROG)

cleanbin:
	-rm -f $(PROG)

objs:	$(EXOBJS)

rm:
	-rm -f $(PROG)
#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

$(PROG):	$(EXOBJS)
	$(CXX) -o $@ $(EXOBJS) $(LDFLAGS)

$(oDir)/vtcanvas.o:	vtcanvas.cpp vtcanvas.h vcanvas.h vtextcnv.h vcanvas.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vtdialog.o:	vtdialog.cpp vtdialog.h vdialog.h vcolor.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vttogdlg.o:	vttogdlg.cpp vttogdlg.h vdialog.h vcolor.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vtcmdwin.o:	vtcmdwin.cpp vtcmdwin.h vcmdwin.h bruce.vbm
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vtcw2.o:	vtcw2.cpp vtcw2.h vcmdwin.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/testapp.o:	testapp.cpp testapp.h v_defs.h vbaseitm.h \
	vbasewin.h vapp.h  vwindow.h vpane.h vcmdwin.h \
	vtcanvas.h vmenu.h vbtncmd.h vcbtncmd.h vlabelc.h \
	vframec.h vchkboxc.h vradioc.h vsliderc.h vtextinc.h vlistc.h vnotice.h \
	vprogrsc.h vfont.h vreply.h vtdialog.h vttogdlg.h vfilesel.h \
	vtextcnv.h vcomboc.h vtextc.h
	$(CXX) -c $(CFLAGS) -o $@ $<
