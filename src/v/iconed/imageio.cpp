//===============================================================
// imageio.cpp - IO library to read/write various image bitmap formats
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//
//===============================================================

#include "imageio.h"
#include <string.h>
#include <ctype.h>

#define FailMsgs		// If want to use V failure messages

#ifdef FailMsgs
#include <v/vapp.h>
#include <v/vnotice.h>
#endif

//==================>>> imageIO::imageIO <<<====================
  imageIO::imageIO(int maxW, int maxH)
  {
    _r = _g = _b = _pixels = 0;
    _height = _width = _colors = _depth = 0;
    _maxH = maxH; _maxW = maxW;
  }

//==================>>> imageIO::imageIO <<<====================
  imageIO::~imageIO()
  {
    // Destructor: free memory
    ResetMemory();
  }

//==================>>> imageIO::ResetMemory() <<<====================
  void imageIO::ResetMemory()
  {
    // Free the memory that we've allocated

    if (_pixels != 0)
        delete [] _pixels;
    if (_b != 0)
        delete [] _b;
    if (_g != 0)
        delete [] _g;
    if (_r != 0)
        delete [] _r;
    _r = _g = _b = _pixels = 0;
    _height = _width = _colors = _depth = 0;
  }

//==================>>> imageIO::OptimizeColorMap() <<<====================
  void imageIO::OptimizeColorMap()
  {
    // This method will optimize the color map. It also
    // tries to put black first and white second

    int moved[256];
    long used[256];
    long lx;
    int ix;

    if (_pixels == 0 || !_r || !_g || !_b)
	return;

    long limit = _width * _height;
 
    // Find and zap duplicates
    for (ix = 0 ; ix < 256 ; ++ix)      // assume all colors used
        used[ix] = 1;

    for (ix = 0 ; ix < 255 ; ++ix)
      {
        if (used[ix])                   // might be dups
          {
            for (int idup = ix + 1 ; idup < 256 ; ++idup) // scan rest of list
              {
                if (_r[ix] == _r[idup] && _g[ix] == _g[idup] && _b[ix] == _b[idup])
                  {
                    used[idup] = 0;             // fixing now, so don't scan again
                    for (lx = 0 ; lx < limit ; ++lx)    // Scan pixel array
                      {
                        if (_pixels[lx] == idup)
                            _pixels[lx] = ix;   // change to first color
                      }
                  }
              }
          }
      }

    // Now count usage of colors
    for (ix = 0 ; ix < 256 ; ++ix)      // not used yet
      {
        used[ix] = 0;
        moved[ix] = 0;
      }

    for (lx = 0 ; lx < limit ; ++lx)
        ++used[_pixels[lx]];            // count and set usage

    for (ix = 0 ; ix < 255 ; ++ix)      // now squish
      {
        if (used[ix] == 0)              // this one not used
          {
            // Find the next one that IS used
            for (int iy = ix + 1; iy < 256 ; ++iy)      // check rest
              {
                if (used[iy])           // one that IS used
                  {
		    // Swap colors
		    unsigned char tmp;
		    tmp = _r[iy]; _r[iy] = _r[ix]; _r[ix] = tmp;
		    tmp = _g[iy]; _g[iy] = _g[ix]; _g[ix] = tmp;
		    tmp = _b[iy]; _b[iy] = _b[ix]; _b[ix] = tmp;

                    used[ix] = used[iy];  // change
                    used[iy] = 0;
		    for (lx = 0 ; lx < limit ; ++lx)   // fixup pixels
                      {
                        if (_pixels[lx] == iy)
                            _pixels[lx] = ix;   // shift down
                      }
                    break;      // break to look for next unused color
                  }
              }
          }
      }

    // Reset how many colors are used
    for (ix = 0 ; ix < 256 ; ++ix)
      {
        if (!used[ix])
            break;
      }
    _colors = ix;

    if (_colors < 2)		// MUST have 2 colors!
      {
	_colors = 2;
	if (_r[0] == 0)
	  {
	    _r[1] = 255; _g[1] = 255; _b[1] = 255;
	  }
	else
	  {
	    _r[1] = 0; _g[1] = 0; _b[1] = 0;
	  }
      }

    // Finally, put black as 0 and white as 1
    for (ix = 0 ; ix < _colors ; ++ix) 
      {
        if (_r[ix] == 0 && _g[ix] == 0 && _b[ix] == 0)
          {
            // swap colors
            unsigned char tmp;
            tmp = _r[0]; _r[0] = _r[ix]; _r[ix] = tmp;
            tmp = _g[0]; _g[0] = _g[ix]; _g[ix] = tmp;
            tmp = _b[0]; _b[0] = _b[ix]; _b[ix] = tmp;
            for (lx = 0 ; lx < limit ; ++lx)
              {
                if (_pixels[lx] == 0)
                    _pixels[lx] = ix;
                else if (_pixels[lx] == ix)
                    _pixels[lx] = 0;
              }
            break;                      // all done
          }
      }

    for (ix = 0 ; ix < _colors ; ++ix)  // don't move if already 1
      {
        if (_r[ix] == 255 && _g[ix] == 255 && _b[ix] == 255)
          {
            // swap colors
            unsigned char tmp;
            tmp = _r[1]; _r[1] = _r[ix]; _r[ix] = tmp;
            tmp = _g[1]; _g[1] = _g[ix]; _g[ix] = tmp;
            tmp = _b[1]; _b[1] = _b[ix]; _b[ix] = tmp;
            for (lx = 0 ; lx < limit ; ++lx)
              {
                if (_pixels[lx] == 1)
                    _pixels[lx] = ix;
                else if (_pixels[lx] == ix)
                    _pixels[lx] = 1;
              }
            break;                      // all done
          }
      }
  }

