#===============================================================
# V Make Configuration file - Version 1.24 - 3/3/2000
# date:
#
# For Windows9x/NT mingw32
# Note: mingw32 requires the use of mingw32 and
# unix tools ported to WIN, such as cp, rm, and touch.
#
# Copyright (C) 1995-2000  Bruce E. Wampler
#
# This file is part of the V C++ GUI Framework, and is covered
# under the terms of the GNU Lesser General Public License,
# Version 2.1. This library has NO WARRANTY. See the source file
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
#HOMEV	=	C:/v
HOMEV	=	..

# Select one of the following, or provide your own paths
HOMEGNU	=	C:/mingw32
#HOMEGNU	=	C:/egcs
#HOMEGNU	=	D:/mingw32
#HOMEGNU	=	D:/egcs


#---------------------------------------------------------------------
# Tools used in the makefile execution
#---------------------------------------------------------------------
CC	=	gcc
CXX	=	g++
GMAKE	=	gmake
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
INCDIR	=	-I$(HOMEV)/includew
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
LDFLAGS	=	$(LIBDIR) $(LIBS)
