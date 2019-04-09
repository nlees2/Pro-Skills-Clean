#pragma once

void EmitParticle(IMesh* particleMesh, float xPos, float yPos, float zPos, vector3D facingVector);
void particleMain(I3DEngine* myEngine, IMesh* particleMesh, float frameTime, float weaponXPos, float weaponYPos, float weaponZPos);