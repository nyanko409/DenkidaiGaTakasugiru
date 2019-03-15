#include "main.h"
#include "wattConsumer.h"
#include "player.h"
#include "soundManager.h"
#define CONIOEX
#include "conioex.h"

WattConsumer g_Consumer[MAX_CONSUMER];
float respawnSpeed = 0.5f;
float totalConsumption;

char consumerSound[21] = { };

void InitWattConsumer()
{
	// init
	for (int i = 0; i < MAX_CONSUMER; i++)
	{
		g_Consumer[i].xPos = 0;
		g_Consumer[i].yPos = 0;
		g_Consumer[i].hp = 0;
		g_Consumer[i].curHp = 0;
		g_Consumer[i].consumingWatt = 0.0f;
		g_Consumer[i].currentWatt = 0.0f;
		g_Consumer[i].isUsing = false;
		g_Consumer[i].isRespawning = false;
		g_Consumer[i].respawnTimer = 0.0f;
		g_Consumer[i].remainingRespawnTime = 0.0f;
	}

	totalConsumption = 0.0f;

	// make respawn speed fps independent
	respawnSpeed *= TIME_PER_FRAME * 10;

	// place consumer
	SetWattConsumer(10, 7, 10, 100.0f);
	SetWattConsumer(110, 7, 10, 100.0f);

	SetWattConsumer(59, 18, 10, 100.0f);

	SetWattConsumer(10, 26, 10, 100.0f);
	SetWattConsumer(110, 26, 10, 100.0f);
}

void UninitWattConsumer()
{

}

void UpdateWattConsumer()
{
	totalConsumption = 0.0f;

	for (int i = 0; i < MAX_CONSUMER; i++)
	{
		if (g_Consumer[i].isUsing)
		{
			if (g_Consumer[i].isRespawning)
			{
				// calculate respawn timer
				g_Consumer[i].remainingRespawnTime -= respawnSpeed;

				if (g_Consumer[i].remainingRespawnTime <= 0.0f)
				{
					// respawn consumer with set values
					g_Consumer[i].curHp = g_Consumer[i].hp;
					g_Consumer[i].currentWatt = g_Consumer[i].consumingWatt;
					g_Consumer[i].isRespawning = false;
				}
			}
			else
			{
				// increase watt per second if not destroyed
				g_Consumer[i].currentWatt += GetCurrentDate() * TIME_PER_FRAME;

				// calculate total watt per hour
				totalConsumption += g_Consumer[i].currentWatt;
			}
		}
	}
}

void DrawWattConsumer()
{
	for (int i = 0; i < MAX_CONSUMER; i++)
	{
		if (g_Consumer[i].isUsing && !g_Consumer[i].isRespawning)
		{
			// set color
			textcolor(LIGHTCYAN);
			if (g_Consumer[i].currentWatt >= 150)
				textcolor(YELLOW);
			if (g_Consumer[i].currentWatt >= 200)
				textcolor(LIGHTRED);

			// draw watt consumer
			gotoxy(g_Consumer[i].xPos - 1, g_Consumer[i].yPos - 1);
			printf("%.0f\W", g_Consumer[i].currentWatt);
			gotoxy(g_Consumer[i].xPos, g_Consumer[i].yPos);
			printf("Åü");
			gotoxy(g_Consumer[i].xPos, g_Consumer[i].yPos + 1);
			printf("Å°");
			gotoxy(g_Consumer[i].xPos, g_Consumer[i].yPos + 2);
			printf("Å°");
		}

	}

		// print total watt/h
		gotoxy(2, 31);
		textcolor(LIGHTCYAN);
		if (totalConsumption >= 1000)
			textcolor(YELLOW);
		if (totalConsumption >= 1500)
			textcolor(LIGHTRED);
		printf("è¡îÔìdóÕ: %.0f\Wh ", totalConsumption);
		textcolor(WHITE);
}

void SetWattConsumer(int xPos, int yPos, int hp, float consumingWatt, float respawnTime)
{
	for (int i = 0; i < MAX_CONSUMER; i++)
	{
		if (!g_Consumer[i].isUsing)
		{
			g_Consumer[i].xPos = xPos;
			g_Consumer[i].yPos = yPos;
			g_Consumer[i].hp = hp;
			g_Consumer[i].curHp = hp;
			g_Consumer[i].consumingWatt = consumingWatt;
			g_Consumer[i].currentWatt = consumingWatt;
			g_Consumer[i].respawnTimer = respawnTime;
			g_Consumer[i].isUsing = true;

			break;
		}
	}
}

void ApplyDamageConsumer(int index, int damage)
{
	// reduce hp
	g_Consumer[index].curHp--;

	// destroy and set respawn timer
	if (g_Consumer[index].curHp <= 0)
	{
		g_Consumer[index].isRespawning = true;
		g_Consumer[index].remainingRespawnTime = g_Consumer[index].respawnTimer;

		// play sound
		stopSound();
		strcpy(consumerSound, SOUND_CONSUMER_HIT);
		playSoundM(consumerSound, 50);

		// delete afterimage
		gotoxy(g_Consumer[index].xPos - 1, g_Consumer[index].yPos - 1);
		printf("    ");
		gotoxy(g_Consumer[index].xPos, g_Consumer[index].yPos);
		printf(" ");
		gotoxy(g_Consumer[index].xPos, g_Consumer[index].yPos + 1);
		printf(" ");
		gotoxy(g_Consumer[index].xPos, g_Consumer[index].yPos + 2);
		printf(" ");
	}
}

WattConsumer *GetConsumer()
{
	return g_Consumer;
}

float GetTotalConsumption()
{
	return totalConsumption;
}