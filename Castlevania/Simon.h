#pragma once
#include "GameObject.h"

#define SIMON_WALKING_SPEED		0.1f
#define SIMON_JUMP_SPEED_Y		0.4f
#define SIMON_SIT_SPEED_Y       0.2f
#define SIMON_GRAVITY			0.001f
//SIMON_STATE_NORMAL
#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP		300
#define SIMON_STATE_SIT             400
#define SIMON_STATE_ATTACK          500
#define SIMON_STATE_DIE				600
//SIMON_ANI_NORMAL
#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1
#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3
#define SIMON_ANI_JUMP_LEFT 5
#define SIMON_ANI_JUMP_RIGHT 4
#define SIMON_ANI_SIT_RIGHT    4
#define SIMON_ANI_SIT_LEFT     5
#define SIMON_ANI_ATTACK_RIGHT 6
#define SIMON_ANI_ATTACK_LEFT 7
#define SIMON_ANI_ATTACK_AIR_RIGHT 8
#define SIMON_ANI_ATTACK_AIR_LEFT  9
#define SIMON_ANI_ATTACK_SIT_RIGHT 8
#define SIMON_ANI_ATTACK_SIT_LEFT 9
class Simon:public CGameObject
{
private:
	int fx; //check is simon is sit or jump
	int atk;//check is simon is attack or not
public:
	void Update(DWORD dt);
	void SetState(int state);
	int getState() { return this->state; }
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

