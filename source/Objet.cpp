#include <stdio.h>
#include <nds.h>
#include "Objet.h"
//The sprites' datas (bitmaps processed with GRIT -> .h file)
#include "Miroir.h"
#include "LensR.h"
#include "LensG.h"
#include "LensB.h"
#include "LensRG.h"
#include "LensRB.h"
#include "LensGB.h"

//Sprites
u16* SpriteMiroir;
u16* SpriteLensR;
u16* SpriteLensG;
u16* SpriteLensB;
u16* SpriteLensRG;
u16* SpriteLensRB;
u16* SpriteLensGB;

//Rotation counter, the DS can display 32 rotated sprite at a time. It should be enough
int R = 0;

//Try with more object, uncomment this and comment the 2 next instructions
/*
const int NObj = 40;
Obj Objs[NObj] = {{0,0,1,0,0},{1,0,2,0,0},{2,0,3,0,0},{3,0,4,0,0},{4,0,5,0,0},
{0,1,6,0,0},{1,1,7,0,0},{2,1,1,0,0},{3,1,2,0,0},{4,1,3,0,0},
{0,2,4,0,0},{1,2,5,0,0},{2,2,6,0,0},{3,2,7,0,0},{4,2,1,0,0},
{0,3,2,0,0},{1,3,3,0,0},{2,3,4,0,0},{3,3,5,0,0},{4,3,6,0,0},
{0,4,7,0,0},{1,4,1,0,0},{2,4,2,0,0},{3,4,3,0,0},{4,4,4,0,0},
{0,5,5,0,0},{1,5,6,0,0},{2,5,7,0,0},{3,5,1,0,0},{4,5,2,0,0},
{0,6,3,0,0},{1,6,4,0,0},{2,6,5,0,0},{3,6,6,0,0},{4,6,7,0,0},
{0,7,1,0,0},{1,7,2,0,0},{2,7,3,0,0},{3,7,4,0,0},{4,7,5,0,0}}; 
*/

//How many objects
const int NObj = 10;
//Objects list
//TODO: Implement a level editor
Obj Objs[NObj] = {{0,0,1,0,0},{2,0,2,0,0},{4,0,3,0,0},{6,0,4,0,0},{8,0,5,0,0},
{0,1,6,0,0},{2,4,7,0,0},{4,4,1,0,0},{6,6,2,0,0},{8,6,3,0,0}}; 

//Show one object, i is object's number
void ShowObj(int i,int XPos, int YPos)
{
	//With  rotation, 2PI = 32768
	if (Objs[i].Direction != 0)
	{
		oamSet(&oamMain,i ,XPos,YPos, 0, 15, SpriteSize_16x16, SpriteColorFormat_Bmp,Objs[i].Sprite ,R, false,false, false, false, false);
		oamRotateScale(&oamMain, R, (4096*Objs[i].Direction), (1 << 8), (1 << 8));
		R++;
	}
	//Without
	else
	{
		oamSet(&oamMain,i ,XPos,YPos, 0, 15, SpriteSize_16x16, SpriteColorFormat_Bmp,Objs[i].Sprite , -1, false,false, false, false, false);
	}
}

//Load the sprites
void InitSprite()
{
	DC_FlushAll();
	SpriteMiroir = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
    DC_FlushAll();
	dmaCopyHalfWords(3,(void*) &MiroirBitmap, SpriteMiroir , 16*16*2);

	DC_FlushAll();
	SpriteLensR = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
    DC_FlushAll();
	dmaCopyHalfWords(3,(void*) &LensRBitmap, SpriteLensR , 16*16*2);
	DC_FlushAll();

	DC_FlushAll();
	SpriteLensG = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
    DC_FlushAll();
	dmaCopyHalfWords(3,(void*) &LensGBitmap, SpriteLensG , 16*16*2);
	DC_FlushAll();
	
	DC_FlushAll();
	SpriteLensB = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
    DC_FlushAll();
	dmaCopyHalfWords(3,(void*) &LensBBitmap, SpriteLensB , 16*16*2);
	DC_FlushAll();
	
	DC_FlushAll();
	SpriteLensRG = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
    DC_FlushAll();
	dmaCopyHalfWords(3,(void*) &LensRGBitmap, SpriteLensRG , 16*16*2);
	DC_FlushAll();

	DC_FlushAll();
	SpriteLensRB = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
    DC_FlushAll();
	dmaCopyHalfWords(3,(void*) &LensRBBitmap, SpriteLensRB , 16*16*2);
	DC_FlushAll();
	
	DC_FlushAll();
	SpriteLensGB = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
    DC_FlushAll();
	dmaCopyHalfWords(3,(void*) &LensGBBitmap, SpriteLensGB , 16*16*2);
	DC_FlushAll();
}

