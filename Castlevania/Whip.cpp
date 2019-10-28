#include "Whip.h"
#include "Torch.h"
#include "Ground.h"
#include "Simon.h"
#include "Textures.h"
void Whip::Render()
{
	animations[0] -> Render(nx,x,y);
	RenderBoundingBox();
}
void Whip::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (animations[0]->getCurrentFrame() == 2)
	{
		
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT temp = coObjects->at(i);
			if (dynamic_cast<Ground*>(temp)) continue;
			float top, left, bottom, right;
			coObjects->at(i)->GetBoundingBox(left, top, right, bottom);
			if (isCollide(left, top, right, bottom))
			{

				if (dynamic_cast<Torch *>(temp)) // if e->obj is Goomba 
				{
					OutputDebugString(L"Torch is Attacked\n");
				}
			}
		}
	}
	
}
void Whip::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (nx > 0)
	{ 
		left = x + 134;
		top = y + 15;
		right = left + WHIP_BBOX_WIDTH;
		bottom = top + WHIP_BBOX_HEIGHT;
	}
	else
	{
		left = x + 50;
		top = y + 15;
		right = left + WHIP_BBOX_WIDTH;
		bottom = top + WHIP_BBOX_HEIGHT;
	}
}
bool Whip::isCollide(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;
	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return CGameObject::checkAABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}