//==================>>> imageIO::CreatePixels <<<====================
  unsigned char* imageIO::CreatePixels(long size)
  {
    if (_pixels != 0)
        delete [] _pixels;
    _pixels = new unsigned char[size];
    return _pixels;
  }

//==================>>> imageIO::CreateColorMap <<<====================
  void imageIO::CreateColorMap(int size)
  {
    // Create color map entries of given size
    if (_r != 0)
        delete [] _r;
    if (_g == 0)
        delete [] _g;
    if (_b == 0)
        delete [] _b;

    _r = new unsigned char[size];
    _g = new unsigned char[size];
    _b = new unsigned char[size];

    for (int ix = 0 ; ix < size ; ++ix)
      {
        _r[ix] = _g[ix] = _b[ix] = 0;
      }
  }

// The following methods are used to read various elements
// of files. They are encapsulated so that the can be
// replaced if needed with more explicit reading.

//==================>>> imageIO::RdLine <<<====================
  int imageIO::RdLine(char* buff, ifstream& ifs)
  {
    // Read rest of current input line
    ifs.getline(buff, 255);
    return 1;
  }

//==================>>> imageIO::RdStr <<<====================
  int imageIO::RdStr(char* buff, ifstream& ifs)
  {
    // Read a single char token
    ifs >> buff;
    return 1;
  }

//==================>>> imageIO::RdToEOL <<<====================
  int imageIO::RdToEOL(char* buff, ifstream& ifs)
  {
    // Read remainder of line
    ifs.getline(buff, 255);
    return 1;
  }

//==================>>> imageIO::RdIntComma <<<====================
  int imageIO::RdIntComma(ifstream& ifs)
  {
    // Read an int plus trailing comma
    int val;
    char comma;
    ifs >> val >> comma;
    return val;
  }

//==================>>> imageIO::RdInt <<<====================
  int imageIO::RdInt(ifstream& ifs)
  {
    // Read an int
    int val;
    ifs >> val;
    return val;
  }

//==================>>> imageIO::ReadVBM <<<====================
  int imageIO::ReadVBM(char* name)
  {
    // reads in a VBM file and parses
    //-------------------- VBM file format for vbm8
    //  //vbm8
    //  #define select_depth 8
    //  #define select_height 36
    //  #define select_width 45
    //  unsigned char select_bits[] = {
    //  7,
    //  255,255,255,
    //  192,192,192,
    //  64,0,255,
    //  0,0,0,
    //  128,128,128,
    //  0,0,128,
    //  255,0,128,
    //  81,81,251,// begin bitmap
    //  1,2, ... (pixels follow) ... };

    char buff[256];

    ifstream inFile(name);              // open the file to read

    if (!inFile)
        return 0;              // OK?

    RdLine(buff,inFile);        // Read header line
    char bits = buff[5];        // fetch bits position
    buff[5] = 0;                // zap for strcmp
    if (strcmp(buff,"//vbm") != 0) // valid header?
      {
NOT_VBM:
#ifdef FailMsgs
	vNoticeDialog note(theApp);
	note.Notice("Unable to read: Not a valid VBM file.");
#endif
        inFile.close();
        return 0;
      }

    if (bits == '1')            // check for valid depth
        _depth = 1;
    else if (bits == '2')
        _depth = 24;
    else if (bits == '8')
        _depth = 8;
    else
      {
	goto NOT_VBM;
      }

    for (int wh = 0 ; wh < 2 ; ++wh)
      {
GetWH:
	RdStr(buff,inFile);         // #define
	if (strcmp(buff,"#define") != 0)
	    goto NOT_VBM;

	RdStr(buff,inFile);         // xxx_height or xxx_width
	if (strstr(buff,"width") == 0)
	  {
	    if (strstr(buff,"height") == 0)
	      {
		// An upward compatible way to get depth
		if (strstr(buff,"depth") == 0)
		    goto NOT_VBM;
		else
		  {
		    _depth = RdInt(inFile);	// And the height
		    RdToEOL(buff, inFile);
		    goto GetWH;
		  }
	      }
	    else
	      {
		_height = RdInt(inFile);	// And the height
	      }
	  }
	else
	  {
	    _width = RdInt(inFile);		// And the width
	  }
	RdToEOL(buff,inFile);		// Finish line
      }
    
    if (_width > _maxW || _height > _maxH)
      {
#ifdef FailMsgs
	vNoticeDialog note(theApp);
	note.Notice("Sorry, icon exceeds maxmimum sizes allowed.");
#endif
	inFile.close();
	return 0;
      }

    RdLine(buff,inFile);        // the static unsigned line

    if (_depth == 8)            // read 8 bit mapped colors
      {
        CreateColorMap(256);

        _colors = RdIntComma(inFile) + 1;  // The number of colors

        // read all but last color in color map
	int cx;
        for (cx = 0 ; cx < (_colors - 1) ; ++cx)
          {
            _r[cx] = RdIntComma(inFile);
            _g[cx] = RdIntComma(inFile);
            _b[cx] = RdIntComma(inFile);
          }
        _r[cx] = RdIntComma(inFile);    // finish color map
        _g[cx] = RdIntComma(inFile);
        _b[cx] = RdInt(inFile); // last color followed by comment
        RdToEOL(buff,inFile);   // Finish line

        // now ready to read pixels
        long cells = _width * _height;

        if (_pixels != 0)               // make sure not read already
            delete [] _pixels;

        _pixels = new unsigned char [cells]; // allocate memory

	long ix;
        for (ix = 0 ; ix < (cells - 1) ; ++ix)
            _pixels[ix] = RdIntComma(inFile);
        _pixels[ix] = RdInt(inFile);
        inFile.close();                     // finished with file
        return 1;
      }
    else if (_depth == 1)
      {
	int res = RdRestOfXBM(inFile);
        inFile.close();
        return res;
      }
    else if (_depth == 24)
      {
#ifdef FailMsgs
	vNoticeDialog note(theApp);
	note.Notice("Sorry, can't read 24-bit VBM files.");
#endif
        inFile.close();
        return 0;
      }
    goto NOT_VBM;
  }

