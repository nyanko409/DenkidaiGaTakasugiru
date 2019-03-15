#ifndef _WATTCONSUMER_H_
#define _WATTCONSUMER_H_

#define MAX_CONSUMER 5

// watt consumer struct
struct WattConsumer
{
	int xPos;
	int yPos;

	int hp;
	int curHp;

	float consumingWatt;
	float currentWatt;

	bool isUsing;

	bool isRespawning;
	float respawnTimer;
	float remainingRespawnTime;
};

// define functions
void InitWattConsumer();
void UninitWattConsumer();
void UpdateWattConsumer();
void DrawWattConsumer();
void SetWattConsumer(int xPos, int yPos, int hp, float consumingWatt, float respawnTime = 10.0f);
void ApplyDamageConsumer(int index, int damage);
WattConsumer *GetConsumer();
float GetTotalConsumption();

#endif
