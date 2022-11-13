#include "pch.h"
#include "WorldMapManager.h"
#include "InteractionManager.h"
#include "MapEdittor.h"
#include "GameManager.h"
#include "ItemManager.h"
#include "RenderManager.h"
#include "NPCManager.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <time.h>
#include "SoundManager.h"

const POINT RETOUCH_WOOD_DOOR_POS{ 2,4 };
const POINT SPAWN_PLAYER_STAGE1_POS{ 12,13 };

InteractionManager* InteractionManager::instance = nullptr;

InteractionManager::InteractionManager()
{
	srand(static_cast<unsigned int>(time(NULL)));

	mousePos = { 0,0 };
}

InteractionManager::~InteractionManager()
{

}

InteractionManager* InteractionManager::GetInstance()
{
	if (nullptr == instance)
		instance = new InteractionManager();

	return instance;
}

void InteractionManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

void InteractionManager::ChangeMapData(POINT pos)
{
	WorldMap* worldMap = WorldMapManager::GetInstance()->GetWorldMap();

	// 맵 기준 좌표 이탈 시 무시
	if (!(0 <= pos.x && MAP_MAX_X > pos.x))
		return;
	if (!(0 <= pos.y && MAP_MAX_Y > pos.y))
		return;

	switch (worldMap->GetData(SelectMapState::OBJECT, pos))
	{
	case TextureName::lever_off:		// 레버 on 상태로 변경
		worldMap->SetData(SelectMapState::OBJECT, pos, TextureName::lever_on);
		break;
	case TextureName::lever_on:			// 레버 off 상태로 변경
		worldMap->SetData(SelectMapState::OBJECT, pos, TextureName::lever_off);
		break;
	case TextureName::box_off:			// 상자 on 상태로 변경
		worldMap->SetData(SelectMapState::OBJECT, pos, TextureName::box_on);
		break;
	default:
		return;
	}
}

void InteractionManager::ActionEvent(POINT pos)
{
	WorldMap * worldMap = WorldMapManager::GetInstance()->GetWorldMap();

	switch (worldMap->GetData(SelectMapState::EVENT, pos))
	{
	case Event::OPEN_WOOD_HOUSE_DOOR:			// 오두막 문 열기,
		OpenWoodHouseDoor(pos);
		return;
	case Event::CLOSE_WOOD_HOUSE_DOOR:			// 오두막 문 닫기,
		CloseWoodHouseDoor(pos);
		return;
	case Event::OPEN_BOX:						// 아이템 드랍,
		DropItem(pos);
		return;
	case Event::INTERACT_NPC:
		NPCManager::GetInstance()->InteractNPC(pos);	// 상점 이용 상태로 변경
		return;
	}

	switch (worldMap->GetData(SelectMapState::EVENT, GameManager::GetInstance()->GetPlayer()->GetPivotMapPoint()))
	{
	case Event::INTERACT_NPC:
		NPCManager::GetInstance()->InteractNPC(GameManager::GetInstance()->GetPlayer()->GetPivotMapPoint());	// 상점 이용 상태로 변경
		return;
	}
}

void InteractionManager::OpenWoodHouseDoor(const POINT pos)
{
	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			if (TextureName::wood_house_close == WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::OBJECT, { x,y }))
			{
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(SelectMapState::OBJECT, { x,y }, TextureName::wood_house);		// 오두막 문 열기
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(SelectMapState::EVENT, pos, Event::CLOSE_WOOD_HOUSE_DOOR);		// 오두막 문닫히는 이벤트 등록

				WorldMapManager::GetInstance()->GetWorldMap()->SetData(SelectMapState::COLLIDER,		// 문 쪽 콜라이더 제거
					{ x + RETOUCH_WOOD_DOOR_POS.x, y + RETOUCH_WOOD_DOOR_POS.y }, 0);

				WorldMapManager::GetInstance()->GetWorldMap()->SetData(SelectMapState::COLLIDER,		// 문 뒤쪽 콜라이더 생성
					{ x + RETOUCH_WOOD_DOOR_POS.x, y + RETOUCH_WOOD_DOOR_POS.y - 1 }, 1);

				Portal protal;
				protal.pos = { x+ RETOUCH_WOOD_DOOR_POS.x,y + RETOUCH_WOOD_DOOR_POS.y};
				protal.stage = 1;
				protal.spawnPos = { SPAWN_PLAYER_STAGE1_POS.x,SPAWN_PLAYER_STAGE1_POS.y };
				WorldMapManager::GetInstance()->AddProtalData(protal);
				return;
			}
		}
	}
}

