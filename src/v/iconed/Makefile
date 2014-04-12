#=======================================================================
#  Makefile for V Icon Editor program
#  Copyright (C) 1995  Bruce E. Wampler
#
#  This program is part of the V C++ GUI Framework example programs.
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
#=======================================================================

CONFIG=../Config.mk
include $(CONFIG)

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

ifeq ($(TOOLKIT),Athena)
PROG	=	$(Bin)/viconedx
else
ifeq ($(TOOLKIT),gnuwin32)
PROG	=	$(Bin)/viconed.exe
else
PROG	=	$(Bin)/viconed
endif
endif

EXOBJS=\
	$(oDir)/viedapp.o \
	$(oDir)/viedcnv.o \
	$(oDir)/viedcmdw.o \
	$(oDir)/coldlg.o \
	$(oDir)/brshdlg.o \
	$(oDir)/imageio.o

ifeq ($(Arch),gnuwin32)
EXOBJS += $(oDir)/viconrc.o
endif

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

.PHONY: default all objs clean cleanobj cleanall

default: all

all: $(PROG)

clean:
	-rm -f $(CLEANEXTS)
	-rm -f $(EXOBJS)

cleanall: clean
	-rm -f $(PROG)

objs:	$(EXOBJS)

tar: clean
	tar cvfz iconed.tgz Makefile *.cpp *.h *.vbm

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

$(PROG):	$(EXOBJS)
	$(CXX) -o $@ $(EXOBJS) $(LDFLAGS)

$(oDir)/viconrc.o:	viconed.rc viconed.ico vwindow.ico
	$(RES) viconed.rc $(oDir)/viconrc.o

$(oDir)/imageio.o:	imageio.cpp imageio.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/viedcnv.o:	viedcnv.cpp v_defs.h viedcnv.h viedcmdw.h \
	vprinter.h brshdlg.h coldlg.h paldecla.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/viedcmdw.o:	viedcmdw.cpp v_defs.h viedcmdw.h viedcnv.h \
	coldlg.h brshdlg.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/viedapp.o:	viedapp.cpp v_defs.h viedapp.h viedcmdw.h viedcnv.h \
	coldlg.h brshdlg.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/coldlg.o:	coldlg.cpp v_defs.h coldlg.h viedcmdw.h viedcnv.h palclrs.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/brshdlg.o:	brshdlg.cpp v_defs.h brshdlg.h viedcmdw.h viedcnv.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     


