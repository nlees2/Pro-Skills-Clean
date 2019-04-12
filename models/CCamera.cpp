#pragma once
#include "CCamera.h"



CCamera::CCamera(I3DEngine* myEngine, IMesh* dummyMesh)
{
	cameraMod = myEngine->CreateCamera(kManual);
	cameraMod->SetPosition(0.0f, 12.0f, 0.0f);	// sets the camera height to be the players point of view
	cameraDummy = dummyMesh->CreateModel(0, 10.0f, 0);
	cameraMod->AttachToParent(cameraDummy);		// attatches the camera to the dummy, which is then also attatched to the player dummy.
}


CCamera::~CCamera()
{
}