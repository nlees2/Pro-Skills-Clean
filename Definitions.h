#pragma once

#include <string>

// Constants for model arrays
const int kWallQuantity = 4;	  // number of walls, used in the inner area to enclose the player
const int kCrateQuantity = 25;	  // number of tall crates
const int kNumBulletTracers = 10; // number of bullet tracers
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
