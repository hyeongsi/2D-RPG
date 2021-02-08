#pragma once
enum class BGM
{
	MAIN_MENU,
	INGAME,
	STOP,
};

enum class EFFECTSOUND
{
	ATTACK,
	HIT,
	MONSTER_HIT,
	GET_ITEM,
	SELL_ITEM,
	USE_POSION
};

class SoundManager
{
private:
	static SoundManager* instance;

	SoundManager() {};
	~SoundManager() {};
public:
	static SoundManager* GetInstance();
	static void ReleaseInstance();

	void PlaySoundTrack(BGM bgm);
	void PlayEffectSound(EFFECTSOUND effectSound);
};