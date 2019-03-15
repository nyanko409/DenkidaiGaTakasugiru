#ifndef _ENEMY_H_	// ２重インクルード防止のマクロ定義
#define _ENEMY_H_

#define MAX_ENEMY 100

enum EnemyState
{
	EnemyState_None,
	EnemyState_Normal,
	EnemyState_Damage,
	EnemyState_Death
};

// enemy struct
struct Enemy
{
	float xPos;
	float yPos;
	float xPosOld;
	float yPosOld;
	float xSpeed;
	float ySpeed;

	EnemyState state;

	int stateCount;
	int life;
	int xVector;

	bool isActive;
};

// define functions
void InitEnemy();
void UninitEnemy();
void UpdateEnemy();
void DrawEnemy();
void SetEnemy(float xPos, float yPos, int lifePoint);
Enemy* GetEnemy();
void ApplyDamage(int enemyIndex, int damageToApply);

#endif