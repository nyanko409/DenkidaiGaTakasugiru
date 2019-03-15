#include "main.h"
#include "soundManager.h"
#include <string.h>
#define CONIOEX
#include "conioex.h"

//　グローバル変数
int bgmHandle, soundHandle;


void InitSoundManager()
{
	bgmHandle = 0;
	soundHandle = 0;
}

void playBGM(char *file, bool repeat, int volume)
{
	if (bgmHandle == 0)
	{
		bgmHandle = opensound(file);
		setvolume(bgmHandle, volume);
	}

	playsound(bgmHandle, repeat);
}

void stopBGM()
{
	closesound(bgmHandle);
	bgmHandle = 0;
}

void playSoundM(char *file, int volume)
{
	if (soundHandle == 0)
	{
		soundHandle = opensound(file);
		setvolume(soundHandle, volume);
	}

	playsound(soundHandle, false);
}

void stopSound()
{
	closesound(soundHandle);
	soundHandle = 0;
}

void updateBGM()
{
	updatesound(bgmHandle);
}