//========================================================================
//  tcanvas.h -- header file for tutorial canvas class
//
//      Copyright 1995,1996, Bruce E. Wampler, All Rights Reserved.
//========================================================================

#ifndef TCANVAS_H
#define TCANVAS_H

#include <v/vcanvas.h>  // derive from vCanvasPane

    typedef struct point        // simple structure for points
      {
        int x; int y;
      } point;

    class tCanvasPane : public vCanvasPane
      {
      public:           //---------------------------------------- public
        tCanvasPane();
        virtual ~tCanvasPane();

        // Windows
        virtual void Clear();

        // Scrolling
        virtual void HPage(int, int);
        virtual void VPage(int, int);
        virtual void HScroll(int);
        virtual void VScroll(int);

        // Events
        virtual void MouseDown(int, int, int);
        virtual void MouseUp(int, int, int);
        virtual void MouseMove(int, int, int);
        virtual void Redraw(int, int, int, int); // Expose/redraw event
        virtual void Resize(int, int);          // Resize event

      protected:        //------------------------------------- protected

      private:          //--------------------------------------- private
        // Note that we use a leading underscore to indicate
        // private call members.
        int _mouseDown;         // track if mouse down
        int _begx; int _begy;   // starting point
        int _curx; int _cury;   // current point
        point *_pt;             // the array of points
        int _nextpt;            // where next point goes
      };
#endif
