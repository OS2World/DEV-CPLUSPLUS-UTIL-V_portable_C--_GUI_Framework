//=======================================================================
//	videcnv.cpp:	Source for vedTextEditor class
//  Copyright (C) 1995-2000  Bruce E. Wampler
//	last edit date: 01 Mar 2000 01:43:17 PM
//
//  This program is part of the V C++ GUI Framework example programs.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  (see COPYING) along with this program; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=======================================================================

#include <v/vnotice.h>
#include <v/vutil.h>
#include <ctype.h>	// for tolower
#include <v/vos.h>	// for rename
#include <v/vcolor.h>

#define VCmdWindow videCmdWindow
#include "videapp.h"
#include "videcnv.h"
#include "videcmdw.h"

    const int maxBuff = 1000;

    static const int schemes = 6;
    static ChrAttr stdColor[schemes] = { ChNormal, ChNormal, ChWhite,
					ChWhite, ChNormal, ChNormal };
    static ChrAttr keyColor[schemes] = { ChBlue, ChBlue, ChYellow,
					ChYellow, ChBlue, ChBlue };
    static ChrAttr constColor[schemes] = { ChRed, ChRed, ChRed,
					ChRed, ChRed, ChRed };
    static ChrAttr comColor[schemes] = { ChYellow | ChDimColor, ChYellow | ChDimColor, ChGreen,
					ChGreen, ChYellow | ChDimColor, ChYellow | ChDimColor };
    static ChrAttr opColor[schemes] = { ChBlue | ChDimColor, ChBlue | ChDimColor, ChCyan,
					ChCyan, ChMagenta | ChDimColor, ChBlue | ChDimColor };
    static ChrAttr dirColor[schemes] = { ChCyan | ChDimColor, ChCyan | ChDimColor, ChMagenta,
					ChYellow | ChDimColor, ChCyan | ChDimColor, ChCyan | ChDimColor };

    static vColor bgs[schemes];


//===================>>> vedTextEditor::vedTextEditor <<<====================
  vedTextEditor::vedTextEditor(VCmdWindow* parent) : 
      vTextEditor((vCmdWindow*)parent)
  {
    curColors = 0;
    bgs[0].ResetColor(255,255,255);	// Standard
    bgs[1].ResetColor(240,240,240);	// Gray
    bgs[2].ResetColor(20, 20, 120);	// Dark blue
    bgs[3].ResetColor(20, 20, 20);	// Midnight
    bgs[4].ResetColor(250, 245, 220);	// Parchment
    bgs[5].ResetColor(190, 240, 200);	// Pale Green
  }

//===================>>> vedTextEditor::~vedTextEditor <<<====================
  vedTextEditor::~vedTextEditor()
  {
    ((videApp*)theApp)->setCurCmdWin(0);	// destroying, so make it none
  }

#define isSpace(x) (x == ' ' || x == '\t')

#define isOp(x) (x=='+' || x=='-' || x=='*' || x=='/' || x=='<' || x=='>' \
	|| x=='?' || x==':' || x==';' || x=='!' || x=='%' || x=='^' || x=='&' \
        || x=='\\' || x=='|' || x == '=' || x==',' || x=='\'' || x=='\"' \
        || x==')' || x=='(' || x=='[' || x==']' || x=='{' || x=='}')

#define isFortranOp(x) (x=='+' || x=='-' || x=='*' || x=='/'  \
	|| x == '=' || x==',' || x=='\'' \
        || x==')' || x=='(' )

#define isDigit(x) (x >= '0' && x <= '9')
#define isLetter(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))

// ===================>>> vTextEditor::adjustLines <<<======================
  void vedTextEditor::adjustLines(long line_1, int by)
  {
    /* adjust starting line number int note locaetions */

    vTextEditor::adjustLines(line_1, by);	// do original

    (((videApp*)theApp)->getBreakPoints())->adjustLines(
		((VCmdWindow*)_parent)->GetFileName(),line_1, by); // bps

#ifdef FOREXAMPLE
    int ix;

    for (ix = 0 ; ix < 26 ; ++ix)		/* adjust notes */
      {
	if (noteloc[ix] > line_1)		/* page needs fixing */
	    noteloc[ix] += (long) by;
      }
#endif
  }

