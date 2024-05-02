#include <nds.h>
# include <stdio.h>

//The grid is using a palette, each pixel is one byte (a palette index)
//Each word is 2 pixels, either if the X is odd or not, the pixel will be the low byte or the high one

//Set the right pixel to col
inline void A(u16& Pix, u16 col) //PC
{
	Pix = ((Pix&0xFF)|col);
}
//Set both pixel left to col and right to col3
inline u16 B(u16 col, u16 col3) //CC
{
	return (col|col3);
}
//Set the left pixel to col
inline void C(u16 col, u16& Pix) //CP
{
	Pix = ((Pix&0xFF00)|col);
}

//How many objects
const int CELLSIZE = 128;

//Col is the laser's color
//HCol is the pale laser's color
//ColG is highbyte version of the laser's color, for the left pixel
//HColG is highbyte version of the pale laser's color, for the left pixel
//x,y are the cell's coordonates
//X,Y are the backbuffer's coordonate (backBuffer is a point to the background buffer + a constant to
//the top left corner of the grid (YOrg*CELLSIZE+XOrg/2)

//First the corners, it's just about one pixel
//SE corner, just one pixel down right (+ (CELLSIZE*14) + (X+7))
inline void DoCSE(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	HCol = HCol<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		//x is not odd, it's a left pixel
		case 0:
			C(HColG,backBuffer[((Y* 128) + (128*14) + (X+7))]);
		break;
		//x is odd, it's a right pixel
		case 1:
			A(backBuffer[((Y* 128) + (128*14) + (X+7))],HCol); 
		break;
	}
}

//NO corner, just one pixel up left (+0)
inline void DoCNO(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	HCol = HCol<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			C(HColG,backBuffer[((Y* 128) + (X))]);
		break;
		case 1:
			A(backBuffer[((Y* 128) + (X))],HCol); 
		break;
	}
}

inline void DoCSO(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	HCol = HCol<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			C(HColG,backBuffer[((Y* 128) + (128*14) + (X))]);
		break;
		case 1:
			A(backBuffer[((Y* 128) + (128*14) + (X))],HCol); 
		break;
	}
}

inline void DoCNE(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	HCol = HCol<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			C(HColG,backBuffer[((Y* 128) + (X+7))]);
		break;
		case 1:
			A(backBuffer[((Y* 128) + (X+7))],HCol); 
		break;
	}
}

//From the middle of the cell (+ (128*6) + (X+4)) till the north east (X+7)
inline void DoNE(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	u16 ColG = Col;
	HCol = HCol<<8;
	Col = Col<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			A(backBuffer[((Y* 128)  + (128*6) + (X+3))],HCol); backBuffer[((Y* 128)  + (128*6) + (X+4))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*5) + (X+4))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*5) + (X+5))]);
			A(backBuffer[((Y* 128) + (128*4) + (X+4))],HCol); backBuffer[((Y* 128) + (128*4) + (X+5))]=B(ColG,HCol); 
			backBuffer[((Y* 128)  + (128*3) + (X+5))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*3) + (X+6))]);
			A(backBuffer[((Y* 128) + (128*2) + (X+5))],HCol); backBuffer[((Y* 128) + (128*2) + (X+6))]=B(ColG,HCol); 
			backBuffer[((Y* 128)  + (128*1) + (X+6))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*1) + (X+7))]);
			A(backBuffer[((Y* 128) + (X+6))],HCol); C(ColG,backBuffer[((Y* 128) + (X+7))]); 
		break;
		case 1:
			backBuffer[((Y* 128)  + (128*6)  + (X+4))]=B(HColG,Col);C(HColG,backBuffer[((Y* 128) + (128*6) + (X+5))]);
			A(backBuffer[((Y* 128) + (128*5) + (X+4))],HCol);backBuffer[((Y* 128) + (128*5) + (X+5))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*4)  + (X+5))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*4) + (X+6))]);
			A(backBuffer[((Y* 128) + (128*3) + (X+5))],HCol);backBuffer[((Y* 128) + (128*3) + (X+6))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*2)  + (X+6))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*2) + (X+7))]);
			A(backBuffer[((Y* 128) + (128*1) + (X+6))],HCol);backBuffer[((Y* 128) + (128*1) + (X+7))]=B(ColG,HCol);
			backBuffer[((Y* 128) + (X+7))]=B(HColG,Col);
		break;
	}
}

