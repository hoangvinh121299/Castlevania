#pragma once
#include "GameObject.h"

#define GROUND_BBOX_WIDTH  35
#define GROUND_BBOX_HEIGHT 35

class Ground : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};