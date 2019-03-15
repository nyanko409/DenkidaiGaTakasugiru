#include "main.h"
#include "explosion.h"
#define CONIOEX
#include "conioex.h"

#define MAX_EXPLOSION 100

Explosion g_Explosion[MAX_EXPLOSION];


void InitExplosion()
{
	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		g_Explosion[i].xPos = 0;
		g_Explosion[i].yPos = 0;
		g_Explosion[i].counter = 0;
		g_Explosion[i].isUsing = false;
		g_Explosion[i].type = ExplosionType_None;
	}
}

void UninitExplosion()
{

}

void UpdateExplosion()
{
	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_Explosion[i].isUsing)
		{
			g_Explosion[i].counter++;
			if (g_Explosion[i].counter > 24)
			{
				gotoxy(g_Explosion[i].xPos, g_Explosion[i].yPos);
				printf(" ");
				g_Explosion[i].isUsing = false;
			}
		}
	}
}

void DrawExplosion()
{
	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_Explosion[i].isUsing)
		{
			// set explosion color
			if (g_Explosion[i].type == ExplosionType_Bullet)
				textcolor(RED);
			else
				textcolor(YELLOW);

			// draw explosion
			gotoxy(g_Explosion[i].xPos, g_Explosion[i].yPos);
			if (g_Explosion[i].counter < 6) printf("œ");
			else if (g_Explosion[i].counter < 12) printf("›");
			else if (g_Explosion[i].counter < 18) printf("–");
			else printf("E");
		}
	}

	textcolor(WHITE);
}

void SetExplosion(float xPos, float yPos, ExplosionType type)
{
	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (!g_Explosion[i].isUsing)
		{
			g_Explosion[i].xPos = xPos;
			g_Explosion[i].yPos = yPos;
			g_Explosion[i].counter = 0;
			g_Explosion[i].type = type;
			g_Explosion[i].isUsing = true;

			break;
		}
	}
}