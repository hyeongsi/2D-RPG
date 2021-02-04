#include "pch.h"
#include "HUD.h"
#include "Timmer.h"

void HUD::Init()
{
    stringHudVector.clear();
}

std::vector<textHudData>* HUD::GetStringHud()
{
	return &stringHudVector;
}

void HUD::DeleteEndStringHud()
{
    for (auto iterator = stringHudVector.begin(); iterator != stringHudVector.end();)
    {
        if (GetTickCount64() > (*iterator).tick + PRINT_HUD_TIME)
        {
            iterator = stringHudVector.erase(iterator);
        }
        else
        {
            iterator++;
            return;
        }
    }
}

void HUD::RisingHud()
{
    for (auto& iterator : stringHudVector)
    {
        iterator.pos = { iterator.pos.x ,  iterator.pos.y - (HUD_SPEED * Timmer::GetInstance()->deltaTime) };
    }
}