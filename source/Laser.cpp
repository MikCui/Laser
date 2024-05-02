#include <stdio.h>
#include <nds.h>
#include "Laser.h"
#include "grid.cpp"


//Bright Pal: A main color + a bit of the 2 other ones for the brightness

//Center of the Laser (Main color)
const int LumMaxA=31;
//Border of the Laser (Main color)
const int LumMoyA=16;
//Center of the Laser (A bit of the 2 non main colors, for the brightness)
const int LumaMaxA=8;
//Border of the Laser (A bit of the 2 non main colors, for the brightness)
const int LumaMoyA=4;

//Dark Pal: Just main color

//Center of the Laser (Main color)
const int LumMaxB=24;
//Border of the Laser (Main color)
const int LumMoyB=8;

//Check if there is an object on the Laser path
// TODO: Implement the object functionnality here (Miror: reflect the laser...)
bool CheckObj(Light* LaserToCheck)
{
	if (Grid[(LaserToCheck->posx/2)][(LaserToCheck->posy/2)].ObjOn !=  -1)
	{
	// Object: Turn/Destroy/Split Laser
		return true;
	}
	return false;
}

// Set the dark palette
void SetPalDark()
{
	//0: is dark
	BG_PALETTE[0] = 0;
	//1: is red
	BG_PALETTE[1] = (RGB15(LumMaxB, 0, 0) | BIT(15));
	//2: is blue
	BG_PALETTE[2] = (RGB15(0, LumMaxB, 0) | BIT(15));
	//3: is purple (red + blue)
	BG_PALETTE[3] = (RGB15(LumMaxB, LumMaxB, 0) | BIT(15));
	//4: is green
	BG_PALETTE[4] = (RGB15(0, 0, LumMaxB) | BIT(15));
	//5: is yellow (red + green)
	BG_PALETTE[5] = (RGB15(LumMaxB, 0, LumMaxB) | BIT(15));
	//6: is cyan (blue + green)
	BG_PALETTE[6] = (RGB15(0, LumMaxB, LumMaxB) | BIT(15));
	//7: is white (red + green + blue)
	BG_PALETTE[7] = (RGB15(LumMaxB, LumMaxB, LumMaxB) | BIT(15));

	//8: is pale red 
	BG_PALETTE[8] = (RGB15(LumMoyB, 0, 0) | BIT(15));
	//9: is pale blue
	BG_PALETTE[9] = (RGB15(0, LumMoyB, 0) | BIT(15));
	//10: is pale purple
	BG_PALETTE[10] = (RGB15(LumMoyB, LumMoyB, 0) | BIT(15));
	//11: is pale green
	BG_PALETTE[11] = (RGB15(0, 0, LumMoyB) | BIT(15));
	//12: is pale yellow
	BG_PALETTE[12] = (RGB15(LumMoyB, 0, LumMoyB) | BIT(15));
	//13is pale cyan
	BG_PALETTE[13] = (RGB15(0, LumMoyB, LumMoyB) | BIT(15));
	//14: is pale white
	BG_PALETTE[14] = (RGB15(LumMoyB, LumMoyB, LumMoyB) | BIT(15));
}

// Set the bight palette
void SetPalBright()
{
	BG_PALETTE[0] = 0;
	BG_PALETTE[1] = (RGB15(LumMaxA, LumaMaxA, LumaMaxA) | BIT(15));
	BG_PALETTE[2] = (RGB15(LumaMaxA, LumMaxA, LumaMaxA) | BIT(15));
	BG_PALETTE[3] = (RGB15(LumMaxA, LumMaxA, LumaMaxA) | BIT(15));
	BG_PALETTE[4] = (RGB15(LumaMaxA, LumaMaxA, LumMaxA) | BIT(15));
	BG_PALETTE[5] = (RGB15(LumMaxA, LumaMaxA, LumMaxA) | BIT(15));
	BG_PALETTE[6] = (RGB15(LumaMaxA, LumMaxA, LumMaxA) | BIT(15));
	BG_PALETTE[7] = (RGB15(LumMaxA, LumMaxA, LumMaxA) | BIT(15));
	BG_PALETTE[8] = (RGB15(LumMoyA, LumaMoyA, LumaMoyA) | BIT(15));
	BG_PALETTE[9] = (RGB15(LumaMoyA, LumMoyA, LumaMoyA) | BIT(15));
	BG_PALETTE[10] = (RGB15(LumMoyA, LumMoyA, LumaMoyA) | BIT(15));
	BG_PALETTE[11] = (RGB15(LumaMoyA, LumaMoyA, LumMoyA) | BIT(15));
	BG_PALETTE[12] = (RGB15(LumMoyA, LumaMoyA, LumMoyA) | BIT(15));
	BG_PALETTE[13] = (RGB15(LumaMoyA, LumMoyA, LumMoyA) | BIT(15));
	BG_PALETTE[14] = (RGB15(LumMoyA, LumMoyA, LumMoyA) | BIT(15));
}