// ======================>>> vTextEditor::setColorScheme <<<=====================
  void vedTextEditor::setColorScheme()
  {

    curColors = ((videApp*)theApp)->getColorScheme();
    if (curColors < 0 || curColors >= schemes)
	curColors = 0;

    SetBackground(bgs[curColors]);
  }

// ======================>>> vTextEditor::paintLine <<<=====================
  void vedTextEditor::paintLine(char* linout, int lineStart,
        int hiStart, int hiLast, long lineNum)
  {
    // paint a line.
    // linout: the line to output with tabs converted to spaces, etc.
    // lineStart: where to begin printing the line (for hoiz. scrolling)
    // hiStart, hiLast: reverse text attribute
    // lineNum: the real line number in the buffer this is.
    // This version overrides the original to handle syntax highlighting


    ChrAttr attrs[MAX_LINE+1];	// for attributes
    int wasComment = 0;

    int linlen = strlen(linout);
    if (linlen <= 0)             // only draw if there!
        return;

    for (int ix = 0 ; ix <= MAX_LINE ; ++ix)	// assume normal
        attrs[ix] = stdColor[curColors];

    // Parse the line for syntax

    if (GetFileType() == CPP)			// if a C file, parse
        wasComment = parseC(linout,attrs,lineNum,CPP);
    else if (GetFileType() == Java)			// if a Java file, parse
        wasComment = parseC(linout,attrs,lineNum,Java);
    else if (GetFileType() == Perl)			// if a Perl file, parse
        wasComment = parseC(linout,attrs,lineNum,Perl);
    else if (GetFileType() == Fortran)			// if a Fortran file, parse
        wasComment = parseFortran(linout,attrs,lineNum);
    else if (GetFileType() == HTML)			// if a HTML file, parse
        wasComment = parseHTML(linout,attrs,lineNum);
    else if (GetFileType() == TeX)			// if a TeX file, parse
        wasComment = parseTeX(linout,attrs,lineNum);
    else if (GetFileType() == gccError || GetFileType() == bccError)
      {
        int ig = 0;
        while (isSpace(linout[ig]))
          ++ig;
        if (linout[ig] == '>' || linout[ig] == '+')
          {
	    if (linout[ig] == '+')
		attrs[ig++] = dirColor[curColors];
            while (linout[ig])
                attrs[ig++] = keyColor[curColors];
          }
        else if (linout[ig] == '!' || linout[ig] == '*')
          {
	    if (linout[ig] == '*')
		attrs[ig++] = dirColor[curColors];
            while (linout[ig])
                attrs[ig++] = constColor[curColors];
          }
      }

    // Now fill in highlight attributes
    for (int ih = 0 ; linout[ih] != 0 ; ++ih)
      {
        if (ih >= hiStart && ih < hiLast)
            attrs[ih] = getHighlight();		// override syntax colors
      }

    if ( ((videApp*)theApp)->isBP(((VCmdWindow*)_parent)->GetFileName(),
		lineNum))
      {
	for (int ij = 0 ; linout[ij] != 0 ; ++ij)
	  {
	    attrs[ij] = ChBlackOnBG | ChYellow;
	  }
      }
    if ( (((videApp*)theApp)->getBreakPoints())->
		checkIfCurPC(((VCmdWindow*)_parent)->GetFileName(),
		lineNum))
      {
	for (int ij = 0 ; linout[ij] != 0 ; ++ij)
	  {
	    attrs[ij] = ChBlackOnBG | ChRed;
	  }
      }

//  new - V version 1.23 - added DrawAttrsText
    DrawAttrsText((const char*)&linout[lineStart], (const ChrAttr*)&attrs[lineStart]);

//   old code - dumb, slow way to do this
//    for (int ixx = lineStart ; linout[ixx] != 0 ; ++ixx)
//	DrawChar(linout[ixx],attrs[ixx]);

  }

