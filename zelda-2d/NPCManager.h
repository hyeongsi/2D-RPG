#pragma once
#include"ShopNPC.h"
#include <string>

class NPCManager
{
private:
	vector<ShopNPC> shopNPCVector;

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

};

