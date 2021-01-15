#pragma once

constexpr const int TILE_SIZE = 32;	// 맵 한 칸의 크기 ( 32 x 32)

constexpr const int MAP_MAX_X = 25;	// 행의 개수
constexpr const int MAP_MAX_Y = 19;	// 열의 개수

constexpr const POINT CHAR_COLLIDER_BOX_LEFT_TOP = { 1,47 };	// pivot 좌표 
constexpr const POINT CHAR_COLLIDER_BOX_RIGHT_BOTTOM = {31,51};	// pivot 좌표

constexpr const int LIMIT_MAP_X_CORRECTION = 32;	// 맵 밖으로 나가는 경우 보정 크기
constexpr const int LIMIT_MAP_Y_CORRECTION = 46;	// 맵 밖으로 나가는 경우 보정 크기

constexpr const POINT START_BUTTON_POINT = { 370,430 };
constexpr const POINT MAPEDITTOR_BUTTON_POINT = { 370,480 };
constexpr const SIZE BUTTON_SIZE = { 100,30 };

constexpr const char BACKGROUND_BITMAP_PATH[] = { "data/bitmapinfo/BackgroundBitmapInfo.txt" };
constexpr const char OBJECT_BITMAP_PATH[] = { "data/bitmapinfo/ObjectBitmapInfo.txt" };
constexpr const char UI_BITMAP_PATH[] = { "data/bitmapinfo/UIBitmapInfo.txt" };

constexpr const char STAGE1_PATH[] = { "data/mapinfo/TileMapInfo.txt" };

constexpr const char CHARACTER_ANIMATION_PATH[] = { "data/bitmapinfo/AnimationBitmapInfo.txt" };

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
		castle_tile
	};

	enum Object
	{
		grassBlock1 = 1,
		grassBlock2,
		grassBlock3,
		grassBlock4,
		wood_house,
		castle_door_close,
		castle_pillar,
		lever_off,
		lever_on,
		box_off,
		box_on,
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

	enum Animation
	{
		CHARACTER_WALK,
		CHARACTER_ATTACK,
	};
}

enum class MapEdittorSelectState
{
	BACKGROUND,
	OBJECT,
	COLLIDER
};

enum class BitmapKind
{
	BACKGROUND,
	OBJECT,
	UI
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
		START,
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

	};
}
