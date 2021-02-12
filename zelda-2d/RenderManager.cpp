#include "pch.h"

#include "RenderManager.h"
#include "WorldMapManager.h"
#include "ItemManager.h"
#include "NPCManager.h"
#include "Time.h"
#define DEBUG

extern HWND g_hWnd;
extern SIZE g_clientSize;
extern HINSTANCE hInst;
extern HWND g_hStartButton;
#ifdef DEBUG
extern HWND g_hMapEdittorButton;
#endif // DEBUG

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

    if (memDC != nullptr)
    {
        SetBkMode(memDC, TRANSPARENT);    // 글자 출력 시 배경 색 투명으로 설정
        SetTextColor(memDC, RGB(255, 0, 0));    // 글자 빨간색으로 설정
    }

    // 상점 크기 설정
    for (int i = 0; i < SELL_ITEM_SIZE; i++)
    {
        SHOP_ITEM_BOX_POS[i].left = SHOP_SPAWN_POS.x + SHOP_INTERVAL_SIZE.cx;
        SHOP_ITEM_BOX_POS[i].top = SHOP_SPAWN_POS.y + SHOP_INTERVAL_SIZE.cx + (SHOP_ITEMBOX_SIZE.cy * i) + (SHOP_INTERVAL_SIZE.cy * i);
        SHOP_ITEM_BOX_POS[i].right = SHOP_SPAWN_POS.x + SHOP_INTERVAL_SIZE.cx + SHOP_ITEMBOX_SIZE.cx;
        SHOP_ITEM_BOX_POS[i].bottom = SHOP_SPAWN_POS.y + SHOP_INTERVAL_SIZE.cx + SHOP_ITEMBOX_SIZE.cy + (SHOP_ITEMBOX_SIZE.cy * i) + (SHOP_INTERVAL_SIZE.cy * i);
    }
    // 인벤 크기 설정
    for (int i = 0; i < INVEN_SIZE; i++)
    {
        INVENTORY_ITEM_POS[i].left = (INVENTORY_SPAWN_POS.x + INVENTORY_interval_SIZE.cx) + ((i % INVEN_SIZE_X) * TILE_SIZE) + ((i % INVEN_SIZE_X) * INVENTORY_interval_SIZE.cx);
        INVENTORY_ITEM_POS[i].top = (INVENTORY_SPAWN_POS.y + INVENTORY_interval_SIZE.cy) + (i / INVEN_SIZE_X * TILE_SIZE) + ((i / INVEN_SIZE_X) * INVENTORY_interval_SIZE.cy);
        INVENTORY_ITEM_POS[i].right = (INVENTORY_SPAWN_POS.x + INVENTORY_interval_SIZE.cx) + ((i % INVEN_SIZE_X) * TILE_SIZE) + ((i % INVEN_SIZE_X) * INVENTORY_interval_SIZE.cx) + TILE_SIZE;
        INVENTORY_ITEM_POS[i].bottom = (INVENTORY_SPAWN_POS.y + INVENTORY_interval_SIZE.cy) + (i / INVEN_SIZE_X * TILE_SIZE) + ((i / INVEN_SIZE_X) * INVENTORY_interval_SIZE.cy) + TILE_SIZE;
    }
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

HUD* RenderManager::GetHud()
{
    return &hud;
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
#ifdef DEBUG
    SaveMemDcData(g_hMapEdittorButton, MAPEDITTOR_BUTTON_POINT);
#endif // DEBUG
    Render();                   // 출력
}

void RenderManager::MapEdittorDataRender()
{
    RenderInitSetting();        // 출력 전 초기화 작업

    DrawWorldMapData(GameState::MAPEDITTOR);         // 출력할 비트맵 세팅 작업
    DrawCheckPattern(memDC, g_clientSize);
    DrawCursorFollowBitmap();

    Render();                   // 출력
}

