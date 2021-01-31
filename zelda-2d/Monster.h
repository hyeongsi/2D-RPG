#pragma once
#include "Character.h"

class Monster : public Character
{
private:
	int index;
	int exp;

	friend class MonsterManager;
public:
	Monster();
	Monster(const DPOINT pos, const int hp,  const int speed, const int damage, const int index, const int exp);

	const int GetIndex();
	void SetIndex(const int index);

	const int GetExp();
};

