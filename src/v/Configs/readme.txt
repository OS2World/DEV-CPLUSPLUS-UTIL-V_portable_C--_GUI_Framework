This folder contains various Config.mk files that should
be copied to the /V root development directory depending on
which system you are compiling for.

Summary of the files:

ConfigM.mk - the LessTif version for X
ConfigX.mk - the Athena version for X on Unix/Linux with V 3D widgets
ConfigXno3d.mk - the Athena version for X on Unix/Linux using default lib
CfgMingw.mk - the configuration for mingw32 on MS-Windows
CfgCygW.mk - the configuration for Windows cygnus gnu32 on MS-Windows
CfgCygX.mk - the configuration for X version cygnus gnu32 on MS-Windows

In general, you copy one of the above files to Config.mk on /v.
Edit that file as needed to point to the correct directories on
your system. Then, you can build V.

On Unix/Linux:

From /V, you may simply run 'make', and the V library and utilities
will be build.

On MS-Windows, both under the Cygnus shell or a standard DOS command
window, you must 


