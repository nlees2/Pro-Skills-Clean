#pragma once

#include <TL-Engine.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include "Definitions.h"
using namespace tle;


struct vector3D
{
	float x;
	float y;
	float z;
};
extern struct vector2D
{
	float x;
	float z;
};

const EKeyCode kQuitKey = Key_Escape;   //Closes the game
const EKeyCode kForwardsKey = Key_W;    //Moves the Player Forwards
const EKeyCode kBackwardsKey = Key_S;   //Moves the player backwards
const EKeyCode kRightKey = Key_D;	    //moves the player right
const EKeyCode kLeftKey = Key_A;		//moves the player left
const EKeyCode kJumpKey = Key_Space;	//makes the player jump
const EKeyCode kReloadKey = Key_R;		//reloads the players weapon
const EKeyCode kFireKey = Mouse_LButton;//fires the weapon
const EKeyCode kWeapon1Key = Key_1;		//selects the M4Colt
const EKeyCode kWeapon2Key = Key_2;		//Selects the Desert Eagle

										///////////// Enums



enum playerState { playing };


enum weaponState { active, inactive, reloading };

enum activeWeapon { M4ColtWeapon, desertEagleWeapon };


enum collisionSide { leftSide, rightSide, frontSide, backSide, noSide };

/////////////// Structs


struct model // Structure used to create world models and store information
{
	float XPos;
	float ZPos;
	float YPos = 0;
	float rotation;
	float rotationLimit;
	bool onFloor;
	int xSize;
	int ySize;
	int zSize;
	IModel* worldModel;
};

struct target // structure created specifically for targets
{
	int xSize = 12.0f;
	int ySize = 12.0f;
	int zSize = 2.0f;

	bool hostage;

	float rotationAmount;
	int xPosition;
	int yPosition;
	int zPosition;
	float resetTimer;
	float targetSinkLimit = 15;
	targetState currentTargetState;
	IModel* robberTarget;
};

struct collision  // structure used to make collision boxes that are not specific to a single wodel (ie: collision box made up of multiple walls)
{
	int xPosition;
	int yPosition;
	int zPosition;

	int xSize;
	int ySize;
	int zSize;

	float rotationAmount;
};


void readMap(int &numberOfWalls, int &numberOfBoxes, int & numberOfTargets, int &numberOfWallBoxes, vector<model> &wall, vector<collision> &wallCollision, vector<model> &box, vector<target> &targetThief, string &mapName);
void weaponAnimation(float &animationTimer, IModel* M4Colt, IModel* player, float frameTime);
void desertEagleAnimation(float &animationTimer, IModel* desertEagle, IModel* player, float frameTime);
int nextInArray(int i, int arraySize);
bool compare(const highScore& a, const  highScore& b);