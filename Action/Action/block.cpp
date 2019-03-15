#include "main.h"
#include "block.h"
#define CONIOEX
#include "conioex.h"

// global variables
Block g_Block[MAX_BLOCK];


void initBlock()
{
	// init values
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		g_Block[i].count = 0;
		g_Block[i].rCount = 0;
		g_Block[i].posX = 0.0f;
		g_Block[i].posY = 0.0f;
		g_Block[i].oldX = 0.0f;
		g_Block[i].oldY = 0.0f;
		g_Block[i].moveX = 0.0f;
		g_Block[i].moveY = 0.0f;
		g_Block[i].width = 0.0f;
		g_Block[i].isUsing = false;
		g_Block[i].isMoving = false;
		g_Block[i].type = blockNone;
	}

	// level layout
	setBlock(1.0f, 29.0f, 119.0f);

	setBlock(1.0f, 23.0f, 10.0f);
	setBlock(111.0f, 23.0f, 10.0f);

	setBlock(52.0f, 21.0f, 16.0f);

	setBlock(18.0f, 18.0f, 30.0f);
	setBlock(72.0f, 18.0f, 30.0f);

	setBlock(1.0f, 10.0f, 40.0f);
	setBlock(81.0f, 10.0f, 40.0f);

	setBlock(1.0f, 2.0f, 119.0f);
}

void uninitBlock()
{

}

void updateBlock()
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		// set last position of the block
		if (g_Block[i].isUsing)
		{
			g_Block[i].oldX = g_Block[i].posX;
			g_Block[i].oldY = g_Block[i].posY;
		}

		// move horizontal and vertical block
		if (g_Block[i].type == blockHmove || g_Block[i].type == blockVmove)
		{
			(g_Block[i].type == blockHmove) ? g_Block[i].posX += g_Block[i].moveX : g_Block[i].posY += g_Block[i].moveY;
			g_Block[i].count++;

			// reverse block after rCount is reached
			if (g_Block[i].count >= g_Block[i].rCount)
			{
				(g_Block[i].type == blockHmove) ? g_Block[i].moveX *= -1.0f : g_Block[i].moveY *= -1.0f;
				g_Block[i].count = 0;
			}
		}

		// set isMoving flag
		g_Block[i].isMoving = g_Block[i].oldX != g_Block[i].posX || g_Block[i].oldY != g_Block[i].posY;
	}
}

void drawBlock()
{
	// set block color
	textcolor(DARKGRAY);

	// delete and draw block at new position
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (g_Block[i].isUsing)
		{
			char buf[121];
			strcpy(buf, "\0");

			// delete old moving block
			if (g_Block[i].isMoving)
			{
				gotoxy(g_Block[i].oldX, g_Block[i].oldY);
				for (int j = 0; j < g_Block[i].width / 2; j++)
				{
					printf("   ");
				}
			}

			// draw block
			gotoxy(g_Block[i].posX, g_Block[i].posY);
			for (int j = 0; j < g_Block[i].width / 2; j++)
			{
				strcat(buf, "¡");
			}

			printf("%s", buf);
		}
	}

	// reset color
	textcolor(WHITE);
}

void setBlock(float posX, float posY, float width, BlockType type, float moveX, float moveY, int rCount)
{
	// set block properties
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (!g_Block[i].isUsing)
		{
			g_Block[i].posX = posX;
			g_Block[i].posY = posY;
			g_Block[i].width = width;
			g_Block[i].moveX = moveX;
			g_Block[i].moveY = moveY;
			g_Block[i].isUsing = true;
			g_Block[i].type = type;
			g_Block[i].rCount = rCount;
			break;
		}
	}
}

Block* GetBlock()
{
	return g_Block;
}