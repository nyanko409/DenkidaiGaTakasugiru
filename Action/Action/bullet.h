#ifndef _BULLET_H_	// �Q�d�C���N���[�h�h�~�̃}�N����`
#define _BULLET_H_

// bullet struct
struct Bullet
{
	float xPos;
	float yPos;
	float xPosOld;
	float yPosOld;
	float xSpeed;
	float ySpeed;
	bool isUsing;
};

// define functions
void InitBullet();
void UninitBullet();
void UpdateBullet();
void DrawBullet();
void SetBullet(float xPosition, float yPosition, float xBulletSpeed, float yBulletSpeed);
Bullet* GetBullet();

#endif