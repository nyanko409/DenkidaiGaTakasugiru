#include "menu.h"
#include "main.h"
#include "player.h"
#include "soundManager.h"
#define CONIOEX
#include "conioex.h"

Menu_Type type;

float passedTime;

float playerYPos;

float animEnterTime;
bool dispEnterText;

float animCharTime;
bool dispNextChar;

char resultText[40];

bool resetTimer;
bool initResult;
bool resultFinished;
int resultIndex;

bool animPlaying;

char menuSound[20] = { 0 };
char menuBGM[20] = { 0 };
bool bgmStarted;

bool mainMenuDrawFinished;

bool smokeLeft;

float houseDestroyOffset;


void InitMenu()
{
	type = Menu_Main;
	animPlaying = false;
	resetTimer = false;
	dispEnterText = false;
	dispNextChar = false;
	initResult = false;
	resultFinished = false;
	playerYPos = 25.0f;
	passedTime = 0.0f;
	animEnterTime = 1.0f;
	animCharTime = 1.0f;
	resultIndex = 0;
	bgmStarted = false;
	mainMenuDrawFinished = false;
	smokeLeft = false;
	houseDestroyOffset = 0;
}

Menu_Type GetCurMenu()
{
	return type;
}

Menu_Type *GetMenuPointer()
{
	return &type;
}

void UpdateMenu()
{
	// main menu
	if (type == Menu_Main)
	{
		if (!bgmStarted)
		{
			// play menu bgm
			stopBGM();
			strcpy(menuSound, SOUND_MENUBGM);
			playBGM(menuSound, true, 25);
			bgmStarted = true;
		}

		// store passed time
		passedTime += TIME_PER_FRAME;

		// set enter animation flag
		if (passedTime >= animEnterTime)
		{
			dispEnterText = !dispEnterText;
			smokeLeft = !smokeLeft;
			animEnterTime += 1.0f;
			updateBGM();
		}

		// calculate player pos animation
		if (animPlaying)
		{
			playerYPos -= TIME_PER_FRAME * 2;
		}

		// wait for player to press enter
		if (!animPlaying)
		{
			rewind(stdin);
			if (inport(PK_ENTER))
			{
				// set animation flag
				animPlaying = true;
			}
		}

		// player pressed enter, animate player
		else
		{
			if (playerYPos <= 18.0f)
			{
				type = Menu_Gameloop;
				*GetInit() = false;
				clrscr();
			}
		}
	}

	// result menu
	else if (type == Menu_Result)
	{
		// reset timer
		if (!resetTimer)
		{
			passedTime = 0.0f;
			resetTimer = true;
		}

		// stop if finished drawing
		if (!resultFinished)
		{
			// store passed time
			passedTime += TIME_PER_FRAME;

			// set text and house animation flag
			if (passedTime >= animCharTime)
			{
				// text
				dispNextChar = true;
				animCharTime += 0.3f;

				if (resultIndex == 0 && GetCurrentDate() <= 10)
					resultIndex++;
				else
					resultIndex += 2;

				// house
				houseDestroyOffset++;
			}
		}

		// go back to main menu if enter is pressed
		rewind(stdin);
		if (inport(PK_ENTER))
		{
			while (inport(PK_ENTER));

			type = Menu_Main;
			InitMenu();
			*GetInit() = false;
			clrscr();
		}
	}
}

void DrawMenu()
{
	// draw main menu
	if (type == Menu_Main)
	{
		// draw once
		if (!mainMenuDrawFinished)
		{
			textbackground(CYAN);

			// draw background
			char buffer[121] = { };
			for (int i = 0; i < 120; i++)
			{
				buffer[i] = ' ';
			}

			for (int i = 1; i < 35; i++)
			{
				if (i > 17)
					textbackground(GREEN);

				gotoxy(1, i);
				printf("%s", buffer);
			}

			// draw title
			textbackground(CYAN);
			textcolor(BLACK);
			gotoxy(54, 5);
			printf("高すぎる電気代");

			mainMenuDrawFinished = true;
		}

		// draw house and player
		textcolor(BLACK);
		PrintHouse(20, 1);
		textcolor(LIGHTBLUE);
		PrintPlayer(36, playerYPos);

		// display and animate press enter
		textbackground(GREEN);
		if (dispEnterText)
		{
			gotoxy(55, 30);
			textcolor(MAGENTA);
			printf("PRESS ENTER");
		}
		else
		{
			gotoxy(55, 30);
			printf("           ");
		}

		textbackground(BLACK);
		textcolor(WHITE);
	}

	// draw result screen
	else if (type == Menu_Result)
	{
		// stop if finished drawing
		if (!resultFinished)
		{
			// init
			if (!initResult)
			{
				// init text
				sprintf(resultText, "%.0f日間いきのこりました。。。", GetCurrentDate());

				//init text sfx
				stopBGM();
				strcpy(menuBGM, SOUND_TEXTBEEP);

				// play defeat sound
				stopSound();
				strcpy(menuSound, SOUND_DEFEAT);
				playSoundM(menuSound, 25);

				// draw background
				textbackground(CYAN);
				char buffer[121] = { };
				for (int i = 0; i < 120; i++)
				{
					buffer[i] = ' ';
				}

				for (int i = 1; i < 35; i++)
				{
					if (i > 17)
						textbackground(GREEN);

					gotoxy(1, i);
					printf("%s", buffer);
				}

				// press enter text
				gotoxy(50, 30);
				textcolor(MAGENTA);
				printf("PRESS ENTER TO RESTART");

				initResult = true;
			}

			if (dispNextChar)
			{
				// anim house
				textcolor(BLACK);
				PrintHouseAnim(20, 1 + houseDestroyOffset);

				// animate result text
				char text[40] = { };
				for (int i = 0; i < resultIndex; i++)
				{
					if (resultText[i] == '\0')
						resultFinished = true;

					else
						text[i] = resultText[i];
				}

				// if \0, skip
				if (!resultFinished)
				{
					gotoxy(50, 5);
					textbackground(CYAN);
					textcolor(BLACK);
					printf("%s", text);
					playBGM(menuBGM, false, 10);
					dispNextChar = false;
				}

				textbackground(BLACK);
				textcolor(WHITE);
			}
		}
	}
}

