#pragma once
#include "CAmmoClip.h"

CAmmoClip::CAmmoClip()
{

}


CAmmoClip::~CAmmoClip()
{

}

void CAmmoClip::createAmmoClip(I3DEngine* myEngine, string meshName)
{
	ammoClipMesh = myEngine->LoadMesh(meshName);


	XPos = 100000.0f;		// sets the starting location of ammo clips to very far away. (gravity effects stop them being initializes underground)
	YPos = 0.0f;
	ZPos = 0.0f;
	rotation = 0.0f;


	// creates the ammo clips, scaling them to the appropriate size and skinning them

	worldModel = ammoClipMesh->CreateModel(XPos, YPos, ZPos);
	worldModel->ScaleY(0.2f);
	worldModel->ScaleX(0.05f);
	worldModel->ScaleZ(0.02f);
	worldModel->SetSkin("AmmoClip.jpg");


}

void CAmmoClip::ammoClipGravity(float frameTime)
{
	//for (int i = 0; i < kNumAmmoClips; i++)  // cycles through all ammo clips
	//{
		if (YPos >= 1.0f) // checks if the ammo clip is above the floor
		{
			YPos -= frameTime * 50; // makes the ammo clip fall by reducing itrs y value
		}
		if (YPos <= 1.0f) // checks if the ammo clip is on the floor
		{
			YPos = 0.8f;  // sets the ammo clip to be on the floor
			rotationLimit = 90; // rotates the ammo clip 90 degrees so it is lying down
			if (rotation < rotationLimit) // checks if the ammo clip has been rotated up to the limit
			{
				worldModel->RotateLocalX(90); // rotates the ammo clip
				rotation += 90; // increases the rotation variable
			}

		}
		worldModel->SetPosition(XPos, YPos, ZPos); // moves the ammo clip to its new position

//	}
}