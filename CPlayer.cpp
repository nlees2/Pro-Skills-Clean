#pragma once
#include "CPlayer.h"
#include "CWeapon.h"
#include "functions.h"

CPlayer::CPlayer(float frameTime, I3DEngine* myEngine)
{
	dummyMesh = myEngine->LoadMesh("dummy.x");
	playerDummy = dummyMesh->CreateModel(0.0f, 10.0f, 0.0f);

	myCamera = new CCamera(myEngine, dummyMesh);
	mPosition.x = myCamera->cameraMod->GetX();
	mPosition.y = myCamera->cameraMod->GetY();
	mPosition.z = myCamera->cameraMod->GetZ();
	myCamera->cameraDummy->AttachToParent(playerDummy);
}


CPlayer::~CPlayer()
{

}

void CPlayer::timers(float &frameTime, I3DEngine* myEngine)
{
	frameTime = myEngine->Timer(); // updates frame time and all of the game speeds in relation to it
	float gameSpeed = 20.0f;
	mMoveSpeed = gameSpeed * frameTime * 2.0f;
	mPlayerOnGround = true;
	mJumpSpeed = (gameSpeed * frameTime) * 3.0f;
	mFallSpeed = (gameSpeed * frameTime) * 1.5f;
	mLookSpeed = (gameSpeed * frameTime) * 3.0f;
	//mJumpTimer = 0;
}

void CPlayer::move(I3DEngine* myEngine)
{
	if (myEngine->KeyHeld(kForwardsKey)) // uses the forwards key to move the player
	{
		playerDummy->MoveLocalZ(mMoveSpeed);
	}

	if (myEngine->KeyHeld(kBackwardsKey)) // uses the backwards key to move the player
	{
		playerDummy->MoveLocalZ(-mMoveSpeed);
	}

	if (myEngine->KeyHeld(kLeftKey)) // uses the left key to move the player
	{
		playerDummy->MoveLocalX(-mMoveSpeed);
	}

	if (myEngine->KeyHeld(kRightKey)) // uses the right key to move the player
	{
		playerDummy->MoveLocalX(mMoveSpeed);
	}
}

void CPlayer::Look(I3DEngine* myEngine)
{
	const float xRotationLimit = 180;	 // limiter to stop the camera rotating further than 90 degrees in the X (directly up and down)
	float mouseMovement = myEngine->GetMouseMovementY(); // captures the players mouse movement (up and down)

	playerDummy->RotateY(myEngine->GetMouseMovementX() * mLookSpeed);  // rotates the camera according to mouse movement

	if (mouseMovement < 0 && myCamera->xRotationCounter < xRotationLimit) // checks if the mouse was moved up and the player os not already looking straight up
	{
		myCamera->cameraDummy->RotateLocalX(mouseMovement * mLookSpeed); // rotates the camera dummy to look up
		myCamera->xRotationCounter += abs(mouseMovement) * mLookSpeed;   // increments the rotation counter
	}
	else if (mouseMovement > 0 && myCamera->xRotationCounter > 0) // checks if the mouse was moved down and the player os not already looking straight down
	{
		myCamera->cameraDummy->RotateLocalX(mouseMovement * mLookSpeed); // rotates the camera dummy to look down
		myCamera->xRotationCounter -= abs(mouseMovement) * mLookSpeed;	// decrements the rotation counter
	}
}

void CPlayer::Jump(I3DEngine* myEngine, float frameTime)
{
	if (myEngine->KeyHit(kJumpKey) && mPlayerOnGround == true) // causes the player to jump, but will only run if they are not already jumping
	{
		mJumpTimer = 0.5f;
	}
	if (mJumpTimer > 0) // causes the player to move upwards for 0.5 seconds after jumping
	{
		playerDummy->MoveY(mJumpSpeed);
		mJumpTimer -= frameTime;
		mPlayerOnGround = false;
	}
	if (playerDummy->GetY() > 0) // if the player is above the ground, makes them fall, simulating gravity
	{
		playerDummy->MoveY(-mFallSpeed);
	}
	else // else catch to prevent the player from falling below the ground
	{
		playerDummy->SetY(0.0f);
		mPlayerOnGround = true;
	}
}