//==================>>> imageIO::WriteVBM <<<====================
  int imageIO::WriteVBM(char* fname, char* iname)
  {
    // Write out the image save in the data structure

    if (!fname || !*fname)
        return 0;               // must supply name

    if (_pixels == 0 || !_r || !_g || !_b)
        return 0;

    ofstream ofs(fname);        // open the file

    if (!ofs)
        return 0;               // check that was opened

    if (_depth == 1 || 
	(_colors == 2 && 
	   (((_r[0] == 0 && _g[0] == 0 && _b[0] == 0) &&
	     (_r[1] == 255 && _g[1] == 255 && _b[1] == 255)) ||
	    ((_r[1] == 0 && _g[1] == 0 && _b[1] == 0) &&
	     (_r[0] == 255 && _g[0] == 255 && _b[0] == 255)))))
      {
        ofs << "//vbm1\n";
	ofs << "#define " << iname << "_depth 1\n";
	int res = WriteRestOfXBM(ofs, iname, 1);
	ofs.close();
	return res;
      }
    else if (_depth == 8)
      {
        ofs << "//vbm8\n";
	ofs << "#define " << iname << "_depth 8\n";
        ofs << "#define " << iname << "_width " << _width << "\n";
        ofs << "#define " << iname << "_height " << _height << "\n";
        ofs << "static unsigned char " << iname << "_bits[] = {\n";
        ofs << (_colors - 1) << ",\n";
        for (int ic = 0 ; ic < _colors ; ++ic)
          {
            ofs << (unsigned int)_r[ic] << "," << (unsigned int) _g[ic] <<
                 "," << (unsigned int) _b[ic] << ",\n";
          }
        if (_width < 32)                // neat format if narrow
          {
            long px = 0;
            for (int ir = 0 ; ir < _height ; ++ir)
              {
                for (int ic = 0 ; ic < _width ; ++ic)
                  {
                    if (ic < (_width - 1))
                        ofs << (unsigned int)_pixels[px++] << ",";
                    else
                        ofs << (unsigned int)_pixels[px++];
                  }
                if ( ir < (_height - 1))
                    ofs << ",\n";
                else
                    ofs << "\n};\n";
              }
          }
        else
          {
            long limit = _width * _height;
	    long lx;
            for (lx = 0 ; lx < (limit - 1) ; )
              {
                ofs << (unsigned int)_pixels[lx++] << ",";
                if ((lx % 32) == 0)
                    ofs << "\n";
              }
            ofs << (unsigned int)_pixels[lx] << "\n};\n";
          }
        ofs.close();
        return 1;
      }
    else if (_depth == 24)
      {
      }
    ofs.close();
    return 0;
  }

