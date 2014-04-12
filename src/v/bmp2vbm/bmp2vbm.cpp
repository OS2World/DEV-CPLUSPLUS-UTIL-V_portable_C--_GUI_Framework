#include <stdio.h>

#define BI_RGB  0
#define BI_RLE8 1
#define BI_RLE4 2

#define WIN 40
#define OS2 64

  int bmp2vbm(FILE* fileIn, FILE* fileOut, char* vbmName);
  static unsigned int rdInt16(FILE *fileIn);
  static unsigned int rdInt32(FILE *fileIn);
  static int rdBMP1(FILE* fileIn, unsigned char* rev,
		    unsigned int w, unsigned int h);
  static int rdBMP4(FILE* fileIn, unsigned char* rev,
		    unsigned int w, unsigned int h, unsigned int comp);
  static int rdBMP8(FILE* fileIn, unsigned char* rev,
		    unsigned int w, unsigned int h, unsigned comp);
  static int rdBMP24(FILE* fileIn, unsigned char* rev,
		     unsigned int w, unsigned int h);
  static unsigned int rdInt16(FILE *fileIn);
  static unsigned int rdInt32(FILE *fileIn);

// ==========================>>> main <<<==========================
  int main(int argc, char** argv)
  {
    // simple driver for bmp2vbm

    FILE* fin;
    FILE* fout;

    if (argc != 4)
      {
	fprintf(stderr,"Usage: b2v in.bmp out.vbm iconname\n");
        return 0;
      }
    if (!(fin = fopen(argv[1],"rb")))
      {
	fprintf(stderr,"Input .bmp file %s not found.\n",argv[1]);
        return 0;
      }
    if (!(fout = fopen(argv[2],"w")))
      {
	fprintf(stderr,"Can't create output .vbm file %s.\n",argv[2]);
	fclose(fin);
        return 0;
      }


    int res = bmp2vbm(fin, fout, argv[3]);
    if (!res)
	fprintf(stderr,"Conversion failed\n");
    fclose(fin);
    fclose(fout);

    return 0;
  }

