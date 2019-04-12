#pragma once
#include "CModel.h"
#include "CWeapon.h"
#include "Definitions.h"

class CBullet : public CModel
{
public:

	IMesh * bulletMesh;




	CBullet();
	~CBullet();
	void createBullet(I3DEngine* myEngine, string meshName);
	void BulletGravity(float frameTime);
	void SetPosition(int weaponXPos, int weaponYPos, int weaponZPos, int &currentBullet);
};