//===================>>> vedTextEditor::TextMouseDown <<<====================
  void vedTextEditor::TextMouseDown(int row, int col, int button)
  {
    static int clicks = 0;
    int btn = (GetFileType() == gccError || GetFileType() == bccError)
	 ? 1 : button;

    long oldLine = GetCurLine();		// remember current position
    int oldCol = getColPos();
    
    vTextEditor::TextMouseDown(row, col, btn);	// translate to left

    if (button == 1 && oldLine == GetCurLine() && oldCol == getColPos()) // double click...
      {
	++clicks;
	if (clicks > 3)
	    clicks = 1;
	setSelection(clicks);
      }
    else
      {
	clicks = 0;
      }

  }

//===================>>> vedTextEditor::TextMouseUP <<<====================
  void vedTextEditor::TextMouseUp(int row, int col, int button)
  {
    int btn = (GetFileType() == gccError || GetFileType() == bccError)
	 ? 1 : button;

    vTextEditor::TextMouseUp(row, col, btn);
    if (button == 3 && (GetFileType() == gccError
	|| GetFileType() == bccError))  // open error file
      {
        ((VCmdWindow*)_parent)->GotoErrorLine();
      }
    else if (button == 3)		// right click - ctags...
      {
	char sel[100];
	sel[0] = 0;
	getSelection(sel,99);			// retrieve selection
        ((VCmdWindow*)_parent)->lookupCtag(sel);
      }
  }

//===================>>> vedTextEditor::parseC <<<====================
  int vedTextEditor::parseC(char* linout, ChrAttr* attrs, 
	long lineNum, FileType ft)
  {
    // FileType will be either CPP or Java

    int ix = 0;
    int wasComment = 0;
    char token[MAX_LINE+1];

    for (ix = 0 ; linout[ix] != 0 ; ++ix)
        if (!isSpace(linout[ix]))
            break;			// skip leading spaces

    if (linout[ix] == '*' && ft != Perl) // probably a * comment
      {
	for (long ln = lineNum + 1 ;
	     ln < lineNum + 200 && ln <= lastLineBF() ; ++ln)
	  {
	    if (strstr((char*)GLine(ln),"//")!=0)
		break;
	    else if (strstr((char*)GLine(ln),"/*")!=0)
		break;
	    else if (strstr((char*)GLine(ln),"*/")!=0)	// is part of comment
	      {
                /* A comment...
                 * of this form, widely used in Java
                 */
                int ij;
                for (ij = ix ; linout[ij] ; ++ij)
                  {
                    attrs[ij] = comColor[curColors];
                    if (linout[ij] =='/' && linout[ij-1] == '*') // "*/"
                        break;
                  }
                return 1;
	      }
	  }

      }

    while (linout[ix] != 0)
      {
        // gather a token
        int iy;
        for (iy = ix ; linout[iy] != 0 ; ++iy)
          {
            if (isOp(linout[iy]) || isSpace(linout[iy]) )
                break;
          }
        // token goes from ix to iy
        int from = ix;
        if (ix == iy)
            ++iy;		// single char tokens

        char *tp;
        for (tp = token ; from < iy ; ++from)
          *tp++ = linout[from];
        *tp = 0;
        // OK, now highlight the token appropriately
        if (isOp(*token))
          {
            if (*token == '"')
              {
                // A String...
                int ij;
                for (ij = ix ; linout[ij] ; ++ij)
                  {
                    attrs[ij] = constColor[curColors];
                    if (linout[ij] == '\\')
                      {
		        if (linout[ij+1])
			  {
                            attrs[ij+1] = constColor[curColors];
                            ++ij;
			  }
                      }
                    else if (linout[ij] == '"' && ij != ix)
                        break;
                  }
                if (linout[ij] == 0)
		    break;
		else
		    iy = ij+1;
              }
            else if ((*token == '/' && linout[ix+1] == '/' && ft != Perl))
              {
                // A comment...
                int ij;
                for (ij = ix ; linout[ij] ; ++ij)
                    attrs[ij] = comColor[curColors];
                iy = ij;
                wasComment = 1;
              }
            else if (*token == '/' && linout[ix+1] == '*')
              {
                /* A comment... */
                int ij;
                for (ij = ix ; linout[ij] ; ++ij)
                  {
                    attrs[ij] = comColor[curColors];
                    if (linout[ij] =='/' && linout[ij-1] == '*') // "*/"
                        break;
                  }
                wasComment = 1;
                if (linout[ij] == 0)
                    break;
                else
                    iy = ij+1;
              }
            else if (*token == '*' && linout[ix+1] == '/' && linout[ix+2] != '*')
              {
                /* A comment...  with trailing * / */
                int ij;
                for (ij = 0 ; linout[ij] && ij <= ix+1 ; ++ij)
                  {
                    attrs[ij] = comColor[curColors];
                  }
                wasComment = 1;
                if (linout[ij] == 0)
                    break;
                else
                    iy = ij+1;
              }
            else
                attrs[ix] = opColor[curColors];
          }
        else if (*token == '#')
          {
	    if (ft == CPP)
	      {
		for (int ij = ix ; ij < iy ; ++ij)
		    attrs[ij] = dirColor[curColors];
	      }
	    else if (ft == Perl)
	      {
                // A Perl # comment...
                int ij;
                for (ij = ix ; linout[ij] ; ++ij)
                    attrs[ij] = comColor[curColors];
                iy = ij;
                wasComment = 1;
	      }
          }
        else if (*token >= '0' && *token <= '9')
          {
            for (int ij = ix ; ij < iy ; ++ij)
                attrs[ij] = constColor[curColors];
          }
        else if (ft == CPP && isCKeyWord(token))
          {
            for (int ij = ix ; ij < iy ; ++ij)
                attrs[ij] = keyColor[curColors];
          }
        else if (ft == Java && isJavaKeyWord(token))
          {
            for (int ij = ix ; ij < iy ; ++ij)
                attrs[ij] = keyColor[curColors];
          }
        else if (ft == Perl && isPerlKeyWord(token))
          {
            for (int ij = ix ; ij < iy ; ++ij)
                attrs[ij] = keyColor[curColors];
          }
        else if (ft == Perl && isPerlOp(token))
          {
            for (int ij = ix ; ij < iy ; ++ij)
                attrs[ij] = opColor[curColors];
          }

        ix = iy;
      }
    return wasComment;
  }

