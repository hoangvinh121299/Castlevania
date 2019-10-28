#include "Torch.h"
void Torch::Render()
{
	animations[0]->Render(nx, x, y);
	RenderBoundingBox();
}
void Torch::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + TORCH_BBOX_WIDTH;
	bottom = y + TORCH_BBOX_HEIHT;
}