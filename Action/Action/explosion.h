#ifndef _EXPLOSION_H_
#define _EXPLISION_H_

enum ExplosionType
{
	ExplosionType_None,
	ExplosionType_Bullet,
	ExplosionType_Player,
	ExplosionType_Enemy
};

struct Explosion
{
	float xPos;
	float yPos;
	int counter;
	ExplosionType type;
	bool isUsing;
};

void InitExplosion();
void UninitExplosion();
void UpdateExplosion();
void DrawExplosion();
void SetExplosion(float xPos, float yPos, ExplosionType type);

#endif