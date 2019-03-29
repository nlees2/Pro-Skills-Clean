#pragma once
#include <TL-Engine.h>
#include "Functions.h"

using namespace tle;

vector2D scalar(float s, vector2D v)
{
	return { s * v.x, s * v.z };
}


void readMap(int &numberOfWalls, int &numberOfBoxes, int & numberOfTargets, int &numberOfWallBoxes, vector<model> &wall, vector<collision> &wallCollision, vector<model> &box, vector<target> &targetThief, string &mapName)
{

	mapName = ("Map2.txt");
	float worldMultiplier = 1.0f;;
	string linetest;
	string comment;
	float temp;

	if (mapName == "Map1.txt")
	{
		worldMultiplier = 20.0f;
	}

	vector<IModel*>xModelLine;

	ifstream fileInput;
	fileInput.open(mapName); // opens the input file

	if (fileInput.is_open()) // Checks to see that the file has been opened sucessfully
	{
		// For loop to read in file
		//fileInput >> temp; // reads in the first value from the file and sets the qty variable


		// reads in the levels walls

		fileInput >> numberOfWalls;

		for (int i = 0; i < numberOfWalls; i++) // runs through the whole file, based on the value from the first line of the file
		{
			model wallBlock;
			fileInput >> temp;// inputs the x elements from the file
			wallBlock.XPos = temp * worldMultiplier;

			fileInput >> temp;	// inputs the z elements from the file
			wallBlock.ZPos = temp * worldMultiplier;

			fileInput >> temp;
			wallBlock.rotation = temp;

			wallBlock.xSize = 15.0f;
			wallBlock.ySize = 22.5f;
			wallBlock.zSize = 15.0f;

			wall.insert(wall.begin(), wallBlock);
		}


		// reads in seperate collisions for walls

		if (mapName == "Map1.txt")
		{

			fileInput >> numberOfWallBoxes;

			for (int i = 0; i < numberOfWallBoxes; i++) // runs through the whole fil
			{
				collision wallBox;
				fileInput >> temp;// inputs the x elements from the file
				wallBox.xPosition = temp * worldMultiplier;

				fileInput >> temp;	// inputs the z elements from the file
				wallBox.zPosition = temp * worldMultiplier;

				fileInput >> temp;
				wallBox.xSize = temp * worldMultiplier;

				wallBox.ySize = 22.5f;

				fileInput >> temp;
				wallBox.zSize = temp * worldMultiplier;

				/*fileInput >> temp;
				wallBox.rotationAmount = temp;*/

				/*if (wallBox.rotationAmount == 90.0f)
				{
				temp = wallBox.xSize;
				wallBox.xSize = wallBox.zSize;
				wallBox.zSize = temp;
				}*/

				wallCollision.insert(wallCollision.begin(), wallBox);
			}

		}
		// reads in the levels boxes
		fileInput >> numberOfBoxes;

		for (int i = 0; i < numberOfBoxes; i++) // runs through the whole file, based on the value from the first line of the file
		{
			model boxBlock;
			fileInput >> temp;// inputs the x elements from the file
			boxBlock.XPos = temp * worldMultiplier;

			fileInput >> temp;	// inputs the z elements from the file
			boxBlock.ZPos = temp * worldMultiplier;

			box.insert(box.begin(), boxBlock);
		}

		// reads in the levels boxes
		fileInput >> numberOfTargets;

		for (int i = 0; i < numberOfTargets; i++)// runs through the whole file, based on the value from the first line of the file
		{
			target targetBlock;
			fileInput >> temp;// inputs the x elements from the file
			targetBlock.xPosition = temp * worldMultiplier;

			fileInput >> temp;	// inputs the z elements from the file
			targetBlock.zPosition = temp * worldMultiplier;

			fileInput >> temp;	// inputs the z elements from the file
			targetBlock.rotationAmount = temp;

			targetBlock.hostage = false;

			cout << targetBlock.xPosition / 15 << " " << targetBlock.zPosition / 15 << endl;

			targetThief.insert(targetThief.begin(), targetBlock);
		}

		fileInput.close(); // Closes the file


	}

}

void weaponAnimation(float &animationTimer, IModel* M4Colt, IModel* player, float frameTime)
{
	animationTimer -= frameTime;
	if (animationTimer < 0.1f && animationTimer > 0.05f)
	{
		// move weapon backwards
		M4Colt->MoveLocalZ(-frameTime * 5);
	}
	else if (animationTimer > 0.0f && animationTimer <= 0.05f)
	{
		// move weapon forwards

		M4Colt->MoveLocalZ(frameTime * 5);
	}

	else if (animationTimer < 0.0f)
	{
		M4Colt->SetPosition(player->GetX(), player->GetY(), player->GetZ());
		M4Colt->SetLocalX(2.0f);
		M4Colt->SetLocalY(10.0f);
		M4Colt->SetLocalZ(7.0f);
	}

	float x = M4Colt->GetX();
	float y = M4Colt->GetY();
	float z = M4Colt->GetZ();



}

