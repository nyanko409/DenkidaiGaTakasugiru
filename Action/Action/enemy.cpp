#include "main.h"
#include "enemy.h"
#define CONIOEX
#include "conioex.h"

Enemy g_Enemy[MAX_ENEMY];


void InitEnemy()
{
	// init enemy array
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].xPos = 0.0f;
		g_Enemy[i].yPos = 0.0f;
		g_Enemy[i].xPosOld = 0.0f;
		g_Enemy[i].yPosOld = 0.0f;
		g_Enemy[i].xSpeed = 0.0f;
		g_Enemy[i].ySpeed = 0.0f;

		g_Enemy[i].state = EnemyState_None;

		g_Enemy[i].stateCount = 0;
		g_Enemy[i].life = 0;
		g_Enemy[i].xVector = 0;

		g_Enemy[i].isActive = false;
	}
}

void UninitEnemy()
{

}

void UpdateEnemy()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].isActive)
		{
			// update old position of enemy
			g_Enemy[i].xPosOld = g_Enemy[i].xPos;
			g_Enemy[i].yPosOld = g_Enemy[i].yPos;

			// switch enemy state
			switch (g_Enemy[i].state)
			{
				case EnemyState_Normal:
					break;

				case EnemyState_Damage:
					g_Enemy[i].stateCount--;
					if (g_Enemy[i].stateCount <= 0)
					{
						g_Enemy[i].state = EnemyState_Normal;
					}
					break;

				case EnemyState_Death:
					gotoxy(g_Enemy[i].xPosOld, g_Enemy[i].yPosOld);
					printf(" ");
					gotoxy(g_Enemy[i].xPosOld, g_Enemy[i].yPosOld - 1.0f);
					printf(" ");
					g_Enemy[i].isActive = false;
					break;

				default: 
					break;
			}
		}
	}
}

void DrawEnemy()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].isActive)
		{
			// change enemy color
			if (g_Enemy[i].state == EnemyState_Damage)
				textcolor(LIGHTRED);
			else
				textcolor(BLUE);

			// Žc‘œ‚ðÁ‚·
			gotoxy(g_Enemy[i].xPosOld, g_Enemy[i].yPosOld);
			printf(" ");
			gotoxy(g_Enemy[i].xPosOld, g_Enemy[i].yPosOld - 1);
			printf(" ");

			// draw enemy
			gotoxy(g_Enemy[i].xPos, g_Enemy[i].yPos);
			printf("¥");
			gotoxy(g_Enemy[i].xPos, g_Enemy[i].yPos - 1);
			printf("œ");
		}
	}

	textcolor(WHITE);
}

void SetEnemy(float xPos, float yPos, int life)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!g_Enemy[i].isActive)
		{
			g_Enemy[i].xPos = xPos;
			g_Enemy[i].yPos = yPos;
			g_Enemy[i].xPosOld = xPos;
			g_Enemy[i].yPosOld = yPos;
			g_Enemy[i].life = life;
			g_Enemy[i].xVector = -1;
			g_Enemy[i].state = EnemyState_Normal;
			g_Enemy[i].isActive = true;

			break;
		}
	}
}

Enemy* GetEnemy()
{
	return g_Enemy;
}

void ApplyDamage(int enemyIndex, int damage)
{
	// apply damage
	g_Enemy[enemyIndex].life -= damage;

	// set enemy state depending on life
	if (g_Enemy[enemyIndex].life <= 0)
	{
		g_Enemy[enemyIndex].state = EnemyState_Death;
	}
	else
	{
		g_Enemy[enemyIndex].state = EnemyState_Damage;
		g_Enemy[enemyIndex].stateCount = 4;
	}
}
