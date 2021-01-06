#pragma once

#include "DataStruct.h"
#include <map>
#include <fstream>
#include <string>

using namespace std;

class ImageManager
{
private:
	map<int, HBITMAP> backgroundBitmapData;
	map<int, HBITMAP> objectBitmapData;
public:
	void LoadBitmapPathData(const string str);
	void LoadMapEdittorBitmap();

public:
	const map<int, HBITMAP> GetBitmapData(const BitmapKind kind);
};