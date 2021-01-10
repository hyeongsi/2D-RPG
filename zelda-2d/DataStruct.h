#pragma once

constexpr int TILE_SIZE = 32;	// 맵 한 칸의 크기 ( 32 x 32)

constexpr int MAP_MAX_X = 25;	// 행의 개수
constexpr int MAP_MAX_Y = 19;	// 열의 개수

constexpr POINT START_BUTTON_POINT = { 370,430 };
constexpr POINT MAPEDITTOR_BUTTON_POINT = { 370,480 };
constexpr SIZE BUTTON_SIZE = { 100,30 };

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

namespace BackGroundTextureName
{
	enum
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
}

namespace ObjectTextureName
{
	enum
	{
		grassBlock1 = 1,
		grassBlock2,
		grassBlock3,
		grassBlock4,
		wood_house,
		castle_door_close,
		castle_pillar
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
	OBJECT
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