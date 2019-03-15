#ifndef _MENU_H_
#define _MENU_H_

enum Menu_Type
{
	Menu_Main,
	Menu_Gameloop,
	Menu_Result
};

void InitMenu();
Menu_Type GetCurMenu();
Menu_Type *GetMenuPointer();
void DrawMenu();
void UpdateMenu();
void PrintHouse(int xPos, int yPos);
void PrintHouseAnim(int xPos, int yPos);
void PrintPlayer(int xPos, int yPos);

#endif