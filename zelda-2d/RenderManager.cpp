#include "pch.h"

#include "RenderManager.h"

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

    BitBlt(memDC, pos.x, pos.y, BUTTON_SIZE.cx, BUTTON_SIZE.cy, itemDC, 0, 0, SRCCOPY);
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

void RenderManager::MainFrameDataRender()
{
    RenderInitSetting();            // 출력 전 초기화 작업

    SaveMemDcData(ImageManager::GetInstance()->GetMainFrameBitmap(), { 0,0 });      // 출력할 비트맵 세팅 작업
    SaveMemDcData(g_hStartButton, START_BUTTON_POINT);
    SaveMemDcData(g_hMapEdittorButton, MAPEDITTOR_BUTTON_POINT);

    Render();                   // 출력
}

void RenderManager::MapEdittorDataRender()
{

    RenderInitSetting();        // 출력 전 초기화 작업

    DrawWorldMapData(GameState::MAPEDITTOR);         // 출력할 비트맵 세팅 작업
    DrawCheckPattern();
    DrawCursorFollowBitmap();

    Render();                   // 출력
}

void RenderManager::InGameDataRender(Player* character, NPC* npc)
{
    RenderInitSetting();        // 출력 전 초기화 작업

    DrawWorldMapData(GameState::INGAME);        // 맵 출력

    if (character->GetPos().y <= npc->GetPos().y)
    {
        DrawPlayer(character);                  // 캐릭터 출력
        if(MapInfo::NORMAL == GameManager::GetInstance()->GetCurrentStage())
            DrawNPC(npc);                       // NPC 출력
    }
    else
    {
        if (MapInfo::NORMAL == GameManager::GetInstance()->GetCurrentStage())
            DrawNPC(npc);                       // NPC 출력                          
        DrawPlayer(character);                  // 캐릭터 출력
    }

    //UI 출력
    DrawCharUIData(TextureName::Char_Info, { 10,10 });
    DrawCharUIData(TextureName::Money_Info, { 11,110 });

    int money = character->GetMoney();
    if(0 == money)
        DrawCharUIData(TextureName::ZERO, { 105,125 });
    else
    {
        int count = 0;  // 출력 숫자 정하는 변수
        int space = 0;  // 출력 위치 정하는 변수
        while (money > 0)
        {
            count = money % 10;
            space += 1;
            DrawCharUIData(TextureName::ZERO + count, { 115 - (13 * space),125 });
            money /= 10;
        }
    }

    // HP에 따라 출력되는 HP UI 설정 부분   /   ex)(0이면 빈하트 3개 출력) , (2이면 빈하트 2개, 꽉찬하트 1개 출력)
    int hp[3] = { TextureName::HP_Full, TextureName::HP_Full , TextureName::HP_Full };
    int tempCharacterHp = character->GetHp();
    for (int index = HP_UI_COUNT; index > 0; index--)               // 3,2,1 루프
    {
        for (int count = 0; count < (TextureName::HP_Full - TextureName::HP_Empty); count++)    // 2번 루프
        {
            if (tempCharacterHp < INIT_HP)
            {
                hp[index - 1] -= 1;
                tempCharacterHp += 1;
            }
        }
        DrawCharUIData(hp[index - 1], { 70 + (30 * (index - 1)),52 });     // 배열이 0부터 시작하여 i에 -1을 해줌
    }

    Render();                   // 출력
}