// ==========================>>> bmp2vbm <<<==========================
  int bmp2vbm(FILE* fileIn, FILE* fileOut, char* vbmName)
  {
    // Convert MSDOS, OS/2 bmp file to a V .vbm file

    int          i, c, c1, rv;
    unsigned int bfSize, bfOffBits, Size, Width, Height, Planes;
    unsigned int BitCount, Compression, SizeImage, XPelsPerMeter;
    unsigned int YPelsPerMeter, ClrUsed, ClrImportant;
    int bPad;

    // First two bytes must be "BM"

    c = getc(fileIn);  c1 = getc(fileIn);
    if (c !='B' || c1 != 'M')		// anything not BM fails
	return 0;

    bfSize = rdInt32(fileIn);
    rdInt16(fileIn);         		// reserved: ignore
    rdInt16(fileIn);
    bfOffBits = rdInt32(fileIn);

    Size = rdInt32(fileIn);

    if (Size == WIN || Size == OS2)  // read header info
      {
	Width         = rdInt32(fileIn);
	Height        = rdInt32(fileIn);
	Planes        = rdInt16(fileIn);
	BitCount      = rdInt16(fileIn);
	Compression   = rdInt32(fileIn);
	SizeImage     = rdInt32(fileIn);
	XPelsPerMeter = rdInt32(fileIn);
	YPelsPerMeter = rdInt32(fileIn);
	ClrUsed       = rdInt32(fileIn);
	ClrImportant  = rdInt32(fileIn);
      }
    else
	return 0;			// old format, we won't handle!

    // Check to see if things are as they should be

    if (ferror(fileIn) || feof(fileIn))	// something is amis
	return 0;			// so fail

    if ((BitCount!=1 && BitCount!=4 && BitCount!=8 && BitCount!=24) || 
      Planes!=1 || Compression > BI_RLE4)
	return 0;

    if (((BitCount==1 || BitCount==24) && Compression != BI_RGB) ||
	Compression != BI_RGB)	// only uncompressed for now
//         (BitCount==4 && Compression==BI_RLE8) ||
//         (BitCount==8 && Compression==BI_RLE4))
	return 0;

    // Skip to color map
    c = Size - 40;    // 40 bytes read from Size to ClrImportant
    for (i=0; i<c; i++)
	getc(fileIn);
    
    bPad = bfOffBits - (Size + 14);	// padding after color map

    // Now, write the header

    if (BitCount != 24)		// colormap for 1 or 4 or 8
      {
	int i, cmaplen;
	cmaplen = (ClrUsed) ? ClrUsed : 1 << BitCount;

        fprintf(fileOut,"//vbm8\n");
	fprintf(fileOut,"#define %s_height %d\n", vbmName,Height);
	fprintf(fileOut,"#define %s_width %d\n", vbmName,Width);
	fprintf(fileOut,"unsigned char %s_bits[] = {\n%d,\n",vbmName,cmaplen-1);

	int r,g,b;
  	int valsOut = 0;
	for (i=0; i<cmaplen; i++)
	  {
	    b = getc(fileIn);
	    g = getc(fileIn);
	    r = getc(fileIn);
	    (void) getc(fileIn);	// skip pad byte
	    bPad -= 4;			// we just read 4 bytes
	    fprintf(fileOut,"%d,%d,%d,",r,g,b);
	    valsOut += 3;
	    if (valsOut >= 20)
	      {
		valsOut = 0;
	 	fprintf(fileOut,"\n");
	      }
	  }
	fprintf(fileOut,"// begin bitmap\n");
      }
    else				// 24 bit bmp
      {
	fprintf(fileOut,"//vbm24\n");
	fprintf(fileOut,"#define %s_height %d\n", vbmName,Height);
	fprintf(fileOut,"#define %s_width %d\n", vbmName,Width);
	fprintf(fileOut,"unsigned char %s[] = {\n",vbmName);
      }

    if (ferror(fileIn) || feof(fileIn))	// something is amiss
	return 0;			// so fail

    // Now, skip over any unused bytes between the color map (if there
    // was one, and the start of the bitmap data.
    
    while (bPad > 0)
      {
	(void) getc(fileIn);
	bPad--;
      }

    // Now, read rest of file and write out an 8 or 24 bit v icon def

    long limit = (BitCount == 24) ? Width*Height*3 : Width*Height;

    unsigned char* rev = new unsigned char[limit];

    if (BitCount == 1)
	rv = rdBMP1(fileIn,rev,Width,Height);
    else if (BitCount == 4)
	rv = rdBMP4(fileIn,rev,Width,Height,Compression);
    else if (BitCount == 8)
	rv = rdBMP8(fileIn,rev,Width,Height, Compression);
    else
	rv = rdBMP24(fileIn,rev,Width,Height);

    int valsOut = 0;

    // have to reverse the bitmap's row order

    long rowLen = (BitCount == 24) ? Width*3 : Width;

    for (long ix = limit - rowLen ; ix >= 0 ; ix -= rowLen)
      {
	// output one row
	for (long ri = 0 ; ri < rowLen ; ++ri)
	  {
	    fprintf(fileOut,"%d,",rev[ix+ri]);
	    if (++valsOut >= 20)
	      {
		valsOut = 0; fprintf(fileOut,"\n");
	      }
	  }
      }

    fprintf(fileOut,"0};\n");
    if (!rv)
	return 0;

    return 1;

  }  

