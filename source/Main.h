#include <nds.h>
#include <stdio.h>

#ifndef MAINH
#define MAINH

struct Obj
{
	int PosX;
	int PosY;
	int Type;
	int Direction;
	u16* Sprite;
};

struct Square
{
	int N;
	int S;
	int E;
	int O;

	int NE;
	int SO;
	int NO;
	int SE;
	

	int CSE;
	int CNO;
	int CSO;
	int CNE;
	
	int ObjOn;
};

struct Light
{
   int Direction;
   int Col;
   int posx;
   int posy;
   Light *next;
};

const int NLaser = 6;
extern Light OrgLaser[]; 
extern Square Grid[16][11];



const int XOrg = 8;
const int YOrg = 3;
const int Tollerance = 3;
//Obj Objs[NObj] = {{4,4,1,0,NULL}}; 
const int TypeObj = 7;


//extern Obj Objs[]; 

extern int Grab; 
//extern u16* SpriteMiroir;

extern int MDownX;
extern int MDownY;

#endif