void InteractionManager::CloseWoodHouseDoor(const POINT pos)
{
	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			if (TextureName::wood_house == WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::OBJECT, { x,y }))
			{
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(SelectMapState::OBJECT, { x,y }, TextureName::wood_house_close);	// 오두막 문 닫기
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(SelectMapState::EVENT, pos, Event::OPEN_WOOD_HOUSE_DOOR);	// 오두막 문열리는 이벤트 등록

				int count = 0;
				for (const auto& iterator : WorldMapManager::GetInstance()->GetProtalData())
				{
					if (iterator.stage == 1 &&
						iterator.pos.x == x + RETOUCH_WOOD_DOOR_POS.x &&
						iterator.pos.y == y + RETOUCH_WOOD_DOOR_POS.y)
					{
						WorldMapManager::GetInstance()->DeleteProtalData(count);
						return;
					}
					count++;
				} 

				return;
			}
		}
	}
}

void InteractionManager::DropItem(const POINT pos)
{
	// 위 부터 반시계 방향으로 돌아가면서 확인, 상하좌우 검사 한 경우 -> 1사분면,2사분면,3사분면,4사분면 순으로 검사
	POINT tempPos;
	for (int i = 0; i < 8; i++)
	{
		tempPos = pos;
		switch (i)
		{
		case 0:				// 북쪽
			tempPos.y-=1;
			break;
		case 1:				// 서
			tempPos.x -= 1;
			break;
		case 2:				// 남
			tempPos.y += 1;
			break;
		case 3:				// 동
			tempPos.x += 1;
			break;
		case 4:				// 1사분면
			tempPos.x += 1;
			tempPos.y -= 1;
			break;
		case 5:				// 2사분면
			tempPos.x -= 1;
			tempPos.y -= 1;
			break;
		case 6:				// 3사분면
			tempPos.x -= 1;
			tempPos.y += 1;
			break;
		case 7:				// 4사분면
			tempPos.x += 1;
			tempPos.y += 1;
			break;
		}

		if (0 == WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::OBJECT, { tempPos.x,tempPos.y}) &&
			0 == WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::COLLIDER, { tempPos.x,tempPos.y}))
		{
			for (const auto& iterator : (*ItemManager::GetInstance()->GetFieldItem()))
			{
				if (iterator.pos.x == tempPos.x && iterator.pos.y == tempPos.y)
					return;
			}
			// 드랍 이벤트 삭제
			WorldMapManager::GetInstance()->GetWorldMap()->SetData(SelectMapState::EVENT, pos, Event::NONE);		

			// 아이템 스폰
			ItemManager::GetInstance()->AddFieldItem({ tempPos.x,tempPos.y},
				static_cast<int>(rand() % (ItemManager::GetInstance()->GetItemData()->size()+1)));
			return;
		}
	}
}
void InteractionManager::UseItem()
{
	if (!GameManager::GetInstance()->GetInventory()->IsOpen())	// 인벤창 활성화 x 상태면 리턴
		return;

	int itemIndex = FindInventoryItemIndex();
	if (-1 == itemIndex)
		return;

	// 상점 이용 상태가 아닌 경우 - 아이템 사용
	if (!(InteractNPCState::SHOP_NPC == NPCManager::GetInstance()->GetInteractNPCData().state))
	{
		switch (GameManager::GetInstance()->GetInventory()->GetItem()[itemIndex].GetIndex())
		{
		case 1:
			GameManager::GetInstance()->GetPlayer()->SetHp(GameManager::GetInstance()->GetPlayer()->GetHp() + 1);
			SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::USE_POSION);
			break;
		case 2:
			GameManager::GetInstance()->GetPlayer()->SetHp(GameManager::GetInstance()->GetPlayer()->GetHp() + 2);
			SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::USE_POSION);
			break;
		case 3:
			GameManager::GetInstance()->GetPlayer()->SetDamage(GameManager::GetInstance()->GetPlayer()->GetDamage() + 1);
			break;
		case 4:
			break;
		}
	}
	else // 상점 이용 상태 - 아이템 판매
	{
		GameManager::GetInstance()->GetPlayer()->SetMoney(
			GameManager::GetInstance()->GetPlayer()->GetMoney() +
			GameManager::GetInstance()->GetInventory()->GetItem()[itemIndex].GetPrice());

		SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::SELL_ITEM);
	}

	GameManager::GetInstance()->GetInventory()->DeleteItem(itemIndex);
	return;
}

