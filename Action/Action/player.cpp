#include "main.h"
#include "player.h"
#include "bullet.h"
#include "menu.h"
#include "wattConsumer.h"
#include "soundManager.h"
#define CONIOEX
#include "conioex.h"

#define MOVESPEED 30.0f
#define JUMPHEIGHT -2.0f
#define GRAVITY .2f
#define FIRE_RATE .25f
#define STARTING_MONEY 1000.0f


Player g_Player;
float payPerSec;
float currentDate;
float datePerSecond;

char playerSound[20] = { };
char playerBGM[20] = { };
bool bgmStartedGame;


void InitPlayer(void)
{
	g_Player.pBlock = NULL;

	g_Player.isMoving = false;
	g_Player.isJumping = false;
	g_Player.isOnMovingPlatform = false;
	g_Player.isShooting = false;

	g_Player.shotCount = 0;
	g_Player.xVector = 1;

	g_Player.xPos = 20.0f;
	g_Player.yPos = 24.0f;
	g_Player.xOld = 0.0f;
	g_Player.yOld = 0.0f;

	g_Player.moveX = 0.0f;
	g_Player.moveY = 0.0f;

	g_Player.money = STARTING_MONEY;

	payPerSec = 0.0f;
	currentDate = 1.0f;
	datePerSecond = 0.2f * TIME_PER_FRAME;

	bgmStartedGame = false;
}

void UninitPlayer(void)
{
}

void UpdatePlayer(void)
{
	// start bgm
	if (!bgmStartedGame)
	{
		stopBGM();
		strcpy(playerBGM, SOUND_INGAMEBGM);
		playBGM(playerBGM, true, 25);
		bgmStartedGame = true;
	}
	updateBGM();

	// update old position of player
	g_Player.xOld = g_Player.xPos;
	g_Player.yOld = g_Player.yPos;

	// move player
	if (inport(PK_A) || inport(PK_LEFT))
	{
 		g_Player.moveX -= MOVESPEED * TIME_PER_FRAME;
		g_Player.xVector = -1;
	}
	if (inport(PK_D) || inport(PK_RIGHT))
	{
		g_Player.moveX += MOVESPEED * TIME_PER_FRAME;
		g_Player.xVector = 1;
	}

	// shoot bullet
	if (inport(PK_B))
	{
		if (!g_Player.isShooting)
		{
			if(g_Player.xVector >= 1)
				SetBullet(g_Player.xPos + 1.0f, g_Player.yPos, 30.0f * TIME_PER_FRAME, 0.0f);
			else
				SetBullet(g_Player.xPos - 1.0f, g_Player.yPos, -30.0f * TIME_PER_FRAME, 0.0f);
			
			g_Player.isShooting = true;
		}

		g_Player.shotCount++;
		if (g_Player.shotCount >= FIRE_RATE * FPS)
		{
			g_Player.isShooting = false;
			g_Player.shotCount = 0;
		}
	}
	else
	{
		if (g_Player.isShooting)
		{
			g_Player.isShooting = false;
			g_Player.shotCount = 0;
		}
	}

	// set jumpheight and play sound
	if (inport(PK_SP))
	{
		if (!g_Player.isJumping)
		{
			stopSound();
			strcpy(playerSound, SOUND_PLAYER_JUMP);
			playSoundM(playerSound, 25);
			g_Player.moveY = JUMPHEIGHT;
			g_Player.isJumping = true;
 		}
	}

	// set gravity
	g_Player.moveY += GRAVITY;
	g_Player.yPos += g_Player.moveY;

	// acceleration
	g_Player.moveX *= .6f;
	g_Player.xPos += g_Player.moveX;

	// move player to the other side when out of screen
	if (g_Player.xPos < 1.0f)
		g_Player.xPos = 119.0f;

	if (g_Player.xPos > 119.0f)
		g_Player.xPos = 1.0f;

	// block collision
	if (CheckCollisionBlock())
	{
		g_Player.moveY = 0.0f;
		g_Player.isJumping = false;
		g_Player.isOnMovingPlatform = true;
	}
	else
	{
		if (g_Player.isOnMovingPlatform)
		{
			g_Player.isJumping = true;
			g_Player.isOnMovingPlatform = false;
		}
	}

	// set flag if player is moving
	g_Player.isMoving = (g_Player.xOld != g_Player.xPos) || (g_Player.yOld != g_Player.yPos);

	// move player with moving platform
	if (g_Player.pBlock != NULL)
	{
		g_Player.xPos += (g_Player.pBlock->posX - g_Player.pBlock->oldX);
		g_Player.yPos += (g_Player.pBlock->posY - g_Player.pBlock->oldY);
	}


	// calculate current date
	currentDate += datePerSecond;

	// calculate remaining money
	g_Player.money -= (GetTotalConsumption() / 2) * TIME_PER_FRAME;
	payPerSec = (GetTotalConsumption() / 2);

	if (g_Player.money <= 0.0f)
	{
		// show result when money is depleted
		stopBGM();
		*GetMenuPointer() = Menu_Result;
		clrscr();
	}
}