void RenderManager::DrawWorldMapData(const GameState gameState)
{
    GameManager* gameManager = nullptr;
    MapEdittor* mapEdittor = nullptr;
    WorldMap tempWorldMap;

    switch (gameState)
    {
    case GameState::INGAME:
        gameManager = GameManager::GetInstance();
        tempWorldMap = *gameManager->GetWorldMapData(gameManager->GetCurrentStage());
        break;
    case GameState::MAPEDITTOR:
        mapEdittor = MapEdittor::GetInstance();
        tempWorldMap = mapEdittor->GetWorldMapData();
        break;
    default:
        return;
    }

    // 배경 출력
    for (int y = 0; y < MAP_MAX_Y; y++)
    {
        for (int x = 0; x < MAP_MAX_X; x++)
        {
            if (0 != tempWorldMap.GetData(MapEdittorSelectState::BACKGROUND, { x,y }))
            {
                HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::BACKGROUND,
                    tempWorldMap.GetData(MapEdittorSelectState::BACKGROUND, { x,y }));

                BITMAP bit;
                SelectObject(backMemDC, bitmap);
                GetObject(bitmap, sizeof(bit), &bit);
                BitBlt(memDC, x * TILE_SIZE, y * TILE_SIZE, bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, SRCCOPY);
            }
        }
    }

    // 오브젝트 출력
    for (int y = 0; y < MAP_MAX_Y; y++)
    {
        for (int x = 0; x < MAP_MAX_X; x++)
        {
            if (0 != tempWorldMap.GetData(MapEdittorSelectState::OBJECT, { x,y }))
            {
                HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::OBJECT,
                    tempWorldMap.GetData(MapEdittorSelectState::OBJECT, { x,y }));

                BITMAP bit;
                SelectObject(backMemDC, bitmap);
                GetObject(bitmap, sizeof(bit), &bit);
                TransparentBlt(memDC, x * TILE_SIZE, y * TILE_SIZE, bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, bit.bmWidth, bit.bmHeight, RGB(215, 123, 186));
            }
        }
    }

    switch (gameState)
    {
    case GameState::MAPEDITTOR:
        // 콜라이더 출력
        for (int y = 0; y < MAP_MAX_Y; y++)
        {
            for (int x = 0; x < MAP_MAX_X; x++)
            {
                if (0 != tempWorldMap.GetData(MapEdittorSelectState::COLLIDER, { x,y }))
                {
                    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, myBrush);

                    Ellipse(memDC, x * TILE_SIZE, y * TILE_SIZE, x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE);

                    SelectObject(memDC, oldBrush);
                }
            }
        }
        break;
    default:
        return;
    }
}

void RenderManager::DrawPlayer(Player* player)
{
    AnimationObject* walkAnimationObject = ImageManager::GetInstance()->GetAnimationData(TextureName::PLAYER_WALK);
    AnimationObject* attackAnimationObject = ImageManager::GetInstance()->GetAnimationData(TextureName::PLAYER_ATTACK);

    switch (player->GetState())
    {
    case CharacterInfo::WALK:
        InitPlayerAnimation(CharacterInfo::WALK, player);

        DrawAnimation(TextureName::PLAYER_WALK, player->GetPos());
        walkAnimationObject->NextSelectBitmapIndex();                               // 출력 이미지 위치 변경
        break;
    case CharacterInfo::ATTACK:
        InitPlayerAnimation(CharacterInfo::ATTACK, player);

        DrawAnimation(TextureName::PLAYER_ATTACK, player->GetPos());
        if (attackAnimationObject->NextSelectBitmapIndex())      // 출력 이미지 위치 변경
            player->SetState(CharacterInfo::IDLE);

        break;
    case CharacterInfo::IDLE:
    default:
        InitPlayerAnimation(CharacterInfo::IDLE, player);

        walkAnimationObject->SetSelectBitmapIndex(CharacterInfo::IDLE);             // 초기 이미지로 변경
        DrawAnimation(TextureName::PLAYER_WALK, player->GetPos());
        break;
    }
}

