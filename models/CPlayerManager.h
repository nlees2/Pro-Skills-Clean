#pragma once
#include "Functions.h"
class CPlayerManager
{
public:
	float mMoveSpeed;
	bool mPlayerOnGround;
	float mJumpSpeed;
	float mFallSpeed;
	float mLookSpeed;
	vector3D mPosition;
	//CGunManager mPlayerWeapon;

	CPlayerManager();
	virtual void move(I3DEngine* myEngine) = 0;
	virtual void Look(I3DEngine* myEngine) = 0;
	virtual void Jump(I3DEngine* myEngine, float frameTime) = 0;
	~CPlayerManager();
};