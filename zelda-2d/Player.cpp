#include "pch.h"
#include "Player.h"
#include "Timmer.h"
#include "AnimationObject.h"
#include "ImageManager.h"
#include "SoundManager.h"
#include "RenderManager.h"
#include "WorldMapManager.h"

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

POINT Player::GetPivotMapPoint()
{
	POINT pivotPos = { static_cast<LONG>(pos.x),  static_cast<LONG>(pos.y) };
	pivotPos.x += PLAYER_PIVOT_POS.x;
	pivotPos.y += PLAYER_PIVOT_POS.y;

	pivotPos.x /= TILE_SIZE;
	pivotPos.y /= TILE_SIZE;

	return pivotPos;
}

void Player::Attack()
{
	AnimationObject* attackAnimationObject = ImageManager::GetInstance()->GetPlayerAnimationData(TextureName::PLAYER_ATTACK);
	const int hitDamageDelay = 300;

	// 마지막 전 애니메이션 에서만 공격으로 인정
	if (attackAnimationObject->GetSelectBitmapIndex() !=
		(attackAnimationObject->GetBitmapCount(attackAnimationObject->GetSelectAnimationBitmapIndex()) - 1))
	{
		return;
	}

	if (GetTickCount64() > attackTick + hitDamageDelay)
	{
		attackTick = GetTickCount64();
	}
	else
		return;

	const int attackColliderSize = 26;	// 공격 범위
	const int pushedOutSize = 30;		// 피격 시 밀려나는 크기
	RECT colliderRect;
	DPOINT pushOutPos;
	DPOINT pushCorrectionPos;	// 콜라이더 접근 시 이동 보정 수치
	switch (dir)
	{
	case CharacterInfo::DOWN:
		colliderRect.left = static_cast<LONG>(pos.x + PLAYER_PIVOT_POS.x - attackColliderSize);
		colliderRect.top = static_cast<LONG>(pos.y + PLAYER_PIVOT_POS.y);
		colliderRect.right = static_cast<LONG>(pos.x + PLAYER_PIVOT_POS.x + attackColliderSize);
		colliderRect.bottom = static_cast<LONG>(pos.y + PLAYER_PIVOT_POS.y + (static_cast<double>(attackColliderSize) * 2));

		pushOutPos = { 0,pushedOutSize };	// 밀려날 크기 설정
		pushCorrectionPos = { 0 , -1 };
		break;
	case CharacterInfo::RIGHT:
		colliderRect.left = static_cast<LONG>(pos.x + PLAYER_PIVOT_POS.x);
		colliderRect.top = static_cast<LONG>(pos.y + PLAYER_PIVOT_POS.y - attackColliderSize);
		colliderRect.right = static_cast<LONG>(pos.x + PLAYER_PIVOT_POS.x + (static_cast<double>(attackColliderSize) * 2));
		colliderRect.bottom = static_cast<LONG>(pos.y + PLAYER_PIVOT_POS.y + attackColliderSize);

		pushOutPos = { pushedOutSize,0 };	// 밀려날 크기 설정
		pushCorrectionPos = { -1 ,0 };
		break;
	case CharacterInfo::UP:
		colliderRect.left = static_cast<LONG>(pos.x + PLAYER_PIVOT_POS.x - attackColliderSize);
		colliderRect.top = static_cast<LONG>(pos.y + PLAYER_PIVOT_POS.y - (static_cast<double>(attackColliderSize) * 2));
		colliderRect.right = static_cast<LONG>(pos.x + PLAYER_PIVOT_POS.x + attackColliderSize);
		colliderRect.bottom = static_cast<LONG>(pos.y + PLAYER_PIVOT_POS.y);

		pushOutPos = { 0, -pushedOutSize };	// 밀려날 크기 설정
		pushCorrectionPos = { 0 , 1 };
		break;
	case CharacterInfo::LEFT:
		colliderRect.left = static_cast<LONG>(pos.x + PLAYER_PIVOT_POS.x - (static_cast<double>(attackColliderSize) * 2));
		colliderRect.top = static_cast<LONG>(pos.y + PLAYER_PIVOT_POS.y - attackColliderSize);
		colliderRect.right = static_cast<LONG>(pos.x + PLAYER_PIVOT_POS.x);
		colliderRect.bottom = static_cast<LONG>(pos.y + PLAYER_PIVOT_POS.y + attackColliderSize);

		pushOutPos = { -pushedOutSize,0 };	// 밀려날 크기 설정
		pushCorrectionPos = { 1 ,0 };
		break;
	default:
		return;
	}

	SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::ATTACK);

	for (auto& iterator : (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()))
	{
		if (colliderRect.left <= iterator.GetPos().x + MONSTER1_PIVOT_POS.x &&
			colliderRect.right >= iterator.GetPos().x + MONSTER1_PIVOT_POS.x &&
			colliderRect.top <= iterator.GetPos().y + MONSTER1_PIVOT_POS.y &&
			colliderRect.bottom >= iterator.GetPos().y + MONSTER1_PIVOT_POS.y)
		{
			iterator.SetHp(iterator.GetHp() - damage);		// 몬스터 hp -- 처리
			iterator.SetPos({ iterator.GetPos().x + pushOutPos.x , iterator.GetPos().y + pushOutPos.y });	// 밀려남 처리

			iterator.SetHitTick(GetTickCount64());
			iterator.SetState(CharacterInfo::HIT);

			RenderManager::GetInstance()->AddHudData(iterator.GetPos().x + 25, iterator.GetPos().y - 10,
				to_string(damage), 0x0000ff);

			SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::MONSTER_HIT);

			// 밀려남 처리 (맵 나가는거 보정)
			constexpr const int LIMIT_MAP_X_CORRECTION = 32;	// 맵 밖으로 나가는 경우 보정 크기
			constexpr const int LIMIT_MAP_Y_CORRECTION = 46;	// 맵 밖으로 나가는 경우 보정 크기

			if (0 > static_cast<int>(iterator.GetPos().x) || ClientSize::width - LIMIT_MAP_X_CORRECTION < static_cast<int>(iterator.GetPos().x))
			{
				iterator.SetPos({ static_cast<double>(ClientSize::width - LIMIT_MAP_X_CORRECTION) , iterator.GetPos().y });	// 밀려남 처리
				break;
			}
			if (0 > static_cast<int>(iterator.GetPos().y) || ClientSize::height - LIMIT_MAP_Y_CORRECTION < static_cast<int>(iterator.GetPos().y))
			{
				iterator.SetPos({ iterator.GetPos().x , static_cast<double>(ClientSize::height - LIMIT_MAP_Y_CORRECTION) });	// 밀려남 처리
				break;
			}

			// 맵 콜라이더 나가는거 보정
			POINT monsterMapPos;
			monsterMapPos.x = iterator.GetPivotMapPoint().x;
			monsterMapPos.y = iterator.GetPivotMapPoint().y;

			while (0 != WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::COLLIDER, monsterMapPos))
			{
				iterator.SetPos({ iterator.GetPos().x + pushCorrectionPos.x, iterator.GetPos().y + pushCorrectionPos.y });	// 밀려남 처리
				monsterMapPos.x = iterator.GetPivotMapPoint().x;
				monsterMapPos.y = iterator.GetPivotMapPoint().y;
			}
		}
	}
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