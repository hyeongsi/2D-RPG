#include "pch.h"
#include "SoundManager.h"
#include "DataStruct.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

SoundManager* SoundManager::instance = nullptr;

SoundManager* SoundManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SoundManager();

		mciSendString("open data/Sound/getItem.wav alias getItem", 0, 0, 0);
		mciSendString("open data/Sound/moneySound.wav alias sellItem", 0, 0, 0);
		mciSendString("open data/Sound/damage.wav alias hitCharacter", 0, 0, 0);
		mciSendString("open data/Sound/swingSword.wav alias swingSword", 0, 0, 0);
		mciSendString("open data/Sound/ppusic.wav alias explodeCharacter", 0, 0, 0);
		mciSendString("open data/Sound/drinkwater.mp3 alias drink", 0, 0, 0);
	}

	return instance;
}

void SoundManager::ReleaseInstance()
{
	delete instance;
}

void SoundManager::PlaySoundTrack(BGM bgm)
{
	switch (bgm)
	{
	case BGM::MAIN_MENU:
		PlaySound("data/Sound/mainBGM.wav", NULL, SND_ASYNC | SND_LOOP);
		break;
	case BGM::INGAME:
		PlaySound("data/Sound/inGameBGM.wav", NULL, SND_ASYNC | SND_LOOP);
		break;
	default:
		PlaySound(NULL, NULL, SND_ASYNC);
		break;
	}
}

void SoundManager::PlayEffectSound(EFFECTSOUND effectSound)
{
	switch (effectSound)
	{
	case EFFECTSOUND::ATTACK:
		mciSendString("play swingSword from 0", 0, 0, 0);
		break;
	case EFFECTSOUND::HIT:
		mciSendString("play hitCharacter from 0", 0, 0, 0);
		break;
	case EFFECTSOUND::MONSTER_HIT:
		mciSendString("play explodeCharacter from 0", 0, 0, 0);
		break;
	case EFFECTSOUND::USE_POSION:
		mciSendString("play drink from 0", 0, 0, 0);
		break;
	case EFFECTSOUND::GET_ITEM:
		mciSendString("play getItem from 0", 0, 0, 0);
		break;
	case EFFECTSOUND::SELL_ITEM:
		mciSendString("play sellItem from 0", 0, 0, 0);
		break;
	}
}

