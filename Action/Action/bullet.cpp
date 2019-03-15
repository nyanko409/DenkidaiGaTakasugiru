#include "main.h"
#include "bullet.h"
#include "wattConsumer.h"
#include "enemy.h"
#include "explosion.h"
#include "soundManager.h"
#define CONIOEX
#include "conioex.h"

// constant
#define MAX_BULLET 100
#define HITSTOP_MSTIME 25 

// global variables
Bullet g_Bullets[MAX_BULLET];
char sound[20] = {0};

void InitBullet()
{
	// init values
	for (int i = 0; i < MAX_BULLET; i++)
	{
		g_Bullets[i].xPos = 0.0f;
		g_Bullets[i].yPos = 0.0f;
		g_Bullets[i].xPosOld = 0.0f;
		g_Bullets[i].yPosOld = 0.0f;
		g_Bullets[i].xSpeed = 0.0f;
		g_Bullets[i].ySpeed = 0.0f;
		g_Bullets[i].isUsing = false;
	}

	strcpy(sound, SOUND_CONSUMER_HIT);
}

void UninitBullet()
{

}

void UpdateBullet()
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullets[i].isUsing)
		{
			// set last position of the bullet
			g_Bullets[i].xPosOld = g_Bullets[i].xPos;
			g_Bullets[i].yPosOld = g_Bullets[i].yPos;

			// move bullet by its current speed
			g_Bullets[i].xPos += g_Bullets[i].xSpeed;
			g_Bullets[i].yPos += g_Bullets[i].ySpeed;

			// check if bullet hit enemy
			/*Enemy *pEnemy = GetEnemy();
			//for (int j = 0; j < MAX_ENEMY; j++)
			{
				if (pEnemy[j].isActive)
				{
					// if hit from the either side
					if ((g_Bullets[i].xPosOld < pEnemy[j].xPos &&
						g_Bullets[i].xPos >= pEnemy[j].xPos &&
						g_Bullets[i].yPos >= pEnemy[j].yPos - 1.0f &&
						g_Bullets[i].yPos < pEnemy[j].yPos + 1.0f) || (g_Bullets[i].xPosOld > pEnemy[j].xPos &&
							g_Bullets[i].xPos <= pEnemy[j].xPos &&
							g_Bullets[i].yPos >= pEnemy[j].yPos - 1.0f &&
							g_Bullets[i].yPos < pEnemy[j].yPos + 1.0f))
					{
						// delete bullet if hit with enemy
						gotoxy(g_Bullets[i].xPosOld, g_Bullets[i].yPosOld);

						printf(" ");
						g_Bullets[i].isUsing = false;
						ApplyDamage(j, 1);
						SetExplosion(pEnemy[j].xPos, pEnemy[j].yPos, ExplosionType_Bullet);
					}
				}
			} */

			// check if bullet hit with active watt consumer
			WattConsumer *pConsumer = GetConsumer();
			for (int j = 0; j < MAX_CONSUMER; j++)
			{
				if (pConsumer[j].isUsing && !pConsumer[j].isRespawning)
				{
					// if hit from either side
					if ((g_Bullets[i].xPosOld < pConsumer[j].xPos && g_Bullets[i].xPos >= pConsumer[j].xPos &&
						g_Bullets[i].yPos >= pConsumer[j].yPos - 0.0f && g_Bullets[i].yPos < pConsumer[j].yPos + 3.0f) || 
						(g_Bullets[i].xPosOld > pConsumer[j].xPos && g_Bullets[i].xPos <= pConsumer[j].xPos &&
						g_Bullets[i].yPos >= pConsumer[j].yPos - 0.0f &&  g_Bullets[i].yPos < pConsumer[j].yPos + 3.0f))
					{
						// delete bullet if hit with consumer
						gotoxy(g_Bullets[i].xPosOld, g_Bullets[i].yPosOld);

						printf(" ");
						g_Bullets[i].isUsing = false;

						// apply damage, set explosion and sound
						ApplyDamageConsumer(j, 1);
						if (pConsumer[j].curHp > 0)
						{
							//playSound(sound, 100);

							if(g_Bullets[i].yPos >= pConsumer[j].yPos &&  g_Bullets[i].yPos <= pConsumer[j].yPos + 1.0f)
								SetExplosion(pConsumer[j].xPos, pConsumer[j].yPos, ExplosionType_Bullet);
							else if (g_Bullets[i].yPos >= pConsumer[j].yPos + 1.0f &&  g_Bullets[i].yPos <= pConsumer[j].yPos + 1.99f)
								SetExplosion(pConsumer[j].xPos, pConsumer[j].yPos + 1, ExplosionType_Bullet);
							else if (g_Bullets[i].yPos >= pConsumer[j].yPos + 2.0f &&  g_Bullets[i].yPos <= pConsumer[j].yPos + 3.0f)
								SetExplosion(pConsumer[j].xPos, pConsumer[j].yPos + 2, ExplosionType_Bullet);
						}
						else
						{
							// destroyed, apply explosion on all three pos
							SetExplosion(pConsumer[j].xPos, pConsumer[j].yPos, ExplosionType_Bullet);
							SetExplosion(pConsumer[j].xPos, pConsumer[j].yPos + 1, ExplosionType_Bullet);
							SetExplosion(pConsumer[j].xPos, pConsumer[j].yPos + 2, ExplosionType_Bullet);
						}

						// hitstop
						msleep(HITSTOP_MSTIME);
					}
				}
			}

			// when out of screen set flag to false 
			if (g_Bullets[i].xPos < 2.0f || g_Bullets[i].xPos > 119 ||
				g_Bullets[i].yPos < 2.0f || g_Bullets[i].yPos > 29)
			{
				g_Bullets[i].isUsing = false;
				gotoxy(g_Bullets[i].xPosOld, g_Bullets[i].yPosOld);
				printf(" ");
			}
		}
	}
}

void DrawBullet()
{
	// set bullet color
	textcolor(RED);

	// delete and draw bullet at new position
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullets[i].isUsing)
		{
			// delete bullet on last position
			gotoxy(g_Bullets[i].xPosOld, g_Bullets[i].yPosOld);
			printf(" ");

			// draw bullet
			gotoxy(g_Bullets[i].xPos, g_Bullets[i].yPos);
			printf("œ");
		}
	}

	// reset color
	textcolor(WHITE);
}

void SetBullet(float xPos, float yPos, float xSpeed, float ySpeed)
{
	// set bullet properties
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (!g_Bullets[i].isUsing)
		{
			g_Bullets[i].xPos = xPos;
			g_Bullets[i].yPos = yPos;
			g_Bullets[i].xSpeed = xSpeed;
			g_Bullets[i].ySpeed = ySpeed;
			g_Bullets[i].isUsing = true;
			break;
		}
	}
}

Bullet* GetBullet()
{
	return g_Bullets;
}