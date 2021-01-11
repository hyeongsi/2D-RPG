#include "pch.h"
#include "Character.h"

Character::Character()
{
	pos = START_POS;

	maxHp = MAX_HP;
	hp = MAX_HP;

	maxLevel = 99;
	level = 1;

	damage = INIT_DAMAGE;
}

Character::Character(const POINT pos, const int hp, const int level, const int damage)
{
	this->pos = pos;

	maxHp = MAX_HP;
	this->hp = hp;

	maxLevel = 99;
	this->level = level;

	this->damage = damage;
}

Character::~Character()
{

}

const POINT Character::GetPos()
{
	return pos;
}

const int Character::GetHp()
{
	return hp;
}

const int Character::GetLevel()
{
	return level;
}

const int Character::GetDamage()
{
	return damage;
}
