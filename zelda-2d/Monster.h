#pragma once
#include "Character.h"

class Monster : public Character
{
private:
	int exp;

	friend class MonsterManager;
public:
	Monster();
	Monster(const DPOINT pos, const int hp,  const int speed, const int damage, const int exp);

	const int GetExp();
};

