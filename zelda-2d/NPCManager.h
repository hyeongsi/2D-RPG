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
	void LoadNPCData();
	void LoadShopNPCData(const string path);

};

