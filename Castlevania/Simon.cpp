#include "Simon.h"
#include "Game.h"
#include "Whip.h"
#include <algorithm>
DWORD now;
static Whip *whip;
Simon::Simon()
{
    whip = new Whip();
	whip->AddAnimation(300);
	whip->SetPosition(x - 85, y + 3);
}
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
	whip->nx = this->nx;
	/*if (state == SIMON_STATE_SIT&&isAttacking==1)
	{
		whip->SetPosition(x - 85, y + 10);
	}*/
	if(isSitting==1)
		whip->SetPosition(x - 85, y + 15);
	else
	whip->SetPosition(x - 85, y + 3);
	if (this->attackTime != 0)
	{
		//
		if (GetTickCount() - this->attackTime > 300) //thời gian render 4 sprits là 400. render xong 4 cái thì reset.
		{
			this->attackTime= 0;//reset
			this->isAttacking = 0;
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
		if (isJumping != 0)
		{
			isJumping = 0;
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
			if (isAttacking != 0)
			{
				
				whip->Render();
				if (isJumping != 0)
					ani = SIMON_ANI_ATTACK_AIR;
				else
					ani = SIMON_ANI_ATTACK;
				if (isSitting != 0)
					ani = SIMON_ANI_ATTACK_SIT;
				else
					ani = SIMON_ANI_ATTACK;
			}
			else 
			{
				if (isJumping != 0)
				{
					ani = SIMON_ANI_JUMP;
					OutputDebugString(L"SIMON JUMP RIGHT \n");
				}
				else
				{
					ani = SIMON_ANI_IDLE;
					OutputDebugString(L"SIMON IDLE RIGHT \n");
				}
				if (isSitting != 0)
					ani = SIMON_ANI_SIT;
			}
		}
		else
		{
			if (isAttacking != 0)
			{
				whip->Render();
				if (isJumping != 0)
					ani = SIMON_ANI_ATTACK_AIR;
				else
					ani = SIMON_ANI_ATTACK;
				if (isSitting != 0)
					ani = SIMON_ANI_ATTACK_SIT;
			}
			else
			{
				if (isJumping != 0)
					ani = SIMON_ANI_JUMP;
				else
					ani = SIMON_ANI_IDLE;
				if (isSitting != 0)
					ani = SIMON_ANI_SIT;
			}
		}
	}
	else if (vx > 0)
	{
		if (isAttacking != 0)
		{
			whip->Render();
			if (isJumping != 0)
				ani = SIMON_ANI_ATTACK_AIR;
			else
				ani = SIMON_ANI_ATTACK;
			if (isSitting != 0)
				ani = SIMON_ANI_ATTACK_SIT;
			
		}
		else
		{
			if (isJumping != 0)
				ani = SIMON_ANI_JUMP;
			else
				ani = SIMON_ANI_WALKING;
			if (isSitting != 0)
				ani = SIMON_ANI_SIT;
			
		}
	}
	else
	{
		if (isAttacking != 0)
		{
			whip->Render();
			if (isJumping != 0)
				ani = SIMON_ANI_ATTACK_AIR;
			else
				ani = SIMON_ANI_ATTACK;
			if (isSitting != 0)
				ani = SIMON_ANI_ATTACK_SIT;
			
		}
		else
		{
			if (isJumping != 0)
				ani = SIMON_ANI_JUMP;
			else
				ani = SIMON_ANI_WALKING;
			if (isSitting != 0)
				ani = SIMON_ANI_SIT;
			
		}
	}
	animations[ani]->Render(nx,x, y);
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
		isSitting = 0;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		isSitting = 0;
		break;
	case SIMON_STATE_JUMP:
		isJumping = 1;
		isSitting = 0;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_SIT:
		vx = 0;
		isSitting = 1;
		break;
	case SIMON_STATE_IDLE:
		isSitting = 0;
		vx = 0;
		break;
	case SIMON_STATE_ATTACK:
		isAttacking = 1;
		vx = 0;
	
		this->animations[SIMON_ANI_ATTACK]->ResetAni();
		this->animations[SIMON_ANI_ATTACK_AIR]->ResetAni();
		this->animations[SIMON_ANI_ATTACK_SIT]->ResetAni();
		whip->animations[0]->ResetAni();
		this->attackTime = GetTickCount(); //start attack
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
	left = x+15;
	top = y;
	right = left + 35;
	bottom = top + SIMON_BBOX_HEIGHT;
}

