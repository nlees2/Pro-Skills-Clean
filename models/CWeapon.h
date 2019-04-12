#pragma once
#include "Functions.h"
#include "Definitions.h"
#include "CPlayer.h"
#include "CAmmoClip.h"
// Only using SFML audio although it has other modules

class CWeapon
{
public:

	int maxAmmo;					// sie of an Desert Eagle ammo clip
	bool autofireEnabled;			// disables the auto fire option on the Desert Eagle
	float fireRate;					// sets the fire rate of the Desert Eagle (interval between shots in seconds)											
	int ammo;						// sets the current ammo to the max clip size
	weaponState currentWeaponState;	// sets the Desert Eagle as a secondary weapon
	IMesh* weaponMesh;
	IModel* weaponModel;
	float reloadTimer;
	float animationTimer;
	float fireRateTimer;




	CWeapon();
	~CWeapon();
	void createWeapon(I3DEngine* myEngine, int maxAmmoValue, bool autofireValue, float fireRateValue, weaponState currentWeaponStateValue, string meshName, CPlayer myPlayer, float weaponStartingY);
	void fireWeapon();
	void reloadWeapon(CWeapon &currentWeapon, float frameTime, CAmmoClip &mapAmmoClip, int &currentAmmoClip);
	void selectWeapon();
};