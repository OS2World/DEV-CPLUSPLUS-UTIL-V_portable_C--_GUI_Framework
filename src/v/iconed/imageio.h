//===============================================================
// imageio.h - IO library to read/write various image bitmap formats
//
// Copyright (C) 1996 Philip Eckenroth, Mike Tipping, Marilee Padilla,
//                    John Fredric Jr. Masciantoni, and Bruce E. Wampler.
//
// This file is part of the V Icon Editor, and is covered
// under the terms of the GNU General Public License,
// Version 2. This program has NO WARRANTY. See the source file
// viedapp.cpp for more complete information about license terms.
//===============================================================

#ifndef IMAGEIO_H
#define IMAGEIO_H
#include <fstream.h>

  class imageIO
  {
    public:     //-----------------------------------------------
	imageIO(int maxW = 150, int maxH = 150);
	~imageIO();

	int ReadVBM(char* name);
	int WriteVBM(char* fname, char* iname = "icon");

	int ReadBMP(char* name);
	int WriteBMP(char* fname, char* iname);

	int ReadXBM(char* name);
	int ReadXPM(char* name);
	int WriteXPM(char* fname, char* iname);
	int WriteXBM(char* fname, char* iname);

	int Height() { return _height; }
	int Width() { return _width; }
	int Depth() { return _depth; }
	int Colors() { return _colors; }

	void SetHeight(int h) { _height = h; }
	void SetWidth(int w) { _width = w; }
	void SetDepth(int d) { _depth = d; }
	void SetColors(int c) { _colors = c; }

	unsigned char* Pixels() { return _pixels; }
	unsigned char* RedMap() { return _r; }
	unsigned char* GreenMap() { return _g; }
	unsigned char* BlueMap() { return _b; }

  	unsigned char* CreatePixels(long size);
 	void CreateColorMap(int size);

 	void OptimizeColorMap();
	void ResetMemory();

    protected:  //-----------------------------------------------

    private:    //-----------------------------------------------
	int RdLine(char* buff, ifstream& ifs);
	int RdStr(char* buff, ifstream& ifs);
	int RdIntComma(ifstream& ifs);
	int RdInt(ifstream& ifs);
	int RdToEOL(char* buff, ifstream& ifs);
	int RdRestOfXBM(ifstream& ifs);
	int WriteRestOfXBM(ofstream& ofs, char* iname, int isVBM = 0);

	void ByteToStr(unsigned char intg, char* str);
	int Hex_to_Bin(char digit);
	int Hex_to_Bin_Convert(char * hexnumber);

	int rdBMP1(ifstream& fileIn, unsigned char* rev,
		unsigned int w, unsigned int h);
	int rdBMP4(ifstream& fileIn, unsigned char*rev,
		unsigned int w, unsigned int h, unsigned int comp);
	int rdBMP8(ifstream& fileIn, unsigned char* rev,
		unsigned int w, unsigned int h, unsigned int comp);
	unsigned int rdInt16(ifstream& fileIn);
	unsigned int rdInt32(ifstream& fileIn);
	void putint(ofstream& fp, int i);
	void putshort(ofstream& fp, int i);

	int _maxH, _maxW;
	int _height, _width;
	int _colors;
	int _depth;

	unsigned char* _pixels;

	unsigned char* _r;
	unsigned char* _g;
	unsigned char* _b;
  };

#endif
