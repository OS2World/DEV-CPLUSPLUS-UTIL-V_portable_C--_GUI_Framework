#===============================================================
#  bmp2vbm  Makefile
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

PROG=\
	$(Bin)/b2v	

EXOBJS=\
	$(oDir)/bmp2vbm.o

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
	$(CXX) -o $@ $(EXOBJS)

$(oDir)/bmp2vbm.o:	bmp2vbm.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<                     