//=========================>> rdBMP1 <<=============================
  static int rdBMP1(FILE* fileIn, unsigned char* rev,
	unsigned int w, unsigned int h)
  {
    int i,j,c,bitnum,padw;

    c = 0;
    padw = ((w + 31)/32) * 32;  /* 'w', padded to be a multiple of 32 */

    for (i=h-1; i>=0; i--)
      {
	for (j=bitnum=0; j<padw; j++,bitnum++)
	  {
	    if ((bitnum&7) == 0)
	      { /* read the next byte */
		c = getc(fileIn);
		bitnum = 0;
	      }
      
	    if (j<w)
	      {
		*rev++ = (c & 0x80) ? 1 : 0;
		c <<= 1;
	      }
	  }

	if (ferror(fileIn) || feof(fileIn))	// something is amiss
	    return 0;			// so fail
      }

    if (ferror(fileIn) || feof(fileIn))	// something is amiss
	return 0;			// so fail
}  

//=========================>> rdBMP4 <<=============================
  static int rdBMP4(FILE* fileIn, unsigned char*rev,
		    unsigned int w, unsigned int h, unsigned int comp)
  {
    int   i,j,c,c1,x,y,nybnum,padw,rv;

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
		    c = getc(fileIn);
		    nybnum = 0;
		  }

		if (j<w)
		  {
		    *rev++ = (c & 0xf0) >> 4;
		    c <<= 4;
		  }
		if (ferror(fileIn) || feof(fileIn))	// something is amiss
		    return 0;			// so fail
	      }
	  }
      }
    else
	return 0;
  
    if (ferror(fileIn) || feof(fileIn))	// something is amiss
	return 0;			// so fail
    return rv;
  }  

//=========================>> rdBMP8 <<=============================
  static int rdBMP8(FILE* fileIn, unsigned char* rev,
		    unsigned int w, unsigned int h, unsigned int comp)
  {
    int   i,j,c,c1,padw,x,y,rv;
 
    rv = 1;

    if (comp == BI_RGB)		// read uncompressed data
      {
	padw = ((w + 3)/4) * 4; /* 'w' padded to a multiple of 4pix (32 bits) */

	for (i=h-1; i>=0; i--) 
	  {
	    for (j=0; j<padw; j++) 
	      {
		c = getc(fileIn);
		if (c==EOF) 
		    rv = 1;
		if (j<w)
		  {
		    *rev++ = c;
		  }
		if (ferror(fileIn) || feof(fileIn))	// something is amiss
		    return 0;			// so fail
	      }
	  }
      }
    else 
	return 0;

    if (ferror(fileIn) || feof(fileIn))	// something is amiss
	return 0;			// so fail
    return rv;
  }  

//=========================>> rdBMP24 <<=============================
  static int rdBMP24(FILE* fileIn, unsigned char* rev,
		     unsigned int w, unsigned int h)
  {
    int   i,j,padb,rv;
    unsigned int r,g,b;

    padb = (4 - ((w*3) % 4)) & 0x03;  // # of pad bytes to read at EOscanline

    for (i=h-1; i>=0; i--)
      {
	for (j=0; j<w; j++) 
	  {
	    b = getc(fileIn);   // blue
	    g = getc(fileIn);   // green
	    r = getc(fileIn);   // red
	    *rev++ = r; *rev++ = g; *rev++ = b;
	  }

	for (j=0; j<padb; j++) 
	    getc(fileIn);
      }
    return 1;
  }  

//=============================>>> rdInt16 <<<=============================
  static unsigned int rdInt16(FILE *fileIn)
  {
    int c, c1;
    c = getc(fileIn);  c1 = getc(fileIn);
    return ((unsigned int) c) + (((unsigned int) c1) << 8);
  }

//=============================>>> rdInt32 <<<=============================
  static unsigned int rdInt32(FILE *fileIn)
  {
    int c, c1, c2, c3;
    c = getc(fileIn);  c1 = getc(fileIn);  c2 = getc(fileIn);  c3 = getc(fileIn);
    return ((unsigned int) c) +
         (((unsigned int) c1) << 8) + 
	 (((unsigned int) c2) << 16) +
	 (((unsigned int) c3) << 24);
  }
