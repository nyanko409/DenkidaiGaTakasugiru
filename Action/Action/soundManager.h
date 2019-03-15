#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

// link to sound file
#define SOUND_MENUBGM "_sfx/MenuBGM.wav"
#define SOUND_INGAMEBGM "_sfx/IngameBGM.wav"
#define SOUND_CONSUMER_HIT "_sfx/ConsumerHit.wav"
#define SOUND_PLAYER_JUMP "_sfx/Jump.wav"
#define SOUND_DEFEAT "_sfx/Defeat.wav"
#define SOUND_TEXTBEEP "_sfx/TextBeep.wav"

// define functions
void InitSoundManager();
void playBGM(char *, bool, int);
void playSoundM(char *, int);
void stopBGM();
void stopSound();
void updateBGM();

#endif