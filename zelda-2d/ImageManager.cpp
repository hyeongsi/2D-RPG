#include "pch.h"
#include "ImageManager.h"

extern HINSTANCE hInst;

ImageManager* ImageManager::instance = nullptr;
ImageManager::ImageManager()
{
	mainFrameBitmap = (HBITMAP)LoadImageA(hInst, "images/bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

ImageManager::~ImageManager()
{
}

ImageManager* ImageManager::GetInstance()
{
	if (nullptr == instance)
		instance = new ImageManager();

	return instance;
}

void ImageManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

void ImageManager::LoadBitmapPathData(const BitmapKind kind, const string str)
{
	ifstream readFile;
	string number;
	string path;
	string name;

	readFile.open(str);
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			readFile >> number;
			readFile >> path;

			switch (kind)
			{
			case BitmapKind::BACKGROUND:
				readFile >> name;

				backgroundBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				backgroundStringData[stoi(number)] = name;
				break;
			case BitmapKind::OBJECT:
				readFile >> name;

				objectBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				objectStringData[stoi(number)] = name;
				break;
			case BitmapKind::UI:
				uiBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				break;
			}
		}
	}

	readFile.close();
}

void ImageManager::LoadMapBitmapData()
{
	LoadBitmapPathData(BitmapKind::BACKGROUND, BACKGROUND_BITMAP_PATH);
	LoadBitmapPathData(BitmapKind::OBJECT, OBJECT_BITMAP_PATH);
}

void ImageManager::LoadAnimationBitmapData(const string str)
{
	ifstream readFile;
	string size;
	string path;
	string bitmapCount;
	string delay;
	string pos[2];

	readFile.open(str);
	if (readFile.is_open())
	{
		try
		{
			readFile >> size;
			AnimationObject animationObject;
			animationData.emplace_back(animationObject);
			for (int i = 0; i < stoi(size); i++)
			{
				readFile >> path;
				animationData.back().AddAnimationBitmap((HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));

				readFile >> bitmapCount;
				animationData.back().AddBitmapCount(stoi(bitmapCount));

				readFile >> delay;
				animationData.back().AddDelay(stoi(delay));

				readFile >> pos[0];
				readFile >> pos[1];
				animationData.back().AddNormalPos({ stoi(pos[0]),stoi(pos[1]) });
			}
		}
		catch (const std::exception&)
		{
			readFile.close();
			return;
		}
	}

	readFile.close();
}

const HBITMAP ImageManager::GetMainFrameBitmap()
{
	return mainFrameBitmap;
}

const HBITMAP ImageManager::GetBitmapData(const BitmapKind kind,const int selectNumber)
{
	try
	{
		switch (kind)
		{
		case BitmapKind::BACKGROUND:
			return backgroundBitmapData[selectNumber];
		case BitmapKind::OBJECT:
			return objectBitmapData[selectNumber];
		case BitmapKind::UI:
			return uiBitmapData[selectNumber];
		}
	}
	catch (const std::exception&)
	{
		return NULL;
	}
}

const string ImageManager::GetStringData(const BitmapKind kind, const int selectNumber)
{
	try
	{
		switch (kind)
		{
		case BitmapKind::BACKGROUND:
			return backgroundStringData[selectNumber];
		case BitmapKind::OBJECT:
			return objectStringData[selectNumber];
		}
	}
	catch (const std::exception&)
	{
		return NULL;
	}
}

AnimationObject* ImageManager::GetAnimationData(const int uiName)
{
	if (0 < animationData.size() && uiName < animationData.size())
		return &animationData[uiName];
	else
		return nullptr;
}
