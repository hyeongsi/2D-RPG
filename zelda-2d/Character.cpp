#include "pch.h"
#include "Character.h"

Character::Character()
{
	pos = START_POS;

	state = CharacterInfo::IDLE;
	dir = CharacterInfo::DOWN;

	maxHp = INIT_HP;
	hp = INIT_HP;

	speed = INIT_SPEED;
	damage = INIT_DAMAGE;

	money = 0;
}

Character::Character(const DPOINT pos, const int hp, const int speed, const int damage)
{
	this->pos = pos;

	state = CharacterInfo::IDLE;
	dir = CharacterInfo::DOWN;

	maxHp = hp;
	this->hp = hp;

	this->speed = speed;
	this->damage = damage;

	money = 0;
}

Character::~Character()
{
}

const DPOINT Character::GetPos()
{
	return pos;
}

void Character::SetPos(const DPOINT pos)
{
	this->pos = pos;
}

const int Character::GetHp()
{
	return hp;
}

const int Character::GetDamage()
{
	return damage;
}

const int Character::GetState()
{
	return state;
}

void Character::SetState(const int state)
{
	this->state = state;
}

const int Character::GetDir()
{
	return dir;
}

const int Character::GetMoney()
{
	return money;
}
