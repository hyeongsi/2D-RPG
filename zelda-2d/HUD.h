#pragma once
#include "DataStruct.h"
#include <string>
#include <vector>

typedef struct TEXT_HUD_DATA
{
	DPOINT pos;
	std::string msg;
	ULONGLONG tick = GetTickCount64();
	COLORREF color = 0x0000FF;
}textHudData;

class HUD
{
private:
	std::vector<textHudData> stringHudVector;	// hud 출력 문자열 저장 변수
private:
	const int PRINT_HUD_TIME = 1000;
	const int HUD_SPEED = 20;

public:
	const RECT HP_HUD_RECT = { 20,0,20,5 };

public:
	std::vector<textHudData>* GetStringHud();

	void DeleteEndStringHud();	// hud 삭제
	void RisingHud();	// hud 이동
};

