#===============================================================
# V Make Configuration file - Version 1.24 - 12/15/99
#
# For Cygwin32 on Windows95/NT - Native Windows GUI version
#
# Copyright (C) 1995,1996,1997,1998  Bruce E. Wampler
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
# change this to the directory you've installed V to
#---------------------------------------------------------------------
HOMEV   =       ..
HOMEGNU	=	/


#---------------------------------------------------------------------
# Tools used in the makefile execution
#---------------------------------------------------------------------
CC	=	gcc
CXX	=	g++
GMAKE	=	make
AR	=	ar
RANLIB	=	ranlib
RES	=	windres

#---------------------------------------------------------------------
# VPATH for dependencies on header files
#---------------------------------------------------------------------
VPATH=$(HOMEV)/includew/v


#---------------------------------------------------------------------
# Select the architecture of your system.
#---------------------------------------------------------------------

ARCH = gnuwin32

Arch = $(ARCH)

USE_3D = yes

#---------------------------------------------------------------------
# Select Debug or no Debug
#---------------------------------------------------------------------
#DEBUG	=	yes
DEBUG	= no

#---------------------------------------------------------------------
# Select Develop or not - used for original development
#---------------------------------------------------------------------
#DEVEL	=	yes
DEVEL	= no

#---------------------------------------------------------------------
# Define filename extensions that are targeted by cleanup's
#---------------------------------------------------------------------
CLEANEXTS= *.bak *.tmp


#---------------------------------------------------------------------
# Architecture dependent directory locations
#---------------------------------------------------------------------
VLibDir	=	$(HOMEV)/lib
oDir	=	$(HOMEV)/obj
LibDir	=	$(HOMEV)/lib
Bin	=	$(HOMEV)/bin

#---------------------------------------------------------------------
# Architecture independent 
#---------------------------------------------------------------------
INCDIR  =       -I$(HOMEV)/includew
LIBDIR	=	-L$(LibDir)
LIBNAME	=	libV
V1NAME 	=	v1
LIBS	=	-lV -lcomctl32 -mwindows
#
# This is an alternative specification that will cause a DOS
# window to come up
#
#LIBS	=	-lV -lcomdlg32 -luser32 -lgdi32 -lcomctl32



#---------------------------------------------------------------------
# C/C++ compile options
#---------------------------------------------------------------------
CFLAGS	+= $(INCDIR) 
ifeq ($(DEBUG),no)
CFLAGS	+=	-O2
endif
ifeq ($(DEBUG),yes)
CFLAGS	+=	-g
endif
ifeq ($(DEVEL),yes)
CFLAGS	+=	-DDEVEL
endif

#---------------------------------------------------------------------
# LINK/LOAD options
#---------------------------------------------------------------------
# the -e flag prevents a link warning message bout the startup
# the -s flag produces smaller executable files
LDFLAGS	=	-e _mainCRTStartup -s $(LIBDIR) $(LIBS)
