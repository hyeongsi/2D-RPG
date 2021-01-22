#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include <vector>
#include <string>

using namespace std;

typedef struct protal
{
	POINT pos;
	int stage;
	POINT spawnPos;
}Portal;

class WorldMapManager
{
private:
	static WorldMapManager* instance;

	WorldMapManager();
private:
	WorldMap worldMap;
	int currentStage;		// 현재 스테이지
private:
	const char* WORLDMAP_DATA_PATH = { "data/mapinfo/StagePath.txt" };
	
public:
	vector<string> protalPath;
	vector<string> portalEventPath;

	vector<Portal> protalData;
public:
	static WorldMapManager* GetInstance();
	static void ReleaseInstance();

public:
	const POINT ChangePosToMapPoint(const POINT pos);		// 좌표값을 맵 배열에서 사용하는 값으로 변환

public:
	void SetWorldMap(const WorldMap worldmap);
	WorldMap* GetWorldMap();

	void SetCurrentStage(const int currentStage);
	const int GetCurrentStage();
	
	void AddProtalData(const Portal portal);	// 포탈 데이터 추가
	const vector<Portal> GetProtalData();		
	void DeleteProtalData(const int index);		// 포탈 데이터 삭제

	void LoadPathData();	// 맵 경로 관련 데이터 변수에 저장

	void LoadMapData(const GameState state, const int stage);
	void LoadMapData(const GameState state, const char* filePath);
	void LoadEventData(const int stage);
};