void RenderManager::InGameDataRender()
{
    RenderInitSetting();        // 출력 전 초기화 작업

    DrawWorldMapData(GameState::INGAME);     // 맵 출력
    DrawFieldItem();                         // 필드 아이템 출력

    DrawPlayer();           // 캐릭터 출력
    DrawNpcOrderPos();      // NPC와 캐릭터의 출력 순서 보정
    DrawMonsterOrderPos();  // Monster와 캐릭터의 출력 순서 보정

    DrawHudVector();    // hudData 출력 부분

    switch (NPCManager::GetInstance()->GetInteractNPCData().state)
    {
    case InteractNPCState::SHOP_NPC:
        DrawShop();     // 상점 창 출력
        break;
    default:
        break;
    }

    if (GameManager::GetInstance()->GetInventory()->IsOpen())    // 인벤 창 활성화 ->
    {
        DrawInventoryItem();    // 인벤 창 출력
        DrawInvenItemExplain();
    }

    //UI 출력
    DrawCharUIData(TextureName::Char_Info, { DRAW_PLAYERINFO_UI_POS.x,DRAW_PLAYERINFO_UI_POS.y });
    DrawCharUIData(TextureName::Money_Info, { DRAW_MONEYINFO_UI_POS.x, DRAW_MONEYINFO_UI_POS.y});

    int money = GameManager::GetInstance()->GetPlayer()->GetMoney();
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
    int tempCharacterHp = GameManager::GetInstance()->GetPlayer()->GetHp();
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

    hud.DeleteEndStringHud();   // hudStringData 삭제 부분
    hud.RisingHud();            // hud 상승 부분
}

