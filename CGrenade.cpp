#pragma once
#include "CGrenade.h"

CGrenade::CGrenade()
{

}


CGrenade::~CGrenade()
{

}

void CGrenade::createGrenade(I3DEngine* myEngine, string meshName)
{
	grenadeMesh = myEngine->LoadMesh(meshName);
	

	XPos = 100000.0f;		// sets the starting location of grenades to very far away. (gravity effects stop them being initializes underground)
	YPos = 0.0f;
	ZPos = 0.0f;
	rotation = 0.0f;

	worldModel = grenadeMesh->CreateModel(XPos, YPos, ZPos);
	worldModel->ScaleY(0.15f);
	worldModel->ScaleX(0.15f);
	worldModel->ScaleZ(0.15f);
}

void CGrenade::GrenadeGravity(float frameTime, int &currentGrenade)
{
	if (YPos >= 1.0f) // checks if the grenade is above the floor
	{
		YPos -= frameTime * 50; // makes the grenade fall by reducing itrs y value
		worldModel->RotateY(2.0f);
		worldModel->RotateX(0.5f);
		worldModel->RotateZ(0.5f);
	}
	if (YPos <= 1.0f) // checks if the grenade is on the floor
	{
		YPos = 0.8f;  // sets the grenade to be on the floor


	}
	worldModel->SetPosition(XPos, YPos, ZPos);			// sets the grenades new position
}

void CGrenade::SetGrenadePosition(int newXPos, int newYPos, int newZPos, int &currentGrenade)
{
	XPos = newXPos; // sets the X Position of the current ammo clip
	YPos = newYPos + 0.5f; // sets the Y Position of the current ammo clip
	ZPos = newZPos; // sets the Z Position of the current ammo clip
	rotation = 0.0f;							// sets the rotation variable of the ammo clip to upright

	
}

void CGrenade::Detonate(IModel* playerModel, vector3D playerFvNormal, IModel* grenade, IModel* &flashEffect, bool &flashExploded, bool &playerFlashed, bool soundEnabled)
{
	//worldModel->LookAt(playerModel);

	vector3D grenadeFV;

	grenadeFV.x = grenade->GetX() - playerModel->GetX();
	grenadeFV.y = grenade->GetY() - playerModel->GetY();
	grenadeFV.z = grenade->GetZ() - playerModel->GetZ();

	float gLengthOfFV = sqrt((grenadeFV.x * grenadeFV.x) + (grenadeFV.y * grenadeFV.y) + (grenadeFV.z * grenadeFV.z)); // calculates the  length of the facing vector
	vector3D gFvNormal = { grenadeFV.x / gLengthOfFV, grenadeFV.y / gLengthOfFV, grenadeFV.z / gLengthOfFV };	// normalizes the facing



	float dotproduct = (playerFvNormal.x * gFvNormal.x) + (playerFvNormal.y * gFvNormal.y) + (playerFvNormal.z * gFvNormal.z);

	float angle = acos(dotproduct);

	if (dotproduct > 0.2f && flashExploded == false)
	{
		playerFlashed = true;
		flashEffect->SetLocalY(10.0f);
		flashEffect->SetLocalX(40.0f);
	}
	flashExploded = true;

	// dot product between nade and player

	// move white sprite to in front of camera

	// move sprite to the left to fade out

}