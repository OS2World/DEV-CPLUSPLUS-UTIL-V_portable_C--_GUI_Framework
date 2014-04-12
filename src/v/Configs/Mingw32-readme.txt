   Compiling V and applications using 
        Mingw32 g++ 2.8.1
          and EGCS 1.1.


Now that there seems to be a really good, stable version
of GNU C++ for Windows, namely the mingw32 2.8.1 distribution,
V will be kept in sync with that distribution.

It seems that the Ming32 version may be moribund, given the
latest progress with the egcs version of gcc. V will try to
follow these distributions, and keep the makefiles and other
things up to date. As of V 1.21, the egcs compiler will be
the main one used for GNU WIN32 versions.


Instructions for rebuilding V for mingw32.


******* TO REBUILD THE V LIBRARY FOR MINGW32 *******

1. Unzip the V Windows distribution. You must be sure to use
   an unzipper that preserves file case, or manually rename all
   the files to use lower case. While Windows doesn't care,
   gnu make does.

2) Make sure these directories exist:
        v/bin/gnuwin32
        v/lib/gnuwin32
        v/obj/gnuwin32

   If you unzip correctly, they should be there.

3) You must now create the proper version of /v/Config.mk.
   Usually, this is a matter of copying the proper version
   of Config.Mk from the /v/Configs directory.
   For the mingw32 version, the file to copy is /v/Configs/CfgMing.mk.
   Copy it to /v/Config.mk.

   The default version assumes that you have unpacked the V
   distribution to c:/v, and that you have installed mingw32
   on C:/mingw32. You can change this by editing the copied
   version of /v/Config.mk.

4) The Makefile in the /V root directory is intended for use
   on Unix/Linux machines. It does NOT work for Windows.
   However, the Makefiles on the directories with the V
   library and other V applications do work on Windows, and
   are used for both the X version on Linux/Unix, and for
   the MS-Windows GNU WIN32 versions.

   Currently (October 1998), all versions of GNU WIN32 may not
   include the all headers needed to correctly compile V.

   First, TRY to compile V, then if the headers are missing,
   copy them to your GNU WIN32 compiler's include directory.

   You MAY need to copy C:/v/gnuwin32/include/commctrl.h to
   C:/mingw32/include/commctrl.h (or whereever the mingw32
   /include directory is on your system.) You need this file to compile
   V 1.18 and later for the Common Control dll. You will also
   add -lcomctl32 to your link lines in your makefile. (For some
   reason, the mingw32 distribution includes comctl32.a, but
   fails to include the corresponding header file -- that item
   is still being resolved. For now, V will provide the header.)

   Also, if you choose to compile for OpenGL, you may
   need to copy the entire gl subdirectory (found at
   v/gnuwin32/include) to the GNU WIN32 include directory
   (as a /include/gl subdirectory).

5) cd to each of these directories and run `make' for each
   of the following. (The top level Makefile in C:/v has X
   specific stuff and doesn't work. Also, since make is
   case sensitive, you might have to use `make -f makefile'.)

        srcwin		(required)
        appgen		(useful - see documentation)
        iconed		(useful - especially for icons)
        icons		(shows predefined V icons you can use)

        draw		(example only)
        drawex		(example only)
        examp		(example only)
        texted		(example only)
        tutor		(example only)
	vide            (very useful IDE for mingw32!)


6) Putting the V headers in the mingw32 directory path has the
   advantage of eliminating the need for extra include directives in
   your makefiles. (An older version of V found at the mingw32 site
   should be replaced with this latest version.)

   After you build V, you will find it easiest to copy libV.a from
   v/lib/gnuwin32 to mingw32/lib/libV.a, as well as all the
   V headers in from v/includew/v to mingw32/include/v. This 
   will allow you to easily update versions of V, and to compile your
   own applications with V.

   To do this, after you've built the V library, change to the
   home /v directory, and enter:

       make installgnuwin32

   This will copy all the required header, library, and binary
   files to the mingw32 directories as defined in Config.mk.

7) If you want to build OpenGL applications with mingw32, the header
   files are located at gnuwin32/include. 

******* TO COMPILE YOUR OWN APPLICATIONS

Now that you have a working version of V built (and probably
installed on the mingw32 directory path), it is fairly easy
to use and include the V library. The main thing is to
include the required -l switches to g++ to load the proper
libraries. Use:

g++ $(YOUROBJECTS) -lV -lcomctl32 -mwindows
   -or, for OpenGL apps-
g++ $(YOUROBJECTS) -lV -lVgl -lcomctl32 -mwindows

All the libraries needed for Windows are automatically included with the
option -mwindows (you may need to add -lcomctl32).

If you want to add icon resources, see the example vgen.rc file in
the /v/appgen directory. You can replace the .ico file with whatever
icon you want. You then need to add a dependency in your Makefile
to compile the .rc file with windres, and include the resulting
output file on the link line. The Makefile for vgen in /v/appgen
includes an example of how to do this.
