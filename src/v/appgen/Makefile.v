#=======================================================================
#@V@:Note: File automatically generated by VIDE makefile maker (12:24:54 11 Feb 2000) (g++).
#  This file regenerated each time you run VIDE, so save under a
#    new name if you hand edit, or it will be overwritten.
#=======================================================================

# Standard defines:
CC     	=	g++

HOMEV	=	c:\mingw32
VPATH	=	$(HOMEV)/include
oDir	=	../objdll
Bin	=	../bin
Src	=	.
libDirs	=	-L../lib

incDirs	=	-I../includew

LIBS	=	-s -mwindows -lVDLL
C_FLAGS	=	-O\
	-DUSE_V_DLL

SRCS	=\
	$(Src)/vgapp.cpp\
	$(Src)/vgcmdw.cpp\
	$(Src)/vgcnv.cpp\
	$(Src)/vgcode.cpp\
	$(Src)/vgdlmdlg.cpp\
	$(Src)/vgmdlg.cpp

EXOBJS	=\
	$(oDir)/vgapp.o\
	$(oDir)/vgcmdw.o\
	$(oDir)/vgcnv.o\
	$(oDir)/vgcode.o\
	$(oDir)/vgdlmdlg.o\
	$(oDir)/vgmdlg.o

ALLOBJS	=	$(EXOBJS)
ALLBIN	=	$(Bin)/vgendll.exe
ALLTGT	=	$(Bin)/vgendll.exe

# User defines:
# Add resource file .o:vgen.rc
EXOBJS += $(oDir)/vgen.o

#@# Targets follow ---------------------------------

all:	$(ALLTGT)

objs:	$(ALLOBJS)

cleanobjs:
	rm -f $(ALLOBJS)

cleanbin:
	rm -f  $(ALLBIN)

clean:	cleanobjs

cleanall:	cleanobjs cleanbin

#@# User Targets follow ---------------------------------

# Compile resource file
$(oDir)/vgen.o:  $(Src)/vgen.rc
	windres $(Src)/vgen.rc $(oDir)/vgen.o

#@# Dependency rules follow -----------------------------

$(Bin)/vgendll.exe: $(EXOBJS)
	$(CC) -o $(Bin)/vgendll.exe $(EXOBJS) $(incDirs) $(libDirs) $(LIBS)

$(oDir)/vgapp.o: vgapp.cpp vgapp.h vgdefs.h vgcmdw.h vgcnv.h vgmdlg.h \
 vgdlmdlg.h
	$(CC) $(C_FLAGS) $(incDirs) -c -o $@ $<

$(oDir)/vgcmdw.o: vgcmdw.cpp vgcmdw.h vgdefs.h vgcnv.h vgmdlg.h vgdlmdlg.h \
 vgcode.h
	$(CC) $(C_FLAGS) $(incDirs) -c -o $@ $<

$(oDir)/vgcnv.o: vgcnv.cpp vgcnv.h
	$(CC) $(C_FLAGS) $(incDirs) -c -o $@ $<

$(oDir)/vgcode.o: vgcode.cpp vgcode.h vgdefs.h
	$(CC) $(C_FLAGS) $(incDirs) -c -o $@ $<

$(oDir)/vgdlmdlg.o: vgdlmdlg.cpp vgdlmdlg.h vgdefs.h
	$(CC) $(C_FLAGS) $(incDirs) -c -o $@ $<

$(oDir)/vgmdlg.o: vgmdlg.cpp vgmdlg.h vgdefs.h
	$(CC) $(C_FLAGS) $(incDirs) -c -o $@ $<
