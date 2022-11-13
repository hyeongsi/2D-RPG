#pragma once

const POINT HWND_SPAWN_POS = { 500,200 };	// 메인 프레임 출력 위치

constexpr const int TILE_SIZE = 32;	// 맵 한 칸의 크기 ( 32 x 32)

constexpr const int MAP_MAX_X = 25;	// 행의 개수
constexpr const int MAP_MAX_Y = 19;	// 열의 개수

const POINT PLAYER_PIVOT_POS = { 16,47 };	// 캐릭터 콜라이더 기준 좌표 
const POINT MONSTER1_PIVOT_POS = { 32,53 };	// 몬스터1 콜라이더 기준 좌표 

const POINT STAGE2_SPAWN_POS = { 642, 115 };	// 오두막 집 문의 위치, 기준 위치로 부터 + alpah 값
const POINT WOOD_HOUSE_DOOR_POS = {2, 4};	// 오두막 집 문의 위치, 기준 위치로 부터 + alpah 값

const POINT START_BUTTON_POINT = { 370,430 };
const POINT MAPEDITTOR_BUTTON_POINT = { 370,480 };
const SIZE BUTTON_SIZE = { 100,30 };

constexpr const char BACKGROUND_BITMAP_PATH[] = { "data/bitmapinfo/BackgroundBitmapInfo.txt" };
constexpr const char OBJECT_BITMAP_PATH[] = { "data/bitmapinfo/ObjectBitmapInfo.txt" };

constexpr const char PLAYER_ANIMATION_PATH[] = { "data/bitmapinfo/CharacterAnimationBitmapInfo.txt" };

namespace ClientSize
{
	enum
	{
		width = 800,
		height = 608
	};
}

enum class GameState
{
	MAIN,
	MAPEDITTOR,
	INGAME
};

typedef struct DOUBLEPOINT
{
	double x;
	double y;
}DPOINT;

namespace TextureName
{
	enum BackGround
	{
		grass = 1,
		grass_water1,
		grass_water2,
		grass_water3,
		grass_water4,
		grass_water5,
		grass_water6,
		grass_water7,
		grass_water8,
		grass_water9,
		castle_tile,
		black_tile,
		inside_tile,
	};

	enum Object
	{
		grassBlock1 = 1,
		grassBlock2,
		grassBlock3,
		grassBlock4,
		wood_house,
		wood_house_close,
		castle_door_close,
		castle_pillar,
		lever_off,
		lever_on,
		box_off,
		box_on,
		castle_wall_left,
		castle_wall_center,
		castle_wall_right,
		fireplace,
		table,
		chair_center,
		chair_left,
		chair_right,
		bed,
	};

	enum UI
	{
		Char_Info = 1,
		HP_Empty,
		HP_Half,
		HP_Full,
		Money_Info,
		ZERO,
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
	};

	enum PlayerAnimation
	{
		PLAYER_WALK,
		PLAYER_ATTACK,
		PLAYER_HOLD_WALK,
	};

	enum MonsterAnimation
	{
		MONSTER_WALK,
	};
}

enum class AnimationKind
{
	PLAYER,
	MONSTER,
};

enum class SelectMapState
{
	BACKGROUND,
	OBJECT,
	COLLIDER,
	EVENT,
	NPC,
};

namespace MapInfo
{

	enum MapStage
	{
		NORMAL,
		WOOD_HOUSE,
	};
}

namespace Event
{
	enum eventKind
	{
		NONE,
		OPEN_WOOD_HOUSE_DOOR,
		CLOSE_WOOD_HOUSE_DOOR,
		OPEN_BOX,
		INTERACT_NPC,
	};
}

enum class BitmapKind
{
	BACKGROUND,
	OBJECT,
	UI,
	ITEM,
	NPC,
};

enum class ClickLR
{
	NONE,
	LEFT,
	RIGHT
};

namespace ButtonKind
{
	enum
	{
		NEW_START,
		MAPEDITTOR
	};
}

namespace CharacterInfo
{
	enum DIR
	{
		DOWN,
		RIGHT,
		UP,
		LEFT,
	};

	enum CharacterState
	{
		IDLE,
		WALK,
		ATTACK,
		INTERACTION,	// 상호작용
		HIT,
		DIE,
	};
}

typedef struct FieldNPC
{
	POINT pos;	// 위치
	int imageIndex;	// npc 이미지 종류
	int kind;	// npc 종류
	int index;	// 인덱스
}FieldNPC;