void RenderManager::DrawWorldMapData(const GameState gameState)
{
    WorldMap tempWorldMap;

    switch (gameState)
    {
    case GameState::INGAME:
        tempWorldMap = *(WorldMapManager::GetInstance()->GetWorldMap());
        break;
    case GameState::MAPEDITTOR:
        tempWorldMap = MapEdittor::GetInstance()->GetWorldMapData();
        break;
    default:
        return;
    }

    // 배경 출력
    for (int y = 0; y < MAP_MAX_Y; y++)
    {
        for (int x = 0; x < MAP_MAX_X; x++)
        {
            if (0 != tempWorldMap.GetData(SelectMapState::BACKGROUND, { x,y }))
            {
                HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::BACKGROUND,
                    tempWorldMap.GetData(SelectMapState::BACKGROUND, { x,y }));

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
            if (0 != tempWorldMap.GetData(SelectMapState::OBJECT, { x,y }))
            {
                HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::OBJECT,
                    tempWorldMap.GetData(SelectMapState::OBJECT, { x,y }));

                BITMAP bit;
                SelectObject(backMemDC, bitmap);
                GetObject(bitmap, sizeof(bit), &bit);
                TransparentBlt(memDC, x * TILE_SIZE, y * TILE_SIZE, bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, bit.bmWidth, bit.bmHeight, RGB(215, 123, 186));
            }
        }
    }

    // NPC 위치 출력
    DrawNPC();
    // monster 출력
    DrawMonster();

    // 포탈 위치 출력
    for (const auto& iterator : WorldMapManager::GetInstance()->GetProtalData())
    {
        HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, myBrush);

        Ellipse(memDC, iterator.pos.x * TILE_SIZE, iterator.pos.y * TILE_SIZE,
            iterator.pos.x * TILE_SIZE + TILE_SIZE, iterator.pos.y * TILE_SIZE + TILE_SIZE);

        SelectObject(memDC, oldBrush);
    }

    switch (gameState)
    {
    case GameState::MAPEDITTOR:

        // 콜라이더 출력
        for (int y = 0; y < MAP_MAX_Y; y++)
        {
            for (int x = 0; x < MAP_MAX_X; x++)
            {
                if (0 != tempWorldMap.GetData(SelectMapState::COLLIDER, { x,y }))
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

void RenderManager::DrawFieldItem()
{
    for (const auto& iterator: (*ItemManager::GetInstance()->GetFieldItem()))
    {
        if (0 != iterator.index)
        {
            HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::ITEM,
                iterator.index);

            BITMAP bit;
            SelectObject(backMemDC, bitmap);
            GetObject(bitmap, sizeof(bit), &bit);
            TransparentBlt(memDC, iterator.pos.x * TILE_SIZE, iterator.pos.y * TILE_SIZE,
                bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, bit.bmWidth, bit.bmHeight, RGB(132, 126, 135));
        }
    }
}

void RenderManager::DrawInventoryItem()
{
    // draw inventory rect 
    Rectangle(memDC, INVENTORY_SPAWN_POS.x, INVENTORY_SPAWN_POS.y, INVENTORY_SPAWN_POS.x + INVENTORY_SIZE.cx, INVENTORY_SPAWN_POS.y + INVENTORY_SIZE.cy);

    // draw item box
    int t = 0;
    for (int i = 0; i < INVEN_SIZE; i++)
    {
        t = i;

        Rectangle(memDC, (INVENTORY_SPAWN_POS.x + INVENTORY_interval_SIZE.cx) + ((t % INVEN_SIZE_X) * TILE_SIZE) + ((t % INVEN_SIZE_X) * INVENTORY_interval_SIZE.cx),
            (INVENTORY_SPAWN_POS.y + INVENTORY_interval_SIZE.cy) + (t / INVEN_SIZE_X * TILE_SIZE) + ((t / INVEN_SIZE_X) * INVENTORY_interval_SIZE.cy),
            (INVENTORY_SPAWN_POS.x + INVENTORY_interval_SIZE.cx) + ((t % INVEN_SIZE_X) * TILE_SIZE) + ((t % INVEN_SIZE_X) * INVENTORY_interval_SIZE.cx) + TILE_SIZE,
            (INVENTORY_SPAWN_POS.y + INVENTORY_interval_SIZE.cy) + (t / INVEN_SIZE_X * TILE_SIZE) + ((t / INVEN_SIZE_X) * INVENTORY_interval_SIZE.cy) + TILE_SIZE);
    }

    // draw item
    t = 0;
    for (int i = 0; i < GameManager::GetInstance()->GetInventory()->GetLastItemIndex(); i++)
    {
        if (0 != GameManager::GetInstance()->GetInventory()->GetItem()[i].GetIndex())
        {
            HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::ITEM,
                GameManager::GetInstance()->GetInventory()->GetItem()[i].GetIndex());

            t = i;

            BITMAP bit;
            SelectObject(backMemDC, bitmap);
            GetObject(bitmap, sizeof(bit), &bit);
            BitBlt(memDC, (INVENTORY_SPAWN_POS.x + INVENTORY_interval_SIZE.cx) + ((t% INVEN_SIZE_X) * TILE_SIZE) + ((t% INVEN_SIZE_X) * INVENTORY_interval_SIZE.cx),
                (INVENTORY_SPAWN_POS.y + INVENTORY_interval_SIZE.cy) + (t / INVEN_SIZE_X * TILE_SIZE) + ((t / INVEN_SIZE_X) * INVENTORY_interval_SIZE.cy),
                bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, SRCCOPY);
        }
    }   
}

void RenderManager::DrawInvenItemExplain()
{
    constexpr const SIZE EXPLAIN_ITEM_PANEL_SIZE = {150,80};
    constexpr const POINT ITEM_TITLE_TEXT_RETOUCH_POS = { 10,5 };
    constexpr const POINT ITEM_EXPLAIN_TEXT_RETOUCH_POS = { 10,40 };

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(g_hWnd, &mousePos);

    int t = 0;
    for (int i = 0; i < GameManager::GetInstance()->GetInventory()->GetLastItemIndex(); i++)
    {
        if (GameManager::GetInstance()->GetInventory()->GetItem()->GetIndex() == 0)
            return;
        
        t = i;

        // 아이템 범위안에 마우스 커서가 들어간 경우
        if (mousePos.x >=
            (INVENTORY_SPAWN_POS.x + INVENTORY_interval_SIZE.cx) + ((t % INVEN_SIZE_X) * TILE_SIZE) + ((t % INVEN_SIZE_X) * INVENTORY_interval_SIZE.cx) &&
            mousePos.x <= (INVENTORY_SPAWN_POS.x + INVENTORY_interval_SIZE.cx) + ((t % INVEN_SIZE_X) * TILE_SIZE) + ((t % INVEN_SIZE_X) * INVENTORY_interval_SIZE.cx) + TILE_SIZE &&
            mousePos.y >= (INVENTORY_SPAWN_POS.y + INVENTORY_interval_SIZE.cy) + (t / INVEN_SIZE_X * TILE_SIZE) + ((t / INVEN_SIZE_X) * INVENTORY_interval_SIZE.cy) &&
            mousePos.y <= (INVENTORY_SPAWN_POS.y + INVENTORY_interval_SIZE.cy) + (t / INVEN_SIZE_X * TILE_SIZE) + ((t / INVEN_SIZE_X) * INVENTORY_interval_SIZE.cy) + TILE_SIZE)
        {
            // 설명 바탕 (글을 담을 테두리 배경 출력)
            Rectangle(memDC, 
                mousePos.x - EXPLAIN_ITEM_PANEL_SIZE.cx, mousePos.y + EXPLAIN_ITEM_PANEL_SIZE.cy,
                mousePos.x, mousePos.y);
            // 아이템 제목 출력
            TextOut(memDC, 
                mousePos.x - EXPLAIN_ITEM_PANEL_SIZE.cx + ITEM_TITLE_TEXT_RETOUCH_POS.x,
                mousePos.y + ITEM_TITLE_TEXT_RETOUCH_POS.y, 
                (GameManager::GetInstance()->GetInventory()->GetItem())[i].GetTitle().c_str(),
                strlen((GameManager::GetInstance()->GetInventory()->GetItem())[i].GetTitle().c_str()));
            // 아이템 설명 출력
            TextOut(memDC,
                mousePos.x - EXPLAIN_ITEM_PANEL_SIZE.cx + ITEM_EXPLAIN_TEXT_RETOUCH_POS.x,
                mousePos.y + ITEM_EXPLAIN_TEXT_RETOUCH_POS.y, (GameManager::GetInstance()->GetInventory()->GetItem())[i].GetExplain().c_str(),
                strlen((GameManager::GetInstance()->GetInventory()->GetItem())[i].GetExplain().c_str()));
        }
    }
}

void RenderManager::DrawESCMenu()
{
    const RECT escMenu = { (g_clientSize.cx / 5) * 2,
                            (g_clientSize.cy / 5) * 2,
                            (g_clientSize.cx / 5) * 3,
                            (g_clientSize.cy / 5) * 3 };

    const POINT toTheGame = { g_clientSize.cx / 3 + 105,
                              g_clientSize.cy / 3 + 70 };

    const POINT toTheMainMenu = { g_clientSize.cx / 3 + 70 ,
                                  g_clientSize.cy / 3 + 125 };

   ESC_TO_THE_GAME_POS = { toTheGame.x - 10 ,toTheGame.y - 5, toTheGame.x + 75, toTheGame.y + 25 };
    ESC_TO_THE_MAIN_MENU_POS = { toTheMainMenu.x - 5 , toTheMainMenu.y - 5, toTheMainMenu.x + 140, toTheMainMenu.y + 25 };

    Rectangle(hdc, escMenu.left, escMenu.top, escMenu.right, escMenu.bottom);   // escMenu print

    Rectangle(hdc, ESC_TO_THE_GAME_POS.left, ESC_TO_THE_GAME_POS.top, 
        ESC_TO_THE_GAME_POS.right, ESC_TO_THE_GAME_POS.bottom);     // toGame print
    TextOut(hdc, toTheGame.x, toTheGame.y, "게임으로", strlen("게임으로"));

    Rectangle(hdc, ESC_TO_THE_MAIN_MENU_POS.left, ESC_TO_THE_MAIN_MENU_POS.top, 
        ESC_TO_THE_MAIN_MENU_POS.right, ESC_TO_THE_MAIN_MENU_POS.bottom);     // toGame print
    TextOut(hdc, toTheMainMenu.x, toTheMainMenu.y, "메인 메뉴로 나가기", strlen("메인 메뉴로 나가기"));
}

void RenderManager::DrawPlayer()
{
    Player* player = GameManager::GetInstance()->GetPlayer();
    AnimationObject* walkAnimationObject = ImageManager::GetInstance()->GetPlayerAnimationData(TextureName::PLAYER_WALK);
    AnimationObject* attackAnimationObject = ImageManager::GetInstance()->GetPlayerAnimationData(TextureName::PLAYER_ATTACK);

    switch (player->GetState())
    {
    case CharacterInfo::WALK:
        InitPlayerAnimation(CharacterInfo::WALK);

        DrawPlayerAnimation(TextureName::PLAYER_WALK, player->GetPos());
        walkAnimationObject->NextSelectBitmapIndex();                               // 출력 이미지 위치 변경
        break;
    case CharacterInfo::ATTACK:
        InitPlayerAnimation(CharacterInfo::ATTACK);

        DrawPlayerAnimation(TextureName::PLAYER_ATTACK, player->GetPos());
        if (attackAnimationObject->NextSelectBitmapIndex())      // 출력 이미지 위치 변경
            player->SetState(CharacterInfo::IDLE);

        break;
    case CharacterInfo::IDLE:
    default:
        InitPlayerAnimation(CharacterInfo::IDLE);

        walkAnimationObject->SetSelectBitmapIndex(CharacterInfo::IDLE);             // 초기 이미지로 변경
        DrawPlayerAnimation(TextureName::PLAYER_WALK, player->GetPos());
        break;
    }
}

void RenderManager::DrawNPC()
{
    // NPC 출력
    for (const auto& iterator : (*WorldMapManager::GetInstance()->GetWorldMap()->GetNPCData()))
    {
        HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::NPC, iterator.imageIndex);

        BITMAP bit;
        SelectObject(backMemDC, bitmap);
        GetObject(bitmap, sizeof(bit), &bit);
        TransparentBlt(memDC,
            iterator.pos.x * TILE_SIZE, iterator.pos.y * TILE_SIZE,
            bit.bmWidth, bit.bmHeight,
            backMemDC, 0, 0, bit.bmWidth, bit.bmHeight, RGB(215, 123, 186));
    }
}

