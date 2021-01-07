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
		grass = 1
	};
}

namespace ObjectTextureName
{
	enum
	{
		grassBlock1 = 1,
		grassBlock2,
		grassBlock3,
		grassBlock4
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