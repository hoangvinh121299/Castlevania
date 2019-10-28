#include "GameObject.h"
#define WHIP_BBOX_WIDTH 50
#define WHIP_BBOX_HEIGHT 15
#define WHIP_MOVING_SPEED	0.1f
#define WHIP_JUMP_SPEED_Y	0.4f
class Whip :public CGameObject
{
private:
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	bool isCollide(float obj_left, float obj_top, float obj_rihgt, float obj_bottom);
	virtual void Render();
    virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