//==================>>> imageIO::ReadXBM <<<====================
  int imageIO::ReadXBM(char* name)
  {
    char buff[256];

    ifstream inFile(name);              // open the file to read

    if (!inFile)
        return 0;		// OK?

    for (int wh = 0 ; wh < 2 ; ++wh)
      {
	RdStr(buff,inFile);         // #define
	if (strcmp(buff,"#define") != 0)
	  {
NOT_XBM:
#ifdef FailMsgs
	    vNoticeDialog note(theApp);
	    note.Notice("Not a valid XBM file.");
#endif
	    inFile.close();
	    return 0;
	  }

	RdStr(buff,inFile);         // xxx_height or xxx_width
	if (strstr(buff,"width") == 0)
	  {
	    if (strstr(buff,"height") == 0)
		goto NOT_XBM;
	    else
	      {
		_height = RdInt(inFile);	// And the height
	      }
	  }
	else
	  {
	    _width = RdInt(inFile);		// And the width
	  }
	RdToEOL(buff,inFile);		// Finish line
      }

    if (_width > _maxW || _height > _maxH)
      {
#ifdef FailMsgs
	vNoticeDialog note(theApp);
	note.Notice("Sorry, icon exceeds maxmimum sizes allowed.");
#endif
	inFile.close();
	return 0;
      }

    int res = RdRestOfXBM(inFile);
    inFile.close();
    return res;
  }

//==================>>> imageIO::RdRestOfXBM <<<====================
  int imageIO::RdRestOfXBM(ifstream& ifs)
  {
    int i, j, k, bit;
    char hex[4];

    long cells = _width * _height;

    _depth = 1;
    _colors = 2;
    CreateColorMap(2);
    _r[0] = _g[0] = _b[0] = 0;		// a b/w colormap
    _r[1] = _g[1] = _b[1] = 255;

    CreatePixels(cells);

    unsigned char c, c1;

    // scan forward until we see the first '0x'

    ifs.get(c); ifs.get(c1);
    while (!ifs.eof() && !(c=='0' && c1=='x') )
      { 
	c = c1;  ifs.get(c1);
      }

    if (ifs.eof())
	return 0;

    // simple checks
    if (_width < 1 || _height < 1 || _width > 10000 || _height > 10000) 
	return 0;

    k = 0;

    // read/convert the image data

    long pix = 0;
    for (i = 0 ; i < _height; i++)
      {
	for (j = 0, bit = 0 ; j < _width ; j++, bit = (++bit)&7)
	  {

	    if (!bit)
	      {
		// get next byte from file.  we're already positioned at it
		ifs.get(c); ifs.get(c1);
		if (ifs.eof())
		    return 0;

		hex[0] = c; hex[1] = c1 ; hex[2] = 0;
		k = Hex_to_Bin_Convert(hex);

		// advance to next '0x'
		if (c != '}')
		  {
		    if (!ifs.eof())
			ifs.get(c);
		    if (!ifs.eof())
			ifs.get(c1);
		    while (!ifs.eof() && !(c=='0' && c1=='x') && c != '}' )
		      { 
			c = c1;  ifs.get(c1);
		      }
		  }
	      }

	    _pixels[pix++] = (unsigned char)((k&1) ? 0 : 1);
	    k = k >> 1;
	  }
      }
    return 1;
  }

//==================>>> imageIO::WriteXBM <<<====================
  int imageIO::WriteXBM(char* fname, char* iname)
  {
    // Write out the image save in the data structure

    if (!fname || !*fname)
        return 0;               // must supply name

    if (_pixels == 0 || !_r || !_g || !_b)
        return 0;

    ofstream ofs(fname);        // open the file

    if (!ofs)
        return 0;               // check that was opened

    int res = WriteRestOfXBM(ofs, iname);
    ofs.close();
    return res;
  }

//==================>>> imageIO::WriteRestOfXBM <<<====================
  int imageIO::WriteRestOfXBM(ofstream& ofs, char* iname, int isVBM)
  {

    int  i,j,k,bit,len,nbytes,flipbw;
    long pix;
    char buff[20];

    ofs << "#define " << iname << "_width " << _width << endl;
    ofs << "#define " << iname << "_height " << _height << endl;
    if (isVBM)
	ofs << "static unsigned char " << iname << "_bits[] = {" << endl;
    else
	ofs << "static char " << iname << "_bits[] = {" << endl;

    ofs << " ";

    // set flipbw if color#0 is black
    flipbw = (_r[0] == 0);	// 0 is Black

    nbytes = _height * ((_width+7)/8);   // # of bytes to write

    for (i=0, len=1, pix = 0 ; i < _height ; i++)
      {
	for (j = bit = k = 0 ; j < _width ; j++)
	  {
	    k = (k>>1);
	    if (_pixels[pix++])
		k |= 0x80;
	    bit++;
	    if (bit == 8)
	      {
		if (flipbw)
		    k = ~k;
		ByteToStr(k&0xff,buff);
		ofs << "0x" << (char)tolower(buff[0]) << (char) tolower(buff[1]) ;
		nbytes--;  len += 4;
		if (nbytes)
		  {
		    ofs << ","; len++;
		  }
		if (len > 72) 
		  { 
		    ofs << endl << " ";
		    len = 1;
		  }
		bit = k = 0;
	      }
	  }

	if (bit)
	  {
	    k = k >> (8-bit);
	    if (flipbw) 
		k = ~k;
	    ByteToStr(k&0xff,buff);
	    ofs << "0x" << (char)tolower(buff[0]) << (char) tolower(buff[1]) ;
	    nbytes--;  len += 4;
	    if (nbytes) 
	      {
		ofs << ",";  len++;  
	      }
	    if (len>72)
	      {
		ofs << endl << " "; len = 1;
	      }
	  }
      }

    ofs << "};" << endl;
    return 1;
  }

