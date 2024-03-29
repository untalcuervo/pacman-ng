//RGBpixmap.h Pixmap and other supporting classes
//-----Gur Saran Adhar

#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;

#include <windows.h> //change if using xWindows
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <glut.h>

typedef unsigned short ushort;
typedef unsigned long ulong;
fstream inf; // global in this file for convenience

//<<<<<<<<<<<<<<<<<<<<< getShort >>>>>>>>>>>>>>>>>>>>
ushort getShort() //helper function
{ //BMP format uses little-endian integer types
  // get a 2-byte integer stored in little-endian form
		char ic;
		ushort ip;
		inf.get(ic); ip = ic;  //first byte is little one 
		inf.get(ic);  ip |= ((ushort)ic << 8); // or in high order byte
		return ip;
}
//<<<<<<<<<<<<<<<<<<<< getLong >>>>>>>>>>>>>>>>>>>
ulong getLong() //helper function
{  //BMP format uses little-endian integer types
   // get a 4-byte integer stored in little-endian form
		ulong ip = 0;
		char ic = 0;
		unsigned char uc = ic;
		inf.get(ic); uc = ic; ip = uc;
		inf.get(ic); uc = ic; ip |=((ulong)uc << 8);
		inf.get(ic); uc = ic; ip |=((ulong)uc << 16);
		inf.get(ic); uc = ic; ip |=((ulong)uc << 24);
		return ip;
	}

//RGB Class
class RGB{ // holds a color triple � each with 256 possible intensities
    public: unsigned char r,g,b;
};

//<<<<<<<<<<<< IntPoint class >>>>>>>>>>>>>>>>
class IntPoint{ // for 2D points with integer coordinates
public: 
	int x,y;
	void set(int dx, int dy){x = dx; y = dy;}
	void set(IntPoint& p){ x = p.x; y = p.y;} 
	IntPoint(int xx, int yy){x = xx; y = yy;}
	IntPoint(){ x = y = 0;}
};



//<<<<<<<<<<<<<<<<<<<  IntRect class >>>>>>>>>>>>>>>>>>>
class IntRect{ // a rectangle with integer border values
public: 
  int left, top, right, bott;
  IntRect(){left = top = right = bott = 0;}
  IntRect(int l, int t, int r, int b)
  {left = l; top = t; right = r; bott = b;}
  void set(int l, int t, int r, int b)
  {left = l; top = t; right = r; bott = b;}
  void set(IntRect& r)
  {left = r.left; top = r.top; right = r.right; bott = r.bott;}
};

// RGBpixmap class to support working with RGB pixmaps.



typedef unsigned char uchar;

