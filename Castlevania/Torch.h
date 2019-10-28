#pragma once
#define TORCH_BBOX_WIDTH 30
#define TORCH_BBOX_HEIHT 60
#include "GameObject.h"
class Torch:public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

