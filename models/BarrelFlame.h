#pragma once

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "CPlayer.h"
using namespace tle;

const int totalParticleSkins = 11;
const int totalParticleSkins2 = 3;
const string particleSkinNames[totalParticleSkins] = {  "flm01_tlxadd.png", "flm02_tlxadd.png", "flm03_tlxadd.png", 
														"flm04_tlxadd.png", "flm05_tlxadd.png", "flm06_tlxadd.png",
														"flm07_tlxadd.png", "flm08_tlxadd.png", "flm09_tlxadd.png", 
														"flm10_tlxadd.png", "flm11_tlxadd.png" };

const string particleSkinNames2[totalParticleSkins2] = { "smoke_tlxadd.png", "smoke2_tlxadd.png", "smoke3_tlxadd.png"};

const int kTotalParticles = 100;  // count of particles in the scene

const float kParicleResetPosY = 43.0f;  // position y of the particle that is reset
const float kStartVelocity = -10.01f;  // inital move speed for particles
const float kGravity = -2.01f;  // reduce move speed of particles

struct SParticle
{
	IModel* mModel;
	float mMoveVector[3];
	float mVelocity;
	bool mActive = false;
};


class CBarrelFlame
{

public:
	CBarrelFlame(I3DEngine* myEngine, float barrelDiam, float barrelPosX, float barrelPosY, float barrelPosZ);
	~CBarrelFlame();

	void EmitParticle();
	void UpdateParticles(IModel* mod, float &frameTime, const float& barrelDiam);
	float Random(int rangeMin, int rangeMax);

private:
	SParticle particles[kTotalParticles];
	SParticle particles2[kTotalParticles];
	float posX, posY, posZ;
};