//==================>>> imageIO::ReadXPM <<<====================
  int imageIO::ReadXPM(char* name)
  {
    char buff[256];
    char ch;

    ifstream inFile(name);              // open the file to read

    if (!inFile)
        return 0;              // OK?

    RdLine(buff,inFile);        // Read header line

    if (strstr(buff,"XPM") == 0)	// Must be XPM in first line
	return 0;

    for ( ; ; )
      {
	if (!inFile.get(ch))		// read a char
	    break;
	if (ch == '"')
	    break;
      }

    // now ready to read size and colors
    int levels;
    char insets[256];
    char r[3], g[3], b[3];
    char pix;
    int x,y;

    inFile >> _width >> _height >> _colors >> levels;
    if (levels != 1)
      {
NOT_XPM:
	inFile.close();
	return 0;		// can't read anything else
      }

    if (_width > _maxW || _height > _maxH)
      {
#ifdef FailMsgs
	vNoticeDialog note(theApp);
	note.Notice("Sorry, icon exceeds maxmimum sizes allowed.");
#endif
	inFile.close();
	return 0;
      }

    levels = 8;

    inFile.getline(buff, 255);    // clear rest of line

    CreateColorMap(256);	// allocate space for rgb

    // read in the color lists and convert from quad hex (only 2 per quad
    // are needed to get color, other two are repeats in 8 bit mode) to
    // 0-255 value for color palette.  Place in front of palette.


    for (x = 0; x < _colors ; x++)
      {
skipIt:
	inFile.getline(buff, 255);
	if (buff[0] != '"')		// skip possible embedded comments
	    goto skipIt;
	pix = buff[1];
	insets[x] = pix;
	int cx;
	for (cx = 2 ; cx < 256 ; ++cx)
	    if (buff[cx] == '#')
		break;
	++cx;			// point to first char after #
	if (cx >= 256)
	    continue;

	if (buff[cx+6] == '"')	// short form hex values
	  {
	    r[0] = buff[cx];  r[1] = buff[cx+1];  r[2] = 0;
	    g[0] = buff[cx+2];  g[1] = buff[cx+3];  g[2] = 0;
	    b[0] = buff[cx+4];  b[1] = buff[cx+5];  b[2] = 0;
	  }
	else			// long form of hex colors
	  {
	    r[0] = buff[cx];  r[1] = buff[cx+1];  r[2] = 0;
	    g[0] = buff[cx+4];  g[1] = buff[cx+5];  g[2] = 0;
	    b[0] = buff[cx+8];  b[1] = buff[cx+9];  b[2] = 0;
	  }
	_r[x] = Hex_to_Bin_Convert(r);
	_g[x] = Hex_to_Bin_Convert(g);
	_b[x] = Hex_to_Bin_Convert(b);
      }                     

    insets[_colors] = 0;

    // now ready to read pixels
    long cells = _width * _height;

    if (_pixels != 0)               // make sure not read already
	delete [] _pixels;

    _pixels = new unsigned char [cells]; // allocate memory

    long nextPix = 0;

    for (y = 0 ; y < _height ; ++y)
      {
skip_line:
	RdLine(buff,inFile);
	if (inFile.eof())
	    break;
	if (buff[0] != '"')	// check for correct line
	    goto skip_line;
	// got a row, insert int the pixel array
	for (x = 0 ; x < _width ; ++x)
	  {
	    int px = buff[x+1];
	    int ix;
	    for (ix = 0 ; ix < _colors ; ++ix)
	      {
		if (px == insets[ix])	// found it!
		    break;
	      }
	    if (ix >= _colors)
		ix = 0;
	    _pixels[nextPix++] = ix;	// fill in the index
	  }
      }
    inFile.close();
    return 1;
  }