//===================>>> vedTextEditor::parseFortran <<<====================
  int vedTextEditor::parseFortran(char* linout, ChrAttr* attrs, 
	long lineNum)
  {
    // FileType will be Fortran

    int ix = 0;
    int wasComment = 0;
    char token[MAX_LINE+1];

    // First, check for comments
    if (linout[0] == 'C' || linout[0] == 'c' || linout[0] == '*')
      {
	// A comment...
	int ij;
	for (ij = 0 ; linout[ij] ; ++ij)
	    attrs[ij] = comColor[curColors];
	wasComment = 1;
	return wasComment;
      }

    for (ix = 0 ; linout[ix] != 0 ; ++ix)
        if (!isSpace(linout[ix]))
            break;			// skip leading spaces

    while (linout[ix] != 0)
      {
        // gather a token
        int iy;
	if (linout[ix] == '.')		// logical op or number
	  {
	    if (isDigit(linout[ix+1]))	// .45 type number
	      {
		for (iy = ix ; linout[iy] != 0 ; ++iy)
		  {
		    if (!isDigit(linout[iy]) && linout[iy] != '.'
			 && linout[iy] != 'e' && linout[iy] != 'E'
			 && linout[iy] != 'd' && linout[iy] != 'D')
			break;
		  }
	      }
	    else	 	// most likely a logical op
	      {
		for (iy = ix+1 ; linout[iy] != 0 ; ++iy)
		  {
		    if (isFortranOp(linout[iy]) || isSpace(linout[iy]))
			break;
		    if (linout[iy] == '.')
		      {
			++iy;
			break;
		      }
		  }
	      }
	  }
	else if (isDigit(linout[ix]))		// parse numbers separately
	  {
	    for (iy = ix ; linout[iy] != 0 ; ++iy)
	      {
		if (!isDigit(linout[iy]) && linout[iy] != '.'
			&& linout[iy] != 'e' && linout[iy] != 'E'
			&& linout[iy] != 'd' && linout[iy] != 'D')
		    break;
	      }
	  }
	else
	  {
	    for (iy = ix ; linout[iy] != 0 ; ++iy)
	      {
		if (isFortranOp(linout[iy]) || isSpace(linout[iy])
			|| linout[iy] == '.' )
		    break;
	      }
	  }

        // token goes from ix to iy
        int from = ix;
        if (ix == iy)
            ++iy;		// single char tokens

        char *tp;
        for (tp = token ; from < iy ; ++from)
          *tp++ = linout[from];
        *tp = 0;
        // OK, now highlight the token appropriately
        if (isFortranOp(*token))
          {
            if (*token == '\'')
              {
                // A String...
                int ij;
                for (ij = ix ; linout[ij] ; ++ij)
                  {
                    attrs[ij] = constColor[curColors];
                    if (linout[ij] == '\'' && ij != ix)
                        break;
                  }
                if (linout[ij] == 0)
		    break;
		else
		    iy = ij+1;
              }
            else
                attrs[ix] = opColor[curColors];
          }
        else if (*token >= '0' && *token <= '9')
          {
            for (int ij = ix ; ij < iy ; ++ij)
                attrs[ij] = constColor[curColors];
          }
        else if (isFortranKeyWord(token))
          {
            for (int ij = ix ; ij < iy ; ++ij)
                attrs[ij] = keyColor[curColors];
          }
        else if (*token == '.' && token[strlen(token)-1] == '.')
          {
	    // .gt. type operator
            for (int ij = ix ; ij < iy ; ++ij)
                attrs[ij] = opColor[curColors];
          }

        ix = iy;
      }
    return wasComment;
  }

