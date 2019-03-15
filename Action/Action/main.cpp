#include "main.h"
#include "player.h"
#include "block.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "wattConsumer.h"
#include "soundManager.h"
#include "menu.h"
#include "conioex.h"

void Init(void);
void Uninit(void);
void Update(void);
void Draw(void);

#ifdef _DEBUG
void DispDebug();
#endif

int g_nCountFPS;				// FPS�J�E���^
bool init;

void main(void)
{
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	int nCountFrame;

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime =	nCountFrame = 0;

	// init only once
	char title[] = "��������d�C��";
	setcaption(title);
	InitMenu();
	InitSoundManager();

	// init main game logic
	Init();

	do
	{
		dwCurrentTime = timeGetTime();
		if((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
		{
			g_nCountFPS = nCountFrame * 1000 / (dwCurrentTime - dwFPSLastTime);
			dwFPSLastTime = dwCurrentTime;
			nCountFrame = 0;
		}

		if((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))
		{
			dwExecLastTime = dwCurrentTime;

			if (GetCurMenu() == Menu_Gameloop)
			{
				// init once every reset
				if(!init)
					Init();

				// update game
				Update();

				// draw game, skip if game is finished 
				if(GetCurMenu() == Menu_Gameloop)
					Draw();
			}
			else
			{
				// show current menu
				UpdateMenu();
				DrawMenu();
			}

#ifdef _DEBUG
			DispDebug();
#endif

			nCountFrame++;
		}
	} while(!inport(PK_ESC));

	Uninit();

	// ����\��߂�
	timeEndPeriod(1);
}

void Init(void)
{
	setcursortype(0);
	InitPlayer();
	initBlock();
	InitBullet();
	InitExplosion();
	InitWattConsumer();
	init = true;
}

void Uninit(void)
{
	UninitPlayer();
	uninitBlock();
	UninitBullet();
	UninitExplosion();
	UninitWattConsumer();
}

void Update(void)
{
	updateBlock();
	UpdatePlayer();
	UpdateBullet();
	UpdateExplosion();
	UpdateWattConsumer();
}

void Draw(void)
{
	DrawPlayer();
	DrawWattConsumer();
	drawBlock();
	DrawBullet();
	DrawExplosion();
}

bool *GetInit()
{
	return &init;
}

#ifdef _DEBUG
void DispDebug()
{
	//gotoxy(1, 1);
	//printf("%dFPS, xPos: %.0f, yPos: %.0f, isMoving: %d, isJumping: %d ", g_nCountFPS, getPlayer()->xPos, getPlayer()->yPos, getPlayer()->isMoving, getPlayer()->isJumping);
}
#endif