#include "Simon.h"
#include "Game.h"
void Simon::Update(DWORD dt)
{
	CGameObject::Update(dt);
	// simple fall down
	vy += SIMON_GRAVITY * dt;
	y += dy;
	x += dx;
	if (y >= 100)
	{
		vy = 0; y = 100.0f;
	}
	//// simple screen edge collision!!!
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
				if (fx < 0)ani = SIMON_ANI_ATTACK_SIT_RIGHT;
				else 
					ani = SIMON_ANI_ATTACK_RIGHT;
			}
			else
			{
				if (fx < 0)ani = SIMON_ANI_SIT_RIGHT;
				else
					ani = SIMON_ANI_IDLE_RIGHT;
			}
		}
		else
		{
			if (atk != 0)
			{
				if (fx < 0)ani = SIMON_ANI_ATTACK_SIT_LEFT;
				else 
					ani = SIMON_ANI_ATTACK_LEFT;
			}
			else
			{
				if (fx < 0)ani = SIMON_ANI_SIT_LEFT;
				else
					ani = SIMON_ANI_IDLE_LEFT;
			}
		}
	}
	else if (vx > 0)
	{
		if (atk != 0)
			ani = SIMON_ANI_ATTACK_RIGHT;
		else
			ani = SIMON_ANI_WALKING_RIGHT;
	}
	else
	{
		if (atk != 0)
			ani = SIMON_ANI_ATTACK_LEFT;
		else
			ani = SIMON_ANI_WALKING_LEFT;
	}
	//render simon jump
	 if (vy!=0)
	 {
		 
		 if (nx > 0)
		 {
			 if (atk != 0) ani = SIMON_ANI_ATTACK_AIR_RIGHT;
			 else
			 ani = SIMON_ANI_JUMP_RIGHT;
		 }
		 else
		 {
			 if (atk != 0) ani = SIMON_ANI_ATTACK_AIR_LEFT;
			 else
				 ani = SIMON_ANI_JUMP_LEFT;
		 }
	 }
	animations[ani]->Render(x, y);
}
void Simon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		fx = 0;
	
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		fx = 0;
	
		break;
	case SIMON_STATE_JUMP:
		fx = 1;
		atk = 0;
		if (y == 100)
		{
			vy = -SIMON_JUMP_SPEED_Y;
		}
		break;
	case SIMON_STATE_SIT:
		vx = 0;
		fx = -1;
		
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		fx = 0;
		
		break;
	case SIMON_STATE_ATTACK:
		atk = 1;
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
void Simon::GetBoundingBox(float &l, float &t, float &r, float &b)
{

}
