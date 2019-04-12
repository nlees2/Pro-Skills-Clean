#include "BarrelFlame.h"



CBarrelFlame::CBarrelFlame(I3DEngine* myEngine, float barrelDiam, float barrelPosX, float barrelPosY, float barrelPosZ)
{

	//Set up location of particals for each barrel 


	IMesh* particleMesh = myEngine->LoadMesh("particle.x");
	IMesh* particleMesh2 = myEngine->LoadMesh("particle2.x");
	posX = barrelPosX;
	posY = barrelPosY;
	posZ = barrelPosZ;



	const float barrelRadius = (barrelDiam / 2.0f);
	float minX = posX - barrelRadius;
	float maxX = posX + barrelRadius;
	float minZ = posZ - barrelRadius;
	float maxZ = posZ + barrelRadius;



	for (int i = 0; i < kTotalParticles; i++)
	{

		particles[i].mModel = particleMesh->CreateModel(Random(minX, maxX), -5.0f, Random(minZ, maxZ));
		particles2[i].mModel = particleMesh2->CreateModel(Random(minX, maxX), -5.0f, Random(minZ, maxZ));


		// set random skin
		int num = (int)Random(0, totalParticleSkins);
		particles[i].mModel->SetSkin(particleSkinNames[num]);
		num = (int)Random(0, totalParticleSkins2);
		particles2[i].mModel->SetSkin(particleSkinNames2[num]);

		// set random move vectors
		particles[i].mMoveVector[0] = Random(-1, 1);  // X move
		particles[i].mMoveVector[2] = Random(-1, 1);  // Z move

		 //set random move vectors
		particles2[i].mMoveVector[0] = Random(-1, 1);  // X move
		particles2[i].mMoveVector[2] = Random(-1, 1);  // Z move
	}
}


CBarrelFlame::~CBarrelFlame()
{
}

void CBarrelFlame::EmitParticle()
{
	for (int i = 0; i < kTotalParticles; i++)
	{
		// check if active
		if (particles[i].mActive == false)
		{
			// activate and skip rest
			particles[i].mActive = true;
			break;
		}
	}

	for (int i = 0; i < kTotalParticles; i++)
	{
		// check if active
		if (particles2[i].mActive == false)
		{
			// activate and skip rest
			particles2[i].mActive = true;
			break;
		}
	}
}

void CBarrelFlame::UpdateParticles(IModel* mod, float &frameTime, const float& barrelDiam)
{
	const float barrelRadius = (barrelDiam - 3.0f);
	float minX = posX - barrelRadius;
	float maxX = posX + barrelRadius;
	float minZ = posZ - barrelRadius;
	float maxZ = posZ + barrelRadius;

	

	for (int i = 0; i < kTotalParticles; i++)
	{
		// check if active
		if (particles[i].mActive)
		{
			// update rotation
			particles[i].mModel->LookAt(mod);

			// update velocity
			particles[i].mVelocity = particles[i].mVelocity + (kGravity * frameTime);

			// move particle
			particles[i].mModel->MoveLocalX(particles[i].mMoveVector[0] * frameTime);
			particles[i].mModel->MoveLocalY(-particles[i].mVelocity * frameTime);
			particles[i].mModel->MoveLocalZ(particles[i].mMoveVector[2] * frameTime);

			// check if hit the random limit
			if (particles[i].mModel->GetLocalY() > Random(23, 50))
			{
				particles[i].mVelocity = kStartVelocity;
				particles[i].mModel->SetPosition(Random(minX, maxX), 15.0f, Random(minZ, maxZ));
				particles[i].mActive = false;
			}
		}

		 //check if active
		if (particles2[i].mActive)
		{
			// update rotation
			particles2[i].mModel->LookAt(mod);

			// update velocity
			particles2[i].mVelocity = particles[i].mVelocity + (kGravity * frameTime);

			// move particle
			particles2[i].mModel->MoveLocalX(particles[i].mMoveVector[0] * frameTime);
			particles2[i].mModel->MoveLocalY(-particles[i].mVelocity * frameTime);
			particles2[i].mModel->MoveLocalZ(particles[i].mMoveVector[2] * frameTime);

			// check if hit the random limit
			if (particles2[i].mModel->GetLocalY() > Random(15, 700))
			{
				particles2[i].mVelocity = kStartVelocity;
				particles2[i].mModel->SetPosition(Random(minX, maxX), 15.0f, Random(minZ, maxZ));
				particles2[i].mActive = false;
			}
		}
	}
}

float CBarrelFlame::Random(int rangeMin, int rangeMax)
{
	float result = (float)rand() / (float)(RAND_MAX + 1);
	result *= (float)(rangeMax - rangeMin);
	result += rangeMin;

	return result;
}