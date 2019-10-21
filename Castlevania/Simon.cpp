#include "Simon.h"
#include "Game.h"
#include "Whip.h"
#include <algorithm>
DWORD now;
Whip *whip;
//Simon::Simon()
//{
//	whip = new Whip();
//	whip->AddAnimation(271);
//}
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	// simple fall down
	DWORD now = GetTickCount();
	vy += SIMON_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	if (this->attack != 0)
	{
		//
		if (GetTickCount() - this->attack > 400) //thời gian render 4 sprits là 400. render xong 4 cái thì reset.
		{
			this->attack= 0;//reset
			this->atk = 0;
		}
	}
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	//// reset untouchable timer if untouchable time has passed
	//if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		if (fx > 0)
		{
			fx = 0;
		}
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		//
		//// simple screen edge collision!!!
	}
}
void Simon::Render()
{
	int ani;
	if (vx == 0)
	{
		if (nx > 0)
		{
				if (atk != 0)
				{
					
					if (fx < 0)
					{
						ani = SIMON_ANI_ATTACK_SIT_RIGHT;
						OutputDebugString(L"ATTACK Sit right");
					}
					else if (fx > 0)
					{
						ani = SIMON_ANI_ATTACK_AIR_RIGHT;
					}
					else
					{
						OutputDebugString(L"ATTACK  right");
						ani = SIMON_ANI_ATTACK_RIGHT;
					}
				}
				else
			{
					if (fx < 0)
					{
						OutputDebugString(L" Sit right");
						ani = SIMON_ANI_SIT_RIGHT;
					}
					else if (fx > 0)
					{
						ani = SIMON_ANI_JUMP_RIGHT;
					}
					else
					{
						ani = SIMON_ANI_IDLE_RIGHT;
						OutputDebugString(L"Right idle");
					}
			}
		}
		else
		{
			if (atk != 0)
			{
				
				if (fx < 0)ani = SIMON_ANI_ATTACK_SIT_LEFT;
				else if (fx > 0)
				{
					ani = SIMON_ANI_ATTACK_AIR_LEFT;
				}
				else
					ani = SIMON_ANI_ATTACK_LEFT;
			}
			else
			{
				if (fx < 0)ani = SIMON_ANI_SIT_LEFT;
				else if (fx > 0)
				{
					ani = SIMON_ANI_JUMP_LEFT;
				}
				else
					ani = SIMON_ANI_IDLE_LEFT;
			}
		}
	}
	else if (vx > 0)
	{
		
		if (atk != 0)
		{
			if (fx > 0)
				ani = SIMON_ANI_ATTACK_AIR_RIGHT;
			else
			ani = SIMON_ANI_ATTACK_RIGHT;
		}
		else
		{
			if (fx > 0)
				ani = SIMON_ANI_JUMP_RIGHT;
			else
				ani = SIMON_ANI_WALKING_RIGHT;
		}
	}
	else
	{
		if (atk != 0)
		{
			if (fx > 0)
				ani = SIMON_ANI_ATTACK_AIR_LEFT;
			else
			ani = SIMON_ANI_ATTACK_LEFT;
		}
		else
		{
			if (fx > 0)
				ani = SIMON_ANI_JUMP_LEFT;
			else
			ani = SIMON_ANI_WALKING_LEFT;
		}
	}
	//render simon jump
	/*if (fx=1)
	{

		if (nx > 0)
		{
			vx= SIMON_WALKING_SPEED;
			if (atk != 0) ani = SIMON_ANI_ATTACK_AIR_RIGHT;
			else
			ani = SIMON_ANI_JUMP_RIGHT;
		}
		else
		{
			vx = -SIMON_WALKING_SPEED;
			if (atk != 0) ani = SIMON_ANI_ATTACK_AIR_LEFT;
			else
			ani = SIMON_ANI_JUMP_LEFT;
		}
	}*/
	//render simon attack
	/*if (now >= SIMON_ATTACK_SPEED)
	{
		if (vx == 0)
		{
			if (nx > 0)
			{
				{
					if (fx < 0)ani = SIMON_ANI_ATTACK_SIT_RIGHT;
					else
						ani = SIMON_ANI_ATTACK_RIGHT;
				}
			}
			else
			{
				if (fx < 0)ani = SIMON_ANI_ATTACK_SIT_LEFT;
				else
					ani = SIMON_ANI_ATTACK_LEFT;
			}
		}
	}*/
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}
void Simon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		fx = 1;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_SIT:
		vx = 0;
		fx = -1;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_ATTACK:
		atk = 1;
		vx = 0;
		///*whip->SetPosition();*/
		//whip->Render();
		this->attack = GetTickCount(); //start attack
		break;
	/*case SIMON_STATE_SIT:
		vx = 0;
		break;*/
	//	//WHITE MARIO STATE 
	//case MARIO_STATE_WHITE_WALKING_RIGHT:
	//	status = 1;
	//	vx = MARIO_WALKING_SPEED;
	//	nx = 1;
	//	fx = 0;
	//	break;
	//case MARIO_STATE_WHITE_WALKING_LEFT:
	//	status = 1;
	//	vx = -MARIO_WALKING_SPEED;
	//	nx = -1;
	//	fx = 0;
	//	break;
	//case MARIO_STATE_WHITE_JUMP:
	//	status = 1;
	//	if (y == 100)
	//		vy = -MARIO_JUMP_SPEED_Y;
	//	fx = 0;
	//	break;
	//case MARIO_STATE_WHITE_IDLE:
	//	status = 1;
	//	fx = 0;
	//	vx = 0;
	//	break;
	//case MARIO_STATE_WHITE_SIT:
	//	status = 1;
	//	fx = 1;
	//	break;
	//	//LITTLE MARIO STATE
	//case MARIO_STATE_LITTLE_WALKING_RIGHT:
	//	status = 2;
	//	vx = MARIO_WALKING_SPEED;
	//	nx = 1;
	//	fx = 0;
	//	break;
	//case MARIO_STATE_LITTLE_WALKING_LEFT:
	//	status = 2;
	//	vx = -MARIO_WALKING_SPEED;
	//	nx = -1;
	//	fx = 0;
	//	break;
	//case MARIO_STATE_LITTLE_JUMP:
	//	status = 2;
	//	if (y == 100)
	//		vy = -MARIO_JUMP_SPEED_Y;
	//	fx = 0;
	//	break;
	//case MARIO_STATE_LITTLE_IDLE:
	//	fx = 0;
	//	vx = 0;
	//	break;
	}
}
void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
}