//From the South west of the cell (+ (128*14)) till the middle (+ (128*7) + (X+4))
inline void DoSO(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	u16 ColG = Col;
	HCol = HCol<<8;
	Col = Col<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			backBuffer[((Y* CELLSIZE) + (128*14) + (X))]=B(ColG,HCol);
			backBuffer[((Y* 128) + (128*13) + (X))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*13) + (X+1))]);
			A(backBuffer[((Y* 128) + (128*12) + (X))],HCol); backBuffer[((Y* 128) + (128*12) + (X+1))]=B(ColG,HCol); 
			backBuffer[((Y* 128) + (128*11) + (X+1))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*11) + (X+2))]);
			A(backBuffer[((Y* 128) + (128*10) + (X+1))],HCol); backBuffer[((Y* 128) + (128*10) + (X+2))]=B(ColG,HCol); 
			backBuffer[((Y* 128) + (128*9) + (X+2))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*9) + (X+3))]);
			A(backBuffer[((Y* 128) + (128*8) + (X+2))],HCol); backBuffer[((Y* 128) + (128*8) + (X+3))]=B(ColG,HCol); 
			backBuffer[((Y* 128) + (128*7) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*7) + (X+4))]);
		break;
		case 1:
			A(backBuffer[((Y* 128) + (128*14) + (X))],Col);C(HColG,backBuffer[((Y* 128) + (128*14) + (X+1))]);
			A(backBuffer[((Y* 128) + (128*13) + (X))],HCol);backBuffer[((Y* 128) + (128*13) + (X+1))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*12)  + (X+1))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*12) + (X+2))]);
			A(backBuffer[((Y* 128) + (128*11) + (X+1))],HCol);backBuffer[((Y* 128) + (128*11) + (X+2))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*10)  + (X+2))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*10) + (X+3))]);
			A(backBuffer[((Y* 128) + (128*9) + (X+2))],HCol);backBuffer[((Y* 128) + (128*9) + (X+3))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*8)  + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*8) + (X+4))]);
			A(backBuffer[((Y* 128) + (128*7) + (X+3))],HCol);backBuffer[((Y* 128) + (128*7) + (X+4))]=B(ColG,HCol);
		break;
	}
}

inline void DoNO(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	u16 ColG = Col;
	HCol = HCol<<8;
	Col = Col<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			backBuffer[((Y* 128) + (X))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*1) + (X))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*1) + (X+1))]);
			A(backBuffer[((Y* 128) + (128*2) + (X))],HCol); backBuffer[((Y* 128) + (128*2) + (X+1))]=B(ColG,HCol); 
			backBuffer[((Y* 128) + (128*3) + (X+1))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*3) + (X+2))]);
			A(backBuffer[((Y* 128) + (128*4) + (X+1))],HCol); backBuffer[((Y* 128) + (128*4) + (X+2))]=B(ColG,HCol); 
			backBuffer[((Y* 128) + (128*5) + (X+2))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*5) + (X+3))]);
			A(backBuffer[((Y* 128) + (128*6) + (X+2))],HCol); backBuffer[((Y* 128) + (128*6) + (X+3))]=B(ColG,HCol); 
			backBuffer[((Y* 128) + (128*7) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*7) + (X+4))]);
			A(backBuffer[((Y* 128) + (128*8) + (X+3))],HCol);			
		break;
		case 1:
			A(backBuffer[((Y* 128) + (X))],Col);C(HColG,backBuffer[((Y* 128) + (X+1))]);
			A(backBuffer[((Y* 128) + (128*1) + (X))],HCol);backBuffer[((Y* 128) + (128*1) + (X+1))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*2)  + (X+1))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*2) + (X+2))]);
			A(backBuffer[((Y* 128) + (128*3) + (X+1))],HCol);backBuffer[((Y* 128) + (128*3) + (X+2))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*4)  + (X+2))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*4) + (X+3))]);
			A(backBuffer[((Y* 128) + (128*5) + (X+2))],HCol);backBuffer[((Y* 128) + (128*5) + (X+3))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*6)  + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*6) + (X+4))]);
			A(backBuffer[((Y* 128) + (128*7) + (X+3))],HCol);backBuffer[((Y* 128) + (128*7) + (X+4))]=B(ColG,HCol);
			C(HColG,backBuffer[((Y* 128) + (128*8) + (X+4))]);
		break;
	}
}

