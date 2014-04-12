   Compiling V and applications using Cygwin g++ B20.1


Main feature of the Cygwin support for the V library is the fact
that it may be built for both X and Windows GUI targets.

NOTE: To use the X version you will have to download precompiled
      library and header files of X11R6.4 from Cygwin32 Porting
      Project's homepage at http://www-public.rz.uni-duesseldorf.de/~tolj,
      which is also the official supporter and a X windows server
      for your Win32 machine, i.e. X-Win32 from http://www.starnet.com.


******* TO REBUILD THE V LIBRARY (X and WINDOWS) FOR CYGWIN32 *******

1) To extract the compressed v-1.24.tar.gz archive you will have to
   do the following within your bash shell:
        gunzip v-1.24.tar.gz
        tar xf v-1.24.tar

   Now you will have the V distribution within the subdirectory /v.

2) Make sure these directories exist, or do mkdir:
        v/bin/gnuwin32x
        v/bin/gnuwin32
        v/lib/gnuwin32x
        v/lib/gnuwin32
        v/obj/gnuwin32x
        v/obj/gnuwin32

   If you unpack correctly, they should be there.

3) Making the X version: Copy /v/Configs/CfgCygX.mk to /v/Config.mk, and
   edit it as follows:
        - set HOMEV to the home directory of your V distribution
        - set X11RV = X11R6.4 (if using our recommended X11 libraries)

   now you may type "make" and build the library and some example
   applications. Please report any difficulties with this process
   to bruce@objectcentral.com.

4) Making the Windows GUI version: Copy /v/Configs/CfgCygW.mk
   to /v/Config.mk and edit that file as follows:
        - set HOMEV to the home directory of your V distribution

   cd to each of these directories and run "make" for each
   of the following. (Don't use the top level Makefile in /v since
   this is used to build the X version).

        srcwin		(required)
        appgen		(useful - see documentation)
        iconed		(useful - especially for icons)
        icons		(shows predefined V icons you can use)

        test            (tests V lib - not usually needed)
        bmp2vbm         (not needed, functionality now included in iconed)
        draw		(example only)
        drawex		(example only)
        examp		(example only)
        texted		(example only)
        tutor		(example only)
        vide            (very useful IDE for cygwin32!)

5) If you want to build OpenGL applications with cygwin32, the header
   files are located at gnuwin32/include. 

6) You MUST be sure you have only the Cygwin B20.1 distribution. You
   can't mix with mingw32 distributions. It is especially important
   that the PATH does not have any references to the mingw32 compiler.
   One symptom is getting the "can't find cpp" message. You must
   set the PATH in your autoexec.bat (Win9x) or NT environment (Control
   Panel->System->Environment) to the proper Cygwin place. This can
   be found in the cygnus.bat file, usually found at
   c:\cygnus\cygwin~1\cygnus.bat. You must be sure to reboot your
   system after fixing the path.

7) I haven't confirmed this for sure, but it seems that the V IDE
   must be built with Cygwin to work properly with the Cygwin
   environment. Versions built with other compilers seem to
   generate the "Can't find cpp" message when trying to compile.
   This may be a function of how your environment is set up.

8) Prior to the release of the 2.95 gcc compiler, there was
   a problem with the <commctrl.h> file. This has been corrected
   in the latest distributions of Cygwin. If you have problems
   with <commctrl.h>, be sure you are using the latest versions
   of the cygwin compiler.


******* TO COMPILE YOUR OWN APPLICATIONS

Now that you have a working version of V built (and probably
installed on the cygwin\bin directory path), it is fairly easy
to use and include the V library. The main thing is to
include the required -l switches to g++ to load the proper
libraries. Use:

g++ $(YOUROBJECTS) -s -e _mainCRTStartup -lV -lcomctl32 -mwindows

All the libraries needed for Windows are automatically included with the
option -mwindows (you may need to add -lcomctl32).

If you want to avoid using the Cygwin DLL, you can also add the
-no-cygwin switch, and link to the mingw32 libraries. See the Cygwin 
documentation for more info. Also check out Mumit Kahn's info at:
http://www.xraylith.wisc.edu/~khan/software/gnu-win32/mno-cygwin-howto.txt

A similar model line for using the X version of the V library is:

g++ $(YOUROBJECTS) -s -e _mainCRTStartup -L/usr/X11R6.4/lib -lV -lXaw \
  -lXmu -lXt -lXext -lX11 -lICE -lSM

If you want to add icon resources, see the example vgen.rc file in
the /v/appgen directory. You can replace the .ico file with whatever
icon you want. You then need to add a dependency in your Makefile
to compile the .rc file with windres, and include the resulting
output file on the link line. The Makefile for vgen in /v/appgen
includes an example of how to do this.


