//===============================================================
// vmenu.h - vmenu class definitions - X11R5
//
// Copyright (C) 1995,1996  Bruce E. Wampler
//
// This file is part of the V C++ GUI Framework, and is covered
// under the terms of the GNU Library General Public License,
// Version 2. This library has NO WARRANTY. See the source file
// vapp.cxx for more complete information about license terms.
//===============================================================

#ifndef VMENU_H
#define VMENU_H

#include <v/v_defs.h>

#include <v/vpane.h>		// we are derived from a pane

//	The _menuButton array will point to static arrays of menus.
//	The user will allocate these, e.g.:
//
//	vMenu EditMenu[] =
//	  {
//	    {"Item 1", MENU_1, isSens, notChk, noKeyLbl, noKey, noSub},
//	    {"Item 2", MENU_2, isSens, notChk, noKeyLbl, noKey, noSub},
//	    {NULL}
//	  };
//
// and to define the menu bar
//
//	vMenu myMenu[] = 
//	  {
//	    {"Edit", M_File, isSens, notChk, noKeyLbl, noKey, &EditMenu[0]},
//	    {NULL}
//	  };
//
//	the _menuButton will be done dynamically. The call back
//	will pass the value of an item to a given window.

    typedef struct MenuInfo	// Implementation dependent stuff
      {
	Widget WItem;		// Widget for each menu item
	int ItemIndex;
	int SubMenuIndex;
	MenuInfo* NxtInfo;
      } MenuInfo;

    typedef struct MenuButton
      {
	VCONST char* label;		// Label for button
	vMenu* SubMenu;		// the menu items associated with this button
	MenuInfo* mInfo;	// dynamically allocated info list

	// System dependent handles

	Widget XMenuButton;	// an X widget for the button
	Widget XPopUp;		// and X widget for the popup
      } MenuButton;

    const int MAX_MENU_BUTTONS = 20;	// Up to 20 menu buttons + submenus
					// per window

    extern "C"
      {
	void CmenuCB(Widget w, XtPointer ItmPtr, XtPointer call_data);
      };

    class vWindow;
    class vCmdWindow;			// we are part of a vCmdWindow

    class vMenuPane : public vPane
      {
	friend void CmenuCB(Widget w, XtPointer ItmPtr, XtPointer call_data);

        friend class vWindow;
        friend class vCmdWindow;

      public:		//---------------------------------------- public
	// the constructors
	vMenuPane(VCONST vMenu* menu);
	vMenuPane(const vMenuPane& m);		// Copy constructor
	virtual ~vMenuPane();			// Destructor


	// non-virtual methods

	void initialize(vWindow* pWin, Widget pWidget,
		Widget drawCanvas = (Widget)0, Widget drawArea = (Widget)0);
	vMenuPane* GetMenuThis(Widget w);
	void doSubMenu(Widget parent, Widget subitem, int x, int y);
	int CheckAccel(vKey vkey, unsigned int shift, ItemVal& id) VCONST;

      protected:	//--------------------------------------- protected

	virtual int GetPaneValue(ItemVal id, int& val) VCONST;
	virtual void SetPaneValue(ItemVal id, int val, ItemSetType setType);

	virtual void SetPaneString(ItemVal id, VCONST char* str); // set menu item label

      private:		//--------------------------------------- private
	// data

	int _nextMenuButton;
	int _nextSubMenu;
	MenuButton _mb[MAX_MENU_BUTTONS]; // Up to 10 menu items per window

	// methods
	void doAddMenu(int id, Widget parent); 	// build the menu
	void menuCB(Widget w);			// callback for menu selection

	void fixLabel(VCONST char* lbl, VCONST char* key = 0, int isMainMenu = 0) VCONST;
      };

#endif
