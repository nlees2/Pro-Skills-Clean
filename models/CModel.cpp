#pragma once
#include "CModel.h"
#include "CPlayer.h"
#include "Definitions.h"

CModel::CModel()
{

}


CModel::~CModel()
{

}

using ammoClip = CModel[kNumAmmoClips];		// creates the array of models for ammo clips, used when reloading. (this is only for visual effects, weapon ammo is part of the weapon struct)
ammoClip mapAmmoClip;

CTarget::CTarget()
{

}


CTarget::~CTarget()
{

}

void CTarget::createTarget(I3DEngine* myEngine, string meshName, const int numberOfTargets)
{
	// creates the models for the targets.
	for (int i = 0; i < numberOfTargets; i++)
	{
		YPos = targetYPosition;

		targetMesh = myEngine->LoadMesh(meshName);

		// creates each target based of the values stored in the vector (initialized by the map file)
		worldModel = targetMesh->CreateModel(XPos, YPos, ZPos);

		// scales the targets to the appropriate size
		worldModel->ScaleY(1.2f);
		worldModel->ScaleX(1.2f);
		worldModel->ScaleZ(0.2f);
		worldModel->SetSkin("robber.jpg"); // skins the target to look like a robber (enemy)

		currentTargetState = waiting; // sets the state of the target to waiting
		resetTimer = 0;			   // sets the reset timer to 0

		worldModel->RotateLocalY(rotation); // rotates the target to face the correct direction (based off values in the map file)

		int temp;																	 // this for loop checks if the target was rotated. if so it rotates the targets collision boxes
		if (rotation == 90.0f || rotation == 270.0f)
		{
			temp = xSize;
			xSize = zSize;
			zSize = temp;
		}
	}

}

void CTarget::updateTarget(CPlayer &myPlayer, float frameTime, const int numberOfTargets)
{
	for (int i = 0; i < numberOfTargets; i++) // cycles through the targets
	{
		if (hostage == true && resetTimer > 0) // checks if the target is in hostage mode (not to be shot)
		{
			resetTimer -= frameTime; // counts down on the hostage timer
		}
		else if (hostage == true && resetTimer < 0) // checks if teh hostage timer has expired
		{
			currentTargetState = waiting; // sets the taget to waiting
			resetTimer = (rand() % 10) + 2; // sets the reset timer to a random number 
		}

		if (currentTargetState == hit) // checks if the target is hit
		{
			if (hostage == false) // checks if the taraget was a hostage
			{
				myPlayer.score += 1; // increases the players scorre
			}
			else
			{
				myPlayer.score -= 1; // decreases the players score
			}
			currentTargetState = waiting; // sets the target to waiting mode
			resetTimer = (rand() % 10) + 2; // sets the reset timer to a random number
		}
		else if (currentTargetState == waiting && resetTimer <= 0) // checks if the target is waiting and the timer has finished
		{
			if ((rand() % 10) == 1) // gives a 1 in 10 chance for the next target to be a hostage
			{
				hostage = true;	// sets the target to hostage
				resetTimer = 8.0f; // sets the reset timer to 8 seconds
			}
			else
			{
				hostage = false;  // sets the target to enemy
				resetTimer = 0.0f; // sets the reset timer to 0
			}
			currentTargetState = ready;

		}
		else if (currentTargetState == waiting && resetTimer > 0) // checks if the target is waiting and if the reset timer is above 0
		{
			if (targetSinkLimit > 0) // checks if the target Y position is above the sink limit
			{
				worldModel->MoveLocalY(-myPlayer.mLookSpeed); // moves the target down in the Y
				targetSinkLimit -= myPlayer.mLookSpeed;		 // decreements the sink limit
			}
			resetTimer -= frameTime; // decrements the reset timer
		}
		else if (currentTargetState == ready) // checks if the target is ready
		{
			if (targetSinkLimit < 15) // checks if the target is below the sink limit
			{
				worldModel->MoveLocalY(myPlayer.mLookSpeed); // moves the target up in the Y
				targetSinkLimit += myPlayer.mLookSpeed;		// increments the sink limit
			}
		}

		if (hostage == true) // checks if the target is a hostage
		{
			worldModel->SetSkin("hostage.jpg"); // sets the skin to show a hostage
		}
		else
		{
			worldModel->SetSkin("robber.jpg");  // sets the skin to show a robber
		}

	}
}