//===================>>> vedTextEditor::parseHTML <<<====================
  int vedTextEditor::parseHTML(char* linout, ChrAttr* attrs, long lineNum)
  {
    char prevtok;		// track it here!
    int ix = 0;
    int wasComment = 0;
    char token[MAX_LINE+1];

    for (ix = 0 ; linout[ix] != 0 ; ++ix)
        if (!isSpace(linout[ix]))
            break;			// skip leading spaces
    prevtok = 0;

    while (linout[ix] != 0)
      {
        // gather a token
        int iy;
        for (iy = ix ; linout[iy] != 0 ; ++iy)
          {
            if (linout[iy] == '<' || linout[iy] == '>' ||
	      linout[iy] == '\"' || linout[iy] == '/' ||
              linout[iy] == '&' || isSpace(linout[iy]) )
                break;
          }
        // token goes from ix to iy

        int from = ix;
        if (ix == iy)
            ++iy;		// single char tokens

        char *tp;
        for (tp = token ; from < iy ; ++from)
          *tp++ = linout[from];
        *tp = 0;
        // OK, now highlight the token appropriately


        if (*token == '<' || (*token == '/' && prevtok == '<'))
          {
	    attrs[ix] = keyColor[curColors];
	    prevtok = *token;
	  }
	else if (*token == '>')
	  {
	    attrs[ix] = keyColor[curColors];
	    prevtok = '>';
	  }
        else if (*token == '&')
          {
            attrs[ix] = constColor[curColors];
            prevtok = '&';
          }
        else if (*token == '"')
	      {
		// A String...
                prevtok = *token;
		int ij;
		for (ij = ix ; linout[ij] ; ++ij)
		  {
		    attrs[ij] = constColor[curColors];
		    if (linout[ij] == '\\')
		      {
			attrs[ij+1] = constColor[curColors];
			if (linout[ij+1])
			    ++ij;
		      }
		    else if (linout[ij] == '"' && ij != ix)
			break;
		  }
		if (linout[ij])		// oops - ij could go past end!
		    iy = ij+1;
		else
		    iy = ij;
	      }
	    else if (*token >= '0' && *token <= '9')
	      {
                prevtok = *token;
		for (int ij = ix ; ij < iy ; ++ij)
		    attrs[ij] = constColor[curColors];
	      }
	    else if ((prevtok == '<' || prevtok == '/') && isHTMLKeyWord(token))
	      {
                prevtok = *token;
		for (int ij = ix ; ij < iy ; ++ij)
		    attrs[ij] = keyColor[curColors];
	      }
            else
            	prevtok = *token;

	ix = iy;
      }

    return wasComment;
  }