void DrawPlayer(void)
{
	// Žc‘œ‚ðÁ‚·
	if (g_Player.isMoving)
	{
		gotoxy(g_Player.xOld, g_Player.yOld);
		printf(" ");
		gotoxy(g_Player.xOld, g_Player.yOld - 1);
		printf(" ");

		gotoxy(g_Player.xOld + 2, g_Player.yOld);
		printf(" ");
		gotoxy(g_Player.xOld + 2, g_Player.yOld - 1);
		printf(" ");
		gotoxy(g_Player.xOld - 1, g_Player.yOld);
		printf(" ");
		gotoxy(g_Player.xOld - 1, g_Player.yOld - 1);
		printf(" ");
	
	}

	// draw player
	textcolor(LIGHTBLUE);
	gotoxy(g_Player.xPos, g_Player.yPos);
	printf("¡");
	gotoxy(g_Player.xPos, g_Player.yPos - 1);
	printf("œ");

	//draw weapon
	textcolor(BROWN);
	if (g_Player.xVector == 1)
	{
		gotoxy(g_Player.xPos + 2, g_Player.yPos);
		printf("|");
		gotoxy(g_Player.xPos + 2, g_Player.yPos - 1);
		printf("_");
	}
	else
	{
		gotoxy(g_Player.xPos - 1, g_Player.yPos);
		printf("|");
		gotoxy(g_Player.xPos - 1, g_Player.yPos - 1);
		printf("_");
	}
	textcolor(WHITE);

	// draw money and date
	gotoxy(2, 33);
	printf("Žc‚è‚ÌàŽY: %.0f\\ (%.0f\\/s)  ", g_Player.money, payPerSec);

	gotoxy(58, 31);
	printf("“ú•t");
	gotoxy(57, 33);
	printf("1ŒŽ%.0f“ú                                                          ", currentDate);
}

Player* getPlayer()
{
	return &g_Player;
}

float GetCurrentDate()
{
	return currentDate;
}

bool CheckCollisionBlock()
{
	Block *pBlock = GetBlock();

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (pBlock[i].isUsing) // (pBlock + i)->isUsing
		{
			// bounding box collision (feet)
			if ((g_Player.xPos + .9f) > (pBlock + i)->posX &&
				g_Player.xPos < ((pBlock + i)->posX + (pBlock + i)->width - .1f))
			{
				if (g_Player.yPos > ((pBlock + i)->posY - 1.0f) &&
					g_Player.yOld <= ((pBlock + i)->posY - 1.0f))
				{
					g_Player.yPos = (pBlock + i)->posY - 1.0f;
					g_Player.pBlock = (pBlock + i);
					return true;
				}
			}

			// bounding box collision (head)
			if ((g_Player.xPos + .9f) > (pBlock + i)->posX &&
				g_Player.xPos < ((pBlock + i)->posX + (pBlock + i)->width - .1f))
			{
				if (g_Player.yPos - 2.0f < (pBlock + i)->posY &&
					g_Player.yOld - 2.0f >= (pBlock + i)->posY)
				{
					g_Player.yPos = (pBlock + i)->posY + 2.0f;
					g_Player.moveY = 0.0f;
				}
			}
		}
	}
	return false;
}
