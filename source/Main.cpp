#include <nds.h>
#include <stdio.h>

#include "Objet.h"
#include "Laser.h"

#include "Fond.h"

//Stylus position
int YPos = 0;
int XPos = 0;
//Lasers
Light OrgLaser[NLaser]={{2,1,3,3,0},{2,2,15,3,0},{2,4,21,3,0},{2,1,3,19,0},{2,2,15,19,0},{2,4,21,19,0}}; 
//Grid
Square Grid[16][11];

//Object currently grabed (-1 if none)
int Grab = -1; 
//Stylus position where it was pressed the first time
int MDownX;
int MDownY;
//Used to switch palettes
bool PaleDark = true;

//Timer function
void Cl()
{
	if (PaleDark)
	{
		SetPalBright();
	}
	else
	{
		SetPalDark();
	}
	PaleDark = !PaleDark;
}

int main(void)
{	
	bool KeepLooping = true;
	//variable to get the stylus position from the DS
	touchPosition t={0,0,0,0,0,0};

	//Set the timer, with the cl function
	TIMER3_CR = 0;
	irqSet(IRQ_TIMER3, Cl);
	irqEnable(IRQ_TIMER3);
	TIMER3_CR=TIMER_ENABLE|TIMER_DIV_1024|TIMER_IRQ_REQ;
	TIMER3_DATA=TIMER_FREQ_1024(2);

	//Swap screens (use the bottom screen as the main screen)
	lcdSwap();

	//Init the console, for the debug
	consoleDemoInit();
   
	//Setup the video mode and banks
	videoSetMode(MODE_5_2D | DISPLAY_SPR_ACTIVE | DISPLAY_BG3_ACTIVE );
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankB(VRAM_B_MAIN_BG);
	vramSetBankC(VRAM_C_MAIN_BG);

	//Init sprite engin
    oamInit(&oamMain, SpriteMapping_Bmp_1D_128, false);
	//Setup options for the background
    BACKGROUND.control[3] = BG_BMP8_256x256  | BG_BMP_BASE(0) | BG_WRAP_ON; 
  
	int lr = bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 8,0);
	//Flushall should always be called before copying datas
    DC_FlushAll();
	//Palette
	dmaCopy(FondPal, BG_PALETTE, 256*2);
	SetPalDark();
	//back buffer + copy the background in it
	u16* bBuffer = (u16*) bgGetGfxPtr(lr);
    DC_FlushAll();
    dmaCopyHalfWords(3,(void*) &FondBitmap,bBuffer, 256*256);

	bgSetPriority(lr,3);

	//Init Sprite and Objects
	InitSprite();
	InitObj();
 
	XPos =0;
	YPos =0;

	MDownX =0;
	MDownY =0;

	//Empty the grid, then fill it regarding the lasers' paths, then show it
	//TODO: make one function for those 4 ones
	EmptyGrid();
	FillGridObj();
	DoLaser();
	ShowGrid(bBuffer+(((YOrg*128)+(XOrg/2))));

	//Main loop
	while(KeepLooping) 
	{
		//Read the button states
		scanKeys();
		touchRead(&t);
		//The stylus is on the screen ?
		if ((t.px>0)&&(t.py>0))
		{
			XPos = t.px;
			YPos = t.py;
		}
		//If stylus pressed, scan for an object (MDownX & MDownY will be set in the function)
		if (keysDown() & KEY_TOUCH)
		{
			ScanObj(XPos,YPos);
		}
		if (keysUp() & KEY_TOUCH)
		{
			//If an object was selected, then drop it or turn it
			ScanUp(XPos,YPos);
			//TODO: Erase. It was just for testing, if you click on a laser, it will turn it
			//To remove, implement lasers' sources as an object
			for(int i = 0; i < NLaser; i++) 
			{
				if ((OrgLaser[i].posx == ((XPos-XOrg) / 15*2)+1)&&((OrgLaser[i].posy == ((YPos-YOrg) / 15*2)+1)))
					OrgLaser[i].Direction = (OrgLaser[i].Direction + 9 )%8;
			}
			
			//Redraw the background, and then the grid
			dmaCopyHalfWords(3,(void*) &FondBitmap,bBuffer, 256*170*2);
			EmptyGrid();
			FillGridObj();
			DoLaser();
			ShowGrid(bBuffer+(((YOrg*128)+(XOrg/2))));
		}
		//TODO: Implement buttons, or use for test
		if (keysUp() & KEY_A)
		{
			KeepLooping = false;
		}/*
		if (keysUp() & KEY_B)
		{
		}
		if (keysUp() & KEY_X)
		{
		}
		if (keysUp() & KEY_Y)
		{
		}*/

		//Display objects, with actual stylus position in cas one is "flying"
		ShowSprites(XPos,YPos);
		//Update the bg
		bgUpdate();
		//Wait for V-sinc
		swiWaitForVBlank();
		//Update the video
		oamUpdate(&oamMain);
	}
	//Clean up everything
	CleanUp();
	
	return 0;
}