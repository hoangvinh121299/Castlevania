#include "Ground.h"

void Ground::Render()
{
	animations[0]->Render(nx, x, y);
	/*RenderBoundingBox();*/
}

void Ground::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x-20;
	t = y;
	r = x + GROUND_BBOX_WIDTH;
	b = y + GROUND_BBOX_HEIGHT;
}