void desertEagleAnimation(float &animationTimer, IModel* desertEagle, IModel* player, float frameTime)
{
	animationTimer -= frameTime;
	if (animationTimer < 0.1f && animationTimer > 0.05f)
	{
		// move weapon backwards
		desertEagle->MoveLocalZ(-frameTime * 2);
		desertEagle->MoveLocalY(frameTime * 2);
		desertEagle->RotateLocalX(-frameTime * 800);
	}
	else if (animationTimer > 0.0f && animationTimer <= 0.05f)
	{
		// move weapon forwards

		desertEagle->MoveLocalZ(frameTime * 2);
		desertEagle->MoveLocalY(-frameTime * 2);
		desertEagle->RotateLocalX(frameTime * 800);
	}

	else if (animationTimer < 0.0f)
	{
		desertEagle->SetPosition(player->GetX(), player->GetY(), player->GetZ());
		desertEagle->SetLocalX(2.0f);
		desertEagle->SetLocalY(10.0f);
		desertEagle->SetLocalZ(7.0f);
		desertEagle->ResetOrientation();
	}

	float x = desertEagle->GetX();
	float y = desertEagle->GetY();
	float z = desertEagle->GetZ();



}
int nextInArray(int i, int arraySize) // Function to select the next point in a patrol (or cycle through any array)
{
	int lastCheckpoint = i; // number in the array for current checkpoint (one reached)
	int nextCheckpoint;  // number in the array for the next checkpoint

	nextCheckpoint = (lastCheckpoint + 1) % arraySize;  // cycles to the next member of the array using modulus (remainder) 0>1>2>3>4>0

	return nextCheckpoint;
}

int previousInArray(int i, int arraySize) // Function to select the previous point in a patrol (or cycle through any array)
{
	int lastCheckpoint = i; // number in the array for current checkpoint (one reached)
	int previousCheckpoint;  // number in the array for the next checkpoint

	previousCheckpoint = (lastCheckpoint - 1) % arraySize;  // cycles to the previous member of the array using modulus (remainder) 0>1>2>3>4>0

	if (previousCheckpoint == -1)
	{
		previousCheckpoint = 25;
	}

	return previousCheckpoint;
}

//void fireWeapon(player player, IModel* playerDummy, IModel* cameraDummy, IModel* bulletTracer[], vector3D facingVector, float lengthOfFV, vector3D fvNormal, vector3D dummyPosition, int bulletTracerSelection, const int kNumBulletTracers, string mapName,
//				wall mapWall,)
//{
//	float pMatrix[4][4];
//	float cMatrix[4][4];
//
//	if (player.currentWeapon.ammo <= 0 && player.currentWeapon.currentWeaponState != reloading)
//	{
//		player.currentWeapon.currentWeaponState = reloading;
//		player.currentWeapon.reloadTimer = 0.0f;
//	}
//	if (player.currentWeapon.currentWeaponState == active)
//	{
//		playerDummy->GetMatrix(&pMatrix[0][0]); // calls the matrix for the player, recording both the facing vector and the world position
//		cameraDummy->GetMatrix(&cMatrix[0][0]); // calls the matrix for the camera, recording both the facing vector and the world position
//
//		facingVector = { pMatrix[2][0], cMatrix[2][1], pMatrix[2][2] };
//		lengthOfFV = sqrt((pMatrix[2][0] * pMatrix[2][0]) + (cMatrix[2][1] * cMatrix[2][1]) + (pMatrix[2][2] * pMatrix[2][2]));
//		fvNormal = { pMatrix[2][0] / lengthOfFV, cMatrix[2][1] / lengthOfFV, pMatrix[2][2] / lengthOfFV };
//
//		dummyPosition = { pMatrix[3][0], pMatrix[3][1], pMatrix[3][2] };
//
//		bulletTracerSelection = (bulletTracerSelection + 1) % kNumBulletTracers;
//
//		bool wallHit = false;
//		if (mapName == "Map1.txt")
//		{
//			if (mapWall.size() != 0)
//			{
//				wallHit = raycastWall(fvNormal, dummyPosition, mapWall, bulletTracer[bulletTracerSelection], numberOfTargets, numberOfWalls);
//			}
//		}
//		if (!wallHit)
//		{
//			raycastShoot(fvNormal, dummyPosition, mapTarget, bulletTracer[bulletTracerSelection], numberOfTargets);//, 12.0f, 12.0f, 2.0f);
//		}
//
//		player.currentWeapon.ammo -= 1;
//		player.animationTimer = 0.1f;
//	}
//}
bool compare(const highScore& a, const  highScore& b)
{
	return (a.score > b.score);
}