//Fill the grid with the objects
void FillGridObj()
{
	for(int i = 0; i < NObj; i++) 
	{
	
		Grid[Objs[i].PosX][Objs[i].PosY].ObjOn = i;
	}
}

//Set the respective sprites for every object
void InitObj()
{
	for(int i = 0; i < NObj; i++) 
	{
	
		Grid[Objs[i].PosX][Objs[i].PosY].ObjOn= i;
	
		switch (Objs[i].Type) {
		case 1:
			Objs[i].Sprite = SpriteMiroir;
		break;
		case 2:
			Objs[i].Sprite = SpriteLensR;
		break;
		case 3:
			Objs[i].Sprite = SpriteLensG;
		break;
		case 4:
			Objs[i].Sprite = SpriteLensB;
		break;
		case 5:
			Objs[i].Sprite = SpriteLensRG;
		break;
		case 6:
			Objs[i].Sprite = SpriteLensRB;
		break;
		case 7:
			Objs[i].Sprite = SpriteLensGB;
		break;
		}
	}
}

//Go through every object and display them
void ShowSprites(int XPos, int YPos)
{
	//So far no rotation
	R = 0;
	for(int i = 0; i < NObj; i++) 
	{
		//If object is selected, then display it under the stylus
		if (i == Grab)
		{
			ShowObj(i,XPos-8,YPos-8);
		}
		//else at his position
		else
		{
			ShowObj(i,(Objs[i].PosX*15)+XOrg-1,(Objs[i].PosY*15)+YOrg-1);
		}
	}
	//Too much rotations
	if (R>31)
		iprintf("Too much rotation: %i\n", R);
}

//Scan presence of an object under the stylus
void ScanObj(int XPos, int YPos)
{
	for(int i = 0; i < NObj; i++) 
	{
		if ((Objs[i].PosX == ((XPos-XOrg)/15))&&(Objs[i].PosY == ((YPos-YOrg)/15)))
		{
			//i is the selected object
			Grab = i;
			//save the mouse position
			MDownX =XPos;
			MDownY =YPos;
			//as the object is selected, it is then removed from the grid (like "flying")
			Grid[Objs[i].PosX][Objs[i].PosY].ObjOn = -1;
		}
	}
}

//Stylus is up, only intersting if an object was "flying"
void ScanUp(int XPos, int YPos)
{
	if (Grab >-1)
	{
		//if the object didn't move enough (more or less Tollerance pixels), then it's a rotation
		if (((XPos>(MDownX-Tollerance))&&(XPos<(MDownX+Tollerance)))&&((YPos>(MDownY-Tollerance))&&(YPos<(MDownY+Tollerance))))
		{
			//if L or R are pressed, then turn to the right
			scanKeys();
			if ((keysHeld() & KEY_L)||(keysHeld() & KEY_R))
			{
				Objs[Grab].Direction = (Objs[Grab].Direction + 7 )%8;
			}
			else
			//to the left
			{	
				Objs[Grab].Direction = (Objs[Grab].Direction + 9 )%8;	
			}
		}
		//Set the object to his new position
		Objs[Grab].PosX = ((XPos-XOrg)/15);
		Objs[Grab].PosY = ((YPos-YOrg)/15);
		//Ensure it's inside the screen
		if (Objs[Grab].PosX<0)
			Objs[Grab].PosX = 0;
		if (Objs[Grab].PosX>=15)
			Objs[Grab].PosX = 15;
		if (Objs[Grab].PosY<0)
			Objs[Grab].PosY = 0;
		if (Objs[Grab].PosY>=10)
			Objs[Grab].PosY = 10;
		//Setup this object on the grid
		Grid[Objs[Grab].PosX][Objs[Grab].PosY].ObjOn = Grab;
		//No more object selected
		Grab = -1;
	}
}

//Free the sprites
void CleanUp()
{
	oamFreeGfx(&oamMain,SpriteMiroir);
	oamFreeGfx(&oamMain,SpriteLensR);
	oamFreeGfx(&oamMain,SpriteLensG);
	oamFreeGfx(&oamMain,SpriteLensB);
	oamFreeGfx(&oamMain,SpriteLensRG);
	oamFreeGfx(&oamMain,SpriteLensRB);
	oamFreeGfx(&oamMain,SpriteLensGB);
}