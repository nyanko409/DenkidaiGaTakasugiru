#ifndef _BLOCK_H_
#define _BLOCK_H_

// constants
#define MAX_BLOCK 40

// blocktype enum
enum BlockType
{
	blockNone,
	blockNormal,
	blockHmove,
	blockVmove,
	blockTypemax
};

// block struct
struct Block
{
	int count;
	int rCount;
	float posX;
	float posY;
	float oldX;
	float oldY;
	float moveX;
	float moveY;
	float width;
	bool isUsing;
	bool isMoving;
	BlockType type;
};

// define functions
void initBlock();
void uninitBlock();
void updateBlock();
void drawBlock();
void setBlock(float posX, float posY, float Width, BlockType Type = blockNormal, float XSpeed = 0.0f, float YSpeed = 0.0f, int Reverse = 15);
Block* GetBlock();

#endif