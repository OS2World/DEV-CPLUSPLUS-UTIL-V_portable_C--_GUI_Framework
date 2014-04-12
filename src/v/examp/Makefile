#=======================================================================
#  Makefile for example V program
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
PROG	=	$(Bin)/protox
else
ifeq ($(TOOLKIT),gnuwin32)
PROG	=	$(Bin)/proto.exe
else
PROG	=	$(Bin)/proto
endif
endif

EXOBJS=\
	$(oDir)/protoapp.o \
	$(oDir)/mydialog.o \
	$(oDir)/mymodal.o \
	$(oDir)/mycanvas.o \
	$(oDir)/mycmdwin.o

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

$(oDir)/mycanvas.o:	mycanvas.cpp v_defs.h mycanvas.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/mydialog.o:	mydialog.cpp v_defs.h mydialog.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/mymodal.o:	mymodal.cpp v_defs.h mymodal.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/mycmdwin.o:	mycmdwin.cpp v_defs.h mycmdwin.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/protoapp.o:	protoapp.cpp v_defs.h mydialog.h mymodal.h \
	protoapp.h mycmdwin.h
	$(CXX) -c $(CFLAGS) -o $@ $<