bool CPlayer::raycastShoot(vector3D facingVector, vector3D dummyPosition, vector<target> &target, IModel* bulletTracer, const int ktargetQuantity, CPlayer myplayer)
{

	//int gunDamage = 1;
	//float fireRate = 0.25f;
	float weaponRange = 300.0f;  // the distance that the ray will travel, longer range means more calculations
								 //float hitForce = 100.0f;
	bool hitWall = false;
	bool hitObject = false;      // bool to see if an object was hit
	float distToObject = 1.0f;	 // the value incremented for the ray trace (how far the ray travels before being checked) 


	float testPointX;
	float testPointY;
	float testPointZ;

	vector3D rayTrace;


	rayTrace.x = facingVector.x * distToObject;
	rayTrace.y = facingVector.y * distToObject;
	rayTrace.z = (facingVector.z * distToObject);

	testPointX = dummyPosition.x + rayTrace.x;
	testPointY = dummyPosition.y + rayTrace.y;
	testPointZ = dummyPosition.z + rayTrace.z;

	for (int i = 0; i < ktargetQuantity; i++)
	{

		for (distToObject = 0; distToObject < weaponRange; distToObject++)
			//while (!hitObject && distToObject < rayLength)
		{
			//distToObject += 1.0f;
			testPointX += rayTrace.x;
			testPointY += rayTrace.y;
			testPointZ += rayTrace.z;


			if (target[i].currentTargetState == ready)
			{

				hitObject = SphereToBox2(testPointX, testPointY + 22.0, testPointZ, target[i].xSize, target[i].ySize, target[i].zSize, target[i].xPosition, target[i].yPosition, target[i].zPosition, 0.05f);
				if (hitObject)
				{


					bulletTracer->AttachToParent(target[i].robberTarget);
					bulletTracer->SetPosition(testPointX - rayTrace.x, testPointY + 22.0, testPointZ - rayTrace.z);

					float tempX = bulletTracer->GetX();
					float tempY = bulletTracer->GetY();
					float tempZ = bulletTracer->GetZ();

					target[i].currentTargetState = hit;
					//cube[i]->SetY(20.0f);

					return true;
				}
			}

		}

		testPointX = dummyPosition.x + rayTrace.x;
		testPointY = dummyPosition.y + rayTrace.y;
		testPointZ = dummyPosition.z + rayTrace.z;
		////////////////////////////////////////

	}
	return false;
}

bool CPlayer::raycastWall(vector3D facingVector, vector3D dummyPosition, vector<model> &wall, IModel* bulletTracer, const int ktargetQuantity, int numberOfWalls)
{

	//int gunDamage = 1;
	//float fireRate = 0.25f;
	float weaponRange = 200.0f;  // the distance that the ray will travel, longer range means more calculations
								 //float hitForce = 100.0f;
	bool hitWall = false;
	bool hitObject = false;      // bool to see if an object was hit
	float distToObject = 1.0f;	 // the value incremented for the ray trace (how far the ray travels before being checked) 


	float testPointX;
	float testPointY;
	float testPointZ;

	vector3D rayTrace;


	rayTrace.x = facingVector.x * distToObject;
	rayTrace.y = facingVector.y * distToObject;
	rayTrace.z = (facingVector.z * distToObject);

	testPointX = dummyPosition.x + rayTrace.x;
	testPointY = dummyPosition.y + rayTrace.y;
	testPointZ = dummyPosition.z + rayTrace.z;

	for (int i = 0; i < numberOfWalls; i++)
	{

		for (distToObject = 0; distToObject < weaponRange; distToObject++)
			//while (!hitObject && distToObject < rayLength)
		{
			//distToObject += 1.0f;
			testPointX += rayTrace.x;
			testPointY += rayTrace.y;
			testPointZ += rayTrace.z;


			hitObject = SphereToBox2(testPointX, testPointY, testPointZ, wall[i].xSize, wall[i].ySize, wall[i].zSize, wall[i].XPos, wall[i].YPos, wall[i].ZPos, 0.05f);
			if (hitObject)
			{
				bulletTracer->SetPosition(testPointX, testPointY + 22.0, testPointZ);
				return true;
			}


		}

		testPointX = dummyPosition.x + rayTrace.x;
		testPointY = dummyPosition.y + rayTrace.y;
		testPointZ = dummyPosition.z + rayTrace.z;
		////////////////////////////////////////

	}
	return false;
}
bool CPlayer::raycastMenu(vector3D facingVector, vector3D dummyPosition, IModel* &target, IModel* bulletTracer, CPlayer myplayer, int blockYSize)
{

	//int gunDamage = 1;
	//float fireRate = 0.25f;
	float weaponRange = 200.0f;  // the distance that the ray will travel, longer range means more calculations
								 //float hitForce = 100.0f;
	bool hitWall = false;
	bool hitObject = false;      // bool to see if an object was hit
	float distToObject = 1.0f;	 // the value incremented for the ray trace (how far the ray travels before being checked) 


	float testPointX;
	float testPointY;
	float testPointZ;

	vector3D rayTrace;


	rayTrace.x = facingVector.x * distToObject;
	rayTrace.y = facingVector.y * distToObject;
	rayTrace.z = (facingVector.z * distToObject);

	testPointX = dummyPosition.x + rayTrace.x;
	testPointY = dummyPosition.y + rayTrace.y;
	testPointZ = dummyPosition.z + rayTrace.z;

	

		for (distToObject = 0; distToObject < weaponRange; distToObject++)
			//while (!hitObject && distToObject < rayLength)
		{
			//distToObject += 1.0f;
			testPointX += rayTrace.x;
			testPointY += rayTrace.y;
			testPointZ += rayTrace.z;

			hitObject = SphereToBox2(testPointX, testPointY + 22.0, testPointZ, 10.0f, blockYSize, 1.0f, target->GetX(), target->GetY(), target->GetZ(), 0.05f);
			if (hitObject)
			{
				bulletTracer->SetPosition(testPointX, testPointY + 22.0, testPointZ);
				return true;
			}
			

		}

		testPointX = dummyPosition.x + rayTrace.x;
		testPointY = dummyPosition.y + rayTrace.y;
		testPointZ = dummyPosition.z + rayTrace.z;
		////////////////////////////////////////

	
	return false;
}

