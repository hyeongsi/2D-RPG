#pragma once
#include"ShopNPC.h"
#include <string>

enum NPCKind
{
	SHOP_NPC,
};

enum class InteractNPCState
{
	NONE,
	SHOP_NPC,
};

typedef struct InteractNPCInfo
{
	InteractNPCState state;	// 상태
	int index;					// 해당 상태의 인덱스 값
}InteractNPCInfo;

class NPCManager
{
private:
	vector<ShopNPC> shopNPCVector;

	InteractNPCInfo interactNPCInfo;
private:
	const char* DATA_PATH = { "data/NPCInfo/NPCPath.txt" };

private:
	static NPCManager* instance;

	NPCManager();
	~NPCManager();
public:
	static NPCManager* GetInstance();
	static void ReleaseInstance();

public:
	void LoadNPCData();			// npc 데이터 파일 불러오기
	void LoadShopNPCData(const string path);	//shopNPc 데이터 설정

	void InteractNPC(const POINT pos);

public:
	vector<ShopNPC>* GetshopNPCVector();
	const InteractNPCInfo GetInteractNPCData();
	void SetInteractNPCData(const InteractNPCInfo interactNPCInfo);

	void InitInteractNPCInfo();
};