inline void DoSE(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	u16 ColG = Col;
	HCol = HCol<<8;
	Col = Col<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			backBuffer[((Y* 128)  + (128*8) + (X+4))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*9) + (X+4))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*9) + (X+5))]);
			A(backBuffer[((Y* 128) + (128*10) + (X+4))],HCol); backBuffer[((Y* 128) + (128*10) + (X+5))]=B(ColG,HCol); 
			backBuffer[((Y* 128)  + (128*11) + (X+5))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*11) + (X+6))]);
			A(backBuffer[((Y* 128) + (128*12) + (X+5))],HCol); backBuffer[((Y* 128) + (128*12) + (X+6))]=B(ColG,HCol); 
			backBuffer[((Y* 128)  + (128*13) + (X+6))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*13) + (X+7))]);
			A(backBuffer[((Y* 128) + (128*14) + (X+6))],HCol); C(ColG,backBuffer[((Y* 128) + (128*14) + (X+7))]); 
		break;
		case 1:
			A(backBuffer[((Y* 128) + (128*8) + (X+4))],Col);C(HColG,backBuffer[((Y* 128) + (128*8) + (X+5))]);
			A(backBuffer[((Y* 128) + (128*9) + (X+4))],HCol);backBuffer[((Y* 128) + (128*9) + (X+5))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*10)  + (X+5))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*10) + (X+6))]);
			A(backBuffer[((Y* 128) + (128*11) + (X+5))],HCol);backBuffer[((Y* 128) + (128*11) + (X+6))]=B(ColG,HCol);
			backBuffer[((Y* 128)  + (128*12)  + (X+6))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*12) + (X+7))]);
			A(backBuffer[((Y* 128) + (128*13) + (X+6))],HCol);backBuffer[((Y* 128) + (128*13) + (X+7))]=B(ColG,HCol);
			backBuffer[((Y* 128) + (128*14) + (X+7))]=B(HColG,Col);
		break;
	}
}

inline void DoE(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	u16 ColG = Col;
	HCol = HCol<<8;
	Col = Col<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			backBuffer[((Y* 128) + (128*6) + (X+4))]=B(HColG,HCol);backBuffer[((Y* 128) + (128*6) + (X+5))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*6) + (X+6))]=B(HColG,HCol);C(HColG,backBuffer[((Y* 128) + (128*6) + (X+7))]);
			backBuffer[((Y* 128) + (128*7) + (X+4))]=B(ColG,Col);backBuffer[((Y* 128) + (128*7) + (X+5))]=B(ColG,Col);
			backBuffer[((Y* 128) + (128*7) + (X+6))]=B(ColG,Col);C(ColG,backBuffer[((Y* 128) + (128*7) + (X+7))]);
			backBuffer[((Y* 128) + (128*8) + (X+4))]=B(HColG,HCol);backBuffer[((Y* 128) + (128*8) + (X+5))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*8) + (X+6))]=B(HColG,HCol);C(HColG,backBuffer[((Y* 128) + (128*8) + (X+7))]);
		break;
		case 1:
			A(backBuffer[((Y* 128) + (128*6) + (X+4))],HCol);backBuffer[((Y* 128) + (128*6) + (X+5))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*6) + (X+6))]=B(HColG,HCol);backBuffer[((Y* 128) + (128*6) + (X+7))]=B(HColG,HCol);
			A(backBuffer[((Y* 128) + (128*7) + (X+4))],Col);backBuffer[((Y* 128) + (128*7) + (X+5))]=B(ColG,Col);
			backBuffer[((Y* 128) + (128*7) + (X+6))]=B(ColG,Col);backBuffer[((Y* 128) + (128*7) + (X+7))]=B(ColG,Col);
			A(backBuffer[((Y* 128) + (128*8) + (X+4))],HCol);backBuffer[((Y* 128) + (128*8) + (X+5))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*8) + (X+6))]=B(HColG,HCol);backBuffer[((Y* 128) + (128*8) + (X+7))]=B(HColG,HCol);
		break;
	}
}


