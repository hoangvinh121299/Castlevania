#include "GameObject.h"
class Whip :public CGameObject
{
private:
public:
	void Update(DWORD dt);
	 virtual void Render();
    virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
