#include <nds.h>
#include <stdio.h>
#include "Main.h"

void InitSprite();
void InitObj();
void ShowSprites(int XPos, int YPos);
void ScanObj(int XPos, int YPos);
void Turnp();
void ShowObj(int i, int XPos, int YPos);
bool CheckObj(Light LaserToCheck);
void ScanUp(int XPos, int YPos);
void Turnm();
void Turna();
void Turnb();
void CleanUp();
void FillGridObj();
