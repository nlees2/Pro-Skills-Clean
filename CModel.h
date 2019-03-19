#include "TL-Engine.h"
#include "CPlayer.h"
using namespace tle;

#pragma once
class CModel
{
public:

	float XPos;
	float ZPos;
	float YPos = 0;
	float rotation;
	float rotationLimit;
	bool onFloor;
	int xSize;
	int ySize;
	int zSize;
	IModel* worldModel;

	CModel();
	~CModel();
};

class CTarget : public CModel
{
public:

	IMesh * targetMesh;
	float resetTimer;
	bool hostage;
	float targetSinkLimit = 15;
	targetState currentTargetState;
	float targetYPosition = 28.0f;



	CTarget();
	~CTarget();
	void createTarget(I3DEngine* myEngine, string meshName, const int numberOfTargets);
	void updateTarget(CPlayer &myPlayer, float frameTime, const int numberOfTargets);
};
