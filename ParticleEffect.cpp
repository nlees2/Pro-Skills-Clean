// Particle Effect.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <string>
#include <list>
#include "MSDefines.h"
#include "BaseMath.h"
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "CCamera.h"
#include "ParticleEffect.h"

using namespace tle;
using namespace std;


struct vector3d { float x; float y; float z; };
struct colour3d { float r; float g; float b; };

struct Particle
{
	// Content to be added
	vector3d position;
	vector3d velocity;
	colour3d colour;
	float life;
	IModel* ParticleModel;
};

// List of particles in the particle system
list<Particle> Particles;

//// for on the floor
//vector3d emitter = { 0.0f, 0.0f, 0.0f };
//float emitterRotation = 0.0f;
//bool emitterInFloor = true;

// for in the air
vector3d emitter = { 0.0f, 60.0f, 0.0f };
float emitterRotation = 90.0f;
bool emitterInFloor = false;


void particleMain(I3DEngine* myEngine,float frameTime,  float weaponXPos, float weaponYPos, float weaponZPos)
{
	//IModel* sphere = sphereMesh-

	//IMesh* ParticleMesh = myEngine->LoadMesh("flashBang.x");
	////ParticleModel->SetScale( 0.05f );

	//// Want the fountain to rain down desert eagles? swap the last block of code for this

	//IMesh* ParticleMesh = myEngine->LoadMesh("Desert_Eagle.x");
	////ParticleModel->SetScale(2.0f);

	//// for cute hearts

	//IMesh* ParticleMesh = myEngine->LoadMesh("heart.x");
	////ParticleModel->SetScale(0.4f);

	//// for spheres

	//IMesh* ParticleMesh = myEngine->LoadMesh("sphere.x");
	////ParticleModel->SetScale(0.2f);

	// Further data / functions to be added here
	vector3d origin = { 0.0f, 0.0f, 0.0f };

	const float EMITTERINTERVAL = 0.01f; // labsheet says 0.001 (1000 per sec). program allows up to about 0.0003 (3333 per sec) before it starts lagging 
	float emitterTimer = EMITTERINTERVAL;
	float nectParticleCD = 0.05f;
	const float GRAVITY = -30.0f;



	// Models
	IModel* ParticleModel = 0;
	IModel* GroundModel = 0;


	 // Update the scene between rendering. The function is passed the time since the last frame


	 //*************************************
	 // Particle Update
	 //*************************************
	 // Move each particle, and emit new particles. Code to be added for the tutorial

		list<Particle>::iterator itParticle = Particles.begin();
		while (itParticle != Particles.end())
		{
			if (myEngine->KeyHeld(Key_I))
			{
				emitter.z += frameTime;
			}
			if (myEngine->KeyHeld(Key_J))
			{
				emitter.x -= frameTime;
			}
			if (myEngine->KeyHeld(Key_K))
			{
				emitter.z -= frameTime;
			}
			if (myEngine->KeyHeld(Key_L))
			{
				emitter.x += frameTime;
			}

			(*itParticle).velocity.y += (GRAVITY * frameTime);
			(*itParticle).ParticleModel->MoveX((*itParticle).velocity.x * frameTime);
			(*itParticle).ParticleModel->MoveY((*itParticle).velocity.y * frameTime);
			(*itParticle).ParticleModel->MoveZ((*itParticle).velocity.z * frameTime);
			(*itParticle).life -= frameTime;
			(*itParticle).ParticleModel->LookAt(weaponXPos, weaponYPos, weaponZPos);
			(*itParticle).ParticleModel->Scale(0.1f);

			origin = { 0.0f, 0.0f, 0.0f };


			//emitter = { temp.x + 10.0f, temp.y - 2.5f, temp.z - 2.5f };
			emitter = { 0.0f, 50.0f, 0.0f };

			// for making the particles bounce
		/*	if ((*itParticle).ParticleModel->GetY() < 0)
			{
			(*itParticle).velocity = { (*itParticle).velocity.x, -((*itParticle).velocity.y) * 0.8f, (*itParticle).velocity.z };
			}*/

			//// to make them remain on the floor
			if ((*itParticle).position.y < 0.2f)
			{
				(*itParticle).velocity = { 0.0f, 0.0f, 0.0f };
				// scale down in the X to make them a flat circle
			}

			if ((*itParticle).life < 0.0f)
			{
				(*itParticle).ParticleModel->~IModel();
				itParticle = Particles.erase(itParticle);
			}
			else
			{
				itParticle++;
			}
		};

		emitterTimer -= frameTime;

		/*if (emitterTimer < 0.0f)
		{
		emitterTimer = EMITTERINTERVAL;

		EmitParticle();
		}*/

		/*while (emitterTimer < 0.0f)
		{
			emitterTimer += EMITTERINTERVAL;

			EmitParticle(ParticleMesh, weaponXPos, weaponYPos, weaponZPos);
		}*/
}
void EmitParticle(IMesh* particleMesh, float xPos, float yPos, float zPos, vector3D facingVector)
{
	Particle newParticle;

	//newParticle.position = emitter;
	newParticle.position.x = xPos;
	newParticle.position.y = yPos;
	newParticle.position.z = zPos;

	newParticle.ParticleModel = particleMesh->CreateModel(xPos, yPos, zPos);
	newParticle.ParticleModel->Scale(0.1f);
	
	int colour = gen::Random(1, 10);

	switch(colour){
	
	case 1: newParticle.ParticleModel->SetSkin("white.jpg");
		break;
	case 2: newParticle.ParticleModel->SetSkin("lightBlue.jpg");
		break;
	case 3: newParticle.ParticleModel->SetSkin("darkBlue.jpg");
		break;
	case 4: newParticle.ParticleModel->SetSkin("green.jpg");
		break;
	case 5: newParticle.ParticleModel->SetSkin("lightGreen.jpg");
		break;
	case 6: newParticle.ParticleModel->SetSkin("purple.jpg");
		break;
	case 7: newParticle.ParticleModel->SetSkin("red.jpg");
		break;
	case 8: newParticle.ParticleModel->SetSkin("yellow.jpg");
		break;
	case 9: newParticle.ParticleModel->SetSkin("pink.jpg");
		break;
	case 10: newParticle.ParticleModel->SetSkin("orange.jpg");
		break;
	}

	if (emitterInFloor == true)
	{
		newParticle.velocity.x = gen::Random(-8.0f, 8.0f);
		newParticle.velocity.y = gen::Random(70.0f, 90.0f);
		newParticle.velocity.z = gen::Random(-8.0f, 8.0f);
	}
	else
	{
		newParticle.velocity.x = gen::Random((50.0f * facingVector.x),(120.0f* facingVector.x));
		newParticle.velocity.y = gen::Random((90.0f * facingVector.y),(150.0f* facingVector.y));
		newParticle.velocity.z = gen::Random((50.0f * facingVector.z),(120.0f* facingVector.z));
	}

	// for water

	//newParticle.colour.r = gen::Random(0.0f, 0.1f);
	//newParticle.colour.g = gen::Random(0.2f, 0.5f);
	//newParticle.colour.b = gen::Random(0.5f, 0.7f);

	// for RGB

	newParticle.colour.r = gen::Random(0.0f, 1.0f);
	newParticle.colour.g = gen::Random(0.0f, 1.0f);
	newParticle.colour.b = gen::Random(0.0f, 1.0f);

	//// for blood

	//newParticle.colour.r = gen::Random(0.0f, 0.4f);
	//newParticle.colour.g = gen::Random(0.0f, 0.0f);
	//newParticle.colour.b = gen::Random(0.0f, 0.0f);

	// smoke
	//float rand = gen::Random(0.4f, 0.42f);

	//newParticle.colour.r = gen::Random(rand, rand);
	//newParticle.colour.g = gen::Random(rand, rand);
	//newParticle.colour.b = gen::Random(rand, rand);

	//// for fire

	//newParticle.colour.r = gen::Random(1.0f, 1.0f);
	//newParticle.colour.g = gen::Random(0.0f, 0.5f);
	//newParticle.colour.b = gen::Random(0.0f, 0.0f);


	//int rand = gen::Random(0, 100);

	//if (rand > 90)
	//{
	//	newParticle.colour.r = gen::Random(0.4f, 0.4f);
	//	newParticle.colour.g = gen::Random(0.4f, 0.4f);
	//	newParticle.colour.b = gen::Random(0.4f, 0.4f);
	//}

	// not bouncing
	newParticle.life = 4.5f;

	// for bounce
	/*newParticle.life = 7.0f;*/

	Particles.push_back(newParticle);
}