// Empty the grid (Remove lights and objects)
void EmptyGrid()
{
	for(int ix = 0; ix <= 15; ix++)
	for(int iy = 0; iy <=11; iy++) 
	{	
		Grid[ix][iy].N = 0;
		Grid[ix][iy].S = 0;
		Grid[ix][iy].O = 0;
		Grid[ix][iy].E = 0;
	
		Grid[ix][iy].NE = 0;
		Grid[ix][iy].SO = 0;
		Grid[ix][iy].NO = 0;
		Grid[ix][iy].SE = 0;

		Grid[ix][iy].CSE = 0;
		Grid[ix][iy].CNO = 0;
		Grid[ix][iy].CSO = 0;
		Grid[ix][iy].CNE = 0;
	
		Grid[ix][iy].ObjOn= -1;
	}
}

// Show the lasers on the grid, for each case
void ShowGrid(u16* backBuffer)
{

	for(int iy = 0; iy < 11; iy++) 
	{
	for(int ix = 0; ix < 16; ix++)
	{
		if (Grid[ix][iy].CSE)
			DoCSE(backBuffer,ix,iy,Grid[ix][iy].CSE);
		if (Grid[ix][iy].CNO)
			DoCNO(backBuffer,ix,iy,Grid[ix][iy].CNO);
		if (Grid[ix][iy].CSO)
			DoCSO(backBuffer,ix,iy,Grid[ix][iy].CSO);
		if (Grid[ix][iy].CNE)
			DoCNE(backBuffer,ix,iy,Grid[ix][iy].CNE);
		if (Grid[ix][iy].N){
			DoN(backBuffer,ix,iy,Grid[ix][iy].N);}
		if (Grid[ix][iy].NO)
			DoNO(backBuffer,ix,iy,Grid[ix][iy].NO);
		if (Grid[ix][iy].O)
			DoO(backBuffer,ix,iy,Grid[ix][iy].O);
		if (Grid[ix][iy].SO)
			DoSO(backBuffer,ix,iy,Grid[ix][iy].SO);
		if (Grid[ix][iy].S)
			DoS(backBuffer,ix,iy,Grid[ix][iy].S);
		if (Grid[ix][iy].SE)
			DoSE(backBuffer,ix,iy,Grid[ix][iy].SE);
		if (Grid[ix][iy].E)
			DoE(backBuffer,ix,iy,Grid[ix][iy].E);
		if (Grid[ix][iy].NE)
			DoNE(backBuffer,ix,iy,Grid[ix][iy].NE);
	}
	}
}


