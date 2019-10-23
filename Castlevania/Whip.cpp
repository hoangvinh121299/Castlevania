#include "Whip.h"
void Whip::Render()
{
	animations[0] -> Render(nx,x,y);
	OutputDebugString(L"WHIP IS RENDERED");
}
void Whip::Update(DWORD dt)
{

}
void Whip::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{

}
