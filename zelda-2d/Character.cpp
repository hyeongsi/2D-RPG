#include "pch.h"
#include "Character.h"

Character::Character()
{
	pos = START_POS;

	state = CharacterInfo::IDLE;
	dir = CharacterInfo::DOWN;

	maxHp = MAX_HP;
	hp = MAX_HP;

	maxLevel = 99;
	level = 1;

	speed = INIT_SPEED;
	damage = INIT_DAMAGE;
}

Character::Character(const DPOINT pos, const int hp, const int level, const int speed, const int damage)
{
	this->pos = pos;

	state = CharacterInfo::IDLE;
	dir = CharacterInfo::DOWN;

	maxHp = MAX_HP;
	this->hp = hp;

	maxLevel = 99;
	this->level = level;

	this->speed = speed;
	this->damage = damage;
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

const int Character::GetLevel()
{
	return level;
}

const int Character::GetDamage()
{
	return damage;
}

const int Character::GetState()
{
	return state;
}

const int Character::GetDir()
{
	return dir;
}

void Character::Input(const double deltaTime)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)			// 상
	{
		pos.y -= speed * deltaTime;
		state = CharacterInfo::WALK;
		dir = CharacterInfo::UP;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)	// 하
	{
		pos.y += speed * deltaTime;
		state = CharacterInfo::WALK;
		dir = CharacterInfo::DOWN;

	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)	// 좌
	{
		pos.x -= speed * deltaTime;
		state = CharacterInfo::WALK;
		dir = CharacterInfo::LEFT;

	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	// 우
	{
		pos.x += speed * deltaTime;
		state = CharacterInfo::WALK;
		dir = CharacterInfo::RIGHT;

	}
	else
	{
		state = CharacterInfo::IDLE;
	}
}

void Character::Update()
{

}
