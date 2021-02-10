#include "pch.h"
#include "Player.h"
#include "Timmer.h"

Player::Player() : Character()
{
	level = 1;
	exp = 0;
	money = 123;
}

Player::Player(const DPOINT pos, const int hp, const int level, const int speed, const int damage) 
	: Character(pos, hp, speed, damage)
{
	this->level = level;
	exp = 0;
	money = 123;
}

Player::~Player()
{
}

const int Player::GetLevel()
{
	return level;
}

void Player::SetExp(const int exp)
{
	this->exp = exp;
}

POINT Player::GetPivotMapPoint()
{
	POINT pivotPos = { static_cast<LONG>(pos.x),  static_cast<LONG>(pos.y) };
	pivotPos.x += PLAYER_PIVOT_POS.x;
	pivotPos.y += PLAYER_PIVOT_POS.y;

	pivotPos.x /= TILE_SIZE;
	pivotPos.y /= TILE_SIZE;

	return pivotPos;
}

void Player::Input()
{
	if ((GetAsyncKeyState(VK_UP) & 0x8000) && state != CharacterInfo::ATTACK)			// 상
	{
		pos.y -= speed * Timmer::GetInstance()->deltaTime;
		state = CharacterInfo::WALK;
		dir = CharacterInfo::UP;
	}
	else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && state != CharacterInfo::ATTACK)	// 하
	{
		pos.y += speed * Timmer::GetInstance()->deltaTime;
		state = CharacterInfo::WALK;
		dir = CharacterInfo::DOWN;
	}
	else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && state != CharacterInfo::ATTACK)	// 좌
	{
		pos.x -= speed * Timmer::GetInstance()->deltaTime;
		state = CharacterInfo::WALK;
		dir = CharacterInfo::LEFT;
	}
	else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && state != CharacterInfo::ATTACK)	// 우
	{
		pos.x += speed * Timmer::GetInstance()->deltaTime;
		state = CharacterInfo::WALK;
		dir = CharacterInfo::RIGHT;
	}
	else if (GetAsyncKeyState(VK_CONTROL) & 0x8000)	// 공격
	{
		state = CharacterInfo::ATTACK;
	}
	else if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && state != CharacterInfo::ATTACK)	// 상호작용 space
	{
		state = CharacterInfo::INTERACTION;
	}
	else
	{
		if(state != CharacterInfo::ATTACK)
			state = CharacterInfo::IDLE;
	}
}

void Player::Update()
{

}