void RenderManager::DrawNPC(NPC* npc)
{
    AnimationObject* walkAnimationObject = ImageManager::GetInstance()->GetAnimationData(TextureName::NPC_WALK);
    walkAnimationObject->SetSelectAnimationBitmapIndex(npc->GetDir());       // 방향에 따른 방향 애니메이션 설정

    switch (npc->GetState())
    {
    case CharacterInfo::WALK:
        DrawAnimation(TextureName::NPC_WALK, npc->GetPos());
        walkAnimationObject->NextSelectBitmapIndex();                               // 출력 이미지 위치 변경
        break;
  
    case CharacterInfo::IDLE:
    default:
        walkAnimationObject->SetSelectBitmapIndex(CharacterInfo::IDLE);             // 초기 이미지로 변경
        DrawAnimation(TextureName::NPC_WALK, npc->GetPos());
        break;
    }
}

void RenderManager::DrawCharUIData(const int uiName, const POINT pos)
{
    HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::UI, uiName);

    BITMAP bit;
    SelectObject(backMemDC, bitmap);
    GetObject(bitmap, sizeof(bit), &bit);
    TransparentBlt(memDC, pos.x, pos.y, bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, bit.bmWidth, bit.bmHeight, RGB(215, 123, 186));
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

    BitmapKind kind;
    switch (selectState)
    {
    case MapEdittorSelectState::BACKGROUND:
        kind = BitmapKind::BACKGROUND;
        break;
    case MapEdittorSelectState::OBJECT:
        kind = BitmapKind::OBJECT;
        break;
    default:
        return;
    }

    int selectBitmapNumber = mapEdittor->GetSelectIndex();

    POINT mousePoint;       // 마우스 위치를 저장할 변수

    GetCursorPos(&mousePoint);              // 커서 위치를 가져오고
    ScreenToClient(g_hWnd, &mousePoint);    // 클라이언트 영역 좌표로 변환 후

    HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(kind, selectBitmapNumber);
    SelectObject(backMemDC, bitmap);
    BITMAP bit;
    GetObject(bitmap, sizeof(bit), &bit);
    BitBlt(memDC, mousePoint.x, mousePoint.y, bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, SRCCOPY);
}

void RenderManager::DrawAnimation(const int uiName, const DPOINT pos)
{
    AnimationObject* animationObject = ImageManager::GetInstance()->GetAnimationData(uiName);

    if (nullptr == animationObject)
        return;

    int animationIndex = animationObject->GetSelectAnimationBitmapIndex();
    int selectBitmapIndex = animationObject->GetSelectBitmapIndex();
    int count = animationObject->GetBitmapCount(animationIndex);

    HBITMAP bitmap = animationObject->GetAnimationBitmap(animationIndex);
    BITMAP bit;
    SelectObject(backMemDC, bitmap);
    GetObject(bitmap, sizeof(bit), &bit);
    TransparentBlt(memDC, static_cast<int>(pos.x), static_cast<int>(pos.y), bit.bmWidth/count, bit.bmHeight, 
        backMemDC, selectBitmapIndex * (bit.bmWidth / count), 0, bit.bmWidth/count,
        bit.bmHeight, RGB(215, 123, 186));
}

void RenderManager::InitPlayerAnimation(const int state, Player * player)
{
    AnimationObject* walkAnimationObject = ImageManager::GetInstance()->GetAnimationData(TextureName::PLAYER_WALK);
    AnimationObject* attackAnimationObject = ImageManager::GetInstance()->GetAnimationData(TextureName::PLAYER_ATTACK);

    switch (state)
    {
    case CharacterInfo::IDLE:
    case CharacterInfo::WALK:
        attackAnimationObject->SetSelectBitmapIndex(0);                 // 다른 애니메이션 초기화
        walkAnimationObject->SetSelectAnimationBitmapIndex(player->GetDir());       // 방향에 따른 방향 애니메이션 설정
        break;
    case CharacterInfo::ATTACK:
        walkAnimationObject->SetSelectBitmapIndex(0);                 // 다른 애니메이션 초기화
        attackAnimationObject->SetSelectAnimationBitmapIndex(player->GetDir());       // 방향에 따른 방향 애니메이션 설정
        break;
    }
}