bool CPlayer::raycastName(vector3D facingVector, vector3D dummyPosition, IModel* &target, IModel* bulletTracer, CPlayer myplayer, int blockYSize)
{

	//int gunDamage = 1;
	//float fireRate = 0.25f;
	float weaponRange = 200.0f;  // the distance that the ray will travel, longer range means more calculations
								 //float hitForce = 100.0f;
	bool hitWall = false;
	bool hitObject = false;      // bool to see if an object was hit
	float distToObject = 1.0f;	 // the value incremented for the ray trace (how far the ray travels before being checked) 


	float testPointX;
	float testPointY;
	float testPointZ;

	vector3D rayTrace;


	rayTrace.x = facingVector.x * distToObject;
	rayTrace.y = facingVector.y * distToObject;
	rayTrace.z = (facingVector.z * distToObject);

	testPointX = dummyPosition.x + rayTrace.x;
	testPointY = dummyPosition.y + rayTrace.y;
	testPointZ = dummyPosition.z + rayTrace.z;



	for (distToObject = 0; distToObject < weaponRange; distToObject++)
		//while (!hitObject && distToObject < rayLength)
	{
		//distToObject += 1.0f;
		testPointX += rayTrace.x;
		testPointY += rayTrace.y;
		testPointZ += rayTrace.z;

		hitObject = SphereToBox2(testPointX, testPointY + 22.0, testPointZ, 5.0f, blockYSize, 5.0f, target->GetX(), target->GetY(), target->GetZ(), 0.05f);
		if (hitObject)
		{
			return true;
		}


	}

	testPointX = dummyPosition.x + rayTrace.x;
	testPointY = dummyPosition.y + rayTrace.y;
	testPointZ = dummyPosition.z + rayTrace.z;
	////////////////////////////////////////


	return false;
}

collisionSide CPlayer::SphereToBox(float playerX, float playerZ, float cubeXLength, float cubeZLength, float cubeXPos, float cubeZPos, float playerRadius, float playerOldX, float playerOldZ)
{
	float minX = cubeXPos - (cubeXLength / 2.0f);			// Runs a sphere to box collision detection
	float maxX = cubeXPos + (cubeXLength / 2.0f);			// The bounding box is created using the objects position, its width, and its length.
	float minZ = cubeZPos - (cubeZLength / 2.0f);
	float maxZ = cubeZPos + (cubeZLength / 2.0f);

	collisionSide result = noSide;

	if ((playerX > minX - playerRadius && playerX < maxX + playerRadius) && // The function checks if the sphere, created with an objects position and radius, is overlapping the box
		(playerZ > minZ - playerRadius && playerZ < maxZ + playerRadius))
	{
		if (playerOldX < minX) result = leftSide;
		else if (playerOldX > maxX) result = rightSide;
		else if (playerOldZ < minZ) result = frontSide;
		else if (playerOldZ > maxZ)  result = backSide;
	}

	return  (result);

}

