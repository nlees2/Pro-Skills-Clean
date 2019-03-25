#pragma once
#include "CModel.h"
#include "Definitions.h"

class CGrenade : public CModel
{
public:

	IMesh * grenadeMesh;

	CGrenade();
	~CGrenade();
	void createGrenade(I3DEngine* myEngine, string meshName);
	void GrenadeGravity(float frameTime);
	void SetPosition(int weaponXPos, int weaponYPos, int weaponZPos, int &currentBullet);
	void Detonate(IModel* playerModel, vector3D playerFvNormal, IModel* grenade);
};