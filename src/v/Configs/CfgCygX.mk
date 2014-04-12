#===============================================================
# V Make Configuration file - Version 1.24 - 12/15/99
#
# Copyright (C) 1995-19998  Bruce E. Wampler
#
# THIS IS THE Config.mk FILE FOR THE X VERSION OF V for Cygwin
#
# To build an X version, set the TOOLKIT variable to Athena
# for the generic Athena version, or to Motif
# for the Lesstif/Motif version. (See comment below)
#
# This file is part of the V C++ GUI Framework, and is covered
# under the terms of the GNU Library General Public License,
# Version 2. This library has NO WARRANTY. See the source file
# vapp.cxx for more complete information about license terms.
#===============================================================


#---------------------------------------------------------------------
# Version info
#---------------------------------------------------------------------
VV	=	1.24
VVW	=	124


#---------------------------------------------------------------------
# HOMEV info
#---------------------------------------------------------------------

HOMEV   =       /vol/v-$(VV)

#---------------------------------------------------------------------
# Tools used in the makefile execution
#---------------------------------------------------------------------
CC	=	gcc
CXX	=	g++

#---------------------------------------------------------------------
# VPATH for dependencies on header files
#---------------------------------------------------------------------
VPATH=$(HOMEV)/includex/v


#---------------------------------------------------------------------
# Select the architecture of your system.
# These are the architectures that V has been extensively tested with:
# linujx, linuelf, sun4, mips, sgi
# For I486 or i586 choose: intel.
# User contributed definitions are available for:
# hpux, aix, solaris, bsd
#---------------------------------------------------------------------
ARCH   = gnuwin32x
Arch = $(ARCH)


#---------------------------------------------------------------------
# Select Athena or Motif TOOLKIT option.
#---------------------------------------------------------------------
TOOLKIT		=	Athena
#TOOLKIT	=	Motif

#---------------------------------------------------------------------
# Use this define for NO 3D on Athena Widgets
# For Motif use NO 3D i.e. USE_3D=no.
#---------------------------------------------------------------------
ifeq ($(TOOLKIT),Motif)
USE_3D = no
else
USE_3D = yes
endif


#---------------------------------------------------------------------
# Select X11R-version
#---------------------------------------------------------------------
X11RV   = X11R6.4


#---------------------------------------------------------------------
# Select Debug or no Debug
#---------------------------------------------------------------------
#DEBUG	=	yes
DEBUG	=	no

#---------------------------------------------------------------------
# Select Develop or not - used for original development
#---------------------------------------------------------------------
#DEVEL	=	yes
DEVEL	= no


#---------------------------------------------------------------------
# Define filename extensions that are targeted by cleanup's
#---------------------------------------------------------------------
CLEANEXTS= *.bak *~ *.tmp core *.o *.obj


#---------------------------------------------------------------------
# Architecture dependent directory locations
#---------------------------------------------------------------------
VLibDir	=	$(HOMEV)/lib
ifeq ($(TOOLKIT),Motif)
oDir	=	$(HOMEV)/obj
endif
ifeq ($(TOOLKIT),Athena)
oDir	=	$(HOMEV)/objx
endif
LibDir	=	$(HOMEV)/lib
Bin	=	$(HOMEV)/bin


#---------------------------------------------------------------------
# Architecture independent 
#---------------------------------------------------------------------
INCDIR	=	-I$(HOMEV)/includex
LIBDIR	=	-L$(LibDir)
ifeq ($(TOOLKIT),Athena)
	LIBNAME	= libVx
	V1NAME 	= v1x
	LIBS	= -lVx -lXaw
endif
ifeq ($(TOOLKIT),Motif)
	LIBNAME	= libV
	V1NAME 	= v1
	LIBS	= -lV -lXm
endif


#---------------------------------------------------------------------
# Cygwin32 b19 (building the X version): 
#       - working at least with X11R6.4 include and lib files from 
#         Sergey Okhapkin's port at http://www.lexa.ru/sos.
#---------------------------------------------------------------------
ifeq ($(Arch),gnuwin32x)
INCDIR	+=	-I/usr/$(X11RV)/include
LIBDIR	+=	-L/usr/$(X11RV)/lib
LIBS    +=      -lXmu -lXt -lXext -lX11 -lICE -lSM -mwindows
TARZ	=	
RANLIB	=	ranlib
VGPATH	=	
endif


#---------------------------------------------------------------------
# C/C++ compile options
#---------------------------------------------------------------------

CFLAGS	+= $(INCDIR) 
ifeq ($(DEBUG),no)
CFLAGS	+=	-O
endif
ifeq ($(DEBUG),yes)
CFLAGS	+=	-g
endif
ifeq ($(TOOLKIT),Motif)
CFLAGS	+=	-DMotif
endif
ifeq ($(Arch),sgi)
endif
ifeq ($(DEVEL),yes)
CFLAGS	+=	-DDEVEL
endif

#---------------------------------------------------------------------
# LINK/LOAD options
#---------------------------------------------------------------------
LDFLAGS	=	$(LIBDIR) $(LIBS)
