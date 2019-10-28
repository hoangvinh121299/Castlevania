﻿/* =============================================================
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
#include "Whip.h"
#include "Torch.h"
#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"CASTLEVANIA"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255,255, 200)
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 300

#define MAX_FRAME_RATE 120

#define ID_TEX_SIMON 0
#define ID_TEX_ENEMY 10
#define ID_TEX_GROUND 20
#define ID_TEX_WHIP 30
#define ID_TEX_TORCH 40

CGame *game;
Simon *simon;
Torch *torch;
Whip *whis;
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
		if (simon->getisJumping() == 0)
		{
			simon->SetState(SIMON_STATE_JUMP);
		}
		break;
	case DIK_Z:
	{
		if (simon->attackTime == 0)
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
	if (simon->attackTime != 0)
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
	textures->Add(ID_TEX_WHIP, L"Textures\\Weapons\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TORCH, L"Textures\\Ground\\torch.png", D3DCOLOR_XRGB(255, 0, 255));
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	//ADD SPRITES OF GROUND
	LPDIRECT3DTEXTURE9 texGround = textures->Get(ID_TEX_GROUND);
	sprites->Add(10001, 0, 0,31,31 ,texGround);
	//ADD SPRITES OF SIMON
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	sprites->Add(20001,0,0,60,66,texSimon);//SIMON IDLE 
	sprites->Add(20002, 60, 0, 120, 66, texSimon);
	sprites->Add(20003, 120, 0, 180, 66, texSimon);
	sprites->Add(20004, 180, 0, 240, 66, texSimon);


	sprites->Add(20021, 240, 0, 300, 66, texSimon);//Simon jump or sit


	sprites->Add(20041, 300, 0, 360, 66, texSimon);//simon atack 
	sprites->Add(20042, 360, 0,420 , 66, texSimon);
	sprites->Add(20043, 420, 0, 480, 66, texSimon);



	sprites->Add(20061, 420, 66, 480, 132, texSimon);//simon sit atack 
	sprites->Add(20062, 0,  132, 60,198, texSimon);
	sprites->Add(20063, 60, 132,120 , 198,texSimon);


	//Add Animation Simon

	LPDIRECT3DTEXTURE9 texWhip = textures->Get(ID_TEX_WHIP);

	sprites->Add(30001, 0,   0, 240, 66, texWhip);//Add whip attack 
	sprites->Add(30002, 240, 0, 480, 66, texWhip);
	sprites->Add(30003, 480, 0, 720, 66, texWhip);
	LPANIMATION ani;

	//Add Whip animation
	ani = new CAnimation(100);
	ani->Add(30001);
	ani->Add(30002);
	ani->Add(30003);
	animations->Add(300, ani);

	simon = new Simon();

	ani = new CAnimation(100);//Simon_Ani_IDLE_RIGHT
	ani->Add(20001);
	animations->Add(200, ani);
	simon->AddAnimation(200);


	ani = new CAnimation(100);//simon walking right
	ani->Add(20001);
	ani->Add(20002);
	ani->Add(20003);
	ani->Add(20004);
	animations->Add(210, ani);
	simon->AddAnimation(210);



	ani = new CAnimation(100);//Simon_Ani_jump_RIGHT
	ani->Add(20021);
	animations->Add(220, ani);
	simon->AddAnimation(220);

	ani = new CAnimation(100);//simon atack right 
	ani->Add(20041);
	ani->Add(20042);
	ani->Add(20043);
	animations->Add(240, ani);
	simon->AddAnimation(240);

	ani = new CAnimation(100);//simon atack sit right

	ani->Add(20061);
	ani->Add(20062);
	ani->Add(20063);
	animations->Add(260, ani);
	simon->AddAnimation(260);
	objects.push_back(simon);
	simon->SetPosition(0.0f, 100.0f);

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
	

	
	
	//Add Sprite Torch
	LPDIRECT3DTEXTURE9 texTorch = textures->Get(ID_TEX_TORCH);
	sprites->Add(40001, 0, 0, 32, 64, texTorch);
	sprites->Add(40002, 32, 0, 64, 64, texTorch);
	ani = new CAnimation(100);
	ani->Add(40001);
	ani->Add(40002);
	animations->Add(400, ani);
	for (int i = 0; i < 4; i++)
	{
		torch = new Torch();
		torch->AddAnimation(400);
		torch->SetPosition(200 + 100* i, 170);
		objects.push_back(torch);
	}
	//whip = new Whip();
	//whip->SetPosition(0, 100.0f);
	//whip->AddAnimation(300);
	//objects.push_back(whip);

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
		objects[i]->Update(dt,&coObjects);
	}


	// Update camera to follow mario
	float cx, cy;
	simon->GetPosition(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
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
//Sửa con mẹ nó cái lỗi trùng nhau tọa độ boudingbox thì không va chạm hộ bố  mày cái
