#pragma once
#include "Definitions.h"
#include "CCamera.h"
#include <algorithm>

class CPlayer
{
public:
	float mMoveSpeed;
	bool mPlayerOnGround;
	bool mSoundEnabled;
	bool mPlayerFlashed;
	float mJumpSpeed;
	float mFallSpeed;
	float mLookSpeed;
	float mJumpTimer;
	vector3D mPosition;
	IMesh* dummyMesh;
	IModel* playerDummy;
	CCamera* myCamera;
	std::string name = "Nick";
	playerState currentPlayerState;

	int score;
	float playerOldX;					 // records the x position of the player in the previous frame for use in collision resolution
	float playerOldZ;					 // records the z position of the player in the previous frame for use in collision resolution

	


	//ICamera * mPlayerCamera;

	CPlayer(float frameTime, I3DEngine* myEngine);
	~CPlayer();
	void move(I3DEngine* myEngine);
	void Look(I3DEngine* myEngine);
	void Jump(I3DEngine* myEngine, float frameTime);
	void timers(float &frameTime, I3DEngine* myEngine);
	bool raycastShoot(vector3D facingVector, vector3D dummyPosition, vector<target> &target, IModel* bulletTracer, const int ktargetQuantity, CPlayer myPlayer);
	bool raycastWall(vector3D facingVector, vector3D dummyPosition, vector<model> &wall, IModel* bulletTracer, const int ktargetQuantity, int numberOfWalls);
	bool raycastMenu(vector3D facingVector, vector3D dummyPosition, IModel* &target, IModel* bulletTracer, CPlayer myplayer, int blockYSize);
	bool raycastName(vector3D facingVector, vector3D dummyPosition, IModel* &target, IModel* bulletTracer, CPlayer myplayer, int blockYSize);
	collisionSide SphereToBox(float playerX, float playerZ, float cubeXLength, float cubeZLength, float cubeXPos, float cubeZPos, float playerRadius, float playerOldX, float playerOldZ);
	bool SphereToBox2(float pointX, float pointY, float pointZ, float cubeXLength, float cubeYLength, float cubeZLength, float cubeXPos, float cubeYPos, float cubeZPos, float sphereRadius);
	void ResolveCollision(CPlayer myPlayer, collisionSide collision);
	void ResolveCollisionReverse(CPlayer myPlayer, collisionSide collision);
	void LoadHighScore(vector<highScore> &highScores);
	void CPlayer::SaveHighScore(vector<highScore> &highScores, CPlayer currentPlayer, float time, string playerName);
};