void RenderManager::DrawMonster()
{
    for (int i = 0; i < static_cast<int>(ImageManager::GetInstance()->GetMonsterAnimation()->size()); i++)
    {
        if (i >= static_cast<int>(WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->size()) || i < 0)
            return;

        AnimationObject* animationObject = &(*ImageManager::GetInstance()->GetMonsterAnimation())[
            (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[i].GetIndex()];

        switch ((*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[i].GetState())
        {
        case CharacterInfo::WALK:
        case CharacterInfo::ATTACK:
            animationObject->SetSelectAnimationBitmapIndex(
                (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[i].GetDir());       // 방향에 따른 방향 애니메이션 설정

            DrawMonsterAnimation(i);
            animationObject->NextSelectBitmapIndex();                               // 출력 이미지 위치 변경
            break;
        case CharacterInfo::IDLE:
        case CharacterInfo::HIT:
            animationObject->SetSelectAnimationBitmapIndex(
                (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[i].GetDir());       // 방향에 따른 방향 애니메이션 설정

            animationObject->SetSelectBitmapIndex(CharacterInfo::IDLE);             // 초기 이미지로 변경
            DrawMonsterAnimation(i);
        default:
            break;
        }
    } 
}

void RenderManager::DrawNpcOrderPos()
{
    for (const auto& iterator : (*WorldMapManager::GetInstance()->GetWorldMap()->GetNPCData()))
    {
        if (!(((iterator.pos.y)*static_cast<long>(TILE_SIZE) + PLAYER_PIVOT_POS.y) > static_cast<long>(
            (GameManager::GetInstance()->GetPlayer()->GetPos().y) + PLAYER_PIVOT_POS.y)))
            continue;

        HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::NPC, iterator.imageIndex);

        BITMAP bit;
        SelectObject(backMemDC, bitmap);
        GetObject(bitmap, sizeof(bit), &bit);
        TransparentBlt(memDC,
            iterator.pos.x * TILE_SIZE, iterator.pos.y * TILE_SIZE,
            bit.bmWidth, bit.bmHeight,
            backMemDC, 0, 0, bit.bmWidth, bit.bmHeight, RGB(215, 123, 186));
    }
}

void RenderManager::DrawMonsterOrderPos()
{
    for (int i = 0; i < static_cast<int>((*WorldMapManager::GetInstance()
        ->GetWorldMap()->GetMonsterData()).size()); i++)
    {
        if (!((*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[i].GetPos().y + MONSTER1_PIVOT_POS.y > static_cast<long>(
            (GameManager::GetInstance()->GetPlayer()->GetPos().y) + PLAYER_PIVOT_POS.y)))
            continue;

        DrawMonsterAnimation(i);
    }
}

void RenderManager::DrawShop()
{
    int left = SHOP_SPAWN_POS.x + SHOP_INTERVAL_SIZE.cx;
    int top = SHOP_SPAWN_POS.y + SHOP_INTERVAL_SIZE.cx + (SHOP_ITEMBOX_SIZE.cy * 0) + (SHOP_INTERVAL_SIZE.cy * 0);
    int right = SHOP_SPAWN_POS.x + SHOP_INTERVAL_SIZE.cx + SHOP_ITEMBOX_SIZE.cx;
    int bottom = SHOP_SPAWN_POS.y + SHOP_INTERVAL_SIZE.cx + SHOP_ITEMBOX_SIZE.cy + (SHOP_ITEMBOX_SIZE.cy * 0) + (SHOP_INTERVAL_SIZE.cy * 0);

    //  상점 창 출력
    Rectangle(memDC, SHOP_SPAWN_POS.x, SHOP_SPAWN_POS.y, SHOP_SPAWN_POS.x + SHOP_SIZE.cx, SHOP_SPAWN_POS.y + SHOP_SIZE.cy);

    // 아이템 칸 출력
    for (int i = 0; i < SELL_ITEM_SIZE; i++)
    {
        Rectangle(memDC, SHOP_ITEM_BOX_POS[i].left,
            SHOP_ITEM_BOX_POS[i].top,
            SHOP_ITEM_BOX_POS[i].right,
            SHOP_ITEM_BOX_POS[i].bottom);
    }

    int t = 0;
    for (const auto& iterator: (*(*NPCManager::GetInstance()->GetshopNPCVector())
        [NPCManager::GetInstance()->GetInteractNPCData().index].GetSellItemId()))
    {
        HBITMAP bitmap = ImageManager::GetInstance()->GetBitmapData(BitmapKind::ITEM, iterator);

        BITMAP bit;
        SelectObject(backMemDC, bitmap);
        GetObject(bitmap, sizeof(bit), &bit);
        BitBlt(memDC, SHOP_SPAWN_POS.x + SHOP_INTERVAL_SIZE.cx + SHOP_INTERVAL_SIZE.cy,
            SHOP_SPAWN_POS.y + SHOP_INTERVAL_SIZE.cx + (SHOP_ITEMBOX_SIZE.cy * t) + (SHOP_INTERVAL_SIZE.cy * t) + SHOP_INTERVAL_SIZE.cy,
            bit.bmWidth, bit.bmHeight, backMemDC, 0, 0, SRCCOPY);

        // 아이템 제목 출력
        TextOut(memDC,
            SHOP_SPAWN_POS.x + SHOP_INTERVAL_SIZE.cx + SHOP_INTERVAL_SIZE.cy + TILE_SIZE + SHOP_INTERVAL_SIZE.cy,
            SHOP_SPAWN_POS.y + SHOP_INTERVAL_SIZE.cx + (SHOP_ITEMBOX_SIZE.cy * t) + (SHOP_INTERVAL_SIZE.cy * t) + SHOP_INTERVAL_SIZE.cy,
            (*ItemManager::GetInstance()->GetItemData())[iterator-1].GetTitle().c_str(),
            strlen((*ItemManager::GetInstance()->GetItemData())[iterator-1].GetTitle().c_str()));
        // 아이템 설명 출력
        TextOut(memDC,
            SHOP_SPAWN_POS.x + SHOP_INTERVAL_SIZE.cx + SHOP_INTERVAL_SIZE.cy + TILE_SIZE + SHOP_INTERVAL_SIZE.cy,
            SHOP_SPAWN_POS.y + SHOP_INTERVAL_SIZE.cx + (SHOP_ITEMBOX_SIZE.cy * t) + (SHOP_INTERVAL_SIZE.cy * t) + SHOP_INTERVAL_SIZE.cy + (TILE_SIZE / 2),
            (*ItemManager::GetInstance()->GetItemData())[iterator-1].GetExplain().c_str(),
            strlen((*ItemManager::GetInstance()->GetItemData())[iterator-1].GetExplain().c_str()));
        // 가격 출력
        TextOut(memDC,
            SHOP_SPAWN_POS.x + SHOP_INTERVAL_SIZE.cx + SHOP_ITEMBOX_SIZE.cx - TILE_SIZE,
            SHOP_SPAWN_POS.y + SHOP_INTERVAL_SIZE.cx + (SHOP_ITEMBOX_SIZE.cy * t) + (SHOP_INTERVAL_SIZE.cy * t) + SHOP_INTERVAL_SIZE.cy,
            to_string((*ItemManager::GetInstance()->GetItemData())[iterator - 1].GetPrice()).c_str(),
            strlen(to_string((*ItemManager::GetInstance()->GetItemData())[iterator - 1].GetPrice()).c_str()));

        t++;
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

void RenderManager::DrawTextHud(const DPOINT pos, const string msg)
{
    TextOut(memDC, static_cast<int>(pos.x), static_cast<int>(pos.y), msg.c_str(), strlen(msg.c_str()));
}

void RenderManager::DrawHudVector()
{
    float hpGauge = 0;
    HBRUSH oldBrush;

    // hp bar print
    for (auto& iterator: (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()))
    {
        if (CharacterInfo::ATTACK != iterator.GetState())
            continue;

        // 전체 hp bar 출력 (흰색)
        Rectangle(memDC, static_cast<int>(iterator.GetPos().x + hud.HP_HUD_RECT.left),
            static_cast<int>(iterator.GetPos().y + hud.HP_HUD_RECT.top),
            static_cast<int>(static_cast<int>(iterator.GetPos().x) + hud.HP_HUD_RECT.left + hud.HP_HUD_RECT.right),
            static_cast<int>(iterator.GetPos().y + hud.HP_HUD_RECT.bottom));

        oldBrush = (HBRUSH)SelectObject(memDC, CreateSolidBrush(RGB(255, 0, 0)));   // 빨간색 브러쉬 선택

        hpGauge = static_cast<float>(iterator.GetHp()) / static_cast<float>(iterator.GetMaxHp());   // hp 에 따른 빨간 hp bar 값 계산

        // 잔량 hp bar 출력 (빨간색)
        Rectangle(memDC, static_cast<int>(iterator.GetPos().x + hud.HP_HUD_RECT.left),
            static_cast<int>(iterator.GetPos().y + hud.HP_HUD_RECT.top),
            static_cast<int>(static_cast<int>(iterator.GetPos().x) + hud.HP_HUD_RECT.left + 
                static_cast<int>(((static_cast<float>(hud.HP_HUD_RECT.right)) * hpGauge))),
            static_cast<int>(iterator.GetPos().y + hud.HP_HUD_RECT.bottom));

        oldBrush = (HBRUSH)SelectObject(memDC, oldBrush);   // 빨간색 브러쉬 삭제
        DeleteObject(oldBrush);
    }

    // stringHud
    for (const auto& iterator : (*hud.GetStringHud()))
    {
        SetTextColor(memDC, iterator.color);
        DrawTextHud(iterator.pos, iterator.msg);
    }  
    SetTextColor(memDC, RGB(0, 0, 0));
}

void RenderManager::DrawCheckPattern(HDC hdc, const SIZE size)
{
    // 세로선
    for (int i = 0; i < size.cx / TILE_SIZE; i++)
    {
        MoveToEx(hdc, i * TILE_SIZE, 0, NULL);
        LineTo(hdc, i * TILE_SIZE, size.cy);
    }

    // 가로선
    for (int i = 0; i < size.cy / TILE_SIZE; i++)
    {
        MoveToEx(hdc, 0, i * TILE_SIZE, NULL);
        LineTo(hdc, size.cx, i * TILE_SIZE);
    }
}

void RenderManager::DrawCursorFollowBitmap()
{
    MapEdittor* mapEdittor = MapEdittor::GetInstance();
    SelectMapState selectState = mapEdittor->GetSelectState();

    BitmapKind kind;
    switch (selectState)
    {
    case SelectMapState::BACKGROUND:
        kind = BitmapKind::BACKGROUND;
        break;
    case SelectMapState::OBJECT:
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

void RenderManager::AddHudData(const double x, const double y, const string str, const COLORREF color)
{
    // hud 출력 관련 데이터 세팅
    textHudData hudData;
    hudData.pos = { x, y };
    hudData.msg = str;
    hudData.color = color;
    RenderManager::GetInstance()->GetHud()->GetStringHud()->emplace_back(hudData);
}

void RenderManager::DrawPlayerAnimation(const int uiName, const DPOINT pos)
{
    AnimationObject* animationObject = ImageManager::GetInstance()->GetPlayerAnimationData(uiName);

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

void RenderManager::InitPlayerAnimation(const int state)
{
    AnimationObject* walkAnimationObject = ImageManager::GetInstance()->GetPlayerAnimationData(TextureName::PLAYER_WALK);
    AnimationObject* attackAnimationObject = ImageManager::GetInstance()->GetPlayerAnimationData(TextureName::PLAYER_ATTACK);

    switch (state)
    {
    case CharacterInfo::IDLE:
    case CharacterInfo::WALK:
        attackAnimationObject->SetSelectBitmapIndex(0);                 // 다른 애니메이션 초기화
        walkAnimationObject->SetSelectAnimationBitmapIndex(
            GameManager::GetInstance()->GetPlayer()->GetDir());       // 방향에 따른 방향 애니메이션 설정
        break;
    case CharacterInfo::ATTACK:
        walkAnimationObject->SetSelectBitmapIndex(0);                 // 다른 애니메이션 초기화
        attackAnimationObject->SetSelectAnimationBitmapIndex(
            GameManager::GetInstance()->GetPlayer()->GetDir());       // 방향에 따른 방향 애니메이션 설정
        break;
    }
}

void RenderManager::DrawMonsterAnimation(const int index)
{
    AnimationObject* animationObject = &(*ImageManager::GetInstance()->GetMonsterAnimation())[
        (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[index].GetIndex()];

    if (nullptr == animationObject)
        return;

    int animationIndex = animationObject->GetSelectAnimationBitmapIndex();
    int selectBitmapIndex = animationObject->GetSelectBitmapIndex();
    int count = animationObject->GetBitmapCount(animationIndex);
    if ((*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[index].GetDir() == CharacterInfo::RIGHT)
        animationObject->GetBitmapCount(animationIndex);

    HBITMAP bitmap = animationObject->GetAnimationBitmap(animationIndex);
    BITMAP bit;
    SelectObject(backMemDC, bitmap);
    GetObject(bitmap, sizeof(bit), &bit);
    
    TransparentBlt(memDC, 
        static_cast<int>((*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[index].GetPos().x), 
        static_cast<int>((*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData())[index].GetPos().y),
        bit.bmWidth / count, bit.bmHeight,
        backMemDC, selectBitmapIndex * (bit.bmWidth / count), 0, bit.bmWidth / count,
        bit.bmHeight, RGB(215, 123, 186));
}
