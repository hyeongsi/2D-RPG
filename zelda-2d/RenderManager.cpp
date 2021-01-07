#include "pch.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "MapEdittor.h"

extern HWND g_hWnd;
extern SIZE g_clientSize;
extern HINSTANCE hInst;
extern HWND g_hStartButton;
extern HWND g_hMapEdittorButton;

RenderManager* RenderManager::instance = nullptr;
RenderManager::RenderManager()
{
    Init();
}

RenderManager::~RenderManager()
{
	DeleteDC(memDC);
	DeleteDC(backMemDC);
	ReleaseDC(g_hWnd, hdc);
}

RenderManager* RenderManager::GetInstance()
{
    if (nullptr == instance)
        instance = new RenderManager();

    return instance;
}

void RenderManager::ReleaseInstance()
{
    delete instance;
    instance = nullptr;
}

void RenderManager::Init()
{
    if (hdc != nullptr)
    {
        ReleaseDC(g_hWnd, hdc);
        hdc = nullptr;
    }
    if (memDC != nullptr)
    {
        DeleteObject(memDC);
        memDC = nullptr;
    }

    if (backMemDC != nullptr)
    {
        DeleteObject(backMemDC);
        backMemDC = nullptr;
    }

    hdc = GetDC(g_hWnd);
    memDC = CreateCompatibleDC(hdc);
    backMemDC = CreateCompatibleDC(hdc);
}

void RenderManager::RenderInitSetting()
{
    if (oldCreateBitmap != nullptr)
    {
        DeleteObject(oldCreateBitmap);
        oldCreateBitmap = nullptr;
    }

    oldCreateBitmap = (HBITMAP)SelectObject(memDC, CreateCompatibleBitmap(hdc, ClientSize::width, ClientSize::height));
    RECT windowRect{ 0,0,ClientSize::width,ClientSize::height };
    FillRect(memDC, &windowRect, (HBRUSH)GetStockObject(WHITE_BRUSH));      // 바탕 흰색으로 초기화
}

void RenderManager::SaveMemDcData(const HWND itemHwnd, POINT pos)
{
    HDC itemDC = GetDC(itemHwnd);

    DWORD buttonRect = SendMessage(itemHwnd, SBM_GETPOS, 0, 0);

    BitBlt(memDC, pos.x, pos.y, 150, 50, itemDC, 0, 0, SRCCOPY);
    ReleaseDC(itemHwnd, itemDC);
}

void RenderManager::SaveMemDcData(const HBITMAP hbitmap, POINT pos)
{
    SelectObject(backMemDC, hbitmap);
    BITMAP bit;
    GetObject(hbitmap, sizeof(BITMAP), &bit);
    BitBlt(memDC, pos.x, pos.y, bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, SRCCOPY);
}

void RenderManager::Render()
{
    BitBlt(hdc, 0, 0, ClientSize::width, ClientSize::height, memDC, 0, 0, SRCCOPY);
}

void RenderManager::MainFrameDataSetting()
{
    SaveMemDcData(ImageManager::GetInstance()->GetMainFrameBitmap(), { 0,0 });
    SaveMemDcData(g_hStartButton, { 500,100 });
    SaveMemDcData(g_hMapEdittorButton, { 500,200 });
}

void RenderManager::MapEdittorDataSetting()
{
    DrawWorldMapData();
    DrawCheckPattern();
    DrawCursorFollowBitmap();
}

void RenderManager::DrawWorldMapData()
{
    MapEdittor* mapEdittor = MapEdittor::GetInstance();
    WorldMap tempWorldMap = mapEdittor->GetWorldMapData();
    MapEdittorSelectState selectState = mapEdittor->GetSelectState();
    int selectBitmapNumber = mapEdittor->GetSelectBitmapNumber();

    if (MapEdittorSelectState::COLLIDER != selectState)
    {
        for (int y = 0; y < MAP_MAX_Y; y++)
        {
            for (int x = 0; x < MAP_MAX_X; x++)
            {
                // 이미지 출력
                if (0 != tempWorldMap.GetData(selectState, { x,y }))
                {
                    SelectObject(backMemDC, ImageManager::GetInstance()->GetBitmap(selectState, selectBitmapNumber));
                    BitBlt(memDC, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, backMemDC, 0, 0, SRCCOPY);
                }
            }
        }
    }
    else
    {
        for (int y = 0; y < MAP_MAX_Y; y++)
        {
            for (int x = 0; x < MAP_MAX_X; x++)
            {
                // 콜라이더 출력
                if (0 != tempWorldMap.GetData(selectState, { x,y }))
                {
                    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, myBrush);

                    Ellipse(memDC, x * TILE_SIZE, y * TILE_SIZE, x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE);

                    SelectObject(memDC, oldBrush);
                }
            }
        }
    }
}

void RenderManager::DrawCheckPattern()
{
    // 세로선
    for (int i = 0; i < g_clientSize.cx / TILE_SIZE; i++)
    {
        MoveToEx(memDC, i * TILE_SIZE, 0, NULL);
        LineTo(memDC, i * TILE_SIZE, g_clientSize.cy);
    }

    // 가로선
    for (int i = 0; i < g_clientSize.cy / TILE_SIZE; i++)
    {
        MoveToEx(memDC, 0, i * TILE_SIZE, NULL);
        LineTo(memDC, g_clientSize.cx, i * TILE_SIZE);
    }
}

void RenderManager::DrawCursorFollowBitmap()
{
    MapEdittor* mapEdittor = MapEdittor::GetInstance();
    MapEdittorSelectState selectState = mapEdittor->GetSelectState();
    int selectBitmapNumber = mapEdittor->GetSelectBitmapNumber();

    POINT mousePoint;       // 마우스 위치를 저장할 변수

    if (MapEdittorSelectState::COLLIDER != selectState)     // 콜라이더 상태가 아닐 경우 마우스 옆에 이미지 출력
    {
        GetCursorPos(&mousePoint);              // 커서 위치를 가져오고
        ScreenToClient(g_hWnd, &mousePoint);    // 클라이언트 영역 좌표로 변환 후

        SelectObject(backMemDC, ImageManager::GetInstance()->GetBitmap(selectState, selectBitmapNumber));

        BitBlt(memDC, mousePoint.x, mousePoint.y, TILE_SIZE, TILE_SIZE, backMemDC, 0, 0, SRCCOPY);
    }
}