//==================>>> imageIO::WriteXPM <<<====================
  int imageIO::WriteXPM(char* fname, char* iname)
  {
    // Write out the image save in the data structure

    int x, y;
    char insets[] = "# abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWZYZ!@$%&'()*+,-./0123456789:;<=>?@[]^_`{|}~";
    char buff[10];

    if (!fname || !*fname)
        return 0;               // must supply name

    if (_pixels == 0 || !_r || !_g || !_b)
        return 0;

    ofstream ofs(fname);        // open the file

    if (!ofs)
        return 0;               // check that was opened

    // begin with headers, 

    ofs << "/* XPM */" << endl;
    ofs << "static char *" << iname << "_xpm[] = {" << endl;
    ofs << "\"" << _width << " " << _height;
    ofs << " " << _colors << " 1\"," << endl;

    // now the color map - we will write the 16 bit form

    for (x = 0; x < _colors; x++)
    {
      ofs << "\"" << insets[x] << " c #";
      ByteToStr(_r[x],buff);
      ofs << buff << buff;
      ByteToStr(_g[x],buff);
      ofs << buff << buff;
      ByteToStr(_b[x],buff);
      ofs << buff << buff << "\"," << endl;
    }

    // and finally the pixels

    long pix = 0;
    for (y = 0; y < _height; y++)
      {
	ofs << "\"";

	for (x = 0 ; x < _width ; x++)
	  {
	    ofs << insets[_pixels[pix++]];
	  }
	if (y == (_height - 1))		// last line?
	    ofs << "\"};";
	else
	    ofs << "\",";
	ofs << endl;
      }

    // close file now

    ofs.close();                    // done with file
    return 1;
  }

//=============================>>> ByteToStr   <<<============================
  void imageIO::ByteToStr(unsigned char intg, char* str)
  {  // convert byte intg to char string in str
 
    int i;
    int d, intval, j;
    char k;
    static char digits[] = "0123456789ABCDEF";
 
    intval = intg;
    str[0] = '\0';
    i = 0;
    do
      {				// generate digits
        i++;
        d = intval % 16;	// mod 10
        str[i] = digits[d];
        intval = intval / 16;
      }
    while (intval != 0);

    for (j = 0 ; j < i ; j++ )
      {				// then reverse
        k = str[i];
        str[i--] = str[j];
        str[j] = k;
      }
    if (str[1] == 0)		// one char only
      {
	str[1] = str[0]; str[2] = 0; str[0] = '0';
      }
  }

//==================>>> myCanvasPane::Hex_to_Bin <<<===================
  int imageIO::Hex_to_Bin(char digit)
  {
    // note - need to add the tolower function to ensure hex char is lowercase

    char *hex_code = "fedcba9876543210";

    digit = tolower((int)digit);
    return strlen(strchr(hex_code, digit)) - 1;
  }

//==================>>> myCanvasPane::Hex_to_Bin_Convert <<<===========
  int imageIO::Hex_to_Bin_Convert(char * hexnumber)
  {
    // convert from hexadecimal to binary

    int return_val;

    return_val = Hex_to_Bin(hexnumber[0]);
    return_val = return_val * 16;
    return_val = return_val + Hex_to_Bin(hexnumber[1]);

    return return_val;
  }

//***************************************************************
// Defines for BMP files

#define BI_RGB  0
#define BI_RLE8 1
#define BI_RLE4 2