//===================>>> vedTextEditor::parseTeX <<<====================
  int vedTextEditor::parseTeX(char* linout, ChrAttr* attrs, long lineNum)
  {
    char prevtok;		// track it here!
    int ix = 0;
    int wasComment = 0;
    char token[MAX_LINE+1];

    for (ix = 0 ; linout[ix] != 0 ; ++ix)
        if (!isSpace(linout[ix]))
            break;			// skip leading spaces
    prevtok = 0;

    while (linout[ix] != 0)
      {
	if (linout[ix] == '%')		// comment
	  {
	    int ij;
	    for (ij = ix ; linout[ij] ; ++ij)
		attrs[ij] = comColor[curColors];
	    wasComment = 1;
	    return wasComment;
	  }

	// gather a token
        int iy;
	iy = ix;
	if (isDigit(linout[ix]))		// parse numbers separately
	  {
	    for (iy = ix ; linout[iy] != 0 ; ++iy)
	      {
		if (!isDigit(linout[iy]) && linout[iy] != '.')
		    break;
	      }
	  }
	else if (linout[ix] == '{' || linout[ix] == '}' ||
		linout[ix] == '[' || linout[ix] == ']')
	  {
	    attrs[ix] = opColor[curColors];
	  }

	else if (linout[ix] == '\\')
	  {
	    for (iy = ix+1 ; linout[iy] != 0 ; ++iy)
	      {
		if (!(isLetter(linout[iy])))
		    break;
	      }
	  }
        // token goes from ix to iy
        int from = ix;

        char *tp;
        for (tp = token ; from < iy ; ++from)
          *tp++ = linout[from];
        *tp = 0;
        if (ix == iy)
            ++iy;		// single char tokens

	if (*token >= '0' && *token <= '9')
	  {
	    for (int ij = ix ; ij < iy ; ++ij)
		attrs[ij] = constColor[curColors];
	  }
	else if (*token == '\\')
	  {
	    if (!*(token+1))	// \char
	      {
		attrs[ix] = constColor[curColors];
		attrs[ix+1] = constColor[curColors];
		if (linout[ix+1])	// eat the char
		    ++iy;
	      }
	    else
	      {
		for (int ij = ix ; ij < iy ; ++ij)
		    attrs[ij] = keyColor[curColors];
	      }
	  }

	ix = iy;
      }

    return wasComment;
  }

//===================>>> vedTextEditor::isCKeyWord <<<====================
  int vedTextEditor::isCKeyWord(char* token)
  {
    static char* keywords[] =
     {
	"FALSE", "NULL", "TRUE",
	"asm", "auto", "bool", "break", 
	"case", "catch", "char", "class", "const", "const_cast", 
	"continue", "default", "delete", "do", "double", 
	"dynamic_cast", "else", "enum", "explicit", "extern", 
	"false", "float", "for", "friend", "goto", "if", 
	"inline", "int", "long", "mutable", "namespace", "new",
	"operator", "private", "protected", "public", "register", 
	"reinterpret_cast", "return", "short", "signed", "sizeof", 
	"static", "static_cast", "struct", "switch", "template", 
	"this", "throw", "true", "try", "typedef", "typeid", 
	"typename", "union", "unsigned", "using", "virtual", "void", 
	"volatile", "wchar_t", "while", ""
     };

     for (int ix = 0 ; *keywords[ix] ; ++ix)
       {
         if (strcmp(token,keywords[ix]) == 0)
             return 1;
       }
     return 0;
  }

//===================>>> vedTextEditor::isFortranKeyWord <<<====================
  int vedTextEditor::isFortranKeyWord(char* token)
  {
    static char* keywords[] =
     {
	"assign", "backspace", "block", "call", "character",
	"close", "common", "complex", "continue", "data",
	"dimension", "do", "double", "else", "elseif",
	"end", "enddo", "endif", "endfile", "entry", "equivalence", "external",
	"file", "format", "function", "go", "goto", "if",
	"implicit", "inquire", "integer", "intrinsic",
	"logical", "open", "parameter", "pause", "precision", "print",
	"program", "read", "real", "return", "rewind",
	"save", "stop", "subroutine", "then", "to", "while", "write",
	"include", "none", "true", "false", ""
     };

     for (int ix = 0 ; *keywords[ix] ; ++ix)
       {
         if (stricmp(token,keywords[ix]) == 0)
             return 1;
       }
     return 0;
  }

