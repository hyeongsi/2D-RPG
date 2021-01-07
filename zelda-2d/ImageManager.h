#pragma once

#include "DataStruct.h"
#include <map>
#include <fstream>
#include <string>

using namespace std;

class ImageManager
{
private:
	HBITMAP mainFrameBitmap;		// 메인화면 배경

	map<int, HBITMAP> backgroundBitmapData;
	map<int, HBITMAP> objectBitmapData;
private:
	static ImageManager* instance;

	ImageManager();
	~ImageManager();

public:
	static ImageManager* GetInstance();
	static void ReleaseInstance();

public:
	void LoadBitmapPathData(const MapEdittorSelectState state, const string str);
	void LoadMapEdittorBitmap();

public:
	const HBITMAP GetMainFrameBitmap();
	
	const HBITMAP GetBitmap(MapEdittorSelectState state, int selectNumber);
	const map<int, HBITMAP> GetBitmapMapVar(const BitmapKind kind);
};