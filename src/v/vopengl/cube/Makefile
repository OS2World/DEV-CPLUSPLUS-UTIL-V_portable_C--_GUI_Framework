#=======================================================================
#@V@:Note: This file generated by vgen V1.04 (09:06:20 22 Jun 1998).
#=======================================================================

CC	=	g++

HOMEV	=	D:/egcs
LIBS	=	-lV -lVgl -lglu32 -lopengl32 -lcomctl32 -mwindows
VLibDir	=	$(HOMEV)/lib
oDir	=	.
Bin	=	.

VPATH	=	$(HOMEV)/include

#--------------------------------------------------------------
# Architecture dependent

# Flags for includes and libraries

CFLAGS	=	-O -I$(HOMEV)/include

LFLAGS	=	-O -L$(VLibDir)

EXOBJS	=	$(oDir)/cubeapp.o \
		$(oDir)/cubecnv.o \
		$(oDir)/cubecmdw.o

all:	$(Bin)/cube.exe

objs:	$(EXOBJS)

clean:
	rm $(EXOBJS)

$(Bin)/cube.exe:	$(EXOBJS)
	$(CC) -o $@ $(LFLAGS) $(EXOBJS) $(LIBS)

$(oDir)/cubecnv.o:	cubecnv.cpp  cubecnv.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(oDir)/cubecmdw.o:	cubecmdw.cpp cubecmdw.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(oDir)/cubeapp.o:	cubeapp.cpp \
	cubeapp.h cubecmdw.h
	$(CC) -c $(CFLAGS) -o $@ $<