inline void DoO(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	u16 ColG = Col;
	HCol = HCol<<8;
	Col = Col<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			backBuffer[((Y* 128) + (128*6) + (X))]=B(HColG,HCol);backBuffer[((Y* 128) + (128*6) + (X+1))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*6) + (X+2))]=B(HColG,HCol);	backBuffer[((Y* 128) + (128*6) + (X+3))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*7) + (X))]=B(ColG,Col);backBuffer[((Y* 128) + (128*7) + (X+1))]=B(ColG,Col);
			backBuffer[((Y* 128) + (128*7) + (X+2))]=B(ColG,Col);	backBuffer[((Y* 128) + (128*7) + (X+3))]=B(ColG,Col);
			backBuffer[((Y* 128) + (128*8) + (X))]=B(HColG,HCol);backBuffer[((Y* 128) + (128*8) + (X+1))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*8) + (X+2))]=B(HColG,HCol);	backBuffer[((Y* 128) + (128*8) + (X+3))]=B(HColG,HCol);
		break;
		case 1:
			A(backBuffer[((Y* 128) + (128*6) + (X))],HCol);backBuffer[((Y* 128) + (128*6) + (X+1))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*6) + (X+2))]=B(HColG,HCol);	backBuffer[((Y* 128) + (128*6) + (X+3))]=B(HColG,HCol);
			C(HColG,backBuffer[((Y* 128) + (128*6) + (X+4))]);
			A(backBuffer[((Y* 128) + (128*7) + (X))],Col);backBuffer[((Y* 128) + (128*7) + (X+1))]=B(ColG,Col);
			backBuffer[((Y* 128) + (128*7) + (X+2))]=B(ColG,Col);	backBuffer[((Y* 128) + (128*7) + (X+3))]=B(ColG,Col);
			C(ColG,backBuffer[((Y* 128) + (128*7) + (X+4))]);	
			A(backBuffer[((Y* 128) + (128*8) + (X))],HCol);backBuffer[((Y* 128) + (128*8) + (X+1))]=B(HColG,HCol);
			backBuffer[((Y* 128) + (128*8) + (X+2))]=B(HColG,HCol);	backBuffer[((Y* 128) + (128*8) + (X+3))]=B(HColG,HCol);
			C(HColG,backBuffer[((Y* 128) + (128*8) + (X+4))]);
		break;
	}
}



inline void DoN(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	u16 ColG = Col;
	HCol = HCol<<8;
	Col = Col<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;

	switch (x%2) {
		case 0:
			backBuffer[((Y* 128) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (X+4))]);
			backBuffer[((Y* 128) + (128*1) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*1) + (X+4))]);
			backBuffer[((Y* 128) + (128*2) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*2) + (X+4))]);
			backBuffer[((Y* 128) + (128*3) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*3) + (X+4))]);
			backBuffer[((Y* 128) + (128*4) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*4) + (X+4))]);
			backBuffer[((Y* 128) + (128*5) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*5) + (X+4))]);
			backBuffer[((Y* 128) + (128*6) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*6) + (X+4))]);
			backBuffer[((Y* 128) + (128*7) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*7) + (X+4))]);
		break;
		case 1:
			A(backBuffer[((Y* 128) + (X+3))],HCol); backBuffer[((Y* 128) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*1) + (X+3))],HCol); backBuffer[((Y* 128) + (128*1) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*2) + (X+3))],HCol); backBuffer[((Y* 128) + (128*2) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*3) + (X+3))],HCol); backBuffer[((Y* 128) + (128*3) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*4) + (X+3))],HCol); backBuffer[((Y* 128) + (128*4) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*5) + (X+3))],HCol); backBuffer[((Y* 128) + (128*5) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*6) + (X+3))],HCol); backBuffer[((Y* 128) + (128*6) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*7) + (X+3))],HCol); backBuffer[((Y* 128) + (128*7) + (X+4))]=B(ColG,HCol); 
		break;
	}
}

inline void DoS(u16* backBuffer,int x,int y, u16 Col)
{
	u16 HCol = Col + 7;
	u16 HColG = HCol;
	u16 ColG = Col;
	HCol = HCol<<8;
	Col = Col<<8;

	int X = x*15;
	int Y = y*15;
	X = X / 2;
	
	switch (x%2) {
		case 0:
			backBuffer[((Y* 128) + (128*8) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*8) + (X+4))]);
			backBuffer[((Y* 128) + (128*9) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*9) + (X+4))]);
			backBuffer[((Y* 128) + (128*10) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*10) + (X+4))]);
			backBuffer[((Y* 128) + (128*11) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*11) + (X+4))]);
			backBuffer[((Y* 128) + (128*12) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*12) + (X+4))]);
			backBuffer[((Y* 128) + (128*13) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*13) + (X+4))]);
			backBuffer[((Y* 128) + (128*14) + (X+3))]=B(HColG,Col); C(HColG,backBuffer[((Y* 128) + (128*14) + (X+4))]);
		break;
		case 1:
			A(backBuffer[((Y* 128) + (128*8) + (X+3))],HCol); backBuffer[((Y* 128) + (128*8) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*9) + (X+3))],HCol); backBuffer[((Y* 128) + (128*9) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*10) + (X+3))],HCol); backBuffer[((Y* 128) + (128*10) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*11) + (X+3))],HCol); backBuffer[((Y* 128) + (128*11) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*12) + (X+3))],HCol); backBuffer[((Y* 128) + (128*12) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*13) + (X+3))],HCol); backBuffer[((Y* 128) + (128*13) + (X+4))]=B(ColG,HCol); 
			A(backBuffer[((Y* 128) + (128*14) + (X+3))],HCol); backBuffer[((Y* 128) + (128*14) + (X+4))]=B(ColG,HCol); 
		break;
	}
}