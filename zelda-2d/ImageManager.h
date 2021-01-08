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

	map<int, HBITMAP> backgroundBitmapData;		// 비트맵 데이터
	map<int, HBITMAP> objectBitmapData;			// 비트맵 데이터

	map<int, string> backgroundStringData;		// 문자열 데이터
	map<int, string> objectStringData;			// 문자열 데이터
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
	const map<int, string> GetStringMapVar(const BitmapKind kind);
};