// pro skills rebuilt.cpp: A program using the TL-Engine

#pragma once
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "Functions.h"
#include "Definitions.h"
#include "CPlayer.h"
#include "CWeapon.h"
#include "CAmmoClip.h"
#include "CBullet.h"
#include "Sound.h"
#include "CGrenade.h"
using namespace std;
using namespace tle;


void main()
{

	int resolutionX = 1920; // X size of the game window
	int resolutionY = 1080; // Y size of the game window

	float frameTime = 1.0f;				 // frame time used to control game speed

							// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed(1920, 1080);
	//myEngine ->StartFullscreen(resolutionX,resolutionY);

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("./models");

	/**** Set up your scene here ****/

	targetState currentTargetStare = waiting;
	playerState currentPlayerState = playing;
	activeWeapon currentActiveWeapon = M4ColtWeapon;

	vector<highScore> highScores;

	CWeapon M4Colt;
	CWeapon desertEagle;
	CWeapon currentWeapon;

	CPlayer myPlayer(frameTime, myEngine);
	myPlayer.currentPlayerState = notPlaying;
	myPlayer.mSoundEnabled = true;
	myPlayer.mPlayerFlashed = false;

	CAmmoClip ammoClip[kNumAmmoClips];
	for (int i = 0; i < kNumAmmoClips; i++)
	{
		ammoClip[i].createAmmoClip(myEngine, "Block.x");
	}

	CBullet bullet[kNumBullets];
	for (int i = 0; i < kNumBullets; i++)
	{
		bullet[i].createBullet(myEngine, "BulletCasing.x");
	}

	CGrenade grenade[kNumGrenades];
	for (int i = 0; i < kNumGrenades; i++)
	{
		grenade[i].createGrenade(myEngine, "flashBang.x");
		grenade[i].grenadeTimer = 1.0f;
		grenade[i].flashExploded = false;
	}


	//for (int i = 0; i < kNumBullets; i++)
	//{
	//	bullet[i] = bulletMesh->CreateModel(0.0f, -10.0f, 0.0f); // hides the bullet tracers under the world at game start
	//	//bullet[i]->Scale(0.1f);
	//}

	//////////////// Variables

	int currentAmmoClip = 0; // this int will cycle through the numbers 0 - kNumBulletTracers and choose which model to move on each reload
	int currentBullet = 0;
	int currentGrenade = 0;
	int temp; // used for reassigning values
	bool grenadeDropped = false;

	int bulletTracerSelection = 0;  // this int will cycle through the numbers 0 - kNumAmmoClips and choose which model to move to each bullet hole

	const float dummyRadius = 2.0f; // radius of the player and camera dummies used in player collision

	bool boxHit = false; // bool to measure if the player has shot a box
	float pMatrix[4][4];	 // matrix used to store the player models information to be passed to functions such as facing vector and world location
	float cMatrix[4][4];	 // matrix used to store the camera models information to be passed to functions such as facing vector and world location
	float gMatrix[4][4];	 // matrix used to store the grenade models information to be passed to functions such as facing vector and world location

	stringstream scoreText; // screen score output
	stringstream ammoText; // screen ammo output
	stringstream reloadText; // screen reload output
	stringstream timeText;  // screen timer output
	stringstream highScoreText; // high score output
	stringstream fpsText;   // screen fps output
	ISprite* crosshair;     // players crosshair
	ISprite* flashBlind;

	string mapName; // name of the map file to be read in (initialized in read map function)

	int numberOfWalls;		// Number of walls in the map, initialized in read map by the number in the map file (cannot be constant for this reason)
	int numberOfBoxes;		// Number of boxes in the map, initialized in read map by the number in the map file (cannot be constant for this reason)
	int numberOfTargets;	// Number of targets in the map, initialized in read map by the number in the map file (cannot be constant for this reason)
	int numberOfWallBoxes;	// Number of wall collision boxes in the map, initialized in read map by the number in the map file (cannot be constant for this reason)

	vector3D facingVector;				 // Stores the players facing vector, used in ray tracing for bullet collisions
	vector3D dummyPosition;				 // stores the players position in the world
	float lengthOfFV;					 // calculates the length of the facing vector
	vector3D fvNormal;					 // normalizes the facing vector
	

	vector3D gFacingVector;				 // Stores the players facing vector, used in ray tracing for bullet collisions
	float gLengthOfFV;					 // calculates the length of the facing vector
	vector3D gFvNormal;					 // normalizes the facing vector

	using wall = vector<model>;
	wall mapWall;				// creates a vector storing all walls on the map
	float wallXSize = 15.0f;	// and initializes all of their sizes
	float wallYSize = 22.5f;
	float wallZSize = 15.0f;

	using box = vector<model>;
	box mapBox;					// creates a vector storing all boxes on the map
	float boxXSize = 15.0f;		// and initializes all of their sizes
	float boxYSize = 22.5f;
	float boxZSize = 15.0f;

	using targets = vector<target>;	// creates a vector storing all targets on the map
	targets mapTarget;				// and initializes all of their Y positions
	float targetYPosition = 28.0f;
	/*vector<float> wallXPos;
	vector<float> wallZPos;
	vector<IModel*>wall;*/

	using collisions = vector<collision>; // creates a vector storing all wall collision boxes on the map
	collisions mapWallBox;

	crosshair = myEngine->CreateSprite("crosshair.png", (resolutionX / 2) - 15, (resolutionY / 2.0f) - 17, 0.1f); // displays a crosshair in the center of the screen

	//flashBlind = myEngine->CreateSprite("flashbangBlind.png", 0.0f, 0.0f, 0.1f);

	currentWeapon.animationTimer = 0.0f;				// sets the animation timer used for weapons to 0

	enum gameState { Setup, Playing };		// initial game state declaration, to be altered later
	gameState currentGameState = Setup;

	// Visual output variables
	myPlayer.score = 0;						  // players score
	float startTime = 120.0f;
	float time = startTime;						  // amount of time the player starts the next game with, equal to startTime
	int minutes;						  // number of minutes remaining in time (time / 60)
	int seconds;						  // number of seconds in time			 (time % 60)
	bool highScoreDisplay = false;

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36); // Declares the font and font size used when outputting text
	IFont* highScoreFont = myEngine->LoadFont("Comic Sans MS", 80); // Declares the font and font size used when outputting text

	float mouseMovement;				 // stores the players mouse movemnt
	myEngine->StartMouseCapture();		 // starts mouse capture, removing the mouse curser and allowing the player full control of the game

	IMesh* floorMesh = myEngine->LoadMesh("ground.x");					//Mesh to set up the ground
	IMesh* DummyMesh = myEngine->LoadMesh("Dummy.x");					// dummy mesh, used multiple times for both player and camera dummy

	IMesh* desertEagleMesh = myEngine->LoadMesh("Desert_Eagle.x");		// Desert Eagle Weapon
	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");						// Walls
	IMesh* crateMesh = myEngine->LoadMesh("WoodCrate1.x");				// Tall wooden crates
	IMesh* cubeMesh = myEngine->LoadMesh("Block.x");					// small box, reskined and resized for multiple different uses
	IMesh* halfMenuBoxMesh = myEngine->LoadMesh("halfMenuBlock.x");
	IMesh* robberMesh = myEngine->LoadMesh("robberTarget.x");			// Targets
	IMesh* sqaureWallMesh = myEngine->LoadMesh("squareWall.x");			// taller wall mesh
	IMesh* flashBangMesh = myEngine->LoadMesh("flashBang.x");
	IMesh* aidenMesh = myEngine->LoadMesh("Block.x");
	IMesh* bulletMesh = myEngine->LoadMesh("Bullet.x");
	IMesh* quadMesh = myEngine->LoadMesh("quad.x");
	IModel* floor = floorMesh->CreateModel(0.0f, 0.0f, 0.0f);			// Creates the floor model
	IModel* spawnDummy = DummyMesh->CreateModel(0.0f, 00.0f, 0.0f);		// created the spawn location dummy (used to create the box collision for the target range enclosure)
	IModel* highScoreBox = cubeMesh->CreateModel(20.0f, 5.0f, 54.0f);
	IModel* startBox = cubeMesh->CreateModel(10.0f, 5.0f, 54.0f);
	IModel* timeUpBox = halfMenuBoxMesh->CreateModel(0.0f, 7.5f, 54.0f);
	IModel* timeDownBox = halfMenuBoxMesh->CreateModel(0.0f, 2.5f, 54.0f);
	IModel* soundBox = cubeMesh->CreateModel(30.0f, 5.0f, 54.0f);
	IModel* aidenBox = cubeMesh->CreateModel(45.0f, 8.0f, -50.0f);
	IModel* flashEffect = quadMesh->CreateModel(0.0f, 0.0f, 0.0f);
	flashEffect->ScaleX(20.0f);
	flashEffect->AttachToParent(myPlayer.myCamera->cameraDummy);
	flashEffect->SetLocalZ(10.0f);

	aidenBox->Scale(0.1);
	aidenBox->RotateY(180.0);
	aidenBox->SetSkin("aiden face.png");

	highScoreBox->SetSkin("highScoresOff.jpg");
	startBox->SetSkin("startOff.jpg");
	timeUpBox->SetSkin("timeUp.jpg");
	//timeUpBox->ScaleY(0.5f);
	timeDownBox->SetSkin("timeDown.jpg");
	//flashEffect->SetSkin("flashbangBlind.png");
	//timeDownBox->ScaleY(0.5f);

	soundBox->SetSkin("soundOn.jpg");
																		//desertEagle.model = desertEagleMesh->CreateModel(0.0f, -5.0f, 0.0f);// Creates the Desert Eagle (under the map)
	IModel* crate[kCrateQuantity];										// Declares the wooden crate models
	IModel* bulletTracer[kNumBulletTracers];							// Declares the bullet tracer models

	M4Colt.createWeapon(myEngine, 20, true, 0.15f, active, "M4Colt.x", myPlayer, 10.0f);
	desertEagle.createWeapon(myEngine, 7, false, 0.15f, inactive, "Desert_Eagle.x", myPlayer, -10.0f);

	currentWeapon = M4Colt;	// Sets the players current weapon to the M4Colt

	myEngine->Timer(); // calls the timer used for frame time

	 //reads in the map file
	readMap(numberOfWalls, numberOfBoxes, numberOfTargets, numberOfWallBoxes, mapWall, mapWallBox, mapBox, mapTarget, mapName);
	myPlayer.LoadHighScore(highScores);

	// creates the models for bullet tracers
	for (int i = 0; i < kNumBulletTracers; i++)
	{
		bulletTracer[i] = bulletMesh->CreateModel(0.0f, -10.0f, 0.0f); // hides the bullet tracers under the world at game start
		bulletTracer[i]->Scale(0.1f);
	}

	// craetes the models for the walls
	for (int i = 0; i < numberOfWalls; i++)
	{
		mapWall[i].worldModel = sqaureWallMesh->CreateModel(mapWall[i].XPos, 0.0f, mapWall[i].ZPos);
		mapWall[i].worldModel->Scale(2.0f);
		mapWall[i].worldModel->ScaleY(7.0f);
	}

	// creates the models for the boxes (crates)
	for (int i = 0; i < numberOfBoxes; i++)
	{
		mapBox[i].worldModel = crateMesh->CreateModel(mapBox[i].XPos, 0.0f, mapBox[i].ZPos);
		mapBox[i].worldModel->Scale(15.0f);
	}

	// creates the models for the targets.
	for (int i = 0; i < numberOfTargets; i++)
	{
		mapTarget[i].yPosition = targetYPosition;

		// creates each target based of the values stored in the vector (initialized by the map file)
		mapTarget[i].robberTarget = robberMesh->CreateModel(mapTarget[i].xPosition, mapTarget[i].yPosition, mapTarget[i].zPosition);

		// scales the targets to the appropriate size
		mapTarget[i].robberTarget->ScaleY(1.2f);
		mapTarget[i].robberTarget->ScaleX(1.2f);
		mapTarget[i].robberTarget->ScaleZ(0.2f);
		mapTarget[i].robberTarget->SetSkin("robber.png"); // skins the target to look like a robber (enemy)

		mapTarget[i].currentTargetState = waiting; // sets the state of the target to waiting
		mapTarget[i].resetTimer = 0;			   // sets the reset timer to 0

		mapTarget[i].robberTarget->RotateLocalY(mapTarget[i].rotationAmount); // rotates the target to face the correct direction (based off values in the map file)

																			  // this for loop checks if the target was rotated. if so it rotates the targets collision boxes
		if (mapTarget[i].rotationAmount == 90.0f || mapTarget[i].rotationAmount == 270.0f)
		{
			temp = mapTarget[i].xSize;
			mapTarget[i].xSize = mapTarget[i].zSize;
			mapTarget[i].zSize = temp;
		}
	}

	// generates the enclosure for the player on the target range map
	if (mapName == "Map2.txt")
	{
		for (int i = 0; i < numberOfWalls; i++)
		{
			mapWall[i].worldModel->SetSkin("WallGarage.tga");
			if (i < 2)
			{
				mapWall[i].worldModel->ScaleX(6.0);
				mapWall[i].worldModel->ScaleZ(0.5);
				mapWall[i].worldModel->ScaleY(0.15);
			}
			else
			{
				mapWall[i].worldModel->ScaleZ(6.0);
				mapWall[i].worldModel->ScaleX(0.5);
				mapWall[i].worldModel->ScaleY(0.15);
			}
		}
	}

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{

		//flashBlind->MoveX(-frameTime * 20);

		myPlayer.timers(frameTime, myEngine);

		myPlayer.move(myEngine);
		myPlayer.Jump(myEngine, frameTime);
		myPlayer.Look(myEngine);

		scoreText << "Score: " << myPlayer.score;       // outputs the players score to the screen
		myFont->Draw(scoreText.str(), 20, 20);
		scoreText.str("");

		ammoText << "Ammo: " << currentWeapon.ammo << "/" << currentWeapon.maxAmmo;       // outputs the players ammo to the screen
		myFont->Draw(ammoText.str(), 20, 920);
		ammoText.str("");

		if (currentWeapon.currentWeaponState == reloading) // checks to see if the player is currently reloading
		{
			reloadText << "Reloading: " << (int)(currentWeapon.reloadTimer * 100.0f) << "%";       // outputs the players reload timer
			myFont->Draw(reloadText.str(), 850, 720);
			reloadText.str("");
		}

		minutes = (time / 60); // calculates the number of minutes remaining;
		seconds = (int(time) % 60); // calculates the number of seconds remaining;

		timeText << "Time Remaining: " << minutes << " Minutes " << seconds << " Seconds"; // outputs the time remaining to the screen
		myFont->Draw(timeText.str(), 700, 20);
		timeText.str("");

		fpsText << "FPS: " << 1 / frameTime; // outputs the current FPS to the screen
		myFont->Draw(fpsText.str(), 200, 20);
		fpsText.str("");

		if (highScoreDisplay == true)
		{
			for (int i = 0; i < NUMBEROFHIGHSCORES; i++)
			{
				//highScoreText << i + 1 << ". " << "Name: " << highScores[i].name << " Score: " << highScores[i].score << " Time: " << highScores[i].time << " Kills Per Min:  " << highScores[i].killsPerMinute;
				highScoreText << i + 1 << ". "<< highScores[i].name << ": " << highScores[i].score;
				highScoreFont->Draw(highScoreText.str(), 50.0f, i * 80.0f + 120.0f);
				highScoreText.str("");
			}
		}

		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		myPlayer.playerOldX = myPlayer.playerDummy->GetX(); 	// updates the players x position for the previous frame before doing any movement in the current frame
		myPlayer.playerOldZ = myPlayer.playerDummy->GetZ();	// updates the players z position for the previous frame before doing any movement in the current frame

		if (myEngine->KeyHit(kReloadKey) && currentWeapon.currentWeaponState != reloading && currentWeapon.ammo < currentWeapon.maxAmmo) // reloads the players weapon
		{
			currentWeapon.reloadTimer = 0.0f;
			currentWeapon.currentWeaponState = reloading;
			
			soundMain(currentActiveWeapon, currentWeapon.ammo, currentWeapon.currentWeaponState, myPlayer.mSoundEnabled);
		
		}


		float weaponSelect = myEngine->GetMouseWheelMovement(); // gets mouse wheel movement since last frame
 
		if (weaponSelect > 0 || myEngine->KeyHit(kWeapon1Key)) // sets the players weapon to the M4Colt
		{
			currentActiveWeapon = M4ColtWeapon; // sets the current active weapon enum to be the M4Colt

			currentWeapon.currentWeaponState = inactive; // sets the previously equipped weapon to be inactive
			currentWeapon.weaponModel->SetY(-10.0f);			// hides the previous weapon
			currentWeapon = M4Colt;						// sets the current weapon to be the M4Colt
			currentWeapon.currentWeaponState = active;	// sets the current weapon to active
			currentWeapon.weaponModel->SetY(10.0f);			// moves the current weapon to the correct position
		}
		if (weaponSelect < 0 || myEngine->KeyHit(kWeapon2Key)) // sets the players weapon to the Desert Eagle
		{
			currentActiveWeapon = desertEagleWeapon; // sets the current active weapon enum to be the Desert Eagle

			currentWeapon.currentWeaponState = inactive;	// sets the previously equipped weapon to be inactive
			currentWeapon.weaponModel->SetY(-10.0f);			// hides the previous weapon
			currentWeapon = desertEagle;					// sets the current weapon to be the Desert Eagle
			currentWeapon.currentWeaponState = active;	// sets the current weapon to active
			currentWeapon.weaponModel->SetY(10.0f);			// moves the current weapon to the correct position
		}


		if (myEngine->KeyHit(kQuitKey)) //Quits the game
		{
			myPlayer.SaveHighScore(highScores, myPlayer, startTime - time);
			myEngine->Stop();
		}

		///////// Weapon firing including ray cast and animation,

		///////// for single shot weapons
		if (myEngine->KeyHit(kFireKey) && currentWeapon.autofireEnabled == false && currentWeapon.animationTimer <= 0.0f) // fires the players weapon and calls the ray tracing algorithm
		{
			if (currentWeapon.ammo <= 0 && currentWeapon.currentWeaponState != reloading) // checks if the current ammo clip is empty, and that the player is not already reloading
			{
				soundMain(currentActiveWeapon, currentWeapon.ammo, currentWeapon.currentWeaponState, myPlayer.mSoundEnabled);
				
				currentWeapon.currentWeaponState = reloading; // sets the state of the current weapon to reloading
				currentWeapon.reloadTimer = 0.0f;			 // sets the reload timer to 0
			}
			if (currentWeapon.currentWeaponState == active)  // checks if the players weapon is active
			{
				myPlayer.playerDummy->GetMatrix(&pMatrix[0][0]); // calls the matrix for the player, recording both the facing vector and the world position
				myPlayer.myCamera->cameraDummy->GetMatrix(&cMatrix[0][0]); // calls the matrix for the camera, recording both the facing vector and the world position

				facingVector = { pMatrix[2][0], cMatrix[2][1], pMatrix[2][2] }; // calculates the players facing vector using the information from the matricies
				lengthOfFV = sqrt((pMatrix[2][0] * pMatrix[2][0]) + (cMatrix[2][1] * cMatrix[2][1]) + (pMatrix[2][2] * pMatrix[2][2])); // calculates the  length of the facing vector
				fvNormal = { pMatrix[2][0] / lengthOfFV, cMatrix[2][1] / lengthOfFV, pMatrix[2][2] / lengthOfFV };	// normalizes the facing vector for use in the ray cast

				dummyPosition = { pMatrix[3][0], pMatrix[3][1], pMatrix[3][2] }; // gets the players position in the world

				bulletTracerSelection = (bulletTracerSelection + 1) % kNumBulletTracers; // updates the bullet tracer to choose the next one in the array

				bool wallHit = false;
				if (mapName == "Map1.txt") // checks if the first map is being played, as this is the only one containing walls
				{
					if (mapWall.size() != 0)
					{
						// runs a ray cast to see if the bullet has hit a wall, which are not penetratable
						wallHit = myPlayer.raycastWall(fvNormal, dummyPosition, mapWall, bulletTracer[bulletTracerSelection], numberOfTargets, numberOfWalls);
					}
				}
				if (!wallHit) // if no wall was hit, run the ray cast again to see if a target was hit
				{
					myPlayer.raycastShoot(fvNormal, dummyPosition, mapTarget, bulletTracer[bulletTracerSelection], numberOfTargets, myPlayer);
					//raycastShoot(fvNormal, dummyPosition, mapTarget, bulletTracer[bulletTracerSelection], numberOfTargets);
				}
				
				soundMain(currentActiveWeapon, currentWeapon.ammo, currentWeapon.currentWeaponState, myPlayer.mSoundEnabled);
				
				currentWeapon.ammo -= 1; // removes 1 bullet from the ammo clip
				currentWeapon.animationTimer = 0.1f;	// sets the weapons animation timer to 0.1 seconds
			}

			if (myPlayer.raycastMenu(fvNormal, dummyPosition, highScoreBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && highScoreDisplay == true)
			{
				highScoreDisplay = false;
				highScoreBox->SetSkin("highScoresOff.jpg");
			}
			else if (myPlayer.raycastMenu(fvNormal, dummyPosition, highScoreBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY))
			{
				highScoreDisplay = true;
				highScoreBox->SetSkin("highScoresOn.jpg");
			}

			if (myPlayer.raycastMenu(fvNormal, dummyPosition, startBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && myPlayer.currentPlayerState == playing)
			{
				time = startTime; 
				myPlayer.currentPlayerState = notPlaying;
				startBox->SetSkin("startOff.jpg");
			}
			else if (myPlayer.raycastMenu(fvNormal, dummyPosition, startBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && myPlayer.currentPlayerState == notPlaying)
			{
				// set timer to 2 minutes
				// set score to 0
				myPlayer.currentPlayerState = playing;
				time = startTime;
				myPlayer.score = 0;
				startBox->SetSkin("startOn.jpg");
			}
			if (myPlayer.raycastMenu(fvNormal, dummyPosition, timeUpBox, bulletTracer[bulletTracerSelection], myPlayer, HALFMENUBLOCKY) && myPlayer.currentPlayerState == notPlaying && startTime < MAXSTARTTIME)
			{
				startTime += 60.0f;
				time = startTime;
				
			}
			else if (myPlayer.raycastMenu(fvNormal, dummyPosition, timeDownBox, bulletTracer[bulletTracerSelection], myPlayer, HALFMENUBLOCKY) && myPlayer.currentPlayerState == notPlaying && startTime > MINSTARTTIME)
			{
				startTime -= 60.0f;
				time = startTime;
			}
			if (myPlayer.raycastMenu(fvNormal, dummyPosition, soundBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && myPlayer.mSoundEnabled == false)
			{
				soundBox->SetSkin("soundOn.jpg");
				myPlayer.mSoundEnabled = true;

			}
			else if (myPlayer.raycastMenu(fvNormal, dummyPosition, soundBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && myPlayer.mSoundEnabled == true)
			{
				soundBox->SetSkin("soundOff.jpg");
				myPlayer.mSoundEnabled = false;

			}
		}

		if (time > 0.0f && myPlayer.currentPlayerState == playing)
		{
			time -= frameTime;
		}
		if (time < 0.0f && myPlayer.currentPlayerState == playing)
		{
			myPlayer.SaveHighScore(highScores, myPlayer, startTime - time);
			myPlayer.currentPlayerState = notPlaying;

		}
	

		// for automatic fire weapons
		if (myEngine->KeyHeld(kFireKey) && currentWeapon.fireRateTimer <= 0.0f && currentWeapon.autofireEnabled == true)		//////////////////////// automatic firing
		{
			currentWeapon.fireRateTimer = currentWeapon.fireRate; // sets the fire rate timer to the value of the current weapons fire rate

			if (currentWeapon.ammo <= 0 && currentWeapon.currentWeaponState != reloading) // checks if the current ammo clip is empty, and that the player is not already reloading
			{
				
				soundMain(currentActiveWeapon, currentWeapon.ammo, currentWeapon.currentWeaponState, myPlayer.mSoundEnabled);
				
				
				currentWeapon.currentWeaponState = reloading;  // sets the state of the current weapon to reloading
				currentWeapon.reloadTimer = 0.0f;			  // sets the reload timer to 0
			}
			if (currentWeapon.currentWeaponState == active) // checks if the players weapon is active
			{
				myPlayer.playerDummy->GetMatrix(&pMatrix[0][0]); // calls the matrix for the player, recording both the facing vector and the world position
				myPlayer.myCamera->cameraDummy->GetMatrix(&cMatrix[0][0]); // calls the matrix for the camera, recording both the facing vector and the world position

				facingVector = { pMatrix[2][0], cMatrix[2][1], pMatrix[2][2] }; // calculates the players facing vector using the information from the matricies
				lengthOfFV = sqrt((pMatrix[2][0] * pMatrix[2][0]) + (cMatrix[2][1] * cMatrix[2][1]) + (pMatrix[2][2] * pMatrix[2][2])); // calculates the  length of the facing vector
				fvNormal = { pMatrix[2][0] / lengthOfFV, cMatrix[2][1] / lengthOfFV, pMatrix[2][2] / lengthOfFV }; // normalizes the facing vector for use in the ray cast

				dummyPosition = { pMatrix[3][0], pMatrix[3][1], pMatrix[3][2] }; // gets the players position in the world

				bulletTracerSelection = (bulletTracerSelection + 1) % kNumBulletTracers; // updates the bullet tracer to choose the next one in the array

				bool wallHit = false;
				if (mapName == "Map1.txt") // checks if the first map is being played, as this is the only one containing walls
				{
					if (mapWall.size() != 0)
					{
						// runs a ray cast to see if the bullet has hit a wall, which are not penetratable
						wallHit = myPlayer.raycastWall(fvNormal, dummyPosition, mapWall, bulletTracer[bulletTracerSelection], numberOfTargets, numberOfWalls);
					}
				}
				if (!wallHit)  // if no wall was hit, run the ray cast again to see if a target was hit
				{
					//raycastShoot(fvNormal, dummyPosition, mapTarget, bulletTracer[bulletTracerSelection], numberOfTargets);
					myPlayer.raycastShoot(fvNormal, dummyPosition, mapTarget, bulletTracer[bulletTracerSelection], numberOfTargets, myPlayer);
				}
				
				currentWeapon.animationTimer = 0.1f;	// sets the weapons animation timer to 0.1 seconds
				soundMain(currentActiveWeapon, currentWeapon.ammo, currentWeapon.currentWeaponState, myPlayer.mSoundEnabled);
				
				currentWeapon.ammo -= 1;   // removes 1 bullet from the ammo clip
				bullet[currentBullet].SetPosition(currentWeapon.weaponModel->GetX(), currentWeapon.weaponModel->GetY(), currentWeapon.weaponModel->GetZ(), currentBullet);
			}
			if (myPlayer.raycastMenu(fvNormal, dummyPosition, highScoreBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && highScoreDisplay == true)
			{
				highScoreDisplay = false;
				highScoreBox->SetSkin("highScoresOff.jpg");
			}
			else if (myPlayer.raycastMenu(fvNormal, dummyPosition, highScoreBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY))
			{
				highScoreDisplay = true;
				highScoreBox->SetSkin("highScoresOn.jpg");
			}

			if (myPlayer.raycastMenu(fvNormal, dummyPosition, startBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && myPlayer.currentPlayerState == playing)
			{
				time = startTime;
				myPlayer.currentPlayerState = notPlaying;
				startBox->SetSkin("startOff.jpg");
			}
			else if (myPlayer.raycastMenu(fvNormal, dummyPosition, startBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && myPlayer.currentPlayerState == notPlaying)
			{
				// set timer to 2 minutes
				// set score to 0
				myPlayer.currentPlayerState = playing;
				time = startTime;
				myPlayer.score = 0;
				startBox->SetSkin("startOn.jpg");
			}
			if (myPlayer.raycastMenu(fvNormal, dummyPosition, timeUpBox, bulletTracer[bulletTracerSelection], myPlayer, HALFMENUBLOCKY) && myPlayer.currentPlayerState == notPlaying && startTime < MAXSTARTTIME)
			{
				startTime += 60.0f;
				time = startTime;

			}
			else if (myPlayer.raycastMenu(fvNormal, dummyPosition, timeDownBox, bulletTracer[bulletTracerSelection], myPlayer, HALFMENUBLOCKY) && myPlayer.currentPlayerState == notPlaying && startTime > MINSTARTTIME)
			{
				startTime -= 60.0f;
				time = startTime;
			}
			if (myPlayer.raycastMenu(fvNormal, dummyPosition, soundBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && myPlayer.mSoundEnabled == false)
			{
				soundBox->SetSkin("soundOn.jpg");
				myPlayer.mSoundEnabled = true;

			}
			else if (myPlayer.raycastMenu(fvNormal, dummyPosition, soundBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY) && myPlayer.mSoundEnabled == true)
			{
				soundBox->SetSkin("soundOff.jpg");
				myPlayer.mSoundEnabled = false;

			}
			if (myPlayer.raycastMenu(fvNormal, dummyPosition, aidenBox, bulletTracer[bulletTracerSelection], myPlayer, FULLMENUBLOCKY))
			{
				for (int i = 0; i < kNumBulletTracers; i++)
				{
					bulletTracer[i]->SetSkin("aiden face.png");
				}
			}
		}

		currentWeapon.fireRateTimer -= frameTime;    // counts down the fire rate timer, controlling the time between shots

		if (currentActiveWeapon == M4ColtWeapon) // checks if the players current weapon is the M4Colt
		{
			M4Colt.ammo = currentWeapon.ammo; // updates the weapons ammo
			weaponAnimation(currentWeapon.animationTimer, currentWeapon.weaponModel, myPlayer.playerDummy, frameTime); // plays the recoil animation for the M4Colt
		}
		else if (currentActiveWeapon == desertEagleWeapon)  // checks if the players current weapon is the Desert Eagle
		{
			desertEagle.ammo = currentWeapon.ammo; // updates the weapons ammo
			desertEagleAnimation(currentWeapon.animationTimer, currentWeapon.weaponModel, myPlayer.playerDummy, frameTime); // plays the recoil animation for the Desert Eagle
		}


		////////////////////////////// Weapon reloading
		if (currentWeapon.currentWeaponState == reloading) // checks to see if the player is reloading
		{
			currentWeapon.reloadWeapon(currentWeapon, frameTime, ammoClip[currentAmmoClip], currentAmmoClip);
		}

		for (int i = 0; i < kNumAmmoClips; i++)  // cycles through all ammo clips
		{
		ammoClip[i].ammoClipGravity(frameTime);
		}
		for (int i = 0; i < kNumBullets; i++)  // cycles through all bullets
		{
			bullet[i].BulletGravity(frameTime);
		}

		for (int i = 0; i < numberOfTargets; i++) // cycles through the targets
		{
			if (mapTarget[i].hostage == true && mapTarget[i].resetTimer > 0) // checks if the target is in hostage mode (not to be shot)
			{
				mapTarget[i].resetTimer -= frameTime; // counts down on the hostage timer
			}
			else if (mapTarget[i].hostage == true && mapTarget[i].resetTimer < 0) // checks if teh hostage timer has expired
			{
				mapTarget[i].currentTargetState = waiting; // sets the taget to waiting
				mapTarget[i].resetTimer = (rand() % 10) + 2; // sets the reset timer to a random number 
			}

			if (mapTarget[i].currentTargetState == hit) // checks if the target is hit
			{
				if (mapTarget[i].hostage == false && myPlayer.currentPlayerState == playing) // checks if the taraget was a hostage
				{
					myPlayer.score += 1; // increases the players scorre
				}
				else if (myPlayer.currentPlayerState == playing)
				{
					myPlayer.score -= 1; // decreases the players score
				}
				mapTarget[i].currentTargetState = waiting; // sets the target to waiting mode
				mapTarget[i].resetTimer = (rand() % 10) + 2; // sets the reset timer to a random number
			}
			else if (mapTarget[i].currentTargetState == waiting && mapTarget[i].resetTimer <= 0) // checks if the target is waiting and the timer has finished
			{
				if ((rand() % 10) == 1) // gives a 1 in 10 chance for the next target to be a hostage
				{
					mapTarget[i].hostage = true;	// sets the target to hostage
					mapTarget[i].resetTimer = 8.0f; // sets the reset timer to 8 seconds
				}
				else
				{
					mapTarget[i].hostage = false;  // sets the target to enemy
					mapTarget[i].resetTimer = 0.0f; // sets the reset timer to 0
				}
				mapTarget[i].currentTargetState = ready;

			}
			else if (mapTarget[i].currentTargetState == waiting && mapTarget[i].resetTimer > 0) // checks if the target is waiting and if the reset timer is above 0
			{
				if (mapTarget[i].targetSinkLimit > 0) // checks if the target Y position is above the sink limit
				{
					mapTarget[i].robberTarget->MoveLocalY(-myPlayer.mLookSpeed); // moves the target down in the Y
					mapTarget[i].targetSinkLimit -= myPlayer.mLookSpeed;		 // decreements the sink limit
				}
				mapTarget[i].resetTimer -= frameTime; // decrements the reset timer
			}
			else if (mapTarget[i].currentTargetState == ready) // checks if the target is ready
			{
				if (mapTarget[i].targetSinkLimit < 15) // checks if the target is below the sink limit
				{
					mapTarget[i].robberTarget->MoveLocalY(myPlayer.mLookSpeed); // moves the target up in the Y
					mapTarget[i].targetSinkLimit += myPlayer.mLookSpeed;		// increments the sink limit
				}
			}

			if (mapTarget[i].hostage == true) // checks if the target is a hostage
			{
				mapTarget[i].robberTarget->SetSkin("hostage.jpg"); // sets the skin to show a hostage
			}
			else
			{
				mapTarget[i].robberTarget->SetSkin("robber.png");  // sets the skin to show a robber
			}

		}

		// flashbang drops

		if (seconds == 50 && grenadeDropped == false)
		{
			grenade[currentGrenade].XPos = 0.0f;
			grenade[currentGrenade].YPos = 80.0f;
			grenade[currentGrenade].ZPos = 60.0f;
			grenadeDropped = true;
		}

		if (grenadeDropped == true)
		{
			grenade[currentGrenade].grenadeTimer -= frameTime;
		}
		//grenade[currentGrenade].worldModel->GetMatrix(&gMatrix[0][0]); // calls the matrix for the player, recording both the facing vector and the world position

		vector3D cfacingVector = { cMatrix[2][0], cMatrix[2][1], cMatrix[2][2] }; // calculates the players facing vector using the information from the matricies
		float clengthOfFV = sqrt((cMatrix[2][0] * cMatrix[2][0]) + (cMatrix[2][1] * cMatrix[2][1]) + (cMatrix[2][2] * cMatrix[2][2])); // calculates the  length of the facing vector
		vector3D cfvNormal = { cMatrix[2][0] / clengthOfFV, cMatrix[2][1] / clengthOfFV, cMatrix[2][2] / clengthOfFV };	// normalizes the facing vector for use in the ray cast


		for (int i = 0; i < kNumGrenades; i++)
		{
			grenade[i].GrenadeGravity(frameTime);
		}

		if (grenade[currentGrenade].grenadeTimer < 0.0f && grenadeDropped == true && grenade[currentGrenade].flashExploded == false)
		{
			myPlayer.playerDummy->GetMatrix(&pMatrix[0][0]); // calls the matrix for the player, recording both the facing vector and the world position
			myPlayer.myCamera->cameraDummy->GetMatrix(&cMatrix[0][0]); // calls the matrix for the camera, recording both the facing vector and the world position

			gFacingVector = { pMatrix[2][0], cMatrix[2][1], pMatrix[2][2] }; // calculates the players facing vector using the information from the matricies
			gLengthOfFV = sqrt((pMatrix[2][0] * pMatrix[2][0]) + (cMatrix[2][1] * cMatrix[2][1]) + (pMatrix[2][2] * pMatrix[2][2])); // calculates the  length of the facing vector
			gFvNormal = { pMatrix[2][0] / gLengthOfFV, cMatrix[2][1] / gLengthOfFV, pMatrix[2][2] / gLengthOfFV };	// normalizes the facing vector for use in the ray cast
			grenade[currentGrenade].Detonate(myPlayer.myCamera->cameraDummy, gFvNormal, grenade[currentGrenade].worldModel, flashEffect, grenade[currentGrenade].flashExploded, myPlayer.mPlayerFlashed, myPlayer.mSoundEnabled);
			grenadeDropped = false;
			grenade[currentGrenade].grenadeTimer = 1.0f;

			soundGrenade(myPlayer.mPlayerFlashed, myPlayer.mSoundEnabled);
		}

		if (myPlayer.mPlayerFlashed == true)
		{
			flashEffect->MoveLocalX(-frameTime * 5);
		}

		// collisions with crates
		for (int i = 0; i < numberOfBoxes; i++)
		{
			collisionSide boxCollision = myPlayer.SphereToBox(myPlayer.playerDummy->GetX(), myPlayer.playerDummy->GetZ(), boxXSize, boxZSize, mapBox[i].XPos, mapBox[i].ZPos, 6.0f, myPlayer.playerOldX, myPlayer.playerOldZ);

			if (boxCollision != noSide)
			{
				myPlayer.ResolveCollision(myPlayer, boxCollision);
			}
		}

		// collisions with walls
		for (int i = 0; i < numberOfWallBoxes; i++)
		{
			collisionSide wallCollision = myPlayer.SphereToBox(myPlayer.playerDummy->GetX(), myPlayer.playerDummy->GetZ(), mapWallBox[i].xSize, mapWallBox[i].zSize, mapWallBox[i].xPosition, mapWallBox[i].zPosition, 6.0f, myPlayer.playerOldX, myPlayer.playerOldZ);

			if (wallCollision != noSide)
			{
				myPlayer.ResolveCollision(myPlayer, wallCollision);
			}
		}

		//Collisions the the wall enclosure, disabled while building for convinience
		collisionSide collision = myPlayer.SphereToBox(myPlayer.playerDummy->GetX(), myPlayer.playerDummy->GetZ(), 100, 100, spawnDummy->GetX(), spawnDummy->GetZ(), dummyRadius, myPlayer.playerOldX, myPlayer.playerOldZ);

		//myPlayer.ResolveCollisionReverse(myPlayer, collision);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}


