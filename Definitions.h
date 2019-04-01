#pragma once

#include <string>

// Constants for model arrays
const int kWallQuantity = 4;	  // number of walls, used in the inner area to enclose the player
const int kCrateQuantity = 25;	  // number of tall crates
const int kNumBulletTracers = 20; // number of bullet tracers
const int kNumNameBoxes = 4;		// number of name letter boxes for inputting a name
const int kNumBullets = 10;			// number of bullet casings
const int kNumGrenades = 5;
const int kNumRobberTargets = 12; // number of targets
const int kNumAmmoClips = 5;	  // number of empty ammo clips that can fall to the floor when reloading
const int  MAXSTARTTIME = 300.0f;
const int  MINSTARTTIME = 60.0f;
extern int score;
const int NUMBEROFHIGHSCORES = 10;
const int FULLMENUBLOCKY = 10.0F;
const int HALFMENUBLOCKY = 5.0F;

struct highScore
{
	std::string name;
	int score;
	float time;
	float killsPerMinute = score / time;
};

enum targetState { ready, waiting, hit };


//extern int numberOfWalls;		// Number of walls in the map, initialized in read map by the number in the map file (cannot be constant for this reason)
//extern int numberOfBoxes;		// Number of boxes in the map, initialized in read map by the number in the map file (cannot be constant for this reason)
//extern int numberOfTargets;	// Number of targets in the map, initialized in read map by the number in the map file (cannot be constant for this reason)
//extern int numberOfWallBoxes;	// Number of wall collision boxes in the map, initialized in read map by the number in the map file (cannot be constant for this reason)
//
//extern float pMatrix[4][4];	 // matrix used to store the player models information to be passed to functions such as facing vector and world location
//extern float cMatrix[4][4];	 // matrix used to store the camera models information to be passed to functions such as facing vector and world location
//extern float gMatrix[4][4];	 // matrix used to store the grenade models information to be passed to functions such as facing vector and world location
//
//extern vector3D gFacingVector;				 // Stores the players facing vector, used in ray tracing for bullet collisions
//extern float gLengthOfFV;					 // calculates the length of the facing vector
//extern vector3D gFvNormal;					 // normalizes the facing vector
//
//extern vector3D facingVector;				 // Stores the players facing vector, used in ray tracing for bullet collisions
//extern vector3D dummyPosition;				 // stores the players position in the world
//extern float lengthOfFV;					 // calculates the length of the facing vector
//extern vector3D fvNormal;					 // normalizes the facing vector
//
//extern int bulletTracerSelection;
//
//extern string mapName;