#define WIN 40
#define OS2 64
//==================>>> imageIO::ReadBMP <<<====================
  int imageIO::ReadBMP(char* name)
  {
    ifstream inFile(name);              // open the file to read

    if (!inFile)
        return 0;              // OK?

    // Read MSDOS, OS/2 bmp files into internal format

    unsigned char c, c1;
    int          i, rv;
    unsigned int bfSize, bfOffBits, Size, Width, Height, Planes;
    unsigned int BitCount, Compression, SizeImage, XPelsPerMeter;
    unsigned int YPelsPerMeter, ClrUsed, ClrImportant;
    int bPad;

    // First two bytes must be "BM"

    inFile.get(c);  inFile.get(c1);
    if (c !='B' || c1 != 'M')		// anything not BM fails
      {
NOT_BMP:
#ifdef FailMsgs
	vNoticeDialog note(theApp);
	note.Notice("Sorry, can't read this BMP file.");
#endif
	inFile.close();
	return 0;
      }

    bfSize = rdInt32(inFile);
    rdInt16(inFile);         		// reserved: ignore
    rdInt16(inFile);
    bfOffBits = rdInt32(inFile);

    Size = rdInt32(inFile);

    if (Size == WIN || Size == OS2)  // read header info
      {
	Width         = rdInt32(inFile);
	Height        = rdInt32(inFile);
	Planes        = rdInt16(inFile);
	BitCount      = rdInt16(inFile);
	Compression   = rdInt32(inFile);
	SizeImage     = rdInt32(inFile);
	XPelsPerMeter = rdInt32(inFile);
	YPelsPerMeter = rdInt32(inFile);
	ClrUsed       = rdInt32(inFile);
	ClrImportant  = rdInt32(inFile);
      }
    else
	goto NOT_BMP;			// old format, we won't handle!

    if (Width > _maxW || Height > _maxH)
      {
#ifdef FailMsgs
	vNoticeDialog note(theApp);
	note.Notice("Sorry, icon exceeds maxmimum sizes allowed.");
#endif
	inFile.close();
	return 0;
      }

    // Check to see if things are as they should be

    if (inFile.eof())
	goto NOT_BMP;

    if ((BitCount!=1 && BitCount!=4 && BitCount!=8 && BitCount!=24) || 
      Planes!=1 || Compression > BI_RLE4)
	goto NOT_BMP;

    if (((BitCount==1 || BitCount==24) && Compression != BI_RGB) ||
	Compression != BI_RGB)	// only uncompressed for now
	goto NOT_BMP;

    // Skip to color map
    c = Size - 40;    // 40 bytes read from Size to ClrImportant
    for (i=0; i<c; i++)
	inFile.get(c);
    
    bPad = bfOffBits - (Size + 14);	// padding after color map

    // Now, set stuff

    if (BitCount != 24)		// colormap for 1 or 4 or 8
      {
	_colors = (ClrUsed) ? ClrUsed : 1 << BitCount;
	_height = Height;
	_width = Width;
	_depth = 8;
	CreateColorMap(_colors);  // allocate space for rgb

	// read the color map
	unsigned char r,g,b;
	for (i=0; i<_colors; i++)
	  {
	    inFile.get(b);
	    inFile.get(g);
	    inFile.get(r);
	    inFile.get(c);
	    bPad -= 4;			// we just read 4 bytes
	    _r[i] = r; _g[i] = g; _b[i] = b;
	  }
      }
    else				// 24 bit bmp
      {
	goto NOT_BMP;
      }

    if (inFile.eof())		// something is amiss
	goto NOT_BMP;

    // Now, skip over any unused bytes between the color map (if there
    // was one, and the start of the bitmap data.
    
    while (bPad > 0)
      {
	inFile.get(c);
	bPad--;
      }

    // Now, read rest of file and write out an 8 or 24 bit v icon def

    long limit = (BitCount == 24) ? Width*Height*3 : Width*Height;

    unsigned char* rev = new unsigned char[limit];

    if (BitCount == 1)
	rv = rdBMP1(inFile,rev,Width,Height);
    else if (BitCount == 4)
	rv = rdBMP4(inFile,rev,Width,Height,Compression);
    else if (BitCount == 8)
	rv = rdBMP8(inFile,rev,Width,Height, Compression);
    else
	goto NOT_BMP;
//	rv = rdBMP24(inFile,rev,Width,Height);

    if (rv == 0)
	goto NOT_BMP;

    // have to reverse the bitmap's row order

    long rowLen = (BitCount == 24) ? Width*3 : Width;

    long px = 0;
    CreatePixels(limit);
    
    for (long ix = limit - rowLen ; ix >= 0 ; ix -= rowLen)
      {
	// copy one row
	for (long ri = 0 ; ri < rowLen ; ++ri)
	  {
	    _pixels[px++] = rev[ix+ri];
	  }
      }

    delete rev;

    return 1;
  }

//=========================>> rdBMP1 <<=============================
  int imageIO::rdBMP1(ifstream& fileIn, unsigned char* rev,
	unsigned int w, unsigned int h)
  {
    unsigned char c;
    int i,j,bitnum,padw;

    c = 0;
    padw = ((w + 31)/32) * 32;  /* 'w', padded to be a multiple of 32 */

    for (i=h-1; i>=0; i--)
      {
	for (j=bitnum=0; j<padw; j++,bitnum++)
	  {
	    if ((bitnum&7) == 0)
	      { /* read the next byte */
		fileIn.get(c);
		bitnum = 0;
	      }
      
	    if (j<w)
	      {
		*rev++ = (c & 0x80) ? 1 : 0;
		c <<= 1;
	      }
	  }

	if (fileIn.eof())	// something is amiss
	    return 0;			// so fail
      }

    if (fileIn.eof())	// something is amiss
	return 0;			// so fail
    return 1;
  }  

//=========================>> rdBMP4 <<=============================
  int imageIO::rdBMP4(ifstream& fileIn, unsigned char*rev,
		    unsigned int w, unsigned int h, unsigned int comp)
  {
    unsigned char c, c1;
    int   i,j,nybnum,padw,rv;

    rv = 1;
    c = c1 = 0;

    if (comp == BI_RGB)			// read uncompressed data
      {
	padw = ((w + 7)/8) * 8; 	// 'w' padded to a multiple of 8pix (32 bits)

	for (i=h-1; i>=0; i--)
	  {
	    for (j=nybnum=0; j<padw; j++,nybnum++)
	      {
		if ((nybnum & 1) == 0)	// read next byte
		  { /* read next byte */
		    fileIn.get(c);
		    nybnum = 0;
		  }

		if (j<w)
		  {
		    *rev++ = (c & 0xf0) >> 4;
		    c <<= 4;
		  }
		if (fileIn.eof())	// something is amiss
		    return 0;			// so fail
	      }
	  }
      }
    else
	return 0;

    if (fileIn.eof())	// something is amiss
	return 0;			// so fail
    return rv;
  }