void InteractionManager::BuyItem()
{
	int findItemId = FindBuyItemId();
	if (-1 == findItemId)
		return;

	// 아이템 아이디가 1부터 시작하기 때문에 -1 처리
	findItemId -= 1;
	// 아이템 가격이 소지하고 있는 돈보다 많으면
	if ((*ItemManager::GetInstance()->GetItemData())[findItemId].GetPrice() >
		GameManager::GetInstance()->GetPlayer()->GetMoney())
		return;

	// 구매 처리
	GameManager::GetInstance()->GetPlayer()->SetMoney(
		GameManager::GetInstance()->GetPlayer()->GetMoney() -
		(*ItemManager::GetInstance()->GetItemData())[findItemId].GetPrice());

	// 구매 아이템 인벤토리에 추가
	Item buyItem;
	buyItem = (*ItemManager::GetInstance()->GetItemData())[findItemId];
	GameManager::GetInstance()->GetInventory()->SetItem(buyItem);

	SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::SELL_ITEM);
}

const int InteractionManager::FindInventoryItemIndex()
{
	if (!GameManager::GetInstance()->GetInventory()->IsOpen())	// 인벤창 활성화 x 상태면 리턴
		return -1;

	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);

	for (int i = 0; i < GameManager::GetInstance()->GetInventory()->GetLastItemIndex(); i++)
	{
		if (GameManager::GetInstance()->GetInventory()->GetItem()[i].GetIndex() == 0)
			continue;

		// 아이템 범위안에 마우스 커서가 들어간 경우
		if (mousePos.x >= RenderManager::GetInstance()->INVENTORY_ITEM_POS[i].left &&
			mousePos.x <= RenderManager::GetInstance()->INVENTORY_ITEM_POS[i].right &&
			mousePos.y >= RenderManager::GetInstance()->INVENTORY_ITEM_POS[i].top &&
			mousePos.y <= RenderManager::GetInstance()->INVENTORY_ITEM_POS[i].bottom)
		{
			return i;
		}
	}

	return -1;
}

const int InteractionManager::FindBuyItemId()
{
	if (!(InteractNPCState::SHOP_NPC == NPCManager::GetInstance()->GetInteractNPCData().state))
		return -1;

	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);

	for (int i = 0; i < SELL_ITEM_SIZE; i++)
	{
		// 아이템 범위안에 마우스 커서가 들어간 경우
		if (mousePos.x >= (RenderManager::GetInstance()->SHOP_ITEM_BOX_POS[i].left) &&
			mousePos.x <= (RenderManager::GetInstance()->SHOP_ITEM_BOX_POS[i].right) &&
			mousePos.y >= (RenderManager::GetInstance()->SHOP_ITEM_BOX_POS[i].top) &&
			mousePos.y <= (RenderManager::GetInstance()->SHOP_ITEM_BOX_POS[i].bottom))
		{
			int t = 0;
			for (const auto& iterator :
				(*(*NPCManager::GetInstance()->GetshopNPCVector())
					[NPCManager::GetInstance()->GetInteractNPCData().index].GetSellItemId()))
			{
				if (t == i)
					return iterator;

				t++;
			}
		}
	}

	return -1;
}

void InteractionManager::SwitchInventoryItem(const int preIndex)
{
	if (!GameManager::GetInstance()->GetInventory()->IsOpen())	// 인벤창 활성화 x 상태면 리턴
		return;

	int itemIndex = FindInventoryItemIndex();
	if (-1 == itemIndex)	// 해당 값을 찾지 못했을 경우
		return;

	// 소지하고 있는 아이템 이외의 범위 인덱스라면 return
	if (itemIndex >= GameManager::GetInstance()->GetInventory()->GetLastItemIndex() ||
		preIndex >= GameManager::GetInstance()->GetInventory()->GetLastItemIndex())
		return;

	// 바꿀려고 하는 값이 본인과 같다면 바꿀필요 없이 return
	if (itemIndex == preIndex)
		return;

	GameManager::GetInstance()->GetInventory()->SwitchingItem(itemIndex, preIndex);
}

const int InteractionManager::FindEscMenuIndex()
{
	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);

	if (mousePos.x >= (RenderManager::GetInstance()->ESC_TO_THE_GAME_POS.left) &&
		mousePos.x <= (RenderManager::GetInstance()->ESC_TO_THE_GAME_POS.right) &&
		mousePos.y >= (RenderManager::GetInstance()->ESC_TO_THE_GAME_POS.top) &&
		mousePos.y <= (RenderManager::GetInstance()->ESC_TO_THE_GAME_POS.bottom))
	{
		return TO_THE_GAME;
	}
	else if (
		mousePos.x >= (RenderManager::GetInstance()->ESC_TO_THE_MAIN_MENU_POS.left) &&
		mousePos.x <= (RenderManager::GetInstance()->ESC_TO_THE_MAIN_MENU_POS.right) &&
		mousePos.y >= (RenderManager::GetInstance()->ESC_TO_THE_MAIN_MENU_POS.top) &&
		mousePos.y <= (RenderManager::GetInstance()->ESC_TO_THE_MAIN_MENU_POS.bottom))
	{
		return TO_THE_MAIN_MENU;
	}
	else
		return -1;
}