//===================>>> vedTextEditor::isPerlKeyWord <<<====================
  int vedTextEditor::isPerlKeyWord(char* token)
  {
    static char* keywords[] =
    {
	"bless", "caller", "close", "continue", "do", "else", "elsif",
	"for", "foreach", "goto", "if", "import", "last", "local", "my",
	"next", "no", "open", "package", "print", "pop", "push", "redo", "require",
	"return", "tie", "split", "sub", "system", "untie", "until",
	"unless", "use", "while", ""

     };

     for (int ix = 0 ; *keywords[ix] ; ++ix)
       {
         if (strcmp(token,keywords[ix]) == 0)
             return 1;
       }
     return 0;
  }

//===================>>> vedTextEditor::isPerlOp <<<====================
  int vedTextEditor::isPerlOp(char* token)
  {
    static char* keywords[] =
    {
	"cmp", "eq", "ne", "gt", "lt", "ge", "le", ""
    };

     for (int ix = 0 ; *keywords[ix] ; ++ix)
       {
         if (strcmp(token,keywords[ix]) == 0)
             return 1;
       }
     return 0;
  }

//===================>>> vedTextEditor::isJavaKeyWord <<<====================
  int vedTextEditor::isJavaKeyWord(char* token)
  {
    static char* keywords[] =
    {
	"abstract", "boolean", "break", "byte",
	"case", "catch", "char", "class", "const",
	"continue", "default", "do", "double", 
	"else", "extends",
	"false", "final", "finally", "float", "for", "future",
	"generic", "goto", "if", 
	"implements", "import", "inner", "instanceof", "int",
	"interface", "long", "native", "new", "null",
	"operator", "outer", 
	"private", "protected", "public", "rest", 
	"return", "short",
	"static", "String", "super", "switch", "synchronized",
	"this", "throw", "throws", "true", "try",
	"var", "void", 
	"volatile", "while", ""
     };

     for (int ix = 0 ; *keywords[ix] ; ++ix)
       {
         if (strcmp(token,keywords[ix]) == 0)
             return 1;
       }
     return 0;
  }

//===================>>> vedTextEditor::isHTMLKeyWord <<<====================
  int vedTextEditor::isHTMLKeyWord(char* token)
  {
    static char* keywords[] =
     {
	"a", "address", "applet", "area", "b", "base", "basefont",
	"bdo", "bgsound", "big", "blink", "blockquote", "body",
	"br", "button", "caption", "center", "cite", "code", "col",
	"colgroup", "dd", "del", "dfn", "dir", "div", "dl",
	"dt", "em", "embed", "filedset", "font", "form", "frame",
	"frameset", "h1", "h2", "h3", "h4", "h5", "h6", "head",
	"hr", "html", "i", "iframe", "ilayer", "img", "input",
	"ins", "isindex", "kbd", "label", "layer", "legend",
	"li", "link", "listing", "map", "marquee", "menu", "meta",
	"multicol", "nobr", "noframes", "nolayer", "noscript",
	"object", "ol", "option", "p", "param", "plaintext",
	"pre", "q", "s", "samp", "script", "select", "small",
	"spacer", "span", "strike", "strong", "style", "sub",
	"sup", "table", "tbody", "td", "textarea", "tfoot",
	"th", "thead", "title", "tr", "tt", "u", "ul", "var",
	"wbr", "xmp", ""
     };
    char tryword[80];
    // char* cp;
    int tk;

    for (tk = 0 ; tk < 79 && *token ; )
	tryword[tk++] = tolower(*token++);
    tryword[tk] = 0;

    for (int ix = 0 ; *keywords[ix] ; ++ix)
       {
         if (strcmp(tryword,keywords[ix]) == 0)
             return 1;
       }
     return 0;
  }

