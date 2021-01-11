#pragma once

constexpr POINT START_POS = { 400,400 };
constexpr int MAX_HP = 6;
constexpr int MAX_LEVEL = 99;
constexpr int INIT_DAMAGE = 1;

class Character
{
private:
	POINT pos;			// À§Ä¡°ª

	int maxHp;			// maxHp
	int hp;				// hp

	int maxLevel;		// maxLevel
	int level;			// level

	int damage;			// damage

public:
	Character();
	Character(const POINT pos, const int hp, const int level, const int damage);
	~Character();

public:
	const POINT GetPos();
	const int GetHp();
	const int GetLevel();
	const int GetDamage();
};

