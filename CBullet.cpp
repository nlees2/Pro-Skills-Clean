#pragma once
#include "CBullet.h"

CBullet::CBullet()
{

}


CBullet::~CBullet()
{

}

void CBullet::createBullet(I3DEngine* myEngine, string meshName)
{
	bulletMesh = myEngine->LoadMesh(meshName);


	XPos = 100000.0f;		// sets the starting location of ammo clips to very far away. (gravity effects stop them being initializes underground)
	YPos = 0.0f;
	ZPos = 0.0f;
	rotation = 0.0f;


	// creates the ammo clips, scaling them to the appropriate size and skinning them

	worldModel = bulletMesh->CreateModel(XPos, YPos, ZPos);
	worldModel->ScaleY(0.02f);
	worldModel->ScaleX(0.01f);
	worldModel->ScaleZ(0.01f);
}

void CBullet::BulletGravity(float frameTime)
{
	//for (int i = 0; i < kNumAmmoClips; i++)  // cycles through all ammo clips
	//{
	if (YPos >= 1.0f) // checks if the ammo clip is above the floor
	{
		YPos -= frameTime * 25; // makes the ammo clip fall by reducing itrs y value
		worldModel->RotateY(2.0f);
	}
	if (YPos <= 1.0f) // checks if the ammo clip is on the floor
	{
		YPos = 0.8f;  // sets the ammo clip to be on the floor
		//rotationLimit = 90; // rotates the ammo clip 90 degrees so it is lying down
		//if (rotation < rotationLimit) // checks if the ammo clip has been rotated up to the limit
		//{
		//	worldModel->RotateLocalX(90); // rotates the ammo clip
		//	rotation += 90; // increases the rotation variable
		//}

	}
	worldModel->SetPosition(XPos, YPos, ZPos); // moves the ammo clip to its new position

//	}
}

void CBullet::SetPosition(int weaponXPos, int weaponYPos, int weaponZPos, int &currentBullet)
{
	XPos = weaponXPos; // sets the X Position of the current ammo clip
	YPos = weaponYPos + 0.5f; // sets the Y Position of the current ammo clip
	ZPos = weaponZPos; // sets the Z Position of the current ammo clip
	rotation = 0.0f;							// sets the rotation variable of the ammo clip to upright
	worldModel->ResetOrientation();			// resets teh ammo clips orientation
	worldModel->RotateLocalZ(90);
	currentBullet = nextInArray(currentBullet, kNumBullets);			// sets the current ammo clip to the next in the array, ready for the next reload
}