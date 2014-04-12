#=======================================================================
#  Makefile for V IDE
#  Copyright (C) 1998  Bruce E. Wampler
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
PROG=\
	$(Bin)/videx
else
ifeq ($(Arch),gnuwin32)
PROG=\
	$(Bin)/vide.exe
else
PROG=\
	$(Bin)/vide
endif
endif

CFLAGS	+= -I$(HOMEV)/vide

ifeq ($(DEBUG),no)
LDFLAGS	+= -s
endif

EXOBJS=\
	$(oDir)/videapp.o \
	$(oDir)/videcnv.o \
	$(oDir)/vbaseci.o \
	$(oDir)/vseeci.o \
	$(oDir)/vstdci.o \
	$(oDir)/vvici.o \
	$(oDir)/videmake.o \
	$(oDir)/vrundlg.o \
	$(oDir)/videcmdw.o \
	$(oDir)/videjava.o \
	$(oDir)/videlib.o \
	$(oDir)/mmaker.o \
	$(oDir)/cprojdlg.o \
	$(oDir)/cnewmdlg.o \
	$(oDir)/gdbdlg.o \
	$(oDir)/jdbdlg.o \
	$(oDir)/vdbcmdw.o \
	$(oDir)/jprojdlg.o \
	$(oDir)/vprefs.o \
	$(oDir)/prefmdlg.o \
	$(oDir)/abtvdlg.o

ifeq ($(Arch),gnuwin32)
EXOBJS += $(oDir)/viderc.o
endif

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

.PHONY: default all objs clean cleanobj cleanall videStatic

default: all

all: $(PROG)

videStatic:	$(PROG)Stat

clean:
	-rm -f $(CLEANEXTS) $(EXOBJS)

cleanobj: clean
	-rm -f $(EXOBJS)

cleanall: cleanobj
	-rm -f $(PROG)

cleanexe: cleanbin

cleanbin:
	-rm -f $(PROG)

objs:	$(EXOBJS)

#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

$(PROG):	$(EXOBJS)
	$(CXX) -o $@ $(EXOBJS) $(LDFLAGS)

$(PROG)Stat:	$(EXEOBJS)
	$(CXX) -o $@ $(EXOBJS) $(LDFLAGS) -lSM -lICE -static

$(oDir)/viderc.o:	vide.rc vide.ico vwindow.ico
	$(RES) vide.rc $(oDir)/viderc.o

$(oDir)/videcnv.o:	videcnv.cpp v_defs.h videcnv.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/mmaker.o: mmaker.cpp videapp.h mmaker.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/cprojdlg.o: cprojdlg.cpp cprojdlg.h mmaker.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/cnewmdlg.o: cnewmdlg.cpp cnewmdlg.h mmaker.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/gdbdlg.o: gdbdlg.cpp gdbdlg.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/jdbdlg.o: jdbdlg.cpp jdbdlg.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/jprojdlg.o: jprojdlg.cpp jprojdlg.h videjava.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/abtvdlg.o: abtvdlg.cpp abtvdlg.h videapp.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/vrundlg.o: vrundlg.cpp vrundlg.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/vbaseci.o:	vbaseci.cpp v_defs.h vbaseci.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/vstdci.o:	vstdci.cpp v_defs.h vstdci.h vbaseci.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/vseeci.o:	vseeci.cpp v_defs.h vseeci.h vbaseci.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/vvici.o:	vvici.cpp v_defs.h vvici.h vbaseci.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/videmake.o:	videmake.cpp v_defs.h videmake.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/vprefs.o:	vprefs.cpp v_defs.h vprefs.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/prefmdlg.o:	prefmdlg.cpp v_defs.h prefmdlg.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/videcmdw.o:	videcmdw.cpp v_defs.h videcmdw.h abtvdlg.h vcmdcmds.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/vdbcmdw.o:	vdbcmdw.cpp v_defs.h vdbcmdw.h gdbdlg.h jdbdlg.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/videjava.o:	videjava.cpp v_defs.h videjava.h jprojdlg.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/videlib.o:	videlib.cpp videlib.h
	$(CXX) -c $(CFLAGS) -o $@ $<

$(oDir)/videapp.o:	videapp.cpp v_defs.h vapp.h \
	videapp.h videcmdw.h
	$(CXX) -c $(CFLAGS) -o $@ $<