bool CPlayer::SphereToBox2(float pointX, float pointY, float pointZ, float cubeXLength, float cubeYLength, float cubeZLength, float cubeXPos, float cubeYPos, float cubeZPos, float sphereRadius)
{
	float minX = cubeXPos - (cubeXLength / 2.0f);
	float maxX = cubeXPos + (cubeXLength / 2.0f);
	float minY = cubeYPos - (cubeYLength / 2.0f);
	float maxY = cubeYPos + (cubeYLength / 2.0f);
	float minZ = cubeZPos - (cubeZLength / 2.0f);
	float maxZ = cubeZPos + (cubeZLength / 2.0f);



	if ((pointX > minX - sphereRadius && pointX < maxX + sphereRadius) &&
		(pointZ > minZ - sphereRadius && pointZ < maxZ + sphereRadius) &&
		(pointY > minY - sphereRadius && pointY < maxY + sphereRadius))
	{
		return true;
	}
	else
	{
		return false;
	}

}

void CPlayer::ResolveCollision(CPlayer myPlayer, collisionSide collision)
{
	if (collision == frontSide)			 // Checks if the collision occured on the front side of the wall 'chain''
	{
		myPlayer.playerDummy->SetZ(playerOldZ - 0.15f); // moves the player back
	}
	if (collision == backSide)			// Checks if the collision occured on the back side of the wall 'chain'
	{
		myPlayer.playerDummy->SetZ(playerOldZ + 0.15f); // moves the player forwards
	}
	if (collision == leftSide)			// Checks if the collision occured on the left side of the wall 'chain'
	{
		myPlayer.playerDummy->SetX(playerOldX - 0.15); // moves the player right
	}
	if (collision == rightSide)			// Checks if the collision occured on the right side of the wall 'chain'
	{
		myPlayer.playerDummy->SetX(playerOldX + 0.15f); // moves teh player left
	}
}

void CPlayer::ResolveCollisionReverse(CPlayer myPlayer, collisionSide collision)
{
	if (collision == frontSide)			 // Checks if the collision occured on the front side of the wall 'chain''
	{
		myPlayer.playerDummy->SetZ(playerOldZ + 0.15f); // moves the player back
	}
	if (collision == backSide)			// Checks if the collision occured on the back side of the wall 'chain'
	{
		myPlayer.playerDummy->SetZ(playerOldZ - 0.15f); // moves the player forwards
	}
	if (collision == leftSide)			// Checks if the collision occured on the left side of the wall 'chain'
	{
		myPlayer.playerDummy->SetX(playerOldX + 0.15); // moves the player right
	}
	if (collision == rightSide)			// Checks if the collision occured on the right side of the wall 'chain'
	{
		myPlayer.playerDummy->SetX(playerOldX - 0.15f); // moves teh player left
	}
}

void CPlayer::LoadHighScore(vector<highScore> &highScores)
{
	float scoreTemp;
	float timeTemp;
	float killsPerMinTemp;
	string nameTemp;

	highScore playerScore;

	ifstream fileInput;
	fileInput.open("highscores.txt"); // opens the input file

	if (fileInput.is_open()) // Checks to see that the file has been opened sucessfully
	{
		//int highScoreQty = highScores.size();
		for (int i = 0; i < NUMBEROFHIGHSCORES; i++)
		{
			fileInput >> nameTemp;

			if (nameTemp == "")
			{
				nameTemp = "unknown";
			}

			fileInput >> scoreTemp;

			fileInput >> timeTemp;
			fileInput >> killsPerMinTemp;

			playerScore.score = scoreTemp;
			playerScore.name = nameTemp;
			playerScore.time = timeTemp;
			playerScore.killsPerMinute = killsPerMinTemp;

			highScores.push_back(playerScore);
		}

	}

}

void CPlayer::SaveHighScore(vector<highScore> &highScores, CPlayer currentPlayer, float time, string playerName)
{
	ofstream fileInput;
	fileInput.open("highscores.txt"); // opens the input file
	if (highScores.size() >= 10)
	{
	
		highScores.pop_back();
	
	}
	highScore playerScore;
	playerScore.name = playerName;
	playerScore.score = currentPlayer.score;
	playerScore.time = time;
	playerScore.killsPerMinute = playerScore.score / (playerScore.time / 60);
	highScores.push_back(playerScore);
	sort(highScores.begin(), highScores.end(), compare);

	if(fileInput.is_open()) // Checks to see that the file has been opened sucessfully
	{
		int highScoreQty = highScores.size();

		for (int i = 0; i < highScoreQty; i++)
		{
			if (highScores[i].name == "")
			{
				highScores[i].name = "unknown";
			}

			fileInput << highScores[i].name << " " << highScores[i].score << " " << highScores[i].time << " " << highScores[i].killsPerMinute << endl;
		}

	}

}