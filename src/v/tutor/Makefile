#===============================================================
#  V turial application Makefile - Version 2/14/1995
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
PROG=\
	$(Bin)/tutappx
else
ifeq ($(Arch),mingw32)
PROG=\
	$(Bin)/tutapp.exe
else
PROG=\
	$(Bin)/tutapp
endif
endif

EXOBJS=\
	$(oDir)/tutapp.o \
	$(oDir)/tdialog.o \
	$(oDir)/tmodal.o \
	$(oDir)/tcanvas.o \
	$(oDir)/tcmdwin.o

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

.PHONY: default all objs clean cleanobj cleanall

default: all

all: $(PROG)

clean:
	-rm -f $(CLEANEXTS)

cleanobj: clean
	-rm -f $(EXOBJS)

cleanall: cleanobj
	-rm -f $(PROG)

objs:	$(EXOBJS)

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

$(PROG):	$(EXOBJS)
	$(CXX) -o $@ $(EXOBJS) $(LDFLAGS)

$(oDir)/tcanvas.o:	tcanvas.cpp v_defs.h tcanvas.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/tdialog.o:	tdialog.cpp v_defs.h tdialog.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/tmodal.o:	tmodal.cpp v_defs.h tmodal.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/tcmdwin.o:	tcmdwin.cpp v_defs.h tcmdwin.h
	$(CXX) -c $(CFLAGS) -o $@ $<                     

$(oDir)/tutapp.o:	tutapp.cpp v_defs.h tdialog.h tmodal.h \
	tutapp.h tcmdwin.h
	$(CXX) -c $(CFLAGS) -o $@ $<