void PrintHouse(int xPos, int yPos)
{
	textbackground(CYAN);
	if (smokeLeft)
	{
		gotoxy(xPos, yPos);   printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("           _)  _._");
	}
	else
	{
		gotoxy(xPos, yPos);	  printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (  ");
		gotoxy(xPos, ++yPos); printf("            )");
		gotoxy(xPos, ++yPos); printf("          (_   _._");
	}
	gotoxy(xPos, ++yPos); printf("          |_|-'_~_`-._");
	gotoxy(xPos, ++yPos); printf("       _.-'-_~_-~_-~-_`-._");
	gotoxy(xPos, ++yPos); printf("   _.-'_~-_~-_-~-_~_~-_~-_`-._");
	gotoxy(xPos, ++yPos); printf("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	gotoxy(xPos, ++yPos); printf("    |  []  []   []   []  [] |");
	textbackground(GREEN);
	gotoxy(xPos, ++yPos); printf("    |           __    ___   |");
	gotoxy(xPos, ++yPos); printf("  ._|  []  []  | .|  [___]  |_._._._._._._._._._._._._._._._._.");
	gotoxy(xPos, ++yPos); printf("  |=|________()|__|()_______|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|");
	gotoxy(xPos, ++yPos); printf("^^^^^^^^^^^^^^^ === ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	gotoxy(xPos, ++yPos); printf("                ===");
	gotoxy(xPos, ++yPos); printf("                ===");
	gotoxy(xPos, ++yPos); printf("                ===");
	gotoxy(xPos, ++yPos); printf("                ===");
	gotoxy(xPos, ++yPos); printf("                ===");
	gotoxy(xPos, ++yPos); printf("                ===");
	textbackground(BLACK);
}

void PrintHouseAnim(int xPos, int yPos)
{
	int yStart = yPos;

	if (yStart < 17)
		textbackground(CYAN);
	else
		textbackground(GREEN);

	if (houseDestroyOffset <= 15)
	{
		gotoxy(xPos, ++yPos); printf("                                                   ");
		gotoxy(xPos, yPos);   printf("          |_|-'_~_`-._");
	}
	if (houseDestroyOffset <= 14)
	{
		gotoxy(xPos, ++yPos); printf("                                         ");
		gotoxy(xPos, yPos); printf("       _.-'-_~_-~_-~-_`-._");
	}
	if (houseDestroyOffset <= 13)
	{
		gotoxy(xPos, ++yPos); printf("                                            ");
		gotoxy(xPos, yPos); printf("   _.-'_~-_~-_-~-_~_~-_~-_`-._");
	}
	if (houseDestroyOffset <= 12)
	{
		gotoxy(xPos, ++yPos); printf("                                            ");
		gotoxy(xPos, yPos); printf("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	}
	if (houseDestroyOffset <= 11)
	{
		gotoxy(xPos, ++yPos); printf("                             ");
		gotoxy(xPos, yPos); printf("    |  []  []   []   []  [] |");
	}
	if (houseDestroyOffset <= 10)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("    |           __    ___   |");
	}
	if (houseDestroyOffset <= 9)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("  ._|  []  []  | .|  [___]  |_._._._._._._._._._._._._._._._._.");
	}
	if (houseDestroyOffset <= 8)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("  |=|________()|__|()_______|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|");
	}
	if (houseDestroyOffset <= 7)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("^^^^^^^^^^^^^^^ === ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	}
	if (houseDestroyOffset <= 6)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("                ===");
	}
	if (houseDestroyOffset <= 5)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("                ===");
	}
	if (houseDestroyOffset <= 4)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("                ===");
	}
	if (houseDestroyOffset <= 3)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("                ===");
	}
	if (houseDestroyOffset <= 2)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("                ===");
	}
	if (houseDestroyOffset <= 1)
	{
		gotoxy(xPos, ++yPos); printf("                                                                    ");
		gotoxy(xPos, yPos); printf("                ===");
	}

	if (yStart < 17)
		textbackground(CYAN);
	else
		textbackground(GREEN);

	gotoxy(xPos, yStart); printf("                                                                    ");
	textbackground(GREEN);
	gotoxy(xPos, yPos); printf("                                                                    ");

	textbackground(BLACK);
}

void PrintPlayer(int xPos, int yPos)
{
	textbackground(GREEN);
	gotoxy(xPos, yPos);   printf("●");
	gotoxy(xPos, yPos + 1); printf("■");
	textcolor(BROWN);
	gotoxy(xPos + 2, yPos);   printf("_");
	gotoxy(xPos + 2, yPos + 1); printf("|");
	textbackground(BLACK);
}