//=========================>> rdBMP8 <<=============================
  int imageIO::rdBMP8(ifstream& fileIn, unsigned char* rev,
		    unsigned int w, unsigned int h, unsigned int comp)
  {
    unsigned char c;
    int   i,j,padw,rv;
 
    rv = 1;

    if (comp == BI_RGB)		// read uncompressed data
      {
	padw = ((w + 3)/4) * 4; /* 'w' padded to a multiple of 4pix (32 bits) */

	for (i=h-1; i>=0; i--) 
	  {
	    for (j=0; j<padw; j++) 
	      {
		fileIn.get(c);
		if (fileIn.eof())
		    rv = 1;
		if (j<w)
		  {
		    *rev++ = c;
		  }
		if (fileIn.eof())
		    return 0;			// so fail
	      }
	  }
      }
    else 
	return 0;

    if (fileIn.eof())
	return 0;			// so fail
    return rv;
  }

//========================>>> imageIO::putint <<<===========================
  void imageIO::putint(ofstream& fp, int i)
  {
    unsigned char c, c1, c2, c3;

    c  = ((unsigned int ) i)      & 0xff;  
    c1 = (((unsigned int) i)>>8)  & 0xff;
    c2 = (((unsigned int) i)>>16) & 0xff;
    c3 = (((unsigned int) i)>>24) & 0xff;
    fp.put(c); fp.put(c1); fp.put(c2); fp.put(c3);
  }

//========================>>> imageIO::putshort <<<===========================
  void imageIO::putshort(ofstream& fp, int i)
  {
    unsigned char c, c1;

    c  = ((unsigned int ) i)      & 0xff;  
    c1 = (((unsigned int) i)>>8)  & 0xff;
    fp.put(c); fp.put(c1);
  }

//========================>>> imageIO::WriteBMP <<<===========================
  int imageIO::WriteBMP(char* fname, char* iname)
  {
    int i, j, nbits, bperlin, padw;
    unsigned char *pp;

    if (!fname || !*fname)
        return 0;		// must supply name

    if (_pixels == 0 || !_r || !_g || !_b)
        return 0;

    ofstream fp(fname);	// open the file

    if (!fp)
        return 0;	// check that was opened 

    nbits = 8;

    bperlin = ((_width * nbits + 31) / 32) * 4;   /* # bytes written per line */

    fp << "BM";			// BMP file id

    /* compute filesize and write it */
    i = 14 +			/* size of bitmap file header */
	40 +			/* size of bitmap info header */
	(_colors * 4) +		/* size of colormap */
	bperlin * _height;	/* size of image data */

    putint(fp, i);
    putshort(fp, 0);		/* reserved1 */
    putshort(fp, 0);		/* reserved2 */
    putint(fp, 14 + 40 + (_colors * 4));  /* offset from BOfile to BObitmap */

    putint(fp, 40);		/* biSize: size of bitmap info header */
    putint(fp, _width);		/* biWidth */
    putint(fp, _height);	/* biHeight */
    putshort(fp, 1);		/* biPlanes:  must be '1' */
    putshort(fp, nbits);	/* biBitCount: 1,4,8, or 24 */
    putint(fp, BI_RGB);		/* biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 */
    putint(fp, bperlin*_height); /* biSizeImage:  size of raw image data */
    putint(fp, 75 * 39);	/* biXPelsPerMeter: (75dpi * 39" per meter) */
    putint(fp, 75 * 39);	/* biYPelsPerMeter: (75dpi * 39" per meter) */
    putint(fp, _colors);	/* biClrUsed: # of colors used in cmap */
    putint(fp, _colors);	/* biClrImportant: same as above */

    /* write out the colormap */
    for (i=0; i<_colors; i++) 
      {
	fp.put(_b[i]);
	fp.put(_g[i]);
	fp.put(_r[i]);
	fp.put((unsigned char)0);
      }

    // now write out the image

    padw = ((_width + 3)/4) * 4; /* 'w' padded to a multiple of 4pix (32 bits) */

    for ( i = _height - 1 ; i >= 0 ; i--)
      {
	pp = _pixels + (i * _width);

	for (j = 0 ; j < _width ; j++)
	    fp.put(*pp++);
	for ( ; j < padw ; j++) 
	    fp.put((unsigned char)0);
      }

    fp.close();
    return 1;
  }

//=============================>>> rdInt16 <<<=============================
  unsigned int imageIO::rdInt16(ifstream& fileIn)
  {
    unsigned char c, c1;
    fileIn.get(c);  fileIn.get(c1);
    return ((unsigned int) c) + (((unsigned int) c1) << 8);
  }

//=============================>>> rdInt32 <<<=============================
  unsigned int imageIO::rdInt32(ifstream& fileIn)
  {
    unsigned char c, c1, c2, c3;
    fileIn.get(c);  fileIn.get(c1);
    fileIn.get(c2);  fileIn.get(c3);
    return ((unsigned int) c) +
         (((unsigned int) c1) << 8) + 
	 (((unsigned int) c2) << 16) +
	 (((unsigned int) c3) << 24);
  }
