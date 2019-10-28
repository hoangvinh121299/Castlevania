#pragma once
#include "GameObject.h"
#define SIMON_ATTACK_SPEED      500
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
#define SIMON_ANI_IDLE		0
#define SIMON_ANI_WALKING	1	
#define SIMON_ANI_JUMP 2
#define SIMON_ANI_SIT    2
#define SIMON_ANI_ATTACK 3
#define SIMON_ANI_ATTACK_AIR 3
#define SIMON_ANI_ATTACK_SIT 4

#define SIMON_BBOX_WIDTH  45
#define SIMON_BBOX_HEIGHT 60
class Simon:public CGameObject
{
private:
	int isJumping; //check  simon isjumping ?
	int isAttacking;//check simon is attack or not
	int isSitting; // Check  Simon is siiting or not 
public:
	DWORD attackTime;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	Simon();
	void SetState(int state);
	int getState() { return this->state; }
	void setisJumping(int isJumping) { this->isJumping = isJumping; }
	int getisJumping() { return this->isJumping; }
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

