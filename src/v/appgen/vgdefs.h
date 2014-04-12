//=======================================================================
//	vgdefs.h:	Defs for vg app
//=======================================================================
#ifndef VGDEFS_H
#define VGDEFS_H
// Define where V lives on your system

#define VGEN_VERS "1.07"
#define DEF_VPATH "/include"	// to find v_defs.h, etc. for compile, link

#ifdef GNUWIN32
#define DEF_HOMEV "C:/mingw32" // to find libV.a
#define DEF_LIBS "-lV -lcomctl32 -mwindows"
#define OGLDEF_LIBS "-lV -lVgl -lglu32 -lopengl32 -lcomctl32 -mwindows"

#else

#ifdef V_VersionWindows

#define DEF_HOMEV "C:/mingw32" // to find libV.a
#define DEF_LIBS "-lV -lcomctl32 -mwindows"
#define OGLDEF_LIBS "-lV -lVgl -lglu32 -lopengl32 -lcomctl32 -mwindows"

#else			// X version

#ifdef LINUX
#define DEF_HOMEV "/usr" // to find libV.a
#else
#define DEF_HOMEV "/usr/local/v" // to find libV.a
#endif

#ifndef Athena
#define DEF_LIBS "-lV -lXm -lXmu -lXt -lXext -lX11"
#define OGLDEF_LIBS "-lV -lVglm -lGLw -lGLU -lGL -lXm -lXmu -lXt -lXext -lX11"
#else
#define DEF_LIBS "-lVx -lXaw -lXmu -lXt -lXext -lX11"
#define OGLDEF_LIBS "-lVx -lVgl -lGLw -lGLU -lGL -lXaw -lXmu -lXt -lXext -lX11"
#endif

#endif
#endif

    enum cnvType {Canvas, Text, OpenGL, NoCanvas};
    typedef struct vgOptions
      {
	    char appName[100];	// base name of app classes
	    char fileName[100]; // base name of app files
	    char title[100];    // title of app
	    char home[100];     // V home
	    int	addDialog,	// if add a dialog
		addModal, 	// if add modal dialog
		addMake, 	// if add makefile
		addVproj,	// if add project file
		extraDialog,	// if making an extra dialog
		addMenu,	// for a menuless app
		addToolBar,	// if add tool bar
		addStatus, 	// if add status bar
		addDate,	// if add date/time
		addTimer,	// if add a timer
		vScroll,	// if include v Scroll bar
		hScroll,	// if include h scroll bar
		winSDI,		// if a Windows SDI model
		fullMenu;	// if a full menu or short menu
	    cnvType canvasType;	// kind of canvas to generate

      } vgOptions;
#endif
