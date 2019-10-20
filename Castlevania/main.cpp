/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102

	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions:
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision

		CGameObject::GetBoundingBox

================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Ground.h"
#include "Simon.h"
#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"CASTLEVANIA"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255,255, 200)
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 300

#define MAX_FRAME_RATE 120

#define ID_TEX_SIMON 0
#define ID_TEX_ENEMY 10
#define ID_TEX_GROUND 20

CGame *game;
Simon *simon;
vector<LPGAMEOBJECT> objects;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_Z:
	{
		if (simon->attack == 0)
		{
			simon->SetState(SIMON_STATE_ATTACK);
		}
		break;
	}
	//case DIK_DOWN:
	//	simon->SetState(SIMON_STATE_SIT);
	
	//case DIK_A: // reset
	//	mario->SetState(MARIO_STATE_IDLE);
	//	mario->SetLevel(MARIO_LEVEL_BIG);
	//	mario->SetPosition(50.0f, 0.0f);
	//	mario->SetSpeed(0, 0);
	//	break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	if (simon->attack != 0)
		return;
	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_SIT);
	else if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT); 
	else
		simon->SetState(SIMON_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_GROUND, L"Textures\\Ground\\2.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_SIMON, L"Textures\\Simon\\SIMON.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	//ADD SPRITES OF GROUND
	LPDIRECT3DTEXTURE9 texGround = textures->Get(ID_TEX_GROUND);
	sprites->Add(10001, 0, 0,31,31 ,texGround);
	//ADD SPRITES OF SIMON
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	sprites->Add(20001,431,2,468,65,texSimon);//SIMON IDLE RIGHT
	sprites->Add(20002, 374, 2, 400, 65, texSimon);
	sprites->Add(20003, 314, 2, 340, 65, texSimon);
	sprites->Add(20004, 254, 2, 280, 65, texSimon);

	sprites->Add(20011, 491, 2, 528, 65, texSimon);//SIMON IDLE LEFT
	sprites->Add(20012, 551, 2, 588, 65, texSimon);
	sprites->Add(20013, 611, 2, 648, 65, texSimon);
	sprites->Add(20014, 671, 2, 708, 65, texSimon);

	sprites->Add(20021, 194, 2, 240, 65, texSimon);//Simon jump right

	sprites->Add(20031, 731, 2, 758, 65, texSimon);//Simon jump left

	sprites->Add(20041, 130, 2, 177, 65, texSimon);//simon atack right
	sprites->Add(20042, 70, 2, 115, 65, texSimon);
	sprites->Add(20043, 10, 2, 62, 65, texSimon);

	sprites->Add(20051, 792, 2, 829, 65, texSimon);//simon atack left
	sprites->Add(20052, 848, 2,897 , 65, texSimon);
	sprites->Add(20053, 908, 2, 958, 65, texSimon);

	sprites->Add(20061,0, 67, 40, 134, texSimon);//simon sit atack right
	sprites->Add(20062, 430,132, 481, 195, texSimon);
	sprites->Add(20063, 370, 132, 421, 195,texSimon);

	sprites->Add(20071, 915, 67, 955, 134, texSimon);//simon sit atack left
	sprites->Add(20072, 491, 132, 528, 195, texSimon);
	sprites->Add(20073, 550, 132, 598, 195, texSimon);
	//sprites->Add(20041, 194, 2, 240, 65, texSimon);//Simon jump right
	//Add Animation Ground
	LPANIMATION ani;
	ani = new CAnimation(100);
	ani->Add(10001);
	animations->Add(100, ani);
	for (int i = 0; i < SCREEN_WIDTH / 31; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(100);
		ground->SetPosition(0+31*i,230);
		objects.push_back(ground);
	}
	//Add Animation Simon
	 simon = new Simon();

	ani = new CAnimation(100);//Simon_Ani_IDLE_RIGHT
	ani->Add(20001);
	animations ->Add(200, ani);
	simon->AddAnimation(200);

	ani = new CAnimation(100);//Simon_Ani_IDLE_left
	ani->Add(20011);
	animations->Add(201, ani);
	simon->AddAnimation(201);

	ani = new CAnimation(100);//simon walking right
	ani->Add(20001);
	ani->Add(20002);
	ani->Add(20003);
	ani->Add(20004);
	animations->Add(210, ani);
	simon->AddAnimation(210);

	ani = new CAnimation(100);//simon walking left
	ani->Add(20011);
	ani->Add(20012);
	ani->Add(20013);
	ani->Add(20014);
	animations->Add(211, ani);//simon walking left
	simon->AddAnimation(211);
	
	ani = new CAnimation(100);//Simon_Ani_jump_RIGHT
	ani->Add(20021);
	animations->Add(221, ani);
	simon->AddAnimation(221);
	
	ani = new CAnimation(100);//Simon_Ani_jump_left
	ani->Add(20031);
	animations->Add(231, ani);
	simon->AddAnimation(231);

	ani = new CAnimation(100);//simon atack right 
	ani->Add(20001);
	ani->Add(20041);
	ani->Add(20042);
	ani->Add(20043);
	animations->Add(241, ani);
	simon->AddAnimation(241);

	ani = new CAnimation(100);//simon atack left
	ani->Add(20011);
	ani->Add(20051);
	ani->Add(20052);
	ani->Add(20053);
	animations->Add(251, ani);
	simon->AddAnimation(251);


	ani = new CAnimation(100);//simon atack sit right
	ani->Add(20021);
	ani->Add(20061);
	ani->Add(20062);
	ani->Add(20063);
	animations->Add(261, ani);
	simon->AddAnimation(261);

	ani = new CAnimation(100);//simon atack sit left
	ani->Add(20031);
	ani->Add(20071);
	ani->Add(20072);
	ani->Add(20073);
	animations->Add(271, ani);
	simon->AddAnimation(271);

	simon->SetPosition(0.0f, 100.0f);
	objects.push_back(simon);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt );
	}
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
		
		}
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

      	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);
	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}
//FIX LỖI SIMON, ADD THÊM HIỆU ỨNG TẤN CÔNG, CHO SIMON TƯƠNG TÁC TRỌNG LỰC