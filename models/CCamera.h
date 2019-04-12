#pragma once
#include "Functions.h"
class CCamera
{
public:
	float xRotationCounter = 90;		 // incrementad variable used with xRotationLimit. initialized to 90 degree angle
	ICamera * cameraMod;
	IModel * cameraDummy;
	CCamera(I3DEngine* myEngine, IMesh* dummyMesh);
	~CCamera();
};
