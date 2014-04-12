#=======================================================================
#  Makefile for example V draw program
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

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

ifeq ($(TOOLKIT),Athena)
PROG	=	$(Bin)/vdrawexx
else
ifeq ($(TOOLKIT),gnuwin32)
PROG	=	$(Bin)/vdrawex.exe
else
PROG	=	$(Bin)/vdrawex
endif
endif

EXOBJS=\
	$(oDir)/drawapp.o \
	$(oDir)/drawcnv.o \
	$(oDir)/drawcmdw.o

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

$(oDir)/drawcnv.o:	drawcnv.cpp v_defs.h drawcnv.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/drawcmdw.o:	drawcmdw.cpp v_defs.h drawcmdw.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/drawapp.o:	drawapp.cpp v_defs.h drawapp.h drawcmdw.h drawcnv.h
	$(CXX) -c $(CFLAGS) -o $@ $<
