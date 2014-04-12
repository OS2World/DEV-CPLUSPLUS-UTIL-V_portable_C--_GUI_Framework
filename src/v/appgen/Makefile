#=======================================================================
#  Makefile for V Shell App Generator program
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

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


ifeq ($(TOOLKIT),Athena)
PROG	=	$(Bin)/vgenx
else
ifeq ($(TOOLKIT),gnuwin32)
PROG	=	$(Bin)/vgen.exe
else
PROG	=	$(Bin)/vgen
endif
endif

ifeq ($(Arch),linux)
CFLAGS	+=	-DLINUX
endif
ifeq ($(Arch),linuxelf)
CFLAGS	+=	-DLINUX
endif
ifeq ($(Arch),gnuwin32)
CFLAGS	+=	-DGNUWIN32
endif


EXOBJS=\
	$(oDir)/vgapp.o \
	$(oDir)/vgcmdw.o \
	$(oDir)/vgcode.o \
	$(oDir)/vgmdlg.o \
	$(oDir)/vgdlmdlg.o \
	$(oDir)/vgcnv.o

ifeq ($(Arch),gnuwin32)
EXOBJS += $(oDir)/vgrc.o
endif

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

.PHONY: default all objs clean cleanobj cleanall

default: all

all:	$(PROG)

clean:
	-rm -f $(CLEANEXTS)
	-rm -f $(EXOBJS)

cleanall: clean
	-rm -f $(PROG)

objs:	$(EXOBJS)

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

$(PROG):	$(EXOBJS)
	$(CXX) -o $@ $(EXOBJS) $(LDFLAGS)

# For gnuwin32 only - adds icons to .exe file

$(oDir)/vgrc.o:	vgen.rc vgen.ico vwindow.ico
	$(RES) vgen.rc $(oDir)/vgrc.o

$(oDir)/vgcnv.o:	vgcnv.cpp v_defs.h vgcnv.h vgdefs.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vgcode.o:	vgcode.cpp v_defs.h vgcode.h vgdefs.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vgmdlg.o:	vgmdlg.cpp v_defs.h vgmdlg.h vgdefs.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vgdlmdlg.o:	vgdlmdlg.cpp v_defs.h vgdlmdlg.h vgdefs.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vgcmdw.o:	vgcmdw.cpp v_defs.h vgcmdw.h vgdefs.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/vgapp.o:	vgapp.cpp v_defs.h vgapp.h vgcmdw.h vgdefs.h
	$(CXX) -c $(CFLAGS) -o $@ $<