//===================>>> vedTextEditor::ChangeLoc <<<====================
  void vedTextEditor::ChangeLoc(long line, int col)
  {
    ((VCmdWindow*)_parent)->ChangeLoc(line,col);
  }

//===================>>> vedTextEditor::ChangeInsMode <<<===================
  void vedTextEditor::ChangeInsMode(int IsInsMode, char* msg)
  {
    ((VCmdWindow*)_parent)->ChangeInsMode(IsInsMode, msg);
  }

//===================>>> vedTextEditor::StatusMessage <<<====================
  void vedTextEditor::StatusMessage(char *msg)
  {
    if (state.echof)
	((VCmdWindow*)_parent)->StatusMessage(msg);
  }

//===================>>> vedTextEditor::EnterFocus <<<====================
  void vedTextEditor::EnterFocus()
  {
    ((videApp*)theApp)->setCurCmdWin((VCmdWindow*)_parent);
  }

//===================>>> vedTextEditor::ErrorMsg <<<====================
  void vedTextEditor::ErrorMsg(char *str)
  {
    if (state.echof)
      {
	vBeep();
	((VCmdWindow*)_parent)->StatusMessage(str);
      }
  }

//===================>>> vedTextEditor::ReadFile <<<====================
  int vedTextEditor::ReadFile(char* name, int paintIt)
  {

    char buff[maxBuff+2];

    if (!name || !*name)
	return 0;

    ifstream inFile(name);

    if (!inFile)
	return 0;		// file not there

    resetBuff();		// this buffer is empty

    while (inFile.getline(buff,maxBuff))
      {
	if (!addLine(buff))
	  {
	    vNoticeDialog note(theApp);
	    note.Notice("File too big -- only paritally read.");
	    break;
	  }
      }

    inFile.close();

    if ( ((videApp*)theApp)->getBackup())
      {
	char outname[maxBuff+2];
	vOS os;
	if (strstr(name,".bak") == 0 && strlen(name) < maxBuff - 4)
	  {
	    strcpy(outname,name); strcat(outname,".bak");
	    os.vDeleteFile(outname);	// delete old .bak first
	    if (os.vRenameFile(name,outname))
		SaveFile(name);
	  }
      }

    displayBuff(1,paintIt);		// Now, display the buffer , don't paint
    return 1;
  }

//===================>>> vedTextEditor::SaveFile <<<====================
  int vedTextEditor::SaveFile(char* name)
  {
    char buff[maxBuff+2];

//    char* prefix = "Saved ";

    ofstream ofs(name);

    if (!ofs)
      {
	StatusMessage("Can't save file");
	return 0;
      }

    long lx;
    long lim = GetLines();

    for (lx = 1 ; lx <= 12 && lx <= lim ; ++lx)
      {
	getLine(buff,maxBuff,lx);

    	if (GetFileType() == CPP ||		// check date: if C++
	    GetFileType() == Java ||		// if a Java file
	    GetFileType() == Fortran ||		// if a Perl file
	    GetFileType() == Perl ||		// if a Perl file
	    GetFileType() == HTML)		// if HTML
	  {
	    char* cp = strstr(buff,"date:");
	    if (cp != 0)
	      {
		if (GetFileType() != Fortran || buff[0] == 'c' || buff[0] == 'C')
		  {
		    char time[20];
		    char date[20];
		    vGetLocalDate(date);
		    vGetLocalTime(time);
		    *(cp+5) = 0;		// 5 corresponds to "date:"
		    strcat(buff," ");
		    strcat(buff,date);
		    strcat(buff," ");
		    strcat(buff,time);
		  }
	      }
	  }
	ofs << buff << "\n";
      }

    for ( ; lx <= lim ; ++lx)
      {
	getLine(buff,maxBuff,lx);
	ofs << buff << "\n";
      }

    ofs.close();

//    removed in 1.07 - fatal interaction between this
//	message and autosave while user is entering text.
//    strcpy(buff,prefix);
//    int ix;
//    for (ix = strlen(prefix) ; ix < 40 && *name != 0 ; ++ix)
//	buff[ix] = *name++;
//    buff[ix] = 0;
//    StatusMessage(buff);

    state.changes = 0;
    return 1;
  }