class mRGB{   // the name RGB is already used by Windows
public: uchar r,g,b;
			mRGB(){r = g = b = 0;}
			mRGB(mRGB& p){r = p.r; g = p.g; b = p.b;}
			mRGB(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
			void set(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
};


//<<<<<<<<<<<<<<RGBPixmap class >>>>>>>>>>>>>>>

class RGBpixmap{
private: 
	mRGB* pixel; // array of pixels

public:
	int nRows, nCols; // dimensions of the pixmap
	RGBpixmap() {nRows = nCols = 0; pixel = 0;}
	RGBpixmap(int rows, int cols) //constructor
	{
		nRows = rows;
		nCols = cols;
		pixel = new mRGB[rows*cols]; 
	}
	//int readBMPFile(string fname); // read BMP file into this pixmap
	void freeIt() // give back memory for this pixmap
	{
		delete []pixel; nRows = nCols = 0;
	}
	//<<<<<<<<<<<<<<<<<< copy >>>>>>>>>>>>>>>>>>>
	void copy(IntPoint from, IntPoint to, int x, int y, int width, int height)
	{ // copy a region of the display back onto the display
		if(nRows == 0 || nCols == 0) return;
		glCopyPixels(x, y, width, height,GL_COLOR);
	}
	//<<<<<<<<<<<<<<<<<<< draw >>>>>>>>>>>>>>>>>
	void draw()
	{ // draw this pixmap at current raster position
		if(nRows == 0 || nCols == 0) return;
		//tell OpenGL: don�t align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glDrawPixels(nCols, nRows,GL_RGB, GL_UNSIGNED_BYTE,pixel);
	}
	//<<<<<<<<<<<<<<<<< read >>>>>>>>>>>>>>>>
	int read(int x, int y, int wid, int ht)
	{ // read a rectangle of pixels into this pixmap
		nRows = ht;
		nCols = wid;
		pixel = new mRGB[nRows *nCols]; if(!pixel) return -1;
		//tell OpenGL: don�t align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_PACK_ALIGNMENT,1);
		glReadPixels(x, y, nCols, nRows, GL_RGB,GL_UNSIGNED_BYTE,pixel);
		return 0;
	}
	//<<<<<<<<<<<<<<<<< read from IntRect >>>>>>>>>>>>>>>>
	int read(IntRect r)
	{ // read a rectangle of pixels into this pixmap
		nRows = r.top - r.bott;
		nCols = r.right - r.left;
		pixel = new mRGB[nRows *nCols]; if(!pixel) return -1;
		//tell OpenGL: don�t align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_PACK_ALIGNMENT,1);
		glReadPixels(r.left,r.bott, nCols, nRows, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		return 0;
	}
	//<<<<<<<<<<<<<< setPixel >>>>>>>>>>>>>
	void setPixel(int x, int y, mRGB color)
	{
		if(x>=0 && x <nCols && y >=0 && y < nRows)
			pixel[nCols * y + x] = color;
	}
	//<<<<<<<<<<<<<<<< getPixel >>>>>>>>>>>
	mRGB getPixel(int x, int y)
	{ 		
		mRGB bad(255,255,255);
		assert(x >= 0 && x < nCols);
		assert(y >= 0 && y < nRows);
		return pixel[nCols * y + x];
	}

     //<<<<<<<<<<<<<<<<<<  readBmpFile>>>>>>>>>>>>>
	int readBMPFile(string fname) 
	{  // Read into memory an mRGB image from an uncompressed BMP file.
	// return 0 on failure, 1 on success
	inf.open(fname.c_str(), ios::in|ios::binary); //read binary char's
	if(!inf){ cout << " can't open file: " << fname << endl; return 0;}
	int k, row, col, numPadBytes, nBytesInRow;
	// read the file header information
	char ch1, ch2;
	inf.get(ch1); inf.get(ch2); //type: always 'BM'
	ulong fileSize =      getLong();
	ushort reserved1 =    getShort();    // always 0
	ushort reserved2= 	getShort();     // always 0 
	ulong offBits =		getLong(); // offset to image - unreliable
	ulong headerSize =     getLong();     // always 40
	ulong numCols =		getLong(); // number of columns in image
	ulong numRows = 		getLong(); // number of rows in image
	ushort planes=    	getShort();      // always 1 
	ushort bitsPerPixel=   getShort();    //8 or 24; allow 24 here
	ulong compression =    getLong();      // must be 0 for uncompressed 
	ulong imageSize = 	getLong();       // total bytes in image 
	ulong xPels =    	getLong();    // always 0 
	ulong yPels =    	getLong();    // always 0 
	ulong numLUTentries = getLong();    // 256 for 8 bit, otherwise 0 
	ulong impColors = 	getLong();       // always 0 
	if(bitsPerPixel != 24) 
	{ // error - must be a 24 bit uncompressed image
		cout << "not a 24 bit/pixelimage, or is compressed!\n";
		inf.close(); return 0;
	} 
	//add bytes at end of each row so total # is a multiple of 4
	// round up 3*numCols to next mult. of 4
	nBytesInRow = ((3 * numCols + 3)/4) * 4;
	numPadBytes = nBytesInRow - 3 * numCols; // need this many
	nRows = numRows; // set class's data members
	nCols = numCols;
	pixel = new mRGB[nRows * nCols]; //make space for array
	if(!pixel) return 0; // out of memory!
	long count = 0;
	char dum;
	for(row = 0; row < nRows; row++) // read pixel values
	{
		for(col = 0; col < nCols; col++)
		{
			char r,g,b;
			inf.get(b); inf.get(g); inf.get(r); //read bytes
			pixel[count].r = r; //place them in colors
			pixel[count].g = g;
			pixel[count++].b = b;
		}
   		for(k = 0; k < numPadBytes ; k++) //skip pad bytes at row's end
			inf >> dum;
	}
	inf.close(); return 1; // success
}
void RGBpixmap :: setTexture(GLuint textureName)
{
	glBindTexture(GL_TEXTURE_2D,textureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,nCols,nRows,0, GL_RGB,
               GL_UNSIGNED_BYTE, pixel);
}



	void RGBpixmap:: makeCheckerboard()
{  // make checkerboard patten
	nRows = nCols = 64;
	pixel = new mRGB[3 * nRows * nCols]; 
	if(!pixel){return;}
	long count = 0;
	for(int i = 0; i < nRows; i++)
		for(int j = 0; j < nCols; j++)
		{
			int c = (((i/8) + (j/8)) %2) * 255;  
			pixel[count].r = c; 	// red
			pixel[count].g = c; 	// green
			pixel[count++].b = 0; 	// blue
		}
}


}; //end of class RGBpixmap

