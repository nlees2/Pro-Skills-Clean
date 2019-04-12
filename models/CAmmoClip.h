#pragma once
#include "CModel.h"
#include "Definitions.h"

class CAmmoClip : public CModel
{
public:

	IMesh * ammoClipMesh;




	CAmmoClip();
	~CAmmoClip();
	void createAmmoClip(I3DEngine* myEngine, string meshName);
	void ammoClipGravity(float frameTime);

};