void DoLaser()
{
	//Destroyed laser ? (after meeting an object)
	bool Destroy = false;
	//There is still some lasers to work on
	bool still;

	Light* FirstLaser;
	Light CurLaser;
	Light* Tmp;

	//Create the first laser, define Firstlaser (Set the next to null, then it will be set again in case it's needed)
	Light* T = new Light;
	T->Direction = OrgLaser[0].Direction;
	T->Col = OrgLaser[0].Col;
	T->posx = OrgLaser[0].posx;
	T->posy = OrgLaser[0].posy;
	T->next = NULL;
	FirstLaser = T;

	//Fill the Lasers list (Won't do the first and last element)
	for(int i = 1; i < NLaser-1; i++) 
	{
		Light* T2 = new Light;
		T2->Direction = OrgLaser[i].Direction;
		T2->Col = OrgLaser[i].Col;
		T2->posx = OrgLaser[i].posx;
		T2->posy = OrgLaser[i].posy;
		T->next = T2;
		T = T2;
	}

	//Last laser of the list (if not the first one, if there is just one laser)
	if (NLaser > 1)
	{
		Light* T2 = new Light;
		T2->Direction = OrgLaser[NLaser-1].Direction;
		T2->Col = OrgLaser[NLaser-1].Col;
		T2->posx = OrgLaser[NLaser-1].posx;
		T2->posy = OrgLaser[NLaser-1].posy;
		T->next = T2;
		T2->next = NULL;
	}

	//Start with the first laser
	CurLaser = *FirstLaser;

	//There is some lasers to work on
	still = true;

	while (still)
	{
		//The laser is "living"
		Destroy = false;
		switch (CurLaser.Direction) //Switch direction (0 nord, 2 left...)
		{
			//NORD
			case 0:
				//Laser goes up
				CurLaser.posy = CurLaser.posy-1;
				//Check if the laser in in the north or south part of a block
				//Then add the color in the block part
				if (!(CurLaser.posy & 1))
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].N = Grid[CurLaser.posx / 2][CurLaser.posy / 2].N | CurLaser.Col;
				}	
				else
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].S = Grid[CurLaser.posx / 2][CurLaser.posy / 2].S | CurLaser.Col;
					//Check if there is an object in this block (then compute it)
					Destroy = CheckObj(&CurLaser);
				}
				//If the laser reached the nord border or an object
				if ((CurLaser.posy <= 0)||(Destroy))
				{
					//No more lasers, exit the loop
					if (CurLaser.next == NULL)
					{
						still = false;
					}
					//Still some work, go to the next laser
					else
					{
						CurLaser = *CurLaser.next;
					}
				}
			break;
			//NORD WEST
			case 1:
				//Laser goes north west
				CurLaser.posy = CurLaser.posy-1;
				CurLaser.posx = CurLaser.posx-1;
				//Check if the laser in in the NW (NO in french...) or SE part of a block
				//Then add the color in the block part
				if ((!(CurLaser.posy & 1))||(!(CurLaser.posx & 1)))
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].NO = Grid[CurLaser.posx / 2][CurLaser.posy / 2].NO | CurLaser.Col;
					//if there is a block on the north, then turn on the SW corner of this block
					if (CurLaser.posy >= 1)
						Grid[CurLaser.posx / 2][(CurLaser.posy / 2)-1].CSO = Grid[CurLaser.posx / 2][(CurLaser.posy / 2)-1].CSO | CurLaser.Col;
					//if there is a block on the left, then turn on the NE corner of this block
					if (CurLaser.posx >= 1)
						Grid[(CurLaser.posx / 2)-1][CurLaser.posy / 2].CNE = Grid[(CurLaser.posx / 2)-1][CurLaser.posy / 2].CNE | CurLaser.Col;
				}	
				else
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].SE = Grid[CurLaser.posx / 2][CurLaser.posy / 2].SE | CurLaser.Col;
					//Check if there is an object in this block (then compute it)
					Destroy = CheckObj(&CurLaser);
				}
				//If the laser reached the nord or west border or an object
				if ((CurLaser.posy <= 0)||(CurLaser.posx <= 0)||(Destroy))
				{
					//No more lasers, exit the loop
					if (CurLaser.next == NULL)
					{
						still = false;
					}
					//Still some work, go to the next laser
					else
					{
						CurLaser = *CurLaser.next;
					}
				}
			break;
			//WEST
			case 2:
				CurLaser.posx = CurLaser.posx-1;
				if (!(CurLaser.posx & 1))
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].O = Grid[CurLaser.posx / 2][CurLaser.posy / 2].O | CurLaser.Col;
				}	
				else
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].E = Grid[CurLaser.posx / 2][CurLaser.posy / 2].E | CurLaser.Col;
					Destroy = CheckObj(&CurLaser);
				}
				if ((CurLaser.posx <= 0)||(Destroy))
				{
					if (CurLaser.next == NULL)
					{
						still = false;
					}
					else
					{
						CurLaser = *CurLaser.next;
					}
				}
			break;
			//SOUTHWEST
			case 3:
				CurLaser.posx = CurLaser.posx-1;
				if ((CurLaser.posy & 1)||(!(CurLaser.posx & 1)))
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].SO = Grid[CurLaser.posx / 2][CurLaser.posy / 2].SO | CurLaser.Col;
					if (CurLaser.posy <= 18)
						Grid[CurLaser.posx / 2][(CurLaser.posy / 2)+1].CNO = Grid[CurLaser.posx / 2][(CurLaser.posy / 2)+1].CNO | CurLaser.Col;
					if (CurLaser.posx >= 1)
						Grid[(CurLaser.posx / 2)-1][CurLaser.posy / 2].CSE = Grid[(CurLaser.posx / 2)-1][CurLaser.posy / 2].CSE | CurLaser.Col;
				}	
				else
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].NE = Grid[CurLaser.posx / 2][CurLaser.posy / 2].NE | CurLaser.Col;
					Destroy = CheckObj(&CurLaser);
				}
				if ((CurLaser.posy >= 21)||(CurLaser.posx <= 0)||(Destroy))
				{
					if (CurLaser.next == NULL)
					{
						still = false;
					}
					else
					{
						CurLaser = *CurLaser.next;
					}
				}
				else
				{
					CurLaser.posy = CurLaser.posy+1;
				}
			break;
			//SOUTH
			case 4:				
				if (CurLaser.posy & 1)
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].S = Grid[CurLaser.posx / 2][CurLaser.posy / 2].S | CurLaser.Col;
				}	
				else
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].N = Grid[CurLaser.posx / 2][CurLaser.posy / 2].N | CurLaser.Col;
					Destroy = CheckObj(&CurLaser);
				}
				if ((CurLaser.posy >= 21)||(Destroy))
				{
					if (CurLaser.next == NULL)
					{
						still = false;
					}
					else
					{
						CurLaser = *CurLaser.next;
					}
				}
				else
				{
					CurLaser.posy = CurLaser.posy+1;
				}
			break;
			//EASTSOUTH
			case 5:
				if ((CurLaser.posy & 1)||(CurLaser.posx & 1))
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].SE = Grid[CurLaser.posx / 2][CurLaser.posy / 2].SE | CurLaser.Col;
					if (CurLaser.posy <= 18)
						Grid[CurLaser.posx / 2][(CurLaser.posy / 2)+1].CNE = Grid[CurLaser.posx / 2][(CurLaser.posy / 2)+1].CNE | CurLaser.Col;
					if (CurLaser.posx <= 28)
						Grid[(CurLaser.posx / 2)+1][CurLaser.posy / 2].CSO = Grid[(CurLaser.posx / 2)+1][CurLaser.posy / 2].CSO | CurLaser.Col;
				}	
				else
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].NO = Grid[CurLaser.posx / 2][CurLaser.posy / 2].NO | CurLaser.Col;
					Destroy = CheckObj(&CurLaser);
				}
					
				if ((CurLaser.posy >= 21)||(CurLaser.posx >= 31)||(Destroy))
				{
					if (CurLaser.next == NULL)
					{
						still = false;
					}
					else
					{
						CurLaser = *CurLaser.next;
					}
				}
				else
				{
					CurLaser.posx = CurLaser.posx+1;
					CurLaser.posy = CurLaser.posy+1;
				}
			break;
			//EAST
			case 6:				
				if (CurLaser.posx & 1)
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].E = Grid[CurLaser.posx / 2][CurLaser.posy / 2].E | CurLaser.Col;
				}	
				else
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].O = Grid[CurLaser.posx / 2][CurLaser.posy / 2].O | CurLaser.Col;
					Destroy = CheckObj(&CurLaser);
				}
					
				if ((CurLaser.posx >= 31)||(Destroy))
				{
					if (CurLaser.next == NULL)
					{
						still = false;
					}
					else
					{
						CurLaser = *CurLaser.next;
					}
				}
				else
				{
					CurLaser.posx = CurLaser.posx+1;
				}
			break;
			//NORTHEAST
			case 7:
				CurLaser.posy = CurLaser.posy-1;
				if (!(CurLaser.posy & 1)||(CurLaser.posx & 1))
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].NE = Grid[CurLaser.posx / 2][CurLaser.posy / 2].NE | CurLaser.Col;
					if (CurLaser.posy >= 1)
						Grid[CurLaser.posx / 2][(CurLaser.posy / 2)-1].CSE = Grid[CurLaser.posx / 2][(CurLaser.posy / 2)-1].CSE | CurLaser.Col;
					if (CurLaser.posx <= 28)
						Grid[(CurLaser.posx / 2)+1][CurLaser.posy / 2].CNO = Grid[(CurLaser.posx / 2)+1][CurLaser.posy / 2].CNO | CurLaser.Col;
				}	
				else
				{
					Grid[CurLaser.posx / 2][CurLaser.posy / 2].SO = Grid[CurLaser.posx / 2][CurLaser.posy / 2].SO | CurLaser.Col;
					Destroy = CheckObj(&CurLaser);
				}
					
				if ((CurLaser.posy <= 0)||(CurLaser.posx <= 0)||(Destroy))
				{
					if (CurLaser.next == NULL)
					{
						still = false;
					}
					else
					{
						CurLaser = *CurLaser.next;
					}
				}
				else
				{
					CurLaser.posx = CurLaser.posx+1;
				}
			break;
		}
	}
	//Ckear the laser list
	while(FirstLaser)
	{
		Tmp = FirstLaser->next;
		free(FirstLaser);
		FirstLaser = Tmp;
	}
}