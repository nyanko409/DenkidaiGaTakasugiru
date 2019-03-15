#ifndef _PLAYER_H_	// �Q�d�C���N���[�h�h�~�̃}�N����`
#define _PLAYER_H_

#include "block.h"

// player struct
struct Player
{
	Block *pBlock;
	bool isMoving;
	bool isJumping;
	bool isOnMovingPlatform;
	bool isShooting;
	int shotCount;
	int xVector;
	float xPos;
	float yPos;
	float xOld;
	float yOld;
	float moveX;
	float moveY;
	float money;
};

// define functions
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* getPlayer();
float GetCurrentDate();
bool CheckCollisionBlock();

#endif