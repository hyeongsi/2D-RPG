#pragma once
enum class BGM
{
	MAIN_MENU,
	INGAME,
};

enum class EFFECTSOUND
{
	ATTACK,
	HIT,
	MONSTER_HIT,
	GET_ITEM,
	SELL_